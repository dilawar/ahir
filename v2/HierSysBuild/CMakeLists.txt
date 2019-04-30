cmake_minimum_required(VERSION 2.8)
find_package(Boost COMPONENTS system filesystem REQUIRED)

# Grammar.
include(${CMAKE_CURRENT_SOURCE_DIR}/../common.cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake)

# ANTLR path.
if(NOT PRIVATE_ANTLR)
    find_package(Antlr COMPONENTS CPP REQUIRED)
    ADD_ANTLR_GRAMMAR(grammar/hierSys.g GRAMMER_OUTPUT_FILES)
    message(STATUS "Output files: ${GRAMMER_OUTPUT_FILES}")
    set(ANTLR_LIB ${Antlr_LIBRARIES})
    set(PARSER_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/grammar)
else(PRIVATE_ANTLR)
    set(ANTLR_HOME ${CMAKE_CURRENT_SOURCE_DIR}/../../antlr2/${PKGARCH})
    message(STATUS "ANTRL HOME : ${ANTLR_HOME}")
endif()

if(PRIVATE_ANTLR)
    message(STATUS "Using private antlr")
    find_file(ANTLR_JAR_FILE NAMES antlr.jar 
        PATHS ${ANTLR_HOME}
        PATH_SUFFIXES jar 
        NO_DEFAULT_PATH)
    find_library(ANTLR_LIB NAMES libantlr.a 
        PATHS ${ANTLR_HOME}
        PATH_SUFFIXES lib 
        NO_DEFAULT_PATH)

    # Generate grammar.
    set(GRAMMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/grammar/hierSys.g)
    set(GRAMMER_OUTPUT_FILES hierSysParser.hpp hierSysParser.cpp hierSysLexer.hpp hierSysLexer.cpp)
    set(GRAMMER_DOC_FILES hierSysLexer.html hierSysParser.html)

    add_custom_command(OUTPUT ${GRAMMER_OUTPUT_FILES}
        COMMAND java -cp ${ANTLR_JAR_FILE} antlr.Tool -o ${CMAKE_CURRENT_SOURCE_DIR}/grammar ${GRAMMAR_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating parser/lexer"
        )
    add_custom_target(Antlr DEPENDS ${GRAMMER_OUTPUT_FILES})

    # Generate doc
    add_custom_command(OUTPUT ${GRAMMER_DOC_FILES}
        COMMAND java -cp ${ANTLR_JAR_FILE} antlr.Tool -html ${GRAMMAR_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation (html)"
        )
    add_custom_target(AntlrDocs DEPENDS ${GRAMMER_DOC_FILES})

    message(STATUS "ANTRL JAR FILE ${ANTLR_JAR_FILE}")
    message(STATUS "ANTRL_LIB  ${ANTLR_LIB}")

    if(NOT ANTLR_JAR_FILE OR NOT ANTLR_LIB)
        message(FATAL_ERROR "Could not find antrl jar/libantlr.a .")
    endif()
endif(PRIVATE_ANTLR)

# Build lexer and parser.
add_library(parser ${GRAMMER_OUTPUT_FILES})
set_source_files_properties(${GRAMMER_OUTPUT_FILES} PROPERTIES GENERATED TRUE)

if(PRIVATE_ANTLR)
    add_dependencies(parser Antlr)
endif(PRIVATE_ANTLR)

include_directories(
    ${PARSER_INCLUDE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Value/include
    ${Boost_INCLUDE_DIRS}
)

file(GLOB SRCS src/*.cpp)
add_library(HierSys SHARED ${SRCS})
target_link_libraries(HierSys ${ANTLR_LIB})
add_dependencies(HierSys parser)

# These executable require libValue which is outside this project. To build
# these targets, call top-level cmake file.
add_executable(hierSys2Vhdl util/hierSys2Vhdl.cpp)
target_link_libraries(hierSys2Vhdl HierSys parser Value ${Boost_LIBRARIES})

add_executable(hierSys2C util/hierSys2C.cpp)
target_link_libraries(hierSys2C HierSys parser Value)

add_executable(hierSysCheck util/hierSysCheck.cpp)
target_include_directories(hierSysCheck PRIVATE grammar)
target_link_libraries(hierSysCheck HierSys parser Value)

add_executable(hierSysUniquify util/hierSysUniquify.cpp)
target_include_directories(hierSysUniquify PRIVATE src)
target_link_libraries(hierSysUniquify HierSys Value parser ${ANTLR_LIB})

# install
install(
    TARGETS HierSys hierSys2Vhdl hierSys2C hierSysCheck hierSysUniquify
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )

install(FILES hierSysParser.html hierSysLexer.html
    DESTINATION share/doc/ahir/
    )


