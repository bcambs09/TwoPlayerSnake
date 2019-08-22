//
//  Board.cpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#include "Board.hpp"
#include <ncurses.h>
#include <string>
#include <sstream>

using std::string;
using std::istringstream;

Board::Board(int width, int height) : width(width), height(height) {
    // Initialize ncurses
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    clear();
    resetBoard('.');
}

void Board::resetBoard(char c) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            setCoord(x, y, c);
        }
    }
}

void Board::printScore(int score) {
    for (int x = 0; x < 50; ++x) {
        setCoord(x, height, ' ');
    }
    mvprintw(height, 0, "Score: %i", score);
}

void Board::youWin() {
    resetBoard(' ');
    mvprintw(0, 0, "YOU WIN!");
    updateBoard();
}

void Board::youLose() {
    resetBoard(' ');
    mvprintw(0, 0, "YOU LOSE");
    updateBoard();
}

void Board::setCoord(int x, int y, char c) {
    mvaddch(y, x, c);
}

void Board::updateBoard() {
    refresh();
}

void Board::addSnake(const std::string &str, char c){
    istringstream is(str);
    string x, y;
    while (getline(is, x, ' ') && getline(is, y, ' ')) {
        setCoord(stoi(x), stoi(y), c);
    }
}

void Board::addApple(int x, int y) {
    setCoord(x, y, '*');
}

int Board::getDirection() {
    int ch = getch();
    if (ch == 'i') {
        return 0;
    } else if (ch == 'k') {
        return 1;
    } else if (ch == 'l') {
        return 2;
    } else if (ch == 'j') {
        return 3;
    }
    return -1;
}

Board::~Board() {
    endwin();
}
