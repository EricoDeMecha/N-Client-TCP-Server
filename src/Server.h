//
// Created by erico on 1/30/23.
//

#ifndef MYPROJECT_SERVER_H
#define MYPROJECT_SERVER_H

#include <boost/asio.hpp>
#include "Client.h"

using namespace boost::asio;
constexpr unsigned long MAX_CLIENTS = 100;

class Server{
public:
  Server(io_service& ios, unsigned short port):
                                                 ios_(ios),
                                                 acceptor_(ios, ip::tcp::endpoint(ip::tcp::v4(), port))
  {
    startAccept();
  }
  void handleDisconnect(std::unique_ptr<Client>& client)
  {
    auto iter  = find(clients_.begin(), clients_.end(), client);
    if(iter != clients_.end())
    {
      clients_.erase(iter);
      for(auto& this_client : clients_){
        auto c_id = this_client->getId();
        if(c_id > client->getId())
        {
          this_client->setId(--c_id);
        }
      }
    }
  }
  void broadcast(const char* data, size_t size, const std::unique_ptr<Client>& sender)
  {
    for(auto& client: clients_){
      if(client->getId() > sender->getId()){
        client->sendCmd(std::string(data, size));
      }
    }
  }
  void startRead(std::unique_ptr<Client>& client) {
    async_read(client->getSocket(), buffer(read_buffer_),
      [this, &client](const boost::system::error_code& error, size_t bytes_transferred)
      {
        if(!error)
        {
          broadcast(std::string(read_buffer_.begin(), read_buffer_.end()).c_str(), bytes_transferred, client);
          startRead(client);
        }
        else
        {
          handleDisconnect(client);
        }
      });
  }
  void startAccept()
  {
    if(clients_.size() >= MAX_CLIENTS){
      return;
    }
    auto client = std::make_unique<Client>(ios_, clients_.size());
    acceptor_.async_accept(client->getSocket(),
      [this, &client](const boost::system::error_code& errorCode){
        if(!errorCode){
          clients_.emplace_back(std::move(client));
          startRead(client);
        }
        startAccept();
      });
  }
private:
  std::array<char, MAX_BUF>read_buffer_{};
  io_service& ios_;
  ip::tcp::acceptor  acceptor_;
  std::vector<std::unique_ptr<Client>> clients_;
};
#endif// MYPROJECT_SERVER_H
