//
//  Globals.cpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "Globals.hpp"

Game* game = nullptr;
std::mutex m;
std::mutex gameStart;
std::condition_variable cv;

void InitGame() {
    if (!game) {
        game = new Game(20, 8);
    }
}
