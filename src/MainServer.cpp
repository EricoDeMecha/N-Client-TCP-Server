//
// Created by erico on 1/30/23.
//
#include <optional>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>


// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `myproject`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#include "Server.h"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", myproject::cmake::project_name, myproject::cmake::project_version) };

    std::optional<unsigned short> port;
    app.add_option("-p,--port", port, "Specify the endpoint port");

    CLI11_PARSE(app, argc, argv);
    if(port){
        while(true){
            try{
                boost::asio::io_service ios;
                boost::asio::io_service::strand strand(ios);
                boost::asio::ip::tcp::endpoint const endpoint(boost::asio::ip::tcp::v4(), port.value());
                Server const server(ios, strand, endpoint);
            }catch (std::exception& e){
                spdlog::error("Exception {}", e.what() );
            }
        }
    }else{
      spdlog::error("The port is not specified");
    }
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
