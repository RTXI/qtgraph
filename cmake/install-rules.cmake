install(
    TARGETS qtgraph_exe
    RUNTIME COMPONENT qtgraph_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
