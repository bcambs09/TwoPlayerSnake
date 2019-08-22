//
//  SnakePlayer.hpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef SnakePlayer_hpp
#define SnakePlayer_hpp

#include <deque>

struct Point {
    int x, y;
};

bool compPoint(Point p1, Point p2);

bool pointInBounds(Point p, int w, int h);

class SnakePlayer {
public:
    enum Direction {
        UP = 0,
        DOWN = 1,
        RIGHT = 2,
        LEFT = 3
    };
private:
    std::deque<Point> body;

public:
    Direction direction;
    
    SnakePlayer(Direction startDirection, Point startPoint);
    
    void addFront();
    void popBack();
    void move();
    
    Point nextSpot();
    const char* getBody();
    int getScore();
};

#endif /* SnakePlayer_hpp */
