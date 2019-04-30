cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)
find_package(Antlr COMPONENTS CPP REQUIRED)
message(STATUS "Antlr library: ${Antlr_LIBRARIES}")

ADD_ANTLR_GRAMMAR(grammar/vc.g VC_GRAMMER_OUTPUT_FILES)
add_library(vc_parser STATIC ${VC_GRAMMER_OUTPUT_FILES})
set_target_properties(vc_parser PROPERTIES COMPILE_FLAGS -fPIC)
set_source_files_properties(${VC_GRAMMER_OUTPUT_FILES} PROPERTIES GENERATED TRUE)

file(GLOB LIBVC_SRC src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../BGLWrap/include)

add_library(VC SHARED ${LIBVC_SRC})
add_dependencies(VC vc_parser)
target_link_libraries(VC vc_parser)

add_executable(vcParse util/vcParseMain.cpp)
target_link_libraries(vcParse VC ${Antlr_LIBRARIES})

add_executable(vcAnalyze util/vcAnalyze.cpp)
target_link_libraries(vcAnalyze VC ${Antlr_LIBRARIES})

add_executable(vcFormat util/vcFormat.cpp)
target_link_libraries(vcFormat VC ${Antlr_LIBRARIES})

add_executable(vhdlFormat util/vhdlFormat.cpp)
target_link_libraries(vhdlFormat VC ${Antlr_LIBRARIES})

add_executable(vc2vhdl util/vc2vhdl.cpp)
target_link_libraries(vc2vhdl VC ${Antlr_LIBRARIES})

# INSTALL
install(TARGETS VC vcParse vcAnalyze vcFormat vhdlFormat vc2vhdl
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    )
