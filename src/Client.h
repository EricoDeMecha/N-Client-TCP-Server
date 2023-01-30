//
// Created by erico on 1/30/23.
//

#ifndef MYPROJECT_CLIENT_H
#define MYPROJECT_CLIENT_H

#include <boost/asio.hpp>

using namespace boost::asio;
constexpr  int MAX_BUF = 1024;
class Client{
public:
  Client(io_service&ios, unsigned long id):
                                          socket_(ios),
                                          id_(id)
{}
ip::tcp::socket& getSocket() { return socket_; }
auto getId() const { return id_; }
void setId(unsigned long id) { id_ = id; }
void sendCmd(std::string message){
  write(socket_, buffer(message));
}
std::string readResponse(){
  std::array<char, MAX_BUF> data{};
  read(socket_, buffer(data));
  return std::string{data.begin(), data.end()};
}
private:
  ip::tcp::socket socket_;
  unsigned long id_;

};
#endif// MYPROJECT_CLIENT_H
