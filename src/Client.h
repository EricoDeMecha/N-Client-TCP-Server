//
// Created by erico on 1/30/23.
//

#ifndef MYPROJECT_CLIENT_H
#define MYPROJECT_CLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <array>
#include <deque>
#include <utility>
#include <spdlog/spdlog.h>

constexpr int MAX_BUF  = 1024;
class Client
{
public:
    Client(boost::asio::io_service& ios, boost::asio::ip::tcp::resolver::iterator endpoint_iterator):
    ios_(ios), socket_(ios)
    {
        boost::asio::async_connect(socket_, std::move(endpoint_iterator),
                                   boost::bind(&Client::onConnect, this, _1));
    }
    void write(const std::array<char, MAX_BUF>& command)
    {
        ios_.post(boost::bind(&Client::writeImpl, this, command));
    }
    void close()
    {
        ios_.post(boost::bind(&Client::closeImpl, this));
    }
private:
    void onConnect(const boost::system::error_code& errorCode)
    {
        spdlog::info(read_command_.data());
        if(!errorCode)
        {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_command_, read_command_.size()),
                                    boost::bind(&Client::readHandler, this, _1 ));
        }
        else
        {
            closeImpl();
        }
    }

    void readHandler(const boost::system::error_code& error)
    {
        spdlog::info(read_command_.data());
        if(!error)
        {
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_command_, read_command_.size()),
                                    boost::bind(&Client::readHandler, this , _1));
        }
        else
        {
            closeImpl();
        }
    }
    void writeImpl(std::array<char, MAX_BUF> command)
    {
        bool is_empty = command.empty();
        write_commands_.push_back(command);
        if(!is_empty)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_commands_.front(), write_commands_.front().size()),
                                     boost::bind(&Client::writeHandler, this, _1));
        }
    }
    void writeHandler(const boost::system::error_code& error)
    {
        if(!error)
        {
            write_commands_.pop_front();
            if(!write_commands_.empty())
            {
                boost::asio::async_write(socket_,
                                         boost::asio::buffer(write_commands_.front(), write_commands_.front().size()),
                                         boost::bind(&Client::writeHandler, this, _1));
            }
        }
        else
        {
            closeImpl();
        }
    }
    void closeImpl()
    {
        socket_.close();
    }
    boost::asio::io_service& ios_;
    boost::asio::ip::tcp::socket  socket_;
    std::array<char, MAX_BUF> read_command_{};
    std::deque<std::array<char, MAX_BUF>> write_commands_;
};
#endif// MYPROJECT_CLIENT_H
