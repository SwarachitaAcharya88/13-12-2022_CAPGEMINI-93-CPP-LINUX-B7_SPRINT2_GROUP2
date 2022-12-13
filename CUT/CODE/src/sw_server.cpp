
#include <sw_server.h>
#include <sw_common.h>
#include <string>
#include <cstdint>
#include <sys/select.h>
using namespace std;
extern class TcpServer server;

void TcpServer::initializeSocket() {

  _sockfd = socket(AF_INET , SOCK_STREAM , 0);

  if (_sockfd == -1) 
  {
    throw std::runtime_error(strerror(errno));
  }
}

void TcpServer::bindAddress(int port) {
    memset(&_serverAddress, 0, sizeof(_serverAddress));
    _serverAddress.sin_family = AF_INET;
    _serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    _serverAddress.sin_port = htons(port);

    const int bindResult = bind(_sockfd, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress));
    const bool bindFailed = (bindResult == -1);
    if (bindFailed) {
        throw std::runtime_error(strerror(errno));
    }
}


void TcpServer::listenToClients() {
    const int clientsQueueSize = _maxNumOfClients;
    const bool listenFailed = (listen(_sockfd, clientsQueueSize) == -1);
    if (listenFailed) {
        throw std::runtime_error(strerror(errno));
    }
}


void* handleClientMessages(void *arg)
{
  clientInfo_t *clInfo = (clientInfo_t *) arg;
  char msg[128];

  int ret = -1;

  cout << "test printf in server thread" << endl;

  while(1)
  {
    const size_t numOfBytesReceived = recv(clInfo->clSockFd, msg, sizeof(msg), 0);
    cout << "Bytes received=" << numOfBytesReceived << endl;

    if(numOfBytesReceived < 1)
    {
      std::string errorMsg;
      if (numOfBytesReceived == 0)
      { 
        errorMsg = "Client closed connection"; //client closed connection
        close(clInfo->clSockFd);
        return NULL;
      } else {
        errorMsg = strerror(errno);
      }
      close(clInfo->clSockFd);
      return NULL;

    }

    cout << "Rcv from Client = " << msg << endl;




typedef enum
{
  START_SWITCH   = 1,
  STOP_SWITCH 	 = 2,
  PORT_ENABLE 	 = 3,
  PORT_DISABLE	 = 4,
  SEND_UNICAST_FRAME   = 5,
  SEND_MULTICAST_FRAME = 6,

} msg_types_e;

typedef struct messages
{
  msg_types_e msg_type;
  int         msg_len;
  string      msg_val;

}msg_t;


class Messaging
{
  map<msg_types_e, msg_t>  msg_map;

  public:
  string create_message(msg_types_e option, string frame);
  string create_message(msg_types_e option, int p_enable);
  string create_message(msg_types_e option);
  void initialize_messages();
  void server_process_message(string, char *);
};

class Switching
{
  map<string, string>  starting_map;
  map<string, string>  stop_map;
  map<string, string>  enable_map;
  map<string, string>  disable_map;
  map<string, string>  unicastframe_map;
  map<string, string>  multicastframe_map;

  public:
    int start_switch(string);
    int stop_switch(string);
    int port_enable(int);
    int port_disable(int);
    int send_unicast_frame(string);
    int send_multicast_frame(string);
};

int Switching::start_switch(string swstart)
{
  if(starting_map.find(swstart)!=starting_map.end())
  {
    cout << "Switch is started" << endl;
    return 1;
  }
  else
    return 0;
}

int Switching::stop_switch(string swstop)
{
  if(stop_map.find(swstop)!=stop_map.end())
  {
    cout << "Switch is stopped" << endl;
    return 1;
  }
  else
    return 0;
}

int Switching::port_enable(int penable)
{
  if(enable_map.find(penable)!=enable_map.end())
  {
    cout << "Port is Enable" << endl;
    return 1;
  }
  else
    return 0;
};

int Switching::port_disable(int pdisable)
{
  if(disable_map.find(pdisable)!=disable_map.end())
  {
    cout << "Port is Disable" << endl;
    return 1;
  }
  else
    return 0;
};

int Switching::send_unicast_frame(string uframe)
{
  if(unicastframe_map.find(uframe)!=unicastframe_map.end())
  {
    cout << "Unicast Frame" << endl;
    return 1;
  }
  else
    return 0;
}

int Switching::send_multicast_frame(string mframe)
{
  if(multicastframe_map.find(mframe)!=multicastframe_map.end())
  {
    cout << "Multicast Frame" << endl;
    return 1;
  }
  else
    return 0;
}
}

class Switching sw;
class Messaging msg;

string Messaging ::create_message (msg_types_e option)
{
  char sendBuffer[64];
  string s = "";

  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {
    snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type);
  }

  cout << "message created is " << sendBuffer << endl;

  s = sendBuffer;

  return s;
}

