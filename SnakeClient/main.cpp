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
    User user("localhost", "7750");
    user.play();
    return 0;
}
