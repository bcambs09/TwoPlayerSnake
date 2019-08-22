//
//  Game.hpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include "Board.hpp"
#include "Client.hpp"
#include <string>

using namespace std;

class User {
    Client* client;
    Board* board;
public:
    User(const string& addr, const string& port);
    ~User();
    void connect();
    void play();
    void sleep(double micros);
};

#endif /* Game_hpp */
