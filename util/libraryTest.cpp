#include "../include/Modules/IModule.h"
#include "../../include/SimpleServer/LibraryLoader/SOLoader.h"

#include <string>
#include <memory>
#include <spdlog.h>

void testLoader(const std::string &path){
    // Declare loader and module objects
    arcs::util::SOLoader<arcs::util::IModule> soLoader(path);
    std::shared_ptr<arcs::util::IModule> module;

    // Open library
    soLoader.openLibrary();

    // Get util module instance and store as a shared pointer
    module = soLoader.getClassInstance();

    std::string retMsg;
    std::string moduleID = module->getModuleID();
    module->process("", retMsg); // Test process function

    spdlog::info("Loaded Module: {}", moduleID);
    spdlog::info("Module Description: {}", module->getModuleDesc());
    spdlog::info(retMsg);

    module.reset(); // (1) reset smart pointer
    soLoader.closeLibrary(); // (2) close library
    spdlog::info("{} unloaded", moduleID);
}

int main(){
    testLoader("/home/willsnyder/CLionProjects/ARCS-Utilities/libs/libTestModule.so");
    return 0;
}