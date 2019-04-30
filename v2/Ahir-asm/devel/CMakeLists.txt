cmake_minimum_required(VERSION 2.8)
project(ahir-asm)
include( ../../common.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)

find_package(Antlr COMPONENTS CPP REQUIRED)
message(STATUS "Antlr library: ${Antlr_LIBRARIES}")
ADD_ANTLR_GRAMMAR(grammar/Aa.g AA_GRAMMER_OUTPUT_FILES)

# Add aa library.
file(GLOB AA_SRC src/*.cpp)
include_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Value/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../CtestBench/include
    )

# Build lexer and parser.
add_library(aa_parser STATIC ${AA_GRAMMER_OUTPUT_FILES})
set_target_properties(aa_parser PROPERTIES COMPILE_FLAGS -fPIC)
set_source_files_properties(${AA_GRAMMER_OUTPUT_FILES} PROPERTIES GENERATED TRUE)

add_library(Aa SHARED ${AA_SRC})
target_link_libraries(Aa aa_parser)

add_executable(AaOpt util/AaOpt.cpp)
target_link_libraries(AaOpt Aa Value ${Antlr_LIBRARIES})

add_executable(Aa2VC util/Aa2VC.cpp)
target_link_libraries(Aa2VC Aa Value ${Antlr_LIBRARIES})

add_executable(Aa2C util/Aa2C.cpp)
target_link_libraries(Aa2C Aa Value ${Antlr_LIBRARIES})

add_executable(AaLinkExtMem util/AaLinkExtMem.cpp)
target_link_libraries(AaLinkExtMem Aa Value ${Antlr_LIBRARIES})

# INSTALL 
install( 
    TARGETS Aa AaOpt Aa2VC Aa2C AaLinkExtMem 
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )
