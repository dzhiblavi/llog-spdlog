option(LLOG_BUILD_TESTS "Build tests" ON)

if(LLOG_BUILD_TESTS AND CMAKE_PROJECT_NAME STREQUAL "llog" AND BUILD_TESTING)
    message(STATUS "llog: building tests")

    enable_testing()

    function(add_test path)
        include(CTest)
        include(Catch)
        find_package(Catch2 CONFIG REQUIRED)

        string(REPLACE ".cpp" "" name ${path})
        string(REPLACE "./" "" name ${name})

        add_executable("${name}" "${path}")

        target_link_libraries(${name} PRIVATE Catch2::Catch2WithMain ${ARGN})
        target_include_directories(${name} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>)

        catch_discover_tests(${name} DISCOVERY_MODE PRE_TEST WORKING_DIRECTORY
                             ${CMAKE_CURRENT_SOURCE_DIR})
    endfunction(add_test)
else()
    message(STATUS "llog: not building tests")

    function(add_test path)
        # nothing
    endfunction()
endif()
