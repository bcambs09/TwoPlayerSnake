//
//  Game.hpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include "SnakePlayer.hpp"

class Game {
public:
    enum GameStatus {
        WAITING,
        ONGOING,
        P1_WIN,
        P2_WIN
    };
    
private:
    int width, height;
    GameStatus status;
    SnakePlayer *player1, *player2;
    Point apple;

public:
    Game(int width, int height);
    ~Game();
    
    int getWidth();
    int getHeight();
    int getScore(int playerNum);
    std::string getStatus(int playerNum);
    SnakePlayer* getPlayer(int playerNum);
    SnakePlayer* getPlayer1();
    SnakePlayer* getPlayer2();
    Point getApple();
    
    void setStatus(GameStatus s);
    void generateApple();
    void playFrame();
    int addNewPlayer();
    void removePlayer(int playerNum);
    bool inactive();
};

#endif /* Game_hpp */
