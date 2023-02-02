find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost CONFIG REQUIRED )



# client application
add_executable(client MainClient.cpp Client.h)
target_link_libraries(
        client
  PUBLIC project_options project_warnings
  PRIVATE CLI11::CLI11 spdlog::spdlog ${Boost_LIBRARIES})

target_include_directories(client PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

# server application

add_executable(server MainServer.cpp Server.h)
target_link_libraries(
        server
        PUBLIC project_options project_warnings
        PRIVATE CLI11::CLI11 spdlog::spdlog ${Boost_LIBRARIES})

target_include_directories(server PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")