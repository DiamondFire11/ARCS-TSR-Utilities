//
// Created by Will Snyder on 7/21/24.
//

#include <utility>

#include "../../include/Modules/IModule.h"

arcs::util::IModule::IModule(std::string moduleID, std::string moduleDesc, std::string moduleVersion) {
    this->m_moduleInfo.moduleID = std::move(moduleID);
    this->m_moduleInfo.moduleDesc = std::move(moduleDesc);
    this->m_moduleInfo.moduleVersion = std::move(moduleVersion);
}

std::string arcs::util::IModule::getModuleID() const {
    return this->m_moduleInfo.moduleID;
}

std::string arcs::util::IModule::getModuleDesc() const {
    return this->m_moduleInfo.moduleDesc;
}

std::string arcs::util::IModule::getModuleVersion() const {
    return this->m_moduleInfo.moduleVersion;
}
