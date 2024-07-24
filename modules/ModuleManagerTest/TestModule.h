#pragma once

#include "../../include/Modules/IModule.h"

const char * id = "ModuleManager Test";
const char * desc = "If this module is installed and this text is missing then ModuleManager is broken or missing.";
const char * version = "1.0";

class TestModule : private arcs::util::IModule{
public:
    TestModule();
    ~TestModule() override = default;

    void process(const char *, std::string &) override;
};
