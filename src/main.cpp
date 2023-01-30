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
      io_service ios;
      Server const server(ios, port.value());
      unsigned long const total_clients = 100;
      int const i1 = 0;
      int const i2 = 29;
      int const i3 = 98;
      std::vector<std::unique_ptr<Client>> clients;

      // Create clients and connect to server
      for (unsigned long i = 0; i < total_clients; i++) {
        auto client = std::make_unique<Client>(ios, i);
        client->getSocket().connect(ip::tcp::endpoint(ip::address::from_string("127.0.0.1"), port.value()));
        clients.emplace_back(std::move(client));
      }

      // Client 1 sends a message to the server
      clients[i1]->sendCmd("Message from Client 1\n");
      spdlog::info("Client 1 sent a message to the server");

      // Clients 2 to 29 respond based on their ids
      unsigned long const lim1 = 30;
      for (unsigned long i = 1; i < lim1; i++) {
        if (i % 2 == 0) {
          clients[i]->sendCmd("Received by Client " + std::to_string(i) + "\n");
          spdlog::info("Client {} sent a response to the server", i);
        }
      }

      // Client 30 sends a message to the server
      clients[i2]->sendCmd("Message from Client 30\n");
      spdlog::info("Client 30 sent a message to the server");

      // Client 99 sends a message to the server
      clients[i3]->sendCmd("Message from Client 99\n");
      spdlog::info("Client 99 sent a message to the server");

      ios.run();
    }else{
      spdlog::error("The port is not specified");
    }

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
