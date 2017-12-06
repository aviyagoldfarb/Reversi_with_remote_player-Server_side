//
// Created by aviya on 06/12/17.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "string.h"
#include "iostream"
#include <stdio.h>


using namespace std;
#define MAX_CONNECTED_CLIENTS 2;

Server::Server(int port) :port(port), serverSocket(0) {
    cout << "Server" << endl;
}

void Server::start() {
    //Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw "Error opening socket";
    }

    struct sockaddr_in serverAddress;
    bzero((void*)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error on binding";
    }

    // Start listening to incoming connections
    listen(serverSocket, MAX_CONNECTED_CLIENTS);
// Define the client socket's structures
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen;

    while (true) {
        cout << "Waiting for clients connections..." << endl;
// Accept a new client connection
        int blackClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        cout << "Client 1 connected" << endl;
        if (blackClientSocket == -1)
            throw "Error on accept 1";
        int whiteClientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);
        cout << "Client 2 connected" << endl;
        if (whiteClientSocket == -1)
            throw "Error on accept 2";
        int m = 1;
        int n = write(blackClientSocket, &m, sizeof(m));
        if (n == -1) {
            cout << "Error writing to socket 1" << endl;
            return;
        }
        m = 2;
        n = write(blackClientSocket, &m, sizeof(m));
        if (n == -1) {
            cout << "Error writing to socket 2" << endl;
            return;
        }
        handleClient(blackClientSocket, whiteClientSocket);
// Close communication with the clients
        close(blackClientSocket);
        close(whiteClientSocket);
    }

}

// Handle requests from a specific client
void Server::handleClient(int blackClientSocket, int whiteClientSocket) {
    string theChosenCell;
    int x, y;
    char psic;

    while (true) {
        int* cells;
        int n;
        string result;

        //getting the chosen cell of the black player.
        cells = this->getCell(blackClientSocket);
        if (cells[0] == 0) {
            return;
        }
        cout << "Got exercise: " << x << "," << y  << endl;
        result = to_string(cells[0]) + "," + to_string(cells[1]);
        // Write the result back to the white client
        n = write(whiteClientSocket, &result, sizeof(result));
        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }


        //getting the chosen cell of the white player.
        cells = this->getCell(whiteClientSocket);
        if (cells[0] == 0) {
            return;
        }
        cout << "Got exercise: " << x << "," << y  << endl;
        result = to_string(cells[0]) + "," + to_string(cells[1]);
        // Write the result back to the black client
        n = write(blackClientSocket, &result, sizeof(result));
        if (n == -1) {
            cout << "Error writing to socket" << endl;
            return;
        }



    }

}


void Server::stop() {
    close(serverSocket);
}

int* Server::getCell(int client) {
    int cells[0, 0];
    int x, y;
    char psic;
    int n = read(client, &x, sizeof(x));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return cells;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return cells;
    }
    if (x == 0) {
        return cells;
    }

    n = read(client, &psic, sizeof(psic));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return cells;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return cells;
    }

    n = read(client, &y, sizeof(y));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return cells;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return cells;
    }
    cells[0] = x;
    cells[1] = y;

    return cells;

}