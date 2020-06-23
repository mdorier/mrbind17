cmake_minimum_required (VERSION 3.1)
project (mrbind17 CXX)

set (CMAKE_CXX_STANDARD 17)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
     "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


add_definitions(-g)
option(ENABLE_TESTS "Build tests. May require CppUnit_ROOT" OFF)

option(ENABLE_COVERAGE "Enable code coverage." OFF)
if (ENABLE_COVERAGE)
	add_compile_options(--coverage -O0)
endif ()

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package (Mruby REQUIRED)
include_directories (${Mruby_INCLUDE_DIR})

find_package (CppUnit)
if (CPPUNIT_FOUND)
    message(STATUS "CppUnit found, unit tests will be compiled")
    message(STATUS "CPPUNIT_INCLUDE_DIR : ${CPPUNIT_INCLUDE_DIR}")
    message(STATUS "CPPUNIT_LIBRARIES : ${CPPUNIT_LIBRARIES}")
    include_directories(${CPPUNIT_INCLUDE_DIR})
    enable_testing()
    if(${ENABLE_TESTS})
        add_subdirectory (test)
    endif(${ENABLE_TESTS})
else (CPPUNIT_FOUND)
    message(STATUS "CppUnit not found, unit tests will not be compiled")
endif (CPPUNIT_FOUND)

install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/mrbind17
         DESTINATION include
         FILES_MATCHING PATTERN "*.hpp")
