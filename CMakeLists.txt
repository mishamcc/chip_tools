cmake_minimum_required(VERSION 2.8)

set(libname chip_tools)

#The library options
option(OPT_USE_UCF "UCF Tools utilities" OFF)
option(OPT_TESTS "Unit tests" OFF)

if(OPT_USE_UCF)
    message("UCF Tools has been included")
    add_subdirectory(sources/ucf_tools_impl)
endif()

if (OPT_TESTS)
   message("Tests has been included")
   enable_testing()
   add_subdirectory(tests)
endif()
