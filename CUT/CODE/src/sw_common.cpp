#include <cstdint>
#include <sw_common.h>
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

#include <sys/select.h>

#define SELECT_FAILED -1
#define SELECT_TIMEOUT 0

namespace fd_wait {
    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds) {
        struct timeval tv;
        tv.tv_sec = timeoutSeconds;
        tv.tv_usec = 0;
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(fileDescriptor, &fds);
        const int selectRet = select(fileDescriptor + 1, &fds, nullptr, nullptr, &tv);

        if (selectRet == SELECT_FAILED) {
            return Result::FAILURE;
        } else if (selectRet == SELECT_TIMEOUT) {
            return Result::TIMEOUT;
        }
        return Result::SUCCESS;
    }
}

void Messaging::initialize_messages ()
{
  messages_t m;

  msg_map.insert(pair<msg_types_e, messages_t> (START_SWITCH, {START_SWITCH,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (STOP_SWITCH,  {STOP_SWITCH,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (PORT_ENABLE,  {PORT_ENABLE,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (PORT_DISABLE, {PORT_DISABLE,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (SEND_UNICAST_FRAME, {SEND_UNICAST_FRAME,0,""}));
  msg_map.insert(pair<msg_types_e, messages_t> (SEND_MULTICAST_FRAME, {SEND_MULTICAST_FRAME,0,""}));


  map<msg_types_e, messages_t>::iterator itr;

  cout << "\nThe map msgs is : \n";
  cout << '\t' << "KEY" <<  '\t' << "TYPE" << '\t' << "LENGTH" << '\t' << "VALUE" << endl; 
  for (itr = msg_map.begin(); itr != msg_map.end(); ++itr) 
  {
    cout << '\t' << itr->first << '\t' << itr->second.msg_type << '\t' << itr->second.msg_len << '\t' << itr->second.msg_val
      << '\n';
  }
  cout << endl;
}

string Messaging::create_message (msg_types_e option, string frame) //for unicast and multicast frames

{

  char sendBuffer[64];
  string s = "";
  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {

      msg_map[option].msg_val = frame;
      msg_map[option].msg_len = frame.length();

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%d%s\r\n", msg_map[option].msg_type, 
        msg_map[option].msg_len, msg_map[option].msg_val.c_str());
  }
  cout << "message created is " << sendBuffer << endl;
  s=sendBuffer;
  return s;
}

string Messaging::create_message (msg_types_e option, int p_enable)
{
  char sendBuffer[64];
  string s = "";
  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {

      msg_map[option].msg_val = p_enable;

      snprintf(sendBuffer, sizeof(sendBuffer), "%d%d%s\r\n", msg_map[option].msg_type, 
        msg_map[option].msg_len, msg_map[option].msg_val.c_str());
  }
  cout << "message created is " << sendBuffer << endl;
  s=sendBuffer;
  return s;
}

string Messaging::create_message (msg_types_e option)
{
  char sendBuffer[64];
  string s = "";

  memset(sendBuffer, '0', sizeof(sendBuffer));

  if(msg_map.find(option)!= msg_map.end())
  {
    snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type); 
  }

  cout << "message created is " << sendBuffer << endl;
  s=sendBuffer;
  return s;
}


int IpcMq::createIpcMq(string name, int flags, int maxMsgs, int maxMsgSize)
{

	_mqName = name;
	_mqFlags = flags;

	_mqAttr.mq_maxmsg  = maxMsgs; //max messages in queue is 1
	_mqAttr.mq_msgsize = maxMsgSize; //max size of messages in queue is 4

	_mqFd = mq_open(name.c_str(), _mqFlags, 0777, &_mqAttr);
	cout << "error = " << errno << endl;

	cout << "mq_open return value: " << _mqFd <<endl;

	return 0;
}

void IpcMq::closeIpc()
{
	mq_close(_mqFd); //closes the IPC
}

void IpcMq::deleteIpc()
{
	mq_unlink(_mqName.c_str()); //unlinks the IPC
}

int IpcMq::sendIpcMessage(string msgToSend, int msgPrio)
{
	int ret = -1;
	cout << "sendIPC = " << msgToSend << endl;
	cout << "sendIPC.c_str() =" << msgToSend.c_str() << endl;
	ret = mq_send(_mqFd,
			(const char *) msgToSend.c_str(), strlen(msgToSend.c_str()), msgPrio);
	cout << "MQ SENT " << ret << endl;
	return ret;
}
int IpcMq::recvIpcMessage(string *msgToRcv,unsigned int *msgPrio)
{
	int ret = -1;
	char recvBuff[128];
	memset(recvBuff, 0, sizeof(recvBuff));

	ret = mq_receive(_mqFd, recvBuff, sizeof(recvBuff), msgPrio);

	cout << "MQ REECEIVE " << ret << endl;

	cout << "RECEIVEIPC =" << recvBuff << endl;
	*msgToRcv = recvBuff;
	cout << "msgToRcv = " << *msgToRcv << endl;
	return ret;
}






