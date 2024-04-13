#pragma once
#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <vector>
#include <boost/asio.hpp>

class ConnectionManager
{
public:
	static ConnectionManager* GetInstance();

	void addConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

	void closeConnection(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
	void closeAllConnections();

	const std::size_t getConnectionsCount() const;

private:
	ConnectionManager();

private:
	std::vector<std::shared_ptr<boost::asio::ip::tcp::socket>> connections;
};

#endif // !CONNECTION_MANAGER_H
