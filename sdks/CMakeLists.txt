include(${TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR})

# no ToolsFL/tooBuildEnv here (little part of it is needed much earlier for the build number mechanism)
add_subdirectory(ToolsFL/tooBasicCodeSupport)
add_subdirectory(ToolsFL/tooString)
