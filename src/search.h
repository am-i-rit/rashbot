#pragma once
#include <cstdint>
#include "chessboard.h"
#include <atomic>
#include <chrono>

struct SearchResult
{
    Move move;
    int score;
    bool foundMove;
};

class Search
{
private:

    static const int middlegameTables[6][64];
    static const int endgameTables[6][64];
    
    int scoreMove(const Chessboard& board, const Move &move) const;
    void orderMoves(const Chessboard &board, Move* moves, int numMoves) const;

    std::uint64_t nodes;
    int evaluate(const Chessboard& board) const;    
    int negamax(Chessboard& board, int depth, int alpha, int beta);

    int quiesce(Chessboard &board, int alpha, int beta);

    void moveToFront(Move* moves, int numMoves, const Move& preferredMove) const;

    SearchResult searchRoot(Chessboard& board, int depth, const Move& preferredMove);

    std::atomic<bool> stopRequested{false};

    bool searchStopped = false;
    bool timeLimitEnabled = false;

    std::chrono::steady_clock::time_point deadline;

    bool shouldStop();

    int completedDepth = 0;
    int bestScore = 0;

public:

    Move findBestMove(Chessboard& board, int depth, int timeLimit);
    std::uint64_t getNodes() const;

    int getCompletedDepth() const;
    int getBestScore() const;
    
    void stop();
    void resetStop();


};