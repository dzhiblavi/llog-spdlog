find_package(spdlog REQUIRED)

add_library(llog STATIC)
add_library(llog::llog ALIAS llog)

target_sources(llog PRIVATE ./repository.cpp ./load.cpp)
target_link_libraries(llog PUBLIC spdlog::spdlog)
target_include_directories(llog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
