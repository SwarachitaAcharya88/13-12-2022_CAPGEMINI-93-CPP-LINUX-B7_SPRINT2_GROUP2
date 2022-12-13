
#include <sw_client.h>
#include <sw_common.h>
/*#include <mqueue.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include<unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MQ_NAME "/mq_1"
#define MQ_FLAGS O_RDWR | O_CREAT*/
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
	//char msg[128];
        cout<<"test printf in thread"<<endl;
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


	       //char msg[1024];
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

   /* const int connectResult = connect(_sockfd, (struct sockaddr *)&_server , sizeof(_server));
    const bool connectionFailed = (connectResult == -1);
    if (connectionFailed) {
        return -1;
    }*/
    cout << "pthread create " << endl;

   // pthread_create(&_receiveThread, NULL, receiveTask, this); 
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
    { // inet_addr failed to parse address
        // if hostname is not in IP strings and dots format, try resolve it
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
    { // send failed
        return -1;
    }
    
    if (numBytesSent < size) 
    { // not all bytes were sent
        return -1;
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


