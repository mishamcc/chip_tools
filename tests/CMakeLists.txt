cmake_minimum_required(VERSION 2.8)
if(NOT DEFINED libname)
   set(libname chip_tools)
endif()

set(module_name test)
set(project_name "${libname}_${module_name}")

project(${project_name})

find_package(Boost REQUIRED)
set(include ../include ${Boost_INCLUDE_DIRS})
include_directories(${include})

file(GLOB sources sources/*.cpp)

if(CMAKE_COMPILER_IS_GNUCC)
   add_definitions(-gdwarf-3 -std=c++11 -fPIC)
   if (MINGW)
      add_definitions(-march=i686)
   endif(MINGW)
elseif(MSVC)
   add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS")
endif(CMAKE_COMPILER_IS_GNUCC)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

message("Tests: ")
foreach(test ${sources})
   get_filename_component(test_name ${test} NAME_WE)

   add_executable(${test_name} ${test})
   add_test(NAME ${test_name} COMMAND ${test_name})

   message("- ${test_name}")
endforeach()
