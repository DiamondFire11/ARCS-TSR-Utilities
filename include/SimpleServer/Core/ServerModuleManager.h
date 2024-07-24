#pragma once

#include "../../Modules/IModule.h"
#include "../LibraryLoader/SOLoader.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <sys/stat.h>

#include <spdlog.h>

// Config file include
#include <yaml-cpp/yaml.h>


namespace arcs::util {
    class ServerModuleManager {
    public:
        ServerModuleManager();
        bool isOk() const;
        std::unordered_map<std::string, std::shared_ptr<arcs::util::IModule>> * getLoadedModules();
        void unloadModules();

        void printLoadedModules(); // Implement later

    private:
        YAML::Node m_moduleConfig;
        std::unordered_map<std::string, arcs::util::SOLoader<arcs::util::IModule>> m_soLoaders;
        std::unordered_map<std::string, std::shared_ptr<arcs::util::IModule>> m_modules;
        bool m_moduleConfigGood;

        void m_loadModules();
        void m_loadModule(std::string &, std::string &);
    };

    static const std::string CONFIG_PATH = "/etc/arcs/simple_server/modules.yaml";
}