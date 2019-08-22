//
//  Board.hpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <string>

class Board {
    int width, height;
    
public:
    Board(int width, int height);
    ~Board();
    
    void setCoord(int x, int y, char c);
    void addSnake(const std::string& str, char c);
    void resetBoard(char c);
    void updateBoard();
    void addApple(int x, int y);
    void youWin();
    void youLose();
    void printScore(int score);
    int getDirection();
};

#endif /* Board_hpp */
