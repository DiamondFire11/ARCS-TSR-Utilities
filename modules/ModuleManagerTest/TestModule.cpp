//
// Created by Will Snyder on 7/20/24.
//

#include "TestModule.h"

/*
 * This code is critical to the module interface.
 *
 * If this is missing then the module and server will crash with a segmentation fault.
 * The inclusion of this interface is solely the responsibility of the module programmer.
 */
extern "C" {
    TestModule * allocator(){
        return new TestModule();
    }

    void deleter(TestModule * module){
        delete module;
    }
}

TestModule::TestModule() : IModule(id, desc, version) {}

void TestModule::process(const char * msg, std::string &retMsg) {
    retMsg = msg;
}
