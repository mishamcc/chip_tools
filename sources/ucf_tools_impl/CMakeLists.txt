cmake_minimum_required(VERSION 2.8)
if(NOT DEFINED libname)
   set(libname chip_tools)
endif()

set(module_name ucf)
set(project_name "${libname}_${module_name}")

project(${project_name})

set(include ../../include)
include_directories(${include})

file(GLOB sources sources/*.cpp)

message("${project_name} sources: ")
foreach(src in ${sources})
   get_filename_component(filename ${src} NAME)
   message(${filename})
endforeach()

if(CMAKE_COMPILER_IS_GNUCC)
   add_definitions(-gdwarf-3 -std=c++11 -fPIC)
   if (MINGW)
      add_definitions(-march=i686)
   endif(MINGW)
elseif(MSVC)
   add_definitions("-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS")
endif(CMAKE_COMPILER_IS_GNUCC)

add_library(${project_name} ${sources})
