#pragma once
#include "Game.h"
#include "Square.h"

//
// the classic game of tic tac toe
//

//
// the main game class
//
class Connect4 : public Game
{
public:
    Connect4();
    ~Connect4();

    const static int CONNECT4_ROWS = 6;
    const static int CONNECT4_COLS = 7;

    // Base Game methods
    void        setUpBoard() override;
    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        stopGame() override;

	void        updateAI() override;
    bool        gameHasAI() override { return true; }
    Grid* getGrid() override {return _grid; }

    // Board Game methods
    bool        isColumnFull(int col);
    int         getLowestEmptyRow(int col);

private:

    // Player constants
    static const int RED_PLAYER = 0;
    static const int YELLOW_PLAYER = 1;

    // Game-specific methods
    Bit *       PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int index ) const;

    bool        isAIWinner();
    bool        isAIBoardFull (const std::string& state);
    int         AILowestRow(const std::string& state, int col);
    int         evaluateAIBoard(const std::string& state);
    int         negamax(std::string& state, int depth, int alpha, int beta, int playerNumber);
    std::vector<int>        getValidSquares(const std::string& state);
    int         scoreWindow(int AIPieces, int HumanPieces);
    int         assignScores(const std::string& state, int playerNumber);

    bool        checkForFour(int y, int x, int dy, int dx, Player* player);

    // Board representation
    Grid*        _grid;

    // Game state
};