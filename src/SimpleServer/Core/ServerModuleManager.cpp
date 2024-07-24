//
// Created by Will Snyder on 7/21/24.
//

#include "../../../include/SimpleServer/Core/ServerModuleManager.h"

arcs::util::ServerModuleManager::ServerModuleManager() {
    try{
        this->m_moduleConfig = YAML::LoadFile(arcs::util::CONFIG_PATH);
    }catch (YAML::BadFile &e){
        spdlog::error("[ModuleManager] Module config file not found");
        this->m_moduleConfigGood = false;
        return;
    }catch (YAML::ParserException &e){
        spdlog::error("[ModuleManager] Illegal syntax encountered while parsing config file");
        this->m_moduleConfigGood = false;
        return;
    }

    this->m_moduleConfigGood = true;
    spdlog::info("[ModuleManager] Module config loaded successfully");
    spdlog::info("[ModuleManager] Initializing modules");
    this->m_loadModules();
}

bool arcs::util::ServerModuleManager::isOk() const {
    return this->m_moduleConfigGood;
}

void arcs::util::ServerModuleManager::m_loadModules() {
    YAML::Node modulesList = this->m_moduleConfig["modules"];
    std::string moduleID;
    std::string modulePath;
    for(YAML::const_iterator it = modulesList.begin(); it != modulesList.end(); ++it){
        moduleID = it->first.as<std::string>();
        modulePath = it->second.as<std::string>();

        m_loadModule(moduleID, modulePath);
    }
}

void arcs::util::ServerModuleManager::m_loadModule(std::string & moduleID, std::string & modulePath) {
    struct stat buffer{};
    if(stat(modulePath.c_str(), &buffer) != 0){
        spdlog::warn("[ModuleManager] Library for module \"{}\" could not be found", moduleID);
        return;
    }

    this->m_soLoaders.emplace(moduleID, arcs::util::SOLoader<arcs::util::IModule>(modulePath)); // Instantiate Library Loader
    this->m_soLoaders.at(moduleID).openLibrary(); // Open Library

    // Instantiate an instance of the module class
    this->m_modules.emplace(moduleID, std::shared_ptr<arcs::util::IModule>(this->m_soLoaders.at(moduleID).getClassInstance()));

    spdlog::info("[ModuleManager] Loaded Module: {} v{} - {}",
                 this->m_modules.at(moduleID)->getModuleID(),
                 this->m_modules.at(moduleID)->getModuleVersion(),
                 this->m_modules.at(moduleID)->getModuleDesc());
}

void arcs::util::ServerModuleManager::unloadModules() {
    for(auto & [moduleID, module] : this->m_soLoaders){
        std::string moduleName = this->m_modules.at(moduleID)->getModuleID();
        this->m_modules.at(moduleID).reset(); // Release the module shared_ptr and destruct module
        module.closeLibrary(); // Close library

        spdlog::info("[ModuleManager] Unloaded module: {}", moduleName);
    }
}

std::unordered_map<std::string, std::shared_ptr<arcs::util::IModule>> *
arcs::util::ServerModuleManager::getLoadedModules() {
    return &(this->m_modules);
}
