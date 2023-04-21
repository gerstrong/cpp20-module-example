cmake_minimum_required(VERSION 3.16)
project(cpp20-templates-modules LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

include(cpp20modules.cmake)

#set(SYSTEM_MODULES iostream)

set(MODULE_SRCS mathTemplate.cpp)

# Create a list of only names so we add the extension for module and source respectively
foreach(item ${MODULE_SRCS})
    string(REPLACE ".cpp" "" new_item ${item})
    list(APPEND MODULENAMES ${new_item})
endforeach()

add_custom_target(main_modules SOURCES ${MODULE_SRCS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # Using Clang
  add_compile_options(-fprebuilt-module-path=${PREBUILT_MODULE_PATH})

  add_executable(mainExe main.cpp ${MODULE_SRCS})

  foreach(THE_MODULE ${MODULENAMES})
    add_module(${THE_MODULE} ${THE_MODULE}.cpp)
    add_dependencies(main ${THE_MODULE}.pcm)
  endforeach()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Using GCC
  add_compile_options(-fmodules-ts)

  add_executable(main main.cpp)

  foreach(THE_MODULE ${MODULENAMES})
    add_module(${THE_MODULE} ${THE_MODULE}.cpp)
    target_link_options(main PUBLIC "LINKER:${THE_MODULE}.pcm")
    add_dependencies(main ${THE_MODULE}.pcm)
  endforeach()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Using Visual Studio C++
  add_compile_options(/experimental:module /c)

  add_executable(main main.cpp)

  foreach(THE_MODULE ${MODULENAMES})
    add_module(${THE_MODULE} ${THE_MODULE}.cpp)
    add_dependencies(main ${THE_MODULE}.pcm)
  endforeach()

endif()

