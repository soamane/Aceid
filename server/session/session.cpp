#include "session.h"
#include <iostream>

Session::Session(boost::asio::ip::tcp::socket& socket) : socket(std::move(socket)) { }

void Session::run() {

}
