//
// Created by Will Snyder on 7/21/24.
//

#include "../../include/SimpleServer/Core/ServerCore.h"

int main(){
    arcs::util::ServerCore core;

    // Check if server core encountered an error during initialization
    if(!core.isOk()){
        spdlog::critical("[Bootstrapper] Unable to initialize server core");
        exit(1);
    }

    core.coreListen(); // Listen for new clients
}