//
// Created by erico on 1/30/23.
//

#ifndef MYPROJECT_SERVER_H
#define MYPROJECT_SERVER_H

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <unordered_set>
#include <deque>

constexpr int MAX_BUF = 1024;
/*
class WorkerThread
{
public:
    static void run(std::shared_ptr<boost::asio::io_service> ios)
    {
        std::lock_guard<std::mutex> const lock(mutex_);
        ios->run();
    }
private:
    static std::mutex mutex_;
};*/

class Client
{
public:
    virtual ~Client()= default;
    virtual void onCommand(std::array<char, MAX_BUF>& command)  = 0;
};

class ClientManager
{
public:
    void connect(const std::shared_ptr<Client>& client)
    {
        clients_.insert(client);
        clients_table_[client] = clients_table_.size();
    }
    void disconnect(const std::shared_ptr<Client>& client)
    {
        clients_.erase(client);
        clients_table_.erase(client);
    }
    void broadcast(std::array<char, MAX_BUF>& command, const std::shared_ptr<Client>& client)
    {
        std::vector<std::shared_ptr<Client>> const lower_clients_ = getClientsLessThanCurrent(client);
        std::for_each(lower_clients_.begin(), lower_clients_.end(), [&command](auto && PH1) { PH1->onCommand(command); });
    }
    std::vector<std::shared_ptr<Client>> getClientsLessThanCurrent(const std::shared_ptr<Client>& current)
    {
        std::vector<std::shared_ptr<Client>> result;
        unsigned long const current_id = clients_table_.at(current);
        for(const auto& client: clients_table_){
            if(client.second < current_id){
                result.emplace_back(client.first);
            }
        }
        return result;
    }
private:
    std::unordered_set<std::shared_ptr<Client>> clients_;
    std::unordered_map<std::shared_ptr<Client>, unsigned long> clients_table_;
};

class ClientSession : public Client,
                      public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::io_service& ios, const boost::asio::io_service::strand& strand, ClientManager& clientManager):
            socket_(ios), clientManager_(clientManager), strand_(strand)
    {}
    void start()
    {
        clientManager_.connect(shared_from_this());
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_command_, read_command_.size()),
                                strand_.wrap(boost::bind(&ClientSession::writeHandler, shared_from_this(), _1)));
    }
    void onCommand(std::array<char, MAX_BUF>& command) override
    {
        bool const is_empty = !write_commands_.empty();
        write_commands_.push_back(command);
        if(!is_empty)
        {
            boost::asio::async_write(socket_,
                                     boost::asio::buffer(write_commands_.front(), write_commands_.front().size()),
                                     strand_.wrap(boost::bind(&ClientSession::writeHandler, shared_from_this(), _1))
                                     );
        }
    }
    boost::asio::ip::tcp::socket& socket(){ return socket_; }
private:
    void readHandler(const boost::system::error_code& error)
    {
        if(!error)
        {
            clientManager_.broadcast(read_command_, shared_from_this());
            boost::asio::async_read(socket_,
                                    boost::asio::buffer(read_command_, read_command_.size()),
                                    strand_.wrap(boost::bind(&ClientSession::readHandler, shared_from_this(), _1)));
        }
        else
        {
            clientManager_.disconnect(shared_from_this());
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
                                         strand_.wrap(boost::bind(&ClientSession::writeHandler, shared_from_this(), _1)));
            }
        }
        else
        {
            clientManager_.disconnect(shared_from_this());
        }
    }

    boost::asio::ip::tcp::socket  socket_;
    ClientManager clientManager_;
    boost::asio::io_service::strand strand_;

    std::array<char, MAX_BUF> read_command_{};
    std::deque<std::array<char, MAX_BUF>> write_commands_;
};

class Server
{
public:
    Server(boost::asio::io_service& ios,  boost::asio::io_service::strand& strand, const boost::asio::ip::tcp::endpoint& endpoint):
    ios_(ios), strand_(strand), acceptor_(ios, endpoint)
    {
        run();
    }
private:
    void run()
    {
        std::shared_ptr<ClientSession> const client_session(new ClientSession(ios_, strand_, clientManager_));
        acceptor_.async_accept(client_session->socket(), strand_.wrap([this, client_session](auto && PH1) { onAccept(client_session, std::forward<decltype(PH1)>(PH1)); }));
    }
    void onAccept(const std::shared_ptr<ClientSession>& client_session, const boost::system::error_code& error)
    {
        if(!error)
        {
            client_session->start();
        }
        run();
    }
    boost::asio::io_service& ios_;
    boost::asio::io_service::strand strand_;
    boost::asio::ip::tcp::acceptor acceptor_;
    ClientManager clientManager_;
};
#endif// MYPROJECT_SERVER_H
