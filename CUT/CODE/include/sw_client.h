#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <bits/stdc++.h>

using namespace std;

class TcpClient
{
private:
    int  _sockfd;
    std::atomic<bool> _isConnected;
    std::atomic<bool> _isClosed;
    struct sockaddr_in _server;
    pthread_t _receiveThread;

    void initializeSocket();
    void setAddress(const std::string& address, int port);
    void terminateReceiveThread();

public:
    TcpClient();
    ~TcpClient();
    TcpClient(int fileDescriptor)
    {
	    _sockfd = fileDescriptor;
    }
    bool connectTo(const std::string & address, int port);
    int sendMsg(const char * msg, size_t size);
    bool isConnected() const { return _isConnected; }
    void setIsConnected(bool opt) { _isConnected = opt; }
    void waitForReceiveThread(); 
    int close();
    int getSockFd() const  { return _sockfd; }
};