string Messaging::server_process_mesage(string s, char* sendBuff)
{
  char type, len;
  string rcvString = "";
  int ret;

  cout << "Input String = " << s << endl;

  type = int (s[0]); // 1
  len  = int (s[1]); // 5

  int t = type; 
  rcvString = s.substr(2,len); // ABCDE 

  cout << type  << endl;
  cout << len   << endl;
  cout << rcvString << endl;

  // processing 
  switch (t)
  {
    case 1:
      cout << "Checking for start switch " << rcvString << endl;
      
      ret = sw.start_switch(rcvString);
      if (ret == 1)
      {
        // server should create 
        msg.create_message(START_SWITCH);
      }
      break;

    case 2:
      cout << "Checking for switch stop " << rcvString << endl;
      
      ret = sw.stop_switch(rcvString);
      if (ret == 1)
      {
        // server should create 
        msg.create_message(STOP_SWITCH);
      }
      break;

    case 3:
      cout << "Checks if port is enable " << rcvString << endl;
      
      ret = sw.port_enable(rcvString);
      if (ret == 1)
      {
        // server should create 
        msg.create_message(PORT_ENABLE);
      }
      break;

    case 4:
      cout << "Checks if port is disabled " << rcvString << endl;
      
      ret = sw.port_disable(rcvString);
      if (ret == 0)
      {
        // server should create 
        msg.create_message(PORT_DISABLE);
      }
      break;

    case 5:
      cout << "Send unicast frame " << rcvString << endl;
      
      ret = sw.send_unicast_frame(rcvString);
      if (ret == 1)
      {
        // server should create 
        msg.create_message(SEND_UNICAST_FRAME);
      }
      break;

    case 6:
      cout << "Send multicast frame " << rcvString << endl;
      
      ret = sw.send_multicast_frame(rcvString);
      if (ret == 1)
      {
        // server should create 
        msg.create_message(SEND_MULTICAST_FRAME);
      }
      break;
    default:
      cout << "Default Case " << endl;
      break;
  }
  int to_type = 0;
  string to_message;
  
  value.erase( std::remove(value.begin(), value.end(), '\r'), value.end() );
  value.erase( std::remove(value.begin(), value.end(), '\n'), value.end() );

  cout<<"recieved: "<<s<<" type: "<< s[0]<< " value: "<<value<<endl;
  if(c_msg.type == '1')
  {
	to_type = 1;  
	cout<<"start switch"<<endl;
	cin>> to_message;
	start switch = to_message;
	to_type = 2;
   }
   else if(c_msg.type == '2')
   {
	to_type = 2;
	cout<<"stop switch"<<endl;
	cin >> to_message;
	stop switch = to_message;
	to_type = 3;
   }
   else if(c_msg.type == '3')
   {
	to_type = 3;
	cout<<"port number"<<endl;
	cout<<"Enter port:"<<endl;
	cin >> to_message;
	port number = to_message;
	if(port number == p_enable)
	{
		cout<<"1"<<endl;
	}
	to_type = 4;
   }
   else if(c_msg.type == '4')
   {
	to_type = 4;
	cout<<"port number"<<endl;
	cout<<"Enter port:"<<endl;
	cin >> to_message;
	port number = to_message;
	if(port number == p_disable)
	{
		cout<<"0"<<endl;
	}
	to_type = 5;
   }
   else if(c_msg.type == '5')
   {
	to_type = 5;
	string tmpstr = "gdb";
	cout<<type_message<<tmpstr.length()<<to_message<<endl;
	cout<<"Enter frame:"<<endl;
	cin >> to_message;
	to_type = 6;
   }
   else if(c_msg.type == '6')
   {
	to_type = 6;
	string tmpstr = "gdb";
	cout<<type_message<<tmpstr.length()<<to_message<<endl;
	cout<<"Enter multicast frame:"<<endl;
	cin >> to_message;
   }
   else
   {
	cout<<"Invalid selection"<<endl;
   }
   snprintf(sendBuff, sizeof(sendBuff), "%d%ld%s\r\n", to_type,strlen(to_message.c_str()),to_message.c_str());
    	 	
}

int main()
{
  Messaging m;
  char sendBuff[128];

  memset(sendBuff, 0, sizeof(sendBuff));

  string recvFromClient = "58BHAVITHA";
 

 msg.ProcessMsg(msg) ;
 map<string,string> MAC_map;
 map<string,string> ::iterator it;
 cout<<"port address:"<<endl;
 cout<<"port no"<<endl;
    for(it = MAC_map.begin(); it != MAC_map.end(); ++it)
    {
	    cout<<'\t'<<it->first<<'\t'->second<<endl;
    }	    
 
    
    send(clInfo->clSockFd, sendBuff, strlen(sendBuff), 0);
 }
  
}


int TcpServer::acceptClient()
{
    pthread_t clientThread;
    socklen_t socketSize  = sizeof(_clientAddress);
    const int fileDescriptor = accept(_sockfd, (struct sockaddr*)&_clientAddress, &socketSize);

    const bool acceptFailed = (fileDescriptor == -1);
    if (acceptFailed) {
        throw std::runtime_error(strerror(errno));
    }

    clientInfo_t *newClientInfo = new (clientInfo_t);

    newClientInfo->clIpAddr = inet_ntoa(_clientAddress.sin_addr);
    newClientInfo->clSockFd = fileDescriptor;
    cout << "New Client FD=" << fileDescriptor <<endl;
    

    pthread_create(&clientThread, NULL, handleClientMessages, (void *) newClientInfo);

    _clThread.push_back(clientThread);

    return 1;
}

int TcpServer::close(int sockFd) 
{
      close(sockFd);
      return 1;
}
}
