cmake_minimum_required(VERSION 3.16)
project(Cpp20ModulesExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PREBUILT_MODULE_PATH ${CMAKE_BINARY_DIR}/modules)

set(SYSTEM_MODULES iostream)

function(add_module name)
    file(MAKE_DIRECTORY ${PREBUILT_MODULE_PATH})
          if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            # using Clang
            add_custom_target(${name}.pcm
                    COMMAND
                        ${CMAKE_CXX_COMPILER}
                        -std=c++20
                        -c
                        ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
                        -Xclang -emit-module-interface
                        -o ${PREBUILT_MODULE_PATH}/${name}.pcm
                    )
          elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            # using GCC
            add_custom_target(${name}.pcm
                    COMMAND
                        ${CMAKE_CXX_COMPILER}
                        -std=c++20
                        -fmodules-ts
                        -c
                        ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
                        -o ${name}.pcm
                    )                

#            foreach(sys_module ${SYSTEM_MODULES})
#                file(GLOB_RECURSE fullpath "${sys_module}*" ${CMAKE_CURRENT_BINARY_DIR} )
#                if(NOT EXISTS ${fullpath})
#                    message("Creating the system module ${sys_module}")
#                    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -std=c++20 -fmodules-ts -xc++-system-header ${sys_module})
#                else()
#                    message("Found system module ${sys_module}")
#                endif()
#            endforeach()

          elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
            # using Visual Studio C++
            add_custom_target(${name}.obj
                    COMMAND
                        ${CMAKE_CXX_COMPILER} /experimental:module
                        /c
                        ${CMAKE_CURRENT_SOURCE_DIR}/${ARGN}
                    )
          endif()
endfunction()


set(MODULE_SRCS helloworld.cpp)
set(MODULES helloworld)

add_custom_target(main_modules SOURCES ${MODULE_SRCS})

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # Using Clang
  add_compile_options(-fprebuilt-module-path=${PREBUILT_MODULE_PATH})

  add_module(helloworld ${MODULE_SRCS})
  add_executable(mainExe main.cpp ${MODULE_SRCS})

  add_dependencies(main ${MODULES}.pcm)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # Using GCC
  add_compile_options(-fmodules-ts)
  add_module(helloworld ${MODULE_SRCS})
  add_executable(main main.cpp)

  add_custom_target(module ${MODULE_SRCS})

  target_link_options(main PUBLIC "LINKER:${MODULES}.pcm")
  add_dependencies(main helloworld.pcm)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Using Visual Studio C++
  add_compile_options(/experimental:module /c)

  add_module(helloworld helloworld.cpp)
  add_executable(mainExe main.cpp)


  add_dependencies(main helloworld.pcm)

endif()

