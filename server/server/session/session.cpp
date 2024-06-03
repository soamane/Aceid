#include "session.h"
#include "../api/api.h"

#include "../../general/logsystem/logmanager/logmanager.h"
#include "../../general/utils/utils.h"

#include "../../general/protect/dataencryption/dataencryption.h"

Session::Session(boost::asio::ip::tcp::socket& socket) : m_socket(std::move(socket)), m_packetHandler(std::make_unique<PacketHandler>(m_socket)) {
	LogManager::GetInstance()->InitEventLog(); // Инициализирует объект EventLog для записи будущих event-логов
	CREATE_EVENT_LOG("Session created");
}

Session::~Session() {
	if (m_socket.is_open()) {
		m_socket.close();
		m_packetHandler.reset();

		CREATE_EVENT_LOG("Socket was closed because the session expired");
		return;
	}

	CREATE_EVENT_LOG("Session closed");
}


void Session::Open() {
	auto self(shared_from_this());
	// Получение авторизационных данных клиента в формате JSON
	m_packetHandler->ReceiveClientMessage([self](const std::string& jsonData) {
		if (jsonData.empty()) {
			CREATE_EVENT_LOG("Received message is empty");
			return;
		}

		// Вызов сессионного обработчика данных
		self->HandleClientMessage(jsonData);
	});
}

void Session::HandleClientMessage(const std::string& jsonData) {
	// Инициализация API для обработки авторизационных данных
	API api(jsonData);

	// Получение результата обработки данных
	const AuthStatus authStatus = api.GetAuthStatus();

	// Проверка полученного результата
	if (authStatus == AUTH_SUCCESS) {
		CREATE_EVENT_LOG("Client has successfully logged in");

		// Переименовывает event-лог файл в юзернейм, указанный в структуре данных клиента
		LogManager::GetInstance()->GetEventLog()->RenameAndMove(api.GetAuthDataObject().username);

		// Отсылает успешный ответ на запрос авторизации
		m_packetHandler->SendServerResponse(SUCCESS_AUTH);

		// Конвертирование удаленного файла в массив байтов для последующей передачи
		const std::vector<char>& fileBytes = Utils::ConvertFileToBytes("aceid.exe");
		if (fileBytes.empty()) {
			CREATE_EVENT_LOG("Failed to convert the file");
			return;
		}

		// Отправка прочитанного файла
		m_packetHandler->SendDataBuffer(fileBytes);
	} else {
		CREATE_EVENT_LOG("Client failed to authenticate");

		// Отсылает отклоненный ответ на запрос авторизации
		m_packetHandler->SendServerResponse(FAILED_AUTH);
	}
}
