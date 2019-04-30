cmake_minimum_required(VERSION 2.8)
project(ahir-v2)
include(common.cmake)

# Build dependencies.
find_package(Boost 1.58 REQUIRED)
include_directories( ${Boost_INCLUDE_DIR})

add_subdirectory(Value)
add_subdirectory(iolib)
add_subdirectory(llvmbc2Aa)
add_subdirectory(libAhirV2)
add_subdirectory(Ahir-asm/devel)

add_subdirectory(CtestBench)
add_subdirectory(pipeHandler)
add_subdirectory(functionLibrary)
add_subdirectory(BitVectors)
add_subdirectory(HierSysBuild)
add_subdirectory(rtl2AaMatcher)

# Following two only have headers.
add_subdirectory(pthreadUtils)
add_subdirectory(GnuPthUtils)

install(PROGRAMS 
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/ghdlSanitize.sh 
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/formatVhdlFiles.py 
    ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/gdb_commands.txt  # FIXME: ??
    DESTINATION bin
    )

enable_testing()



