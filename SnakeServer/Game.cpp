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

Game::GameStatus Game::getStatus() {
    return status;
}

void Game::playFrame() {
    
    m.lock();
    Point p1Try = player1->nextSpot();
    Point p2Try = player2->nextSpot();
    if (compPoint(p1Try, p2Try)) {
        status = TIE;
        m.unlock();
        return;
    }
    
    if (compPoint(p1Try, apple)) {
        generateApple();
        if (game->getScore(1) >= 50) {
            status = P1_WIN;
            m.unlock();
            return;
        }
        player2->popBack();
    } else if (compPoint(p2Try, apple)) {
        generateApple();
        if (game->getScore(2) >= 50) {
            status = P2_WIN;
            m.unlock();
            return;
        }
        player1->popBack();
    } else {
        player1->popBack();
        player2->popBack();
    }
    
    bool p1Lost = !pointInBounds(p1Try, width, height)
        || player1->onSnake(p1Try) || player2->onSnake(p1Try);
    bool p2Lost = !pointInBounds(p2Try, width, height)
        || player1->onSnake(p2Try) || player2->onSnake(p2Try);
    
    if (p1Lost && p2Lost) {
        status = TIE;
    } else if (p1Lost) {
        status = P2_WIN;
    } else if (p2Lost) {
        status = P1_WIN;
    }
    
    player1->addFront();
    player2->addFront();
    
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

        case TIE:
            return "TIE";
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
    bool rv = !player1 || !player2 || status == P1_WIN || status == P2_WIN;
    m.unlock();
    return rv;
}

void Game::reset() {
    m.lock();
    if (player1) {
        delete player1;
        player1 = new SnakePlayer(SnakePlayer::RIGHT, {0, 0});
    }
    if (player2) {
        delete player2;
        player2 = new SnakePlayer(SnakePlayer::LEFT, {width-1, height-1});
    }
    status = WAITING;
    m.unlock();
}
