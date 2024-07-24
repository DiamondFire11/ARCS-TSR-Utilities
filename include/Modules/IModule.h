#pragma once

#include <string>

namespace arcs::util {
    struct ModuleIdentifier {
        std::string moduleID;
        std::string moduleDesc;
        std::string moduleVersion;
    };

    class IModule {
    public:
        IModule(std::string, std::string, std::string);
        virtual ~IModule() = default;
        virtual void process(const char * msg, std::string &retMsg) = 0;

        [[nodiscard]] std::string getModuleID() const;
        [[nodiscard]] std::string getModuleDesc() const;
        [[nodiscard]] std::string getModuleVersion() const;

    private:
        arcs::util::ModuleIdentifier m_moduleInfo;
    };
}
