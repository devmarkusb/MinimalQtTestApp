include(${TOO_CMAKE_INC_GOOGLETEST_AS_SUBDIR})

# no ToolsFL/tooBuildEnv here (little part of it is needed much earlier for the build number mechanism)
add_subdirectory(tfl)
add_subdirectory(toolib)
set(uiwrap_USE_IMPLEMENTATION       "qt" CACHE STRING "Choose the desired UI implementation out of: qt (default), wx, ... or own")
include (${TOO_CMAKE_INC_UIWRAP_AS_SUBDIR})
