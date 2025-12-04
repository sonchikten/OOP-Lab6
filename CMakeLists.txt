cmake_minimum_required(VERSION 3.16)
project(OOP_labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  

find_package(GTest REQUIRED)
include(GoogleTest)
add_compile_options(-Wno-psabi)

include_directories(include)

add_executable(lab6 
    main.cpp 
    src/bandit.cpp
    src/elf.cpp
    src/knight.cpp
    src/battle_visitor.cpp
    src/factory.cpp
    src/fight.cpp
    src/npc.cpp
)

add_executable(test_lab6 
    tests/lab6_test.cpp  
    src/bandit.cpp
    src/elf.cpp
    src/knight.cpp
    src/battle_visitor.cpp
    src/factory.cpp
    src/fight.cpp
    src/npc.cpp
)
target_link_libraries(test_lab6 GTest::gtest_main GTest::gtest)

gtest_discover_tests(test_lab6)