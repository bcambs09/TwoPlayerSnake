//
//  Game.cpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "User.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>

using namespace std::chrono;

User::User(const string& addr, const string& port) {
    client = new Client(addr, port);
    int width = client->getWidth();
    int height = client->getHeight();
    board = new Board(width, height);
}

void User::play() {
    while(true) {
        int dir = board->getDirection();
        if (dir != -1) {
            client->sendDirection(dir);
        }
        board->resetBoard('.');
        board->addSnake(client->getSelf(), 'O');
        pair<int, int> apple = client->getApple();
        board->addApple(apple.first, apple.second);
        board->printScore(client->getScore());
        board->updateBoard();
        string status = client->getStatus();
        if (status == "WIN") {
            board->youWin();
            usleep(3000000);
            break;
        } else if (status == "LOSE") {
            board->youLose();
            usleep(3000000);
            break;
        }
    }
}

User::~User() {
    delete board;
}
