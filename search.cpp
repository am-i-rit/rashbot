// search.cpp
#include "search.h"
#include "helper.h"
#include <algorithm>
#include <iostream>

struct scoredMove {
    Move move;
    int score; 
};

static bool higherScore(const scoredMove& first, const scoredMove& second) {
    return first.score > second.score;
}


const int Search::middlegameTables[6][64] = {
    // Pawn table
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    },

    // Knight table
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-20,-30,-30,-30,-30,-20,-50,
    },

    // Bishop table
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    },

    // rook table
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    },

    // queen table
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    },

    // king table
    {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 25,  -20,  0, 10, 30, 20
    }
};

const int Search::endgameTables[6][64] = {
    // pawn table
    {


    },

    // knight table
    {

    },

    // bishop table
    {

    },

    // rook table
    {

    },

    // queen table
    {

    },

    // king table
    {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    }
};

int Search::scoreMove(const Chessboard& board, const Move& move) const
{   
    static int orderValues[6] = {100, 300, 320, 500, 900, 0};
    int score = 0;

    int attacker = board.getPiece(move.from);
    int attackerType = attacker % 6;

    // score promotions highest
    if (move.promotion != EMPTY) {
        int promotionType = move.promotion % 6;
        score += 20000 + orderValues[promotionType];
    }

    if (board.isCapture(move)) {
        int victim = board.getPiece(move.to);
        int victimType;
        if (victim == EMPTY) {
            victimType = 0;
        }
        else {
            victimType = victim % 6;
        }
        
        score += 10000 + 10 * orderValues[victimType] - orderValues[attackerType];
    }
    return score;
}

void Search::orderMoves(const Chessboard &board, Move *moves, int numMoves) const
{
    scoredMove scoredMoves[256];

    // assign a score to each move
    for (int i = 0; i < numMoves; ++i)
    {
        scoredMoves[i].move = moves[i];
        scoredMoves[i].score = scoreMove(board, moves[i]);
    }

    std::stable_sort(scoredMoves, scoredMoves + numMoves, higherScore);

    // copy moves back
    for (int i = 0; i < numMoves; ++i)
    {
        moves[i] = scoredMoves[i].move;
    }
    
}

void Search::moveToFront(Move* moves, int numMoves, const Move& preferredMove) const
{
    if (preferredMove.from >= 64)
    {
        return;
    }

    for (int i = 0; i < numMoves; ++i)
    {
        if (moves[i] == preferredMove)
        {
            Move temporaryMove = moves[0];
            moves[0] = moves[i];
            moves[i] = temporaryMove;

            return;
        }
    }
}

int Search::evaluate(const Chessboard &board) const
{
    // returns an evaluation score relative to white
    // the sign of the score will be made correct in the negamax function
    static constexpr int pieceValues[12] = {
        100, 300, 320, 500, 900, 0, 
        -100, -300, -320, -500, -900, 0
    }; 

    int evaluation = 0;

    for(int i = 0; i < 12; ++i)
    {
        uint64_t bb = board.getBitboard(i);
        evaluation += popcount(bb) * pieceValues[i];

        while (bb) {
            int square = lsbIndex(bb);
            bb &= bb - 1;

            if (i < 6) {
                evaluation += middlegameTables[i][square];
            }
            else {
                int pieceType = i - 6;
                int mirroredSquare = square ^ 56;

                evaluation -= middlegameTables[pieceType][mirroredSquare];
            }
        }
    }   
    return evaluation;
}

int Search::negamax(Chessboard& board, int depth, int alpha, int beta)
{
    ++nodes;

    if ((nodes & 2047ULL) == 0 && shouldStop())
    {
        searchStopped = true;
        return 0;
    }

    if (board.isDrawn())
    {
        return 0;
    }

    // depth limit
    if (depth <= 0)
    {
        return quiesce(board, alpha, beta);
    }

    Move moves[256];
    int numMoves;
    board.pseudoMoves(moves, numMoves);

    // sort moves to search promotions/captures first
    orderMoves(board, moves, numMoves);

    int legalMoves = 0;

    for (int i = 0; i < numMoves; ++i)
    {
        board.move(moves[i]);

        bool illegal;

        if (board.getTurn() == WHITE)
        {
            illegal = board.isAttacked(
                board.bKingSquare(),
                WHITE
            );
        }
        else
        {
            illegal = board.isAttacked(
                board.wKingSquare(),
                BLACK
            );
        }

        if (!illegal)
        {
            ++legalMoves;

            int score = -negamax(board, depth - 1, -beta, -alpha);

            if (score > alpha)
            {
                alpha = score;
            }
        }

        board.undo();
        
        if (searchStopped) {
                return 0;
        }        

        if (alpha >= beta) break;
    }

    if (legalMoves == 0)
    {
        bool inCheck;

        if (board.getTurn() == WHITE)
        {
            inCheck = board.isAttacked(
                board.wKingSquare(),
                BLACK
            );
        }
        else
        {
            inCheck = board.isAttacked(
                board.bKingSquare(),
                WHITE
            );
        }

        if (inCheck)
        {
            return -100000 - depth;
        }

        return 0; // stalemate
    }

    return alpha;
}

int Search::quiesce(Chessboard& board, int alpha, int beta) {
    // increment nodes
    ++nodes;

    if ((nodes & 2047ULL) == 0 && shouldStop())
    {
        searchStopped = true;
        return 0;
    }

    if (board.isDrawn())
    {
        return 0;
    }

    // make sure we're not in check first
    bool inCheck;
    if (board.getTurn() == WHITE) {
        inCheck = board.isAttacked(board.wKingSquare(), BLACK);
    }
    else {
        inCheck = board.isAttacked(board.bKingSquare(), WHITE);
    }

    // stand pat
    if (!inCheck) {
        int staticEval;
        if (board.getTurn() == WHITE) {
            staticEval = evaluate(board);
        }
        else {
            staticEval = -evaluate(board);
        }

        if (staticEval >= beta) {
            return staticEval;
        }

        if (staticEval >= alpha) {
            alpha = staticEval;
        }
    }

    Move moves[256];
    int numMoves;
    board.pseudoMoves(moves, numMoves);

    if (!inCheck)
    {
        int tacticalMoves = 0;

        for (int i = 0; i < numMoves; ++i)
        {
            if (board.isCapture(moves[i]) ||
                moves[i].promotion != EMPTY)
            {
                moves[tacticalMoves] = moves[i];
                ++tacticalMoves;
            }
        }

        numMoves = tacticalMoves;
    }

    orderMoves(board, moves, numMoves);

    int legalMoves = 0;

    for (int i = 0; i < numMoves; ++i) {
        if (!inCheck && !board.isCapture(moves[i]) && moves[i].promotion == EMPTY) {
            continue;
        }

        board.move(moves[i]);
        bool illegal;

        if (board.getTurn() == WHITE)
            illegal = board.isAttacked(board.bKingSquare(), WHITE);
        else
            illegal = board.isAttacked(board.wKingSquare(), BLACK);

        if (!illegal)
        {
            ++legalMoves;

            int score = -quiesce(board, -beta, -alpha);

            board.undo();

            if (searchStopped) {
                return 0;
            }

            if (score >= beta)
                return score;

            if (score > alpha)
                alpha = score;
        }
        else
        {
            board.undo();
        }
    }

    if (inCheck && legalMoves == 0) {
        return -100000;
    }

    return alpha;
}

SearchResult Search::searchRoot(Chessboard& board, int depth, const Move& preferredMove) 
{
    static int NEGATIVE_INFINITY = -1000000;
    static int POSITIVE_INFINITY = 1000000;

    Move moves[256];
    int numMoves;

    board.pseudoMoves(moves, numMoves);

    orderMoves(board, moves, numMoves);
    moveToFront(moves, numMoves, preferredMove);

    Move bestMove = {64, 64, EMPTY};
    int alpha = NEGATIVE_INFINITY;
    bool foundMove = false;

    for (int i = 0; i < numMoves; ++i)
    {
        if (shouldStop())
        {
            searchStopped = true;
            break;
        }

        board.move(moves[i]);

        bool illegal;

        if (board.getTurn() == WHITE)
        {
            illegal = board.isAttacked(board.bKingSquare(), WHITE);
        }
        else
        {
            illegal = board.isAttacked(board.wKingSquare(), BLACK);
        }

        int score = 0;

        if (!illegal)
        {
            score = -negamax(board, depth - 1, -POSITIVE_INFINITY, -alpha);
        }

        board.undo();

        if (searchStopped) {
            break;
        }

        if (!illegal && (!foundMove || score > alpha)) {
            alpha = score;
            bestMove = moves[i];
            foundMove = true;
        }

    }

    if (!foundMove)
    {
        return {
            {64, 64, EMPTY}, 0, false};
    }

    return {
        bestMove, alpha, true};
}

Move Search::findBestMove(Chessboard& board, int maximumDepth, int timeLimit)
{
    searchStopped = false;
    nodes = 0;
    completedDepth = 0;
    bestScore = 0;

    timeLimitEnabled = timeLimit > 0;

    if (timeLimitEnabled) {
        deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeLimit);
    }

    Move bestMove = {64, 64, EMPTY};

    // generate a fallback move just in case engine can't complete depth 1
    Move fallbackMoves[256];
    int numFallbackMoves;

    board.pseudoMoves(fallbackMoves, numFallbackMoves);

    for (int i = 0; i < numFallbackMoves; ++i)
    {
        board.move(fallbackMoves[i]);

        bool illegal;

        if (board.getTurn() == WHITE)
        {
            // Black just moved
            illegal = board.isAttacked(board.bKingSquare(), WHITE);
        }
        else
        {
            // White just moved
            illegal = board.isAttacked(board.wKingSquare(), BLACK);
        }

        board.undo();

        if (!illegal)
        {
            bestMove = fallbackMoves[i];
            break;
        }
    }

    for (int currentDepth = 1; currentDepth <= maximumDepth; ++currentDepth)
    {
        SearchResult result = searchRoot(board, currentDepth, bestMove);

        if (searchStopped)
            break;

        if (!result.foundMove)
            break;

        bestMove = result.move;
        completedDepth = currentDepth;
        bestScore = result.score;

        /* std::cerr << "Completed depth: " << currentDepth
                  << " score: " << result.score
                  << " nodes: " << nodes
                  << '\n'; */
    }

    return bestMove;
}

int Search::getCompletedDepth() const
{
    return completedDepth;
}

int Search::getBestScore() const
{
    return bestScore;
}

bool Search::shouldStop()
{
    if (stopRequested.load(std::memory_order_relaxed))
        return true;

    return timeLimitEnabled && std::chrono::steady_clock::now() >= deadline;
}

void Search::stop()
{
    stopRequested.store(true, std::memory_order_relaxed);
}

void Search::resetStop()
{
    stopRequested.store(false, std::memory_order_relaxed);
}

uint64_t Search::getNodes() const
{
    return nodes;
}

