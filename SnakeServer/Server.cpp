//
//  Server.cpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "Server.hpp"
#include "Globals.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <vector>
#include <chrono>
#include <string>

using namespace std::chrono;
using std::string;

Server::Server(const std::string& port) : port(port) {
    struct addrinfo hints, *res;
    int sockfd, ret;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    if ((ret = getaddrinfo(NULL, port.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
        exit(1);
    }
    
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        fprintf(stderr, "socket");
        exit(1);
    }
    
    int en = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &en,sizeof(int));
    
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        fprintf(stderr, "bind");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 5) == -1) {
        fprintf(stderr, "listen");
        exit(1);
    }
    
    sock = sockfd;
}

void Server::sleep(double timePassed) {
    double frameLength = 1.0 / FPS;
    double sleepLength = frameLength - timePassed;
    if (sleepLength < 0) return;
    usleep(sleepLength * 1000000);
}

void Server::playGame() {
    printf("Waiting for players...\n");
    high_resolution_clock::time_point lastTime = high_resolution_clock::now();
    high_resolution_clock::time_point currTime;
    duration<double> timeSpan;
    
    // Wait for 2 players to join the game
    std::unique_lock<std::mutex> lk(gameStart);
    while(game->inactive()) {
        cv.wait(lk);
    }
    lk.unlock();
    
    printf("Starting game.\n");
   
    m.lock();
    game->setStatus(Game::READY3);
    m.unlock();
    usleep(1000000);

    m.lock();
    game->setStatus(Game::READY2);
    m.unlock();
    usleep(1000000);

    m.lock();
    game->setStatus(Game::READY1);
    m.unlock();
    usleep(1000000);

    while(true) {
        // Sleep game thread to achieve specified FPS
        currTime = high_resolution_clock::now();
        timeSpan = duration_cast<duration<double>>(currTime - lastTime);
        sleep(timeSpan.count());
        lastTime = high_resolution_clock::now();
        

        std::unique_lock<std::mutex> lk(gameStart);
        while(game->inactive()) {
            cv.wait(lk);
        }
        lk.unlock();
        
        game->playFrame();
        
        Game::GameStatus st = game->getStatus();
        if (st == Game::P1_WIN || st == Game::P2_WIN || st == Game::TIE) {
            usleep(5000000);
            game->reset();
        }
    }
}

void Server::runServer() {
    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    int new_fd = 0;
    std::vector<std::thread*> threadList;
    threadList.push_back(new std::thread(playGame));
    while (new_fd != -1) {
        new_fd = accept(sock, (struct sockaddr *)&client_addr, &addr_size);
        printf("Client found.\n");
        threadList.push_back(new std::thread(communicateWithClient, new_fd));
    }
    
    for(auto t : threadList) {
        delete t;
    }
}

void Server::communicateWithClient(int sock) {
    char* buffer = (char*) malloc(64);
    long bytesReceived = 64;
    
    int playerNum;
    if ((playerNum = game->addNewPlayer()) == -1) {
        printf("There are already two players on this server.\n");
        close(sock);
        return;
    }
    
    while(bytesReceived == 64) {
        bytesReceived = recv(sock, buffer, 64, MSG_WAITALL);
        printf("Message received: %s\n", buffer);
        respondToMessage(buffer, playerNum, sock);
    }
    printf("Received %ld bytes from client.\n", bytesReceived);
    close(sock);
    game->removePlayer(playerNum);
    printf("Closed communication with client.\n");
}

void Server::respondToMessage(const char* msg, int playerNum, int sock) {
    if (!strcmp(msg, "WIDTH")) {
        sendInteger(game->getWidth(), sock);
    } else if (!strcmp(msg, "HEIGHT")) {
        sendInteger(game->getHeight(), sock);
    } else if (!strcmp(msg, "SELF")) {
        sendSnake(playerNum, sock);
    } else if (!strcmp(msg, "OTHER")) {
        int snakeNum = playerNum == 1 ? 2 : 1;
        sendSnake(snakeNum, sock);
    } else if (!strcmp(msg, "DIRECTION")) {
        int dir = game->getPlayer(playerNum)->direction;
        sendInteger(dir, sock);
    } else if (!strcmp(msg, "STATUS")) {
        sendString(game->getStatus(playerNum), sock);
    } else if (!strcmp(msg, "SCORE")) {
        sendInteger(game->getScore(playerNum), sock);
    } else if (!strcmp(msg, "APPLE")) {
        sendApple(sock);
    } else if (strstr(msg, "DIRECTION")){
        const char* num = msg+10;
        int newDirection = atoi(num);
        m.lock();
        game->getPlayer(playerNum)->direction = SnakePlayer::Direction(newDirection);
        m.unlock();
        sendInteger(0, sock);
    } else {
        printf("Invalid message code!\n");
    }
}

void Server::sendString(const string& str, int sock) {
    char cstr[64];
    strcpy(cstr, str.c_str());
    send(sock, cstr, 64, 0);
}

void Server::sendSnake(int snakeNum, int sock) {
    SnakePlayer* snake = game->getPlayer(snakeNum);
    if (snake) {
        const char* str = snake->getBody();
        send(sock, str, 256, 0);
        free((void*) str);
    } else {
        char temp[256];
        temp[0] = '\0';
        send(sock, temp, 256, 0);
    }
}

void Server::sendApple(int sock) {
    char cstr[64];
    sprintf(cstr, "%i %i", game->getApple().x, game->getApple().y);
    send(sock, cstr, 64, 0);
}

void Server::sendInteger(int num, int sock) {
    int newNum = htonl(num);
    send(sock, &newNum, sizeof newNum, 0);
}
