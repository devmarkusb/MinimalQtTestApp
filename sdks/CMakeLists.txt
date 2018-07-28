too_include(googletest_as_subdir.cmake)

# no ToolsFL/tooBuildEnv here (little part of it is needed much earlier for the build number mechanism)
add_subdirectory(tfl)
add_subdirectory(toolib)
set(uiwrap_USE_IMPLEMENTATION       "qt" CACHE STRING "Choose the desired UI implementation out of: qt (default), wx, ... or own")
too_include(uiwrap.cmake)
