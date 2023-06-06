# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(qtgraphTests LANGUAGES CXX)

# ---- Tests ----

add_executable(qtgraph_test source/qtgraph_test.cpp)
target_link_libraries(qtgraph_test PRIVATE qtgraph_lib)
target_compile_features(qtgraph_test PRIVATE cxx_std_17)

add_test(NAME qtgraph_test COMMAND qtgraph_test)

# ---- End-of-file commands ----

add_folders(Test)
