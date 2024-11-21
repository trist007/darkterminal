/**
 *
 *  @file TcpServerTest.cc
 *  @author An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/trantor
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the License file.
 *
 *  Trantor
 *
 */

#include <trantor/net/TcpServer.h>
#include <trantor/utils/Logger.h>
#include <trantor/net/EventLoopThread.h>
#include <string>
#include <iostream>
using namespace trantor;
#define USE_IPV6 0

#define MAJOR 0
#define MINOR 1
#define PATCH 33

typedef struct Version
{
    size_t major;
    size_t minor;
    size_t patch;

    std::string printVersion()
    {
        char version[64];
        snprintf(version, 64, "v%zu.%zu.%zu", major, minor, patch);
        return std::string(version);
    }
} Version;

Version current = { MAJOR, MINOR, PATCH };

int main()
{
    LOG_DEBUG << "test start";
    Logger::setLogLevel(Logger::kTrace);
    EventLoopThread loopThread;
    loopThread.run();
#if USE_IPV6
    InetAddress addr(8888, true, true);
#else
    InetAddress addr(8888);
#endif
    TcpServer server(loopThread.getLoop(), addr, "test");
    server.setBeforeListenSockOptCallback([](int fd) {
        std::cout << "setBeforeListenSockOptCallback:" << fd << std::endl;
    });
    server.setAfterAcceptSockOptCallback([](int fd) {
        std::cout << "afterAcceptSockOptCallback:" << fd << std::endl;
    });
    server.startCommand();
    server.setRecvMessageCallback(
        [&server](const TcpConnectionPtr &connectionPtr, MsgBuffer *buffer) {

        size_t id;
        std::string input;
        id = server.isRegistered(connectionPtr);

        input = std::string(buffer->peek(), buffer->readableBytes());
        buffer->retrieveAll();

        if (server.m_user_array[id].authenticated == false)
        {
            server.Authenticate(connectionPtr, input);
        }
        else
        {
            server.ParseInput(connectionPtr, input);
            std::cout << server.m_user_array[id].username << ": " << input << std::endl;
            connectionPtr->send(buffer->peek(), buffer->readableBytes());
        }

        // connectionPtr->forceClose();
        });
    server.setConnectionCallback([&server](const TcpConnectionPtr &connPtr) {
        if (connPtr->connected())
        {
            LOG_DEBUG << "New connection";
            connPtr->send("Welcome to darkterminal " + current.printVersion() + "\n");
            if ((server.AddUser(connPtr)) == -1)
            {
                std::cerr << "Try increasing the max connections" << std::endl;
            }
        }
        else if (connPtr->disconnected())
        {
            LOG_DEBUG << "connection disconnected";
        }
    });
    server.setIoLoopNum(3);
    server.start();
    loopThread.wait();
}
