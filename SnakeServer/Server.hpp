//
//  Server.hpp
//  SnakeServer
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp
#include <string>
#include "Globals.hpp"

class Server {
    std::string port;
    int sock;
    static void communicateWithClient(int sock);
    static void respondToMessage(const char* msg, int playerNum, int sock);
    static void sendInteger(int num, int sock);
    static void sendString(const std::string& str, int sock);
    static void sendSnake(int snakeNum, int sock);
    static void sendApple(int sock);
    static void playGame();
    static void sleep(double timePassed);
    const static int FPS = 6;

public:
    Server(const std::string& port);
    void runServer();
};

#endif /* Server_hpp */
