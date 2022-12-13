#include <iostream>
#include <sw_client.h>
#include <sw_common.h>
using namespace std;

TcpClient::TcpClient() {
    _isConnected = false;
    _isClosed = true;
}

TcpClient::~TcpClient() {
    close();
}
extern class IpcMq mainMq;
void* receiveTask(void *arg)
{
	TcpClient *cl = (TcpClient *) arg;
	int ret = -1;
        cl->setIsConnected(true);
        while(cl->isConnected())
       {
	       string rxMsgFromMain;
	       unsigned int prio;
	       ret = mainMq.recvIpcMessage(&rxMsgFromMain, &prio);
	       cout << "Received :" << ret << endl;
	       cout << "Message :" << rxMsgFromMain << endl;
	       cout << "Prio :" << prio << endl;
	       int sentBytes = send(cl->getSockFd(), rxMsgFromMain.c_str(), strlen(rxMsgFromMain.c_str()), 0);
	       cout  << "Sent to Server" << sentBytes << endl;
	       cout << "Message" << rxMsgFromMain.c_str() <<endl;


       }
return NULL;
}

bool TcpClient::connectTo(const std::string &address, int port) {
    try {
        initializeSocket();
        setAddress(address, port);
    } catch (const std::runtime_error& error) {
        return false;
    }
    cout << "connectTo " << endl;

    cout << "pthread create " << endl;

    _isConnected = true;
    _isClosed = false;
    pthread_create(&_receiveThread, NULL, receiveTask, this); 

    return true;
}

void TcpClient::initializeSocket() {

    _sockfd = socket(AF_INET , SOCK_STREAM , 0);
    
    if (_sockfd == -1) 
    {
        throw std::runtime_error(strerror(errno));
    }
}

void TcpClient::setAddress(const std::string& address, int port) {
    const int inetSuccess = inet_aton(address.c_str(), &_server.sin_addr);

    if(!inetSuccess) 
    {
        struct hostent *host;
        struct in_addr **addrList;
        if ( (host = gethostbyname( address.c_str() ) ) == nullptr){
            throw std::runtime_error("Failed to resolve hostname");
        }
        addrList = (struct in_addr **) host->h_addr_list;
        _server.sin_addr = *addrList[0];
    }
    _server.sin_family = AF_INET;
    _server.sin_port = htons(port);
}


int TcpClient::sendMsg(const char * msg, size_t size) 
{
    const size_t numBytesSent = send(_sockfd, msg, size, 0);

    if (numBytesSent < 0 ) 
    { 
        return -1; //send failed
    }
    
    if (numBytesSent < size) 
    { 
        return -1;  //not all bytes were sent
    }
    return 0;
}



void TcpClient::waitForReceiveThread() 
{
  pthread_join(_receiveThread, NULL);
}

int TcpClient::close(){
    if (_isClosed) {
        return -1;
    }

    _isClosed = true;
    return 0;
}


