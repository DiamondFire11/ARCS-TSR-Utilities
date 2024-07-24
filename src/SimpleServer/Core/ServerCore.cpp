//
// Created by Will Snyder on 7/21/24.
//

#include "../../../include/SimpleServer/Core/ServerCore.h"

arcs::util::ServerCore::ServerCore(int port) {
    this->m_port = port;
    this->m_socketDesc = socket(AF_INET, SOCK_STREAM, 0); // Create Socket

    spdlog::info("[Core] Starting server on port {}", this->m_port);
    if (this->m_socketDesc < 0) {
        spdlog::error("[Core] Unable to create socket");
        this->m_good = false;
        return;
    }

    spdlog::info("[Core] Socket created");

    //Prepare the sockaddr_in struct
    this->m_server.sin_family = AF_INET;
    this->m_server.sin_addr.s_addr = INADDR_ANY;
    this->m_server.sin_port = htons(3001);

    // Bind entry socket to socket file descriptor
    if(bind(this->m_socketDesc, (struct sockaddr *)&this->m_server, sizeof(this->m_server)) < 0) {
        spdlog::error("[Core] Unable to bind socket");
        this->m_good = false;
        return;
    }

    if(!this->m_moduleManager.isOk()){
        spdlog::warn("[Core] Server started without modules");
    }

    spdlog::info("[Core] Server started on port", this->m_port);
    this->m_good = true;
}

arcs::util::ServerCore::~ServerCore() {
    spdlog::info("[Core] Stopping server");
}

void arcs::util::ServerCore::coreListen() {
    std::unordered_map<std::string, std::shared_ptr<arcs::util::IModule>> * modules = this->m_moduleManager.getLoadedModules();
    listen(this->m_socketDesc, 3); //Listen on socket
    spdlog::info("[Core] Listening for incoming connections");

    // Accept new connections and assign connection to new thread
    this->m_c = sizeof(struct sockaddr_in);
    while((this->m_newSocket = accept(this->m_socketDesc, (struct sockaddr *) &this->m_client, (socklen_t *)&this->m_c)) > 0) {
        // Check to make sure the connection was actually successful
        if (this->m_newSocket < 0) {
            spdlog::warn("[Core] Unable to accept new socket");
            continue;
        }

        this->m_message = strdup("Connection successful");
        write(this->m_newSocket, m_message, strlen(m_message));

        pthread_t sniffer_thread;
        this->m_newSock = (int *) malloc(sizeof(int));
        *this->m_newSock = this->m_newSocket;

        arcs::util::socketData newSockData = {m_newSock, modules};
        // m_newSock
        pthread_create(&sniffer_thread, nullptr, arcs::util::ServerCore::connectionHandler, (void*) &newSockData);
    }

    // Cleanup modules
    this->m_moduleManager.unloadModules();
}

bool arcs::util::ServerCore::isOk() const {
    return this->m_good;
}

void * arcs::util::ServerCore::connectionHandler(void *sockDesc) {
    std::string moduleID;
    std::string retMsg;
    char * moduleMsg;
    char * remainingStr;

    arcs::util::socketData data = * (arcs::util::socketData *)sockDesc; // Recover data struct
    int socket = *data.socket;
    long readSize;
    char buffer[10240];
    char msg[256] = "Session thread assigned";

    spdlog::info("[Core] Hello from session thread! A new client has successfully connected");

    write(socket, msg, strlen(msg));
    while((readSize = recv(socket, buffer, sizeof(buffer), 0)) > 0) {
        if(strlen(buffer) > 2) {
            moduleID = strtok_r(buffer, ":", &remainingStr);
            moduleMsg = remainingStr;

            try {
                // Send data to module
                data.modules->at(moduleID)->process(moduleMsg, retMsg);
                spdlog::info("[\033[4;36m{}\033[0m] {}", moduleID, retMsg);
            } catch (std::out_of_range &e) {
                spdlog::warn("[Core] Received message with request for an unknown module: \"{}\"", moduleID);
            }
        }else{
            spdlog::warn("[Core] Empty message received from client");
            retMsg = "";
        }

        // Response message
        strncpy(msg, retMsg.c_str(), strlen(msg));
        write(socket, msg, strlen(msg));
        memset(buffer, '\0', sizeof(buffer));
    }

    spdlog::info("[Core] Client disconnected");

    if(readSize < 0){
        spdlog::warn("[Core] Failed to read message from client");
    }else{
        fflush(stdout);
    }

    free(data.socket);
    return nullptr;
}