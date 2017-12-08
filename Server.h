//
// Created by aviya on 06/12/17.
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
