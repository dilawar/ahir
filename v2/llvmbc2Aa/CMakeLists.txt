cmake_minimum_required(VERSION 2.8)

# Install llvm-2.8
set(LLVM_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/_llvm28)
include(ExternalProject)
ExternalProject_Add(llvm28
    PREFIX llvm28
    URL ${CMAKE_CURRENT_SOURCE_DIR}/external/llvm-2.8.tgz
    PATCH_COMMAND patch
        <SOURCE_DIR>/lib/ExecutionEngine/JIT/Intercept.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/external/llvm.patch
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${LLVM_INSTALL_DIR} -Wno-dev
        -DLLVM_TARGETS_TO_BUILD=X86;ARM
    BUILD_COMMAND $(MAKE) -j4
    )

set(LLVM_CONFIG_BIN ${LLVM_INSTALL_DIR}/bin/llvm-config)
include_directories(${LLVM_INSTALL_DIR}/include)

execute_process(COMMAND
    ${LLVM_CONFIG_BIN} --cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

execute_process(COMMAND
    ${LLVM_CONFIG_BIN} --libfiles core bitreader bitwriter instrumentation scalaropts ipo
    OUTPUT_VARIABLE LLVM_STATIC_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
separate_arguments(LLVM_STATIC_LIBS)

message(STATUS "llvm compiler flags: ${LLVM_CXX_FLAGS}")
message(STATUS "llvm libraries to link:")
foreach(_llvm_lib ${LLVM_STATIC_LIBS})
    message(STATUS "   ${_llvm_lib}")
endforeach()

# Build docs.
find_program(PDFLATEX NAMES pdflatex lualatex xelatex)
if(PDFLATEX)
    set(TEX_SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/docs/llvm2aa.tex)
    set(PDF_FILE ${CMAKE_CURRENT_SOURCE_DIR}/llvm2aa.pdf)
    add_custom_command(OUTPUT ${PDF_FILE}
        COMMAND ${PDFLATEX} ${TEX_SOURCE_FILE}
        COMMENT "Generating PDF from TeX file"
        )
    add_custom_target(pdfdoc ALL DEPENDS ${PDF_FILE})
else(PDFLATEX)
    message(STATUS "pdflatex not found. No pdf will be generated.")
endif(PDFLATEX)

# BUILD llvm2aa
find_package(Threads REQUIRED)
include_directories( 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/../BGLWrap/include 
    )

file(GLOB SRCS src/*.cpp)
add_executable(llvm2aa ${SRCS})
set_target_properties(llvm2aa PROPERTIES COMPILE_FLAGS ${LLVM_CXX_FLAGS})

# Build llvm2.8 before building this target.
add_dependencies(llvm2aa llvm28)

# foreach(llvm_static_lib ${LLVM_STATIC_LIBS})
    # message(STATUS "xx ${llvm_static_lib}")
# endforeach()
target_link_libraries(llvm2aa ${LLVM_STATIC_LIBS})
target_link_libraries(llvm2aa ${CMAKE_THREAD_LIBS_INIT})

# install 
install(TARGETS llvm2aa
    RUNTIME DESTINATION bin
    )

if(EXISTS ${PDF_FILE})
    install(FILES ${PDF_FILE} DESTINATION share/doc/ahir/pdf/)
endif()
