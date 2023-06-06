
cmake_minimum_required(VERSION 3.14)

# ---- Declare library ----
add_library(
    qtgraph_lib OBJECT
    lib.hpp
    lib.cpp
)

target_link_libraries(qtgraph_lib PRIVATE 
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL 
)
target_compile_features(qtgraph_lib PUBLIC cxx_std_17)

# ---- Declare executable ----
add_executable(qtgraph_exe main.cpp)
add_executable(qtgraph::exe ALIAS qtgraph_exe)

set_property(TARGET qtgraph_exe PROPERTY OUTPUT_NAME qtgraph)

target_compile_features(qtgraph_exe PRIVATE cxx_std_17)

target_link_libraries(qtgraph_exe PRIVATE 
    qtgraph_lib Qt5::Widgets Qt5::Gui Qt5::Core Qt5::OpenGL)


