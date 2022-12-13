#pragma once

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <functional>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <mutex>
using namespace std;

typedef struct
{
  int clSockFd;
  std::string clIpAddr = "";

} clientInfo_t;

class TcpServer {

private:

    int _sockfd;
    struct sockaddr_in _serverAddress;
    struct sockaddr_in _clientAddress;
    clientInfo_t *  _client;
    std::vector<pthread_t> _clThread;
    int _maxNumOfClients;

public:
    void initializeSocket();
    void bindAddress(int port);
    void listenToClients();
    int  acceptClient();
    int close(int );
    void printClients();
    void setMaxClients(int maxClients) { _maxNumOfClients = maxClients; }
    int getMaxClients() { return _maxNumOfClients; }
    std::vector<pthread_t> getClientThreads() { return _clThread; }
    void waitForClThread(pthread_t thread) { pthread_join(thread, NULL); }
};

