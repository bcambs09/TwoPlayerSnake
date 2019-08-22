//
//  SnakePlayer.cpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "SnakePlayer.hpp"
#include "Globals.hpp"
#include <cstdlib>

SnakePlayer::SnakePlayer(Direction startDirection, Point startPoint) : direction(startDirection) {
    body.push_back(startPoint);
}

void SnakePlayer::addFront() {
    body.push_front(nextSpot());
}

void SnakePlayer::popBack() {
    body.pop_back();
}

void SnakePlayer::move() {
    addFront();
    popBack();
}

Point SnakePlayer::nextSpot() {
    Point newPoint = body.front();
    if (direction == UP) {
        newPoint.y -= 1;
    } else if (direction == DOWN) {
        newPoint.y += 1;
    } else if (direction == RIGHT) {
        newPoint.x += 1;
    } else if (direction == LEFT) {
        newPoint.x -= 1;
    }
    return newPoint;
}

const char* SnakePlayer::getBody() {
    char* snake = (char*) malloc(256);
    char* head = snake;
    char buff[6];
    m.lock();
    for (int i = 0; i < body.size(); ++i) {
        sprintf(buff, "%d ", body[i].x);
        strcpy(head, buff);
        head = head + strlen(buff);
        sprintf(buff, "%d ", body[i].y);
        strcpy(head, buff);
        head = head + strlen(buff);
    }
    m.unlock();
    printf("%s\n", snake);
    return snake;
}

int SnakePlayer::getScore() {
    return 10 * ((int) body.size() - 1);
}

bool compPoint(Point p1, Point p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

bool pointInBounds(Point p, int w, int h) {
    return p.x >= 0 && p.y >= 0 && p.x < w && p.y < h;
}
