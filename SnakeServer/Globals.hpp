//
//  Globals.hpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/21/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Globals_hpp
#define Globals_hpp

#include "Game.hpp"
#include <mutex>
#include <condition_variable>

extern Game* game;
extern std::mutex m;
extern std::mutex gameStart;
extern std::condition_variable cv;

void InitGame();

#endif /* Globals_hpp */
