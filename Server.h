//
// Created by aviya on 06/12/17.
//

#ifndef EX4_SERVER_SERVER_H
#define EX4_SERVER_SERVER_H


class Server {
public:
    Server(int port);
    void start();
    void stop();
    int* getCell(int client);

private:
    int port;
    // the socket's file descriptor
    int serverSocket;

    void handleClient(int blackClientSocket, int whiteClientSocket);
};



#endif //EX4_SERVER_SERVER_H
