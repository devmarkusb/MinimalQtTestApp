add_subdirectory(util)
add_subdirectory(toolib)
set(uiwrap_USE_IMPLEMENTATION       "qt" CACHE STRING "Choose the desired UI implementation out of: qt (default), wx, ... or own")
ul_include(uiwrap.cmake)
