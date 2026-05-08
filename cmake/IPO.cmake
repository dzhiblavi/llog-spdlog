include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)

option(LLOG_BUILD_IPO "Build with IPO" OFF)

if (LLOG_BUILD_IPO)
    message(STATUS "Enabling the IPO (LTO)")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "Not enabling the IPO (LTO)")
endif()
