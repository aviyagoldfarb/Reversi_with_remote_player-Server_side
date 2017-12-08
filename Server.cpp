//
// Created by aviya on 06/12/17.
//

#include "Server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
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
    // Assign a local address to the socket
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

    // looping until two clients create communication
    while (true) {
        int black = 1;
        int white = 2;
        int n;
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

        n = write(blackClientSocket, &black, sizeof(black));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }

        n = write(whiteClientSocket, &white, sizeof(white));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }

        // the game
        handleClients(blackClientSocket, whiteClientSocket);

        // Close communication with the current clients
        close(blackClientSocket);
        close(whiteClientSocket);
    }

}

// Handle requests from a specific client
void Server::handleClients(int blackClientSocket, int whiteClientSocket) {
    int x, y;

    while (true) {
        int n;
        string result;

        //getting the chosen cell of the black player.
        Point blackPlayerChosenCell = this->readCell(blackClientSocket);
        /*
        if (blackPlayerChosenCell.getX() == 0 && blackPlayerChosenCell.getY() == 0) {
            return;
        }
        */
        cout << "Got cell: " << endl;
        blackPlayerChosenCell.pointToPrint();
        cout << endl;

        x = blackPlayerChosenCell.getX();
        y = blackPlayerChosenCell.getY();

        // write the result back to the white client
        n = write(whiteClientSocket, &x, sizeof(x));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }
        n = write(whiteClientSocket, &y, sizeof(y));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }

        //getting the chosen cell of the white player.
        Point whitePlayerChosenCell = this->readCell(whiteClientSocket);
        /*
        if (whitePlayerChosenCell.getX() == 0 && whitePlayerChosenCell.getY() == 0) {
            return;
        }
        */
        cout << "Got cell: " << endl;
        whitePlayerChosenCell.pointToPrint();
        cout << endl;

        x = whitePlayerChosenCell.getX();
        y = whitePlayerChosenCell.getY();

        // Write the result back to the black client
        n = write(blackClientSocket, &x, sizeof(x));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }
        n = write(blackClientSocket, &y, sizeof(y));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }

    }

}

void Server::stop() {
    close(serverSocket);
}

Point Server::readCell(int client) {
    Point chosenCell(0, 0);
    int x, y;
    int n;

    n = read(client, &x, sizeof(x));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return chosenCell;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return chosenCell;
    }

    n = read(client, &y, sizeof(y));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return chosenCell;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return chosenCell;
    }

    chosenCell.setX(x);
    chosenCell.setY(y);

    return chosenCell;
}