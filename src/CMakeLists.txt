find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost CONFIG REQUIRED )



# Generic test that uses conan libs
add_executable(intro main.cpp Client.h Server.h)
target_link_libraries(
        intro
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11 fmt::fmt spdlog::spdlog ${Boost_LIBRARIES})

target_include_directories(intro PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
