//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX4_SERVER_SERVER_H
#define EX4_SERVER_SERVER_H

#include "Point.h"

class Server {
public:
    Server(int port);
    void start();
    void stop();
    Point readCell(int client);

private:
    int port;
    // the socket's file descriptor
    int serverSocket;

    void handleClients(int blackClientSocket, int whiteClientSocket);
};



#endif //EX4_SERVER_SERVER_H
