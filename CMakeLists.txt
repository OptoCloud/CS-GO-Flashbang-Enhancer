cmake_minimum_required(VERSION 3.5)

set (project CSGO-Flashbang-Enhancer)
project (
    ${project}
    DESCRIPTION "Actually blinds you when you get flash banged in-game"
    LANGUAGES CXX
)

option(VERBOSE "Verbose compiler output" OFF)

# --- Configure default build ---
set (default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message (STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property (CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif ()
# --------------------------------

#if (NOT WIN32)
#    message(FATAL_ERROR "Not supported on other systems than windows")
#endif ()

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package (Threads REQUIRED)

add_executable (${project}
    main.cpp
    gameevent.h
    gameevent.cpp
    gameeventmanager.h
    gameeventmanager.cpp
    gameeventlistener.h
    utils.h
    flashbanglistener.h
    flashbanglistener.cpp
#    resources.rc
)

target_link_libraries (${project} PRIVATE
    Threads::Threads
    ${CMAKE_DL_LIBS}
    ws2_32
    winmm
    Rpcrt4
    Iphlpapi
)
