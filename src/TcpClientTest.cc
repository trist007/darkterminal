#include <trantor/net/TcpClient.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <csignal>
#include <iostream>
#include <atomic>
#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/tcp.h>
#endif

#define BLUE    "\033[34m"
#define GREEN   "\033[32m"
#define MAGENTA "\033[35m"
#define RED     "\033[31m"
#define WHITE   "\033[37m"

using namespace trantor;
#define USE_IPV6 0

void signalHandler(int signum)
{
    std::cout << "Interrupt signal (" << signum << ") received" << std::endl;
    std::cout << "Use /quit instead" << std::endl;
    //exit(signum);
}

int main()
{
    signal(SIGINT, signalHandler); 

    //trantor::Logger::setLogLevel(trantor::Logger::kTrace);
    //LOG_DEBUG << "TcpClient class test!";
    EventLoop loop;
#if USE_IPV6
    InetAddress serverAddr("::1", 8888, true);
#else
    InetAddress serverAddr("127.0.0.1", 8888);
#endif
    std::shared_ptr<trantor::TcpClient> client;
    std::atomic_int connCount;
    connCount = 10;
    client = std::make_shared<trantor::TcpClient>(&loop,
            serverAddr,
            "tcpclienttest");
    client->setSockOptCallback([](int fd) {
            //LOG_DEBUG << "setSockOptCallback!";
#ifdef _WIN32
#elif __linux__
            int optval = 10;
            ::setsockopt(fd,
                    SOL_TCP,
                    TCP_KEEPCNT,
                    &optval,
                    static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                    SOL_TCP,
                    TCP_KEEPIDLE,
                    &optval,
                    static_cast<socklen_t>(sizeof optval));
            ::setsockopt(fd,
                    SOL_TCP,
                    TCP_KEEPINTVL,
                    &optval,
                    static_cast<socklen_t>(sizeof optval));
#else
#endif
    });
    client->setConnectionCallback(
            [&client, &loop, &connCount](const TcpConnectionPtr &conn) {

            if (conn->connected())
            {
                connCount++;
                //LOG_DEBUG << " connected!";
                client->m_user.connected = true;
            }
            else
            {
                LOG_DEBUG << " disconnected";
                //--connCount;
                if (connCount == 0)
                    loop.quit();
            }

            });
    client->setMessageCallback(
            [&client](const TcpConnectionPtr &conn, MsgBuffer *buf) {

            std::string input;

            if (client->m_user.authenticated == true && client->m_user.welcome == true &&
                client->m_user.changenick == false && client->m_user.directMessage == false)
            {
                // conn->shutdown();
                input = std::string(buf->peek(), buf->readableBytes());
                buf->retrieveAll();
                client->ParseServerInput(conn, input);
                //std::cout << std::string(buf->peek(), buf->readableBytes());
                std::cout << input;
                std::cout << WHITE;
            }

            if (client->m_user.directMessage == true)
            {
                client->m_user.directMessage = false;
            }

             if (client->m_user.resetpassword == true)
            {
                client->ResetPasswordResponse(conn, buf);
            }

            if (client->m_user.changenick == true)
            {
                client->ChangeNickResponse(conn, buf);
            }

            if (client->m_user.authenticated == false && client->m_user.welcome == true)
            {
                client->AuthenticateResponse(conn, buf);
            }

            // Show welcome banner
            if (client->m_user.welcome == false)
            {
                std::cout << std::string(buf->peek(), buf->readableBytes());
                buf->retrieveAll();
                client->Authenticate(conn);
                client->m_user.welcome = true;
            }

            });
    client->connect();
    loop.loop();
}
