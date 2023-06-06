cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    qtgraph
    VERSION 0.1.0
    DESCRIPTION "testing graphics opengl within qt context"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
set(CMAKE_COMPILE_COMMANDS ON)

# ---- find libraries ----
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(source)
# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT qtgraph_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of qtgraph"
  )
endif()

include(cmake/dev-mode.cmake)
