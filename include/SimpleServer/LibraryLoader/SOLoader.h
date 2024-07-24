#pragma once

#include <iostream>
#include <dlfcn.h> // Dynamic Library header

#include <memory>
#include <string>
#include <utility>

namespace arcs::util{
    /*
     * Code to load Dynmic Libraries in Linux (.so files)
     * API strictly for UNIX
     * Intended for use in UC Riverside's ARCS Lab.
     *
     * Handles opening, closing, and validity of shared (.so) libraries
     *
     * Adapted from Medium.com tutorial by Theo Penavaire
     * https://theo-penavaire.medium.com/loading-of-a-c-class-from-a-shared-library-modern-c-722d6a830a2b
     */

    template <class T>
    class SOLoader {
    public:
        explicit SOLoader(std::string libraryPath,
                 std::string allocClassSymbol = "allocator",
                 std::string deleteClassSymbol = "deleter") :
                 m_handle(nullptr),
                 m_libraryPath(std::move(libraryPath)),
                 m_allocClassSymbol(std::move(allocClassSymbol)),
                 m_deleteClassSymbol(std::move(deleteClassSymbol)) {};

        ~SOLoader() = default;

        void openLibrary() {
            if (!(m_handle = dlopen(m_libraryPath.c_str(), RTLD_NOW | RTLD_LAZY))) {
                std::cerr << dlerror() << std::endl;
            }
        }

        void closeLibrary() {
            if (dlclose(m_handle) != 0){
                std::cerr << dlerror() << std::endl;
            }
        }

        std::shared_ptr<T> getClassInstance() {
            using allocClass = T *(*)();
            using deleteClass = void (*)(T *);

            auto allocFunc = reinterpret_cast<allocClass>(dlsym(m_handle, m_allocClassSymbol.c_str()));
            auto deleteFunc = reinterpret_cast<deleteClass>(dlsym(m_handle, m_deleteClassSymbol.c_str()));

            if (!allocFunc || !deleteFunc){
                closeLibrary();
                std::cerr << dlerror() << std::endl;
            }

            return std::shared_ptr<T>(allocFunc(), [deleteFunc](T *p){deleteFunc(p);});
        }

    private:
        void * m_handle;
        std::string m_libraryPath;
        std::string m_allocClassSymbol;
        std::string m_deleteClassSymbol;
    };
}