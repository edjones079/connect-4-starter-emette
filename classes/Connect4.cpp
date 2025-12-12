#include "Connect4.h"
#include <print>
#include <string>

Connect4::Connect4() : Game()
{
    _grid = new Grid(CONNECT4_ROWS, CONNECT4_COLS);
}

Connect4::~Connect4()
{
    delete _grid;
}

Bit *Connect4::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == AI_PLAYER ? "yellow.png" : "red.png");
    bit->setOwner(getPlayerAt(playerNumber == AI_PLAYER ? 1 : 0));
    return bit;
}

void Connect4::setUpBoard()
{

    setNumberOfPlayers(2);

    if (gameHasAI())
        setAIPlayer(AI_PLAYER);

    // Set board to 6, 7.

    _gameOptions.rowY = 6;
    _gameOptions.rowX = 7;

    // then we need to setup our 3x3 array in _grid with the correct position of the square, and load the "square.png" sprite for each square
    // we will use the initHolder function on each square to do this

    _grid->initializeSquares(80, "square.png");

    // finally we should call startGame to get everything going

    startGame();

    return;
}

bool Connect4::actionForEmptyHolder(BitHolder &holder)
{

    ChessSquare *clickedSquare = dynamic_cast<ChessSquare *>(&holder);

    if (!clickedSquare)
    {
        return false;
    }

    // std::cout << "Row: " << clickedSquare->getRow() << " Column: " << clickedSquare->getColumn() << std::endl;

    int col = clickedSquare->getColumn();

    if (col == -1)
    {
        return false;
    }

    if (isColumnFull(col))
    {
        return false;
    }

    int row = getLowestEmptyRow(col);
    if (row == -1)
    {
        return false;
    }

    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);

    if (bit)
    {
        ChessSquare *topSquare = _grid->getSquare(0, col);
        ChessSquare *targetSquare = _grid->getSquare(row, col);

        // std::cout << "Top Square: " << topSquare->getRow() << ", " << topSquare->getColumn() << std::endl;
        // std::cout << "Target Square: " << targetSquare->getRow() << ", " << targetSquare->getColumn() << std::endl;

        if (row > 0)
        {
            bit->setPosition(topSquare->getPosition());
            bit->moveTo(targetSquare->getPosition());
            targetSquare->setBit(bit);
            endTurn();
        }
        else
        {
            bit->setPosition(targetSquare->getPosition());
            targetSquare->setBit(bit);
            endTurn();
        }
    }

    return true;
}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // you can't move anything in connect-4
    return false;
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    // you can't move anything in connect-4
    return false;
}

std::string Connect4::initialStateString()
{
    return "000000000000000000000000000000000000000000";
}

std::string Connect4::stateString()
{

    std::string state = "";

    for (int y = 0; y < _grid->getHeight(); y++)
    {
        for (int x = 0; x < _grid->getWidth(); x++)
        {
            Bit *bit = _grid->getSquare(y, x)->bit();
            if (!bit)
            {
                state += '0';
            }
            else
            {
                Player *player = bit->getOwner();

                state += std::to_string(player->playerNumber() + 1);
            }
        }
    }

    // std::cout << state << std::endl;

    return state;
}

void Connect4::setStateString(const std::string &s)
{

    for (int y = 0; y < _grid->getHeight(); y++)
    {
        for (int x = 0; x < _grid->getWidth(); x++)
        {
            int index = y * _grid->getWidth() + x;

            int playerNumber = s[index] - '0';
            ChessSquare *square = _grid->getSquare(y, x);

            if (playerNumber)
            {
                square->setBit(PieceForPlayer(playerNumber));
            }
            else
            {
                square->setBit(nullptr);
            }
        }
    }
    return;
}

void Connect4::stopGame()
{

    for (int y = 0; y < Connect4::CONNECT4_ROWS; y++)
    {
        for (int x = 0; x < Connect4::CONNECT4_COLS; x++)
        {
            _grid->getSquare(y, x)->destroyBit();
        }
    }
    return;
}

Player *Connect4::ownerAt(int index) const
{
    // std::cout << "Index: " << index << std::endl;
    int y = index / 7;
    int x = index % 7;

    if (_grid->getSquare(y, x)->bit())
    {
        return _grid->getSquare(y, x)->bit()->getOwner();
    }

    // std::cout << "Owner got!" << std::endl;

    return nullptr;
}

bool Connect4::checkForFour(int y, int x, int dy, int dx, Player *player)
{

    // For each "potential" piece in the consecutive line . . .
    for (int i = 0; i < 4; i++)
    {

        //std::cout << "Row: " << y << ", Column: " << x << std::endl;
        //std::cout << "DY: " << dy << ", DX: " << dx << std::endl;

        int y2 = y + (i * dy);
        int x2 = x + (i * dx);

        //std::cout << "Operation performed!" << std::endl;

        //std::cout << "Row: " << y << ", Column: " << x << std::endl;
        //std::cout << "DY: " << dy << ", DX: " << dx << std::endl;

        Player *p = ownerAt(_grid->getIndex(y2, x2));
        if (player != p)
        {
            return false;
        }

    }

    return true;
}

Player *Connect4::checkForWinner()
{

    for (int y = 0; y < Connect4::CONNECT4_ROWS; y++)
    {
        for (int x = 0; x < Connect4::CONNECT4_COLS; x++)
        {

            //std::cout << "Row: " << y << ", Column: " << x << std::endl;
            //std::cout << _grid->getIndex(y, x) << std::endl;

            Player *player = ownerAt(_grid->getIndex(y, x));

            if (player)
            {

                // Check Horizontals

                //std::cout << std::endl;
                //std::cout << "Check for four!" << std::endl;
                //std::cout << std::endl;

                std::cout << std::endl;
                std::cout << "Current Player: " << player->playerNumber() << " isAI: " << player->isAIPlayer() << std::endl;
                std::cout << std::endl;

                if (x <= Connect4::CONNECT4_COLS - 4 && checkForFour(y, x, 0, 1, player))
                {
                    std::cout << "HORIZONAL WIN 1" << std::endl;
                    return player;
                }
                if (x >= Connect4::CONNECT4_COLS - 4 && checkForFour(y, x, 0, -1, player))
                {
                    std::cout << "HORIZONAL WIN 2" << std::endl;
                    return player;
                }

                // Check Vertical

                if (y <= Connect4::CONNECT4_ROWS - 4 && checkForFour(y, x, 1, 0, player))
                {
                    std::cout << "VERTICAL WIN" << std::endl;
                    return player;
                }

                // Check Diagonals

                if (x <= Connect4::CONNECT4_COLS - 4 && y <= CONNECT4_ROWS - 4 && checkForFour(y, x, 1, 1, player))
                {
                    std::cout << "DIAGONAL WIN 1" << std::endl;
                    return player;
                }

                if (x >= Connect4::CONNECT4_COLS - 4 && y >= CONNECT4_ROWS - 4 && checkForFour(y, x, -1, -1, player))
                {
                    std::cout << "DIAGONAL WIN 2" << std::endl;
                    return player;
                }

                if (x <= Connect4::CONNECT4_COLS - 4 && y >= CONNECT4_ROWS - 4 && checkForFour(y, x, -1, 1, player))
                {
                    std::cout << "DIAGONAL WIN 3" << std::endl;
                    return player;
                }

                if (x >= Connect4::CONNECT4_COLS - 4 && y <= CONNECT4_ROWS - 4 && checkForFour(y, x, 1, -1, player))
                {
                    std::cout << "DIAGONAL WIN 4" << std::endl;
                    return player;
                }
            }
        }
    }

    return nullptr;
}

bool Connect4::checkForDraw()
{
    for (int x = 0; x < Connect4::CONNECT4_COLS; x++)
    {
        if (!isColumnFull(x))
        {
            return false;
        }
    }

    if (checkForWinner() != nullptr)
    {
        return false;
    }

    return true;
}

void Connect4::updateAI()
{

    int alpha = -99999;
    int beta = 99999;

    int score = -1000;
    int bestSquare = -1;

    std::string state = stateString();

    std::vector<int> validSquares = getValidSquares(state);

    // For every valid square . . .

    for (int i = 0; i < validSquares.size(); i++)
    {
        int s = validSquares[i];

        // If the square is empty . . .

        if (state[s] == '0')
        {

            // AI makes a temporary move . . .

            state[s] = '2';

            // Perform negamax on opposing player . . .

            int eval = -negamax(state, 0, alpha, beta, HUMAN_PLAYER);

            // If the new move is better than the current best move . . .
            if (eval >= score)
            {
                score = eval;
                // std::cout << s << std::endl;
                bestSquare = s;
            }

            // Undo the temporary move

            state[s] = '0';
        }
    }

    // If a best move is found . . .

    if (bestSquare != -1)
    {
        int x = bestSquare % 7;
        int y = bestSquare / 7;
        BitHolder *holder = dynamic_cast<BitHolder *>(_grid->getSquare(y, x));
        actionForEmptyHolder(*holder);
    }

    // Check only valid squares (7 at most)

    return;
}

int Connect4::evaluateAIBoard(const std::string &state)
{

    const int rows = Connect4::CONNECT4_ROWS;
    const int cols = Connect4::CONNECT4_COLS;

    int totalScore = 0;

    // ALL SCORING WINDOWS 4x4!

    // Center Column

    int centerCol = cols / 2;
    for (int r = 0; r < rows; r++)
    {
        if (state[r * cols + centerCol] == '2')
        {
            totalScore += 3;
        }
        else if (state[r * cols + centerCol] == '1')
        {
            totalScore -= 3;
        }
    }

    // Horizontal Windows

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c <= cols - 4; c++)
        {
            int AIPieces = 0;
            int HumanPieces = 0;
            for (int i = 0; i < 4; i++)
            {
                if (state[r * cols + (c + i)] == '2')
                {
                    AIPieces++;
                }
                else if (state[r * cols + (c + i)] == '1')
                {
                    HumanPieces++;
                }
            }

            totalScore += scoreWindow(AIPieces, HumanPieces);
        }
    }

    // Vertical Windows

    for (int c = 0; c < cols; c++)
    {
        for (int r = 0; r <= rows - 4; r++)
        {
            int AIPieces = 0;
            int HumanPieces = 0;
            for (int i = 0; i < 4; i++)
            {
                if (state[(r + i) * cols + c] == '2')
                {
                    AIPieces++;
                }
                else if (state[(r + i) * cols + c] == '1')
                {
                    HumanPieces++;
                }
            }

            totalScore += scoreWindow(AIPieces, HumanPieces);
        }
    }

    // Positive Diagonal (/) Windows

    for (int r = 0; r <= rows - 4; r++)
    {
        for (int c = 0; c <= cols - 4; c++)
        {
            int AIPieces = 0;
            int HumanPieces = 0;
            for (int i = 0; i < 4; i++)
            {
                if (state[(r + i) * cols + (c + i)] == '2')
                {
                    AIPieces++;
                }
                else if (state[(r + i) * cols + (c + i)] == '1')
                {
                    HumanPieces++;
                }
            }

            totalScore += scoreWindow(AIPieces, HumanPieces);
        }
    }

    // Negative Diagonal (\) Windows
    for (int r = 3; r < rows; r++)
    {
        for (int c = 0; c <= cols - 4; c++)
        {
            int AIPieces = 0;
            int HumanPieces = 0;
            for (int i = 0; i < 4; i++)
            {
                if (state[(r - i) * cols + (c + i)] == '2')
                {
                    AIPieces++;
                }
                else if (state[(r - i) * cols + (c + i)] == '1')
                {
                    HumanPieces++;
                }
            }

            totalScore += scoreWindow(AIPieces, HumanPieces);
        }
    }

    return totalScore;
}

int Connect4::scoreWindow(int AIPieces, int HumanPieces)
{
    int score = 0;

    if (AIPieces == 4)
    {
        score += 10000;
    }
    else if (AIPieces == 3 && HumanPieces == 0)
    {
        score += 100;
    }
    else if (AIPieces == 2 && HumanPieces == 0)
    {
        score += 10;
    }

    if (HumanPieces == 4)
    {
        score -= 10000;
    }
    else if (HumanPieces == 3 && AIPieces == 0)
    {
        score -= 100;
    }
    else if (HumanPieces == 2 && AIPieces == 0)
    {
        score -= 10;
    }

    return score;
}

int Connect4::assignScores(const std::string &state, int playerNumber)
{
}

std::vector<int> Connect4::getValidSquares(const std::string &state)
{
    // std::cout << _grid->getWidth() << std::endl;
    std::vector<int> validSquares;

    for (int x = 0; x < _grid->getWidth(); x++)
    {
        for (int y = _grid->getHeight() - 1; y >= 0; y--)
        {
            int index = _grid->getIndex(y, x);

            if (state[index] == '0')
            {
                validSquares.push_back(index);
                break;
            }
        }
    }

    return validSquares;
}

bool Connect4::isAIWinner()
{
    Player *player = checkForWinner();

    // std::cout << player << std::endl;
    // std::cout << "Human: " << HUMAN_PLAYER << " AI: " << AI_PLAYER << std::endl;

    if (player && player->playerNumber() == AI_PLAYER)
    {
        return true;
    }

    return false;
}

bool Connect4::isAIBoardFull(const std::string &state)
{
    // Checks if the board is full

    return state.find('0') == std::string::npos;
}

int Connect4::AILowestRow(const std::string &state, int col)
{
    return -1;
}

int Connect4::negamax(std::string &state, int depth, int alpha, int beta, int playerNumber)
{

    int bestScore = evaluateAIBoard(state);

    // Check terminal states

    if (isAIWinner())
    {
        return bestScore * playerNumber;
    }

    if (depth == 2 || isAIBoardFull(state))
    {
        return bestScore * playerNumber;
    }

    if (abs(bestScore) >= 10000)
    {
        return bestScore * playerNumber;
    }

    // We only have to check, at most, SEVEN initial squares!

    std::vector<int> validSquares = getValidSquares(state);

    int bestEval = -99999;
    for (int x = 0; x < validSquares.size(); x++)
    {
        // If the current column is not full . . .

        if (state[x] == '0')
        {
            // Get the lowest row index

            int index = validSquares[x];

            // Make the move & call negamax for the opposing player's best move.

            state[index] = playerNumber == AI_PLAYER ? '2' : '1';

            int eval = -negamax(state, depth + 1, alpha, beta, -playerNumber);
            bestEval = std::max(bestEval, eval);
            alpha = std::max(alpha, eval);

            if (beta <= alpha)
            {
                break;
            }

            // Undo move

            state[index] = '0';
        }
    }

    return alpha;
}

bool Connect4::isColumnFull(int col)
{
    ChessSquare *topSquare = _grid->getSquare(0, col);

    if (!topSquare)
    {
        return false;
    }

    if (!topSquare->bit())
    {
        return false;
    }

    return true;
}

int Connect4::getLowestEmptyRow(int col)
{
    for (int row = _grid->getHeight() - 1; row >= 0; row--)
    {
        ChessSquare *bottomSquare = _grid->getSquare(row, col);
        if (!bottomSquare->bit())
        {
            return row;
        }
    }

    return -1;
}
