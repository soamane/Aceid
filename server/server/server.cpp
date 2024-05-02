#include "server.h"

#include "session/session.h"
#include "../general/logsystem/logmanager/logmanager.h"

Server::Server(boost::asio::io_context& context, short port)
    : m_acceptor(context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4( ), port))
{ }

void Server::start( )
{
    std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(m_acceptor.get_executor( ));
    m_acceptor.async_accept(*socket, [this, socket](boost::system::error_code errorCode)
    {
        if (!errorCode)
        {
            createSession(socket);
        } else
        {
            CREATE_SERVER_LOG("The socket failed to connect to the server")
        }
        start( );
    });
}

void Server::stop( )
{
    m_acceptor.close( );
}

void Server::createSession(std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    std::shared_ptr<Session> session = std::make_shared<Session>(*socket);
    {
        session->run( );
    }
}