//
//  main.cpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include <iostream>
#include "Server.hpp"
#include "Globals.hpp"

int main(int argc, const char * argv[]) {
    InitGame();
    Server s("7750");
    s.runServer();
    return 0;
}
