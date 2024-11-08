cmake_minimum_required(VERSION 3.22.0)
project(WebScraper VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

find_package(beauty REQUIRED)
find_package(lexbor REQUIRED)

set(WebScraper_SRCS ${CMAKE_SOURCE_DIR}/src/WebScraper.cpp
                    ${CMAKE_SOURCE_DIR}/src/ScrapedPage.cpp)
add_executable(WebScraper main.cpp ${WebScraper_SRCS})

target_compile_options(WebScraper PRIVATE "-DBENCHMARK=ON") # So we can test the performance
target_include_directories(WebScraper PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(WebScraper PUBLIC beauty::beauty lexbor::lexbor_static)