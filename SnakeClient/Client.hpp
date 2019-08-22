//
//  Client.hpp
//  Snake
//
//  Created by Brendan Campbell on 8/20/19.
//  Copyright © 2019 Wolverine Games LLC. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp

#include <string>

using namespace std;

class Client {
    string addr;
    string port;
    int sock;
    int connectToServer();
    int getIntFromServer();
    int getSnakeFromServer();
public:
    Client(const string& addr, const string& port);
    ~Client();
    void sendToServer(const string& str);
    int sendDirection(int direction);
    int getWidth();
    int getHeight();
    int getScore();
    pair<int, int> getApple();
    string getSelf();
    string getStatus();
};

#endif /* Client_hpp */
