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

Version current = { MAJOR, MINOR, PATCH };

int main()
{
    //LOG_DEBUG << "test start";
    //Logger::setLogLevel(Logger::kTrace);
    EventLoopThread loopThread;
    loopThread.run();
#if USE_IPV6
    InetAddress addr(8888, true, true);
#else
    InetAddress addr(8888);
#endif
    TcpServer server(loopThread.getLoop(), addr, "test");
    server.setBeforeListenSockOptCallback([](int fd) {
        //std::cout << "setBeforeListenSockOptCallback:" << fd << std::endl;
    });
    server.setAfterAcceptSockOptCallback([](int fd) {
        //std::cout << "afterAcceptSockOptCallback:" << fd << std::endl;
    });
    server.startCommand();
    server.setRecvMessageCallback(
        [&server](const TcpConnectionPtr &connectionPtr, MsgBuffer *buffer) {

        size_t id;
        std::string input;
        input = std::string(buffer->peek(), buffer->readableBytes());
        buffer->retrieveAll();

        id = server.isRegistered(connectionPtr);
        server.ParseInput(connectionPtr, input, id);

        if (server.m_user_array[id].authenticated == false)
        {
            server.Authenticate(connectionPtr, input);
        }
        else if (server.m_user_array[id].directMessage == true)
        {
            server.directMessage(input, id);
        }
        else if (server.m_user_array[id].changeNick == true)
        {
            server.ChangeNick(connectionPtr, input);
        }
        else
        {
            std::cout << "\n" + server.m_user_array[id].username << ": " << input << std::endl;
        }

        // connectionPtr->forceClose();
        });
    server.setConnectionCallback([&server](const TcpConnectionPtr &connPtr) {

        size_t id;

        if (connPtr->connected())
        {
            //LOG_DEBUG << "New connection";
            connPtr->send("Welcome to darkterminal " + current.printVersion() + "\n\n");
            if ((id = server.AddUser(connPtr)) == -1)
            {
                std::cerr << "Max conn of " << server.m_max_conn <<" reached" << std::endl;
                std::cerr << "Cannot add user" << std::endl;
            }
        }
        else if (connPtr->disconnected())
        {
            for (size_t i = 0; i < server.m_user_array.size(); i++)
            {
                if (server.m_user_array[i].tcp_ptr == connPtr)
                {
                    server.zeroOut(i);
                }
            }
        }

        server.cleanConnections();

    });
    server.setIoLoopNum(3);
    server.start();
    loopThread.wait();
}
