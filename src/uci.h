#pragma once

#include <string>
#include <thread>
#include "chessboard.h"
#include "search.h"


class UCI
{
private:
    Chessboard board;
    Search search;

    Move parseMove(const std::string& text);
    std::string moveToString(const Move& move) const;

    void setPosition(const std::string& command);
    void go(const std::string& command);

    std::thread searchThread;

    void stopSearch();

    

public:
    void run();

    ~UCI();

};