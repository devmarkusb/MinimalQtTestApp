include(${TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR})

# no ToolsFL/tooBuildEnv here (little part of it is needed much earlier for the build number mechanism)
add_subdirectory(ToolsFL/tooBasicCodeSupport)
add_subdirectory(ToolsFL/tooLinklib)
add_subdirectory(ToolsFL/tooMem)
add_subdirectory(ToolsFL/tooString)

add_subdirectory(Toolib)
set(uiwrap_USE_IMPLEMENTATION       "qt" CACHE STRING "Choose the desired UI implementation out of: qt (default), wx, ... or own")
add_subdirectory(uiwrap)
