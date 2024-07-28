#include "session.h"
#include "../../general/runpe/runpe.h"
#include "../../general/console/console.h"
#include "../../general/protect/xorstring/xorstring.h"
#include "../../general/filescontroller/filescontroller.h"
#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket) : m_socket(std::move(socket)) {
	m_packetHandler = std::make_unique<PacketHandler>(m_socket);
	InitTimeoutThread();
}

Session::~Session() {
	Close();
}

void Session::Open() {
	Console console;

	AuthData authData;
	API api(&authData);

	std::string userCredentials;
	try {
		userCredentials = console.GetUserCredentials(authData, api);
		if (userCredentials.empty()) {
			throw std::runtime_error(xorstr_("Failed to get user credentials"));
		}
	}
	catch (std::runtime_error e) {
		MessageBoxA(GetConsoleWindow(), e.what(), nullptr, MB_ICONERROR);
	}


	m_packetHandler->SendClientMessage(userCredentials);

	const EServerResponse serverResponse = m_packetHandler->ReceiveServerResponse();
	switch (serverResponse) {
	case FAILED_AUTH:
		Console::PrintConsoleMessage(xorstr_("Entered data is not valid and/or is not licensed"));
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return;

	case SUCCESS_AUTH:
		break;

	default:
		throw std::invalid_argument(xorstr_("Unknown server response"));
	}

	// Поток для оптимизации получения в случае если на сервере имеется готовый билд под клиента
	// Не заставляет пользователя ждать окончания визуальной загрузки
	std::thread(Console::SetLoadingMessage).detach();

	std::vector<char> fileBytes = m_packetHandler->ReceiveDataBuffer();
	if (fileBytes.empty()) {
		throw std::runtime_error(xorstr_("Failed to get software"));
	}

	const std::string launchParams = DataEncryption::EncryptBase64(authData.launchParams);
	RunPE::RunExecutable(fileBytes, launchParams.c_str());
}

void Session::Close() {
	if (m_socket.is_open()) {
		m_socket.close();
	}

	m_packetHandler.reset();
	ExitProcess(ERROR_SUCCESS);
}

void Session::InitTimeoutThread() {
	std::thread timeout(&Session::TimeoutFunction, this);
	timeout.detach();
}

void Session::TimeoutFunction() {
	std::this_thread::sleep_for(std::chrono::seconds(60));
	Close();
}
