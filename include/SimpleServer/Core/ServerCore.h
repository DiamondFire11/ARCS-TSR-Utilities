#pragma once

// Networking Includes
#include <cstdio>
#include <sys/socket.h>
#include <cstdlib> // strlen
#include <arpa/inet.h> // inet_addr
#include <unistd.h> // WRITE
#include <pthread.h> // THREADING link with lpthread

// Message logging
#include <spdlog.h> // SPDLOG

// Modules Includes
#include <string>
#include "ServerModuleManager.h"

namespace arcs::util {
    struct socketData {
        int * socket;
        std::unordered_map<std::string, std::shared_ptr<arcs::util::IModule>> * modules;
    };

    class ServerCore {
    public:
        explicit ServerCore(int port = 3001);
        ~ServerCore();

        [[nodiscard]] bool isOk() const;
        void coreListen();

    private:
        int m_socketDesc , m_newSocket , m_c , *m_newSock, m_port;
        struct sockaddr_in m_server , m_client;
        char *m_message;

        arcs::util::ServerModuleManager m_moduleManager;

        bool m_good;

        static void * connectionHandler(void *);
    };
}