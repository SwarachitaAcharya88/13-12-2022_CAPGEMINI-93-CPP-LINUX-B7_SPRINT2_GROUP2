#pragma once

#include <cstdio>
#include <string>
#include <bits/stdc++.h>
#include  <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace std;

#define MAX_PACKET_SIZE 4096

namespace fd_wait {
    enum Result {
        FAILURE,
        TIMEOUT,
        SUCCESS
    };

    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds = 1);
};

typedef enum 
{
  START_SWITCH = 1,
  STOP_SWITCH  = 2,
  PORT_ENABLE = 3,
  PORT_DISABLE = 4,
  SEND_UNICAST_FRAME = 5,
  SEND_MULTICAST_FRAME = 6,

} msg_types_e;

typedef struct messages
{
  msg_types_e msg_type;
  int         msg_len;
  string      msg_val;
}messages_t;

class Messaging
{
  map<msg_types_e, messages_t>  msg_map;

  public:
    Messaging()
    {
      initialize_messages();
    }
    string create_message(msg_types_e option, string frame);
    string create_message(msg_types_e option, int p_enable);
    string create_message(msg_types_e option);
    void initialize_messages();
};

class IpcMq
{
	private:
		string   _mqName;
		mqd_t    _mqFd;
		int      _mqFlags;
		struct mq_attr _mqAttr;
	public:
		int createIpcMq(string name, int flags, int maxMsgs, int maxMsgSize);
		int sendIpcMessage(string msgToSend, int msgPrio);
		int recvIpcMessage(string *msgToRcv, unsigned int *msgPrio);
		void closeIpc();
		void deleteIpc();
};
	
