cmake_minimum_required(VERSION 3.14)

project(
    qtgraph
    VERSION 0.1.0
    DESCRIPTION "testing graphics opengl within qt context"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

# These lines help with third-party tooling integration
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

# ---- find libraries ----
find_package(rtxi REQUIRED HINTS "../../../build/test")
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Boost REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(
    qtgraph MODULE
    qtgraph.cpp
    qtgraph.hpp
)

target_link_libraries(qtgraph PUBLIC 
    rtxi::rtxi rtxi::xfifo Qt5::Core Qt5::Gui Qt5::Widgets 
    Boost::headers backtrace dl 
)

# We need to tell cmake to use the c++ version used to compile the dependent library or else...
get_target_property(REQUIRED_COMPILE_FEATURE rtxi::rtxi INTERFACE_COMPILE_FEATURES)
target_compile_features(qtgraph PRIVATE ${REQUIRED_COMPILE_FEATURE})

target_compile_definitions(qtgraph PRIVATE BOOST_STACKTRACE_USE_ADDR2LINE)
target_compile_definitions(qtgraph PRIVATE BOOST_STACKTRACE_USE_BACKTRACE)
