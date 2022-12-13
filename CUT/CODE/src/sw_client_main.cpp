#include <iostream>
#include <sw_common.h>
#include <sw_client.h>

TcpClient client;
Messaging msg;
class IpcMq mainMq;

void printMenu() {
    std::cout << "Select one of the following options: \n" <<
                 "1. Start the switch \n" <<
                 "2. Stop the switch  \n"  <<
                 "3. Port Enable \n" <<
		 "4. Port Disable \n"
                 "5. Send Unicast Frame   \n" <<
                 "6. Send Multicast Frame    \n";
}

int getMenuSelection() {
    int selection = 0;
    std::cin >> selection;
    if (!std::cin) {
        throw std::runtime_error("invalid menu input. expected a number, but got something else");
    }
    std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
    return selection;
}

bool handleMenuSelection(int selection) {
    string tmpStr = "";
    static const int minSelection = START_SWITCH;
    static const int maxSelection = SEND_UNICAST_FRAME +1;
    if (selection < minSelection || selection > maxSelection) {
        std::cout << "hehe invalid selection: " << selection <<
                     ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
        return false;
    }

    switch (selection) {
        case START_SWITCH: { // send message to server
            std::cout << "Sending message to start switch \n";
	    msg.create_message(START_SWITCH);
            tmpStr = msg.create_message(START_SWITCH);
	    cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }
        case STOP_SWITCH: { // close client
	    std::cout<<"stop the switch \n";			  
            msg.create_message(STOP_SWITCH);
            tmpStr=msg.create_message(STOP_SWITCH);
	    cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }
        case PORT_ENABLE: { // port enables
	    int p_enable;
	    cout << "Enter the Port \n";
	    cin >> p_enable;
            msg.create_message(PORT_ENABLE,p_enable);
            tmpStr=msg.create_message(PORT_ENABLE,p_enable);
            cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }
        case PORT_DISABLE: { // port disables 
	    int p_disable;
	    cout << "Enter the Port"<<endl;
	    cin >> p_disable;
            msg.create_message(PORT_DISABLE,p_disable);
            tmpStr=msg.create_message(PORT_DISABLE,p_disable);
	    cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }
        case SEND_UNICAST_FRAME: { // unicast frame
	    string u_frame;
	    cout << "Enter the frame \n" ;
	    cin >> u_frame; //090812345678123456788899JIJIEJDDLFJF;
            msg.create_message(SEND_UNICAST_FRAME,u_frame);
            tmpStr=msg.create_message(SEND_UNICAST_FRAME,u_frame);
	    cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }

        case SEND_MULTICAST_FRAME: { // multicast frame
	    string m_frame;
	    cout << "Enter the frame \n" ;
	    cin >> m_frame; //090812345678123456788899JIJIEJDDLFJF;
            msg.create_message(SEND_MULTICAST_FRAME,m_frame);
            tmpStr=msg.create_message(SEND_MULTICAST_FRAME,m_frame);
	    cout << "tmpStr= " << tmpStr << endl;			   
            break;
        }
     
	default:
	{
		std:: cout << " hhh invalid selection: " << selection <<
                      ". selection must be b/w " << minSelection << " and " << maxSelection << "\n";
	}
    }

    	int ret = mainMq.sendIpcMessage(tmpStr, 1);
    	cout << "Return from IPC " << ret <<endl;
    	return false;
    
}

#define SWITCH_IP    "127.0.0.1";
#define SWITCH_PORT  "65123"
#define MQ_FLAGS O_RDWR | O_CREAT
#define MQ_MAXSIZE 1
#define MQ_MAXMSGSIZE 128

int main() 
{
  mainMq.createIpcMq("/mq1", MQ_FLAGS, MQ_MAXSIZE, MQ_MAXMSGSIZE);
  string ip = "127.0.0.1";
  int port = 65123;

  bool connected = false;

  int x = 0;

  while (!connected) 
  {
    connected = client.connectTo(ip, port);

    if (connected) 
    {
      std::cout << "Client connected successfully\n";
    } else 
    {
      std::cout << "Client failed to connect: " << "\n"
        << "Make sure the server is open and listening\n\n";
      sleep(2);
      std::cout << "Retrying to connect...\n";
      x++;
      if (x == 2)
      {
        std::cout << "Exceeded max retries\n" << endl;
        exit(0);
      }
    }
  };

  bool shouldTerminate = false;
  while(!shouldTerminate)
  {
    printMenu();
    int selection = getMenuSelection();
    cout << " Selection: " << selection << endl;
    shouldTerminate = handleMenuSelection(selection);
  }

  return 0;
}
