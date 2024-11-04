# WebScraperCPP
A Simple WebScraper in C++23 with multithreading capabilities

# Requirements for building
* GCC 13.1 for(C++23 capabilities)

* Conan 2

* CMake 3.22

# How to setup the environment
    conan profile detect
    conan install . --build=missing
    cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -S$(pwd) -B$(pwd)/build/Release -G "Unix Makefiles"
    cmake --build $(pwd)/build/Release --parallel $(nproc) --target WebScraper --


# How to execute the program
Just execute the WebScraper executable on build/Release