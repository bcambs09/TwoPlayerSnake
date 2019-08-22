//
//  Client.cpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "Client.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

Client::Client(const string& addr, const string& port) : addr(addr), port(port){
    sock = connectToServer();
}

int Client::connectToServer() {
    struct addrinfo hints, *res;
    int sockfd;
    // first, load up address structs with getaddrinfo():
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(addr.c_str(), port.c_str(), &hints, &res);
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "connect");
        exit(1);
    }
    printf("Connected to server at %s:%s\n", addr.c_str(), port.c_str());
    return sockfd;
}

void Client::sendToServer(const string& str) {
    char text[64];
    strcpy(text, str.c_str());
    send(sock, text, 64, 0);
}

int Client::sendDirection(int direction) {
    sendToServer("DIRECTION " + to_string(direction));
    return getIntFromServer();
}

int Client::getIntFromServer() {
    int integer;
    recv(sock, &integer, sizeof(integer), MSG_WAITALL);
    return ntohl(integer);
}

int Client::getWidth() {
    sendToServer("WIDTH");
    return getIntFromServer();
}

int Client::getHeight() {
    sendToServer("HEIGHT");
    return getIntFromServer();
}

int Client::getScore() {
    sendToServer("SCORE");
    return getIntFromServer();
}

string Client::getSelf() {
    sendToServer("SELF");
    char s[256];
    recv(sock, s, 256, MSG_WAITALL);
    return string(s);
}

string Client::getOther() {
    sendToServer("OTHER");
    char s[256];
    recv(sock, s, 256, MSG_WAITALL);
    return string(s);
}

string Client::getStatus() {
    sendToServer("STATUS");
    char s[64];
    recv(sock, s, 64, MSG_WAITALL);
    return string(s);
}

pair<int, int> Client::getApple() {
    sendToServer("APPLE");
    char s[64];
    recv(sock, s, 64, MSG_WAITALL);
    string coord(s);
    istringstream is(s);
    string x, y;
    getline(is, x, ' ');
    getline(is, y, ' ');
    return {stoi(x), stoi(y)};
}

Client::~Client() {}
