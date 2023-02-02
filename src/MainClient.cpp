#include <optional>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>


// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `myproject`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#include "Client.h"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv)
{
  try {
    CLI::App app{ fmt::format("{} version {}", myproject::cmake::project_name, myproject::cmake::project_version) };

    std::optional<unsigned short> port;
    app.add_option("-p,--port", port, "Specify the endpoint port");

    CLI11_PARSE(app, argc, argv);
    if(port){
        boost::asio::io_service ios;
        boost::asio::ip::tcp::resolver resolver(ios);
        boost::asio::ip::tcp::endpoint const endpoint(boost::asio::ip::tcp::v4(), port.value());
        boost::asio::ip::tcp::resolver::query const query(endpoint.address().to_string(), std::to_string(endpoint.port()));
        boost::asio::ip::tcp::resolver::iterator const iterator = resolver.resolve(query);
        std::array<char, MAX_BUF> command{};
//        Client client(ios, iterator);
        std::vector<Client> clients;
        std::string command_str = "Message from client";
        constexpr  size_t N_CLIENTS = 100;
        for(size_t i = 0; i < N_CLIENTS; i++)
        {
            clients.emplace_back(ios, iterator);
            std::copy_n(command_str.data(), MAX_BUF, command.data());
            clients[i].write(command);
            clients[i].close();
        }

    }else{
      spdlog::error("The port is not specified");
    }
  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
