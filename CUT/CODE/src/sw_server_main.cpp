#include <iostream>
#include <sw_common.h>
#include <sw_client.h>

class TcpServer server;
int main() 
{
  int clNum = 0;
  int port = 65123;

  server.setMaxClients(1);

  server.initializeSocket();
  server.bindAddress(port);
  server.listenToClients();


  while(1)
  {
    int ret = server.acceptClient();

    if (ret) 
    {
      std::cout << "Client accepted successfully\n";
      clNum++;

      if (clNum == server.getMaxClients())
        break;
    }
  }
  std::vector<pthread_t> clThread = server.getClientThreads();
  pthread_t clientThread;
 
  for (auto i = clThread.begin(); i != clThread.end(); ++i)
  {
    server.waitForClThread(*i);
  }
  return 0;
}

