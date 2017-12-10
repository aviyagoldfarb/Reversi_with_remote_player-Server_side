//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "Server.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int port;
    ifstream inFile;
    inFile.open("../server_configuration_file.txt");
    inFile >> port;

    Server server(port);
    try {
        server.start();
    } catch (const char *msg) {
        cout << "Cannot start server. Reason: " << msg << endl;
        exit(-1);
    }
}