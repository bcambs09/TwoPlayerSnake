//
//  main.cpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include <iostream>
#include <ncurses.h>
#include "User.hpp"

int main(int argc, const char * argv[]) {
    string host = "localhost";
    string port = "7750";
    if (argc >= 2) {
        host = string(argv[1]);
    }
    if (argc >= 3) {
        port = string(argv[2]);
    }
    User user(host, port);
    user.play();
    return 0;
}
