//
//  Game.cpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "Game.hpp"
#include "Globals.hpp"
#include <cstdlib>

using std::string;

Game::Game(int width, int height) : width(width), height(height),
player1(nullptr), player2(nullptr) {
    generateApple();
}

SnakePlayer* Game::getPlayer(int playerNum) {
    switch (playerNum) {
        case 1:
            return player1;
            break;

        case 2:
            return player2;
            break;

        default:
            break;
    }
    return nullptr;
}

void Game::playFrame() {
    
    m.lock();
    Point p1Try = player1->nextSpot();
    /*
    Point p2Try = player2->nextSpot();
    if (compPoint(p1Try, p2Try)) {
        
    }
    */
    if (!pointInBounds(p1Try, width, height)) {
        status = P2_WIN;
    }
    
    if (compPoint(p1Try, apple)) {
        player1->addFront();
        generateApple();
        if (game->getScore(1) >= 50) {
            status = P1_WIN;
        }
    } else {
        player1->move();
    }
    m.unlock();
}

int Game::addNewPlayer() {
    m.lock();
    int playerNum = -1;
    if (!player1) {
        player1 = new SnakePlayer(SnakePlayer::RIGHT, {0, 0});
        playerNum = 1;
        cv.notify_all();
    } else if (!player2) {
        player2 = new SnakePlayer(SnakePlayer::LEFT, {width-1, height-1});
        playerNum = 2;
        cv.notify_all();
    }
    m.unlock();
    return playerNum;
}

Game::~Game() {
    if (player1) {
        delete player1;
    }
    
    if (player2) {
        delete player2;
    }
}

int Game::getWidth() {
    m.lock();
    int w = width;
    m.unlock();
    return w;
}

int Game::getHeight() {
    m.lock();
    int h = height;
    m.unlock();
    return h;
}

Point Game::getApple() {
    m.lock();
    Point p = apple;
    m.unlock();
    return p;
}

void Game::generateApple() {
    int x = rand() % 20;
    int y = rand() % 8;
    apple.x = x;
    apple.y = y;
}

string Game::getStatus(int playerNum) {
    switch (status) {
        case ONGOING:
            return "ONGOING";
            break;
            
        case WAITING:
            return "WAITING";
            break;
            
        case P1_WIN:
            return playerNum == 1 ? "WIN" : "LOSE";
            break;
            
        case P2_WIN:
            return playerNum == 2 ? "WIN" : "LOSE";
            break;
            
        default:
            break;
    }
    return "WAITING";
}

void Game::setStatus(GameStatus s) {
    status = s;
}

SnakePlayer* Game::getPlayer1() {
    return player1;
}

SnakePlayer* Game::getPlayer2() {
    return player2;
}

int Game::getScore(int playerNum) {
    int score = player1->getScore();
    return score;
}

void Game::removePlayer(int playerNum) {
    m.lock();
    if (playerNum == 1) {
        delete player1;
        player1 = nullptr;
    } else if (playerNum == 2) {
        delete player2;
        player2 = nullptr;
    }
    m.unlock();
}

bool Game::inactive() {
    m.lock();
    bool rv = !player1 || status == P1_WIN || status == P2_WIN;
    m.unlock();
    return rv;
}
