# Determine the architecture.

if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    MESSAGE(STATUS "64 bit architecture") 
    set(PKGARCH "x86_64") 
else() 
    MESSAGE(STATUS "32 bit architecture") 
    set(PKGARCH "i386")
endif()



