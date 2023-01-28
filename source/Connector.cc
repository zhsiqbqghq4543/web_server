
#include "Connector.h"
#include "LogFront.h"

#include <string.h>
#include <stdio.h>
#include <functional>

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

Connector::Connector(sockaddr_in addr, std::string &conn_name)
    : addr_(addr), conn_name_(conn_name), timer_index_(-1)
{

    // http_request
    this->http_handle_ = new HttpHandle();
    // buffer
    this->input_buffer_ = new Buffer();
    // buffer
    this->output_buffer_ = new Buffer();
}
Connector::~Connector()
{
    close(this->channel_->get_fd());
    delete this->channel_;
    delete this->http_handle_;
    delete this->input_buffer_;
    delete this->output_buffer_;
    //std::cout << conn_name_ + "\tconnector\thas\tbeen\tdestroyed\n";
    LOG_TRACE << conn_name_ + "\tconnector\thas\tbeen\tdestroyed";
}

void Connector::conn_destroy()
{
    LOG_TRACE << "destroying...";
}

void Connector::close_connection() // timer out zhudong close
{
    // self io
    this->loop_->rm_channel(this->channel_->get_fd());
    // self io rm map  and  main io rm conn
    this->rm_call_back_to_acceptor();
}

void Connector::close_connection_and_timer() // beidong close
{
    // timerclose
    loop_->delete_timer(this->timer_index_);
    // self io
    this->loop_->rm_channel(this->channel_->get_fd());
    // self io rm map  and  main io rm conn
    this->rm_call_back_to_acceptor();
}

void Connector::new_message()
{
    int recv_size = this->input_buffer_->read_fd(this->channel_->get_fd());
    if (recv_size != 0)
    {
        LOG_TRACE << "recv\tmessage\t" + std::to_string(recv_size);
    }
    if (recv_size == 0)
    {
        //std::cout << "get\tclose message\t\n";
        LOG_TRACE << "get\tclose message\t";
        close_connection_and_timer();

        return;
    }
    if (this->http_handle_->recv_message(this->input_buffer_) == false)
    {
        const char *err = "HTTP/1.1 400 Bad Request\r\n\r\n";
        send(this->channel_->get_fd(), err, sizeof(err), 0);
        close_connection_and_timer();
        return;
    }
    if (this->http_handle_->got_all() == true)
    {
        LOG_TRACE << "now http message got_all:";

        std::string send_str = http_handle_->get_send_data();
        //try send, or use buffer
        int size = write(this->channel_->get_fd(), &*send_str.begin(), send_str.size());
        if (size != send_str.size())
        {
            this->output_buffer_->push_data(&*(send_str.begin() + size), send_str.size() - size);
            int epoll_fd = this->loop_->get_epollfd();
            epoll_event event;
            event.data.fd = this->channel_->get_fd();
            event.events = EPOLLOUT | EPOLLIN;
            // std::cout << epoll_fd << ' ' << this->channel_->get_fd() << std::endl;
            //  std::cout << epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->channel_->get_fd(), &event) << std::endl;
            // std::cout << epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->channel_->get_fd(), &event) << std::endl;

            LOG_TRACE << std::to_string(send_str.size()) + "\tsize has send" + std::to_string(size);
        }

        //this->http_handle_->cout_message();
        //  request
        close_connection_and_timer();
    }
}

void Connector::send_message()
{
    std::cout << "Connector::send_message()" << std::endl;
    this->output_buffer_->send_fd(this->channel_->get_fd());
    if (this->output_buffer_->is_empty())
    {
        int epoll_fd = this->loop_->get_epollfd();
        epoll_event event;
        event.data.fd = this->channel_->get_fd();
        event.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_MOD, this->channel_->get_fd(), &event);
    }
}

std::string Connector::get_name()
{
    return this->conn_name_;
}

void Connector::add_channel_to_eventloop(Eventloop *loop, int new_fd) // in thread io
{
    loop_ = loop;
    channel_ = new Channel(loop);
    channel_->set_fd(new_fd);

    // channel_.set_read_callback();
    channel_->set_read_callback(
        std::bind(&Connector::new_message, this));

    channel_->set_write_callback(
        std::bind(&Connector::send_message, this));

    loop->push_Channel(channel_);

    // std::cout << "new\tConn\tadded\tto\tloop\t" << std::endl;
    // now begin timer
    std::shared_ptr<Connector> shared_ptr_conn = Connector::shared_from_this();
    this->timer_index_ = loop->add_timer(60 * 5, std::bind(&Connector::close_connection, shared_ptr_conn));
}
