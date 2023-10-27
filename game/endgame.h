/**
 * @file game.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the EndGame class,
 * which is used for games created for MiniMax in the last moves for a better performance
 * @copyright Copyright (c) 2023
 */



class EndGame {
    public:

    std::array<int, 81> board;

    int smallBoardSelected;
    int smallBoardOfLastMove;

    int lastMove;

    std::array<int, 9> smallBoards;

    int gameIsOver = -1;




    EndGame(std::array<int, 81> board, std::array<int, 9> smallBoards, int lastMove, int player, int move) {
        this->board = board;
        this->smallBoards = smallBoards;
        this->lastMove = lastMove;

        // MAKEMOVE

        // this simply updates the full board with the new move

        this->board[move] = player;

        // this stores the small board in which the next player will have to move

        smallBoardSelected = move % 9;

        // this stores the small board in which the current player just moved

        smallBoardOfLastMove = move / 9;


        // check if the small board is won by player

        if (IsSmallBoardWon(player)) {
            this->smallBoards[smallBoardOfLastMove] = player;

            // mark all cells in that board as not selectable

            for (int i = smallBoardOfLastMove * 9; i < (smallBoardOfLastMove + 1) * 9; i++) {
                if (this->board[i] == -1) this->board[i] = 2;
            }

            // check if game is won by player

            if (IsGameWon(player)) {
                gameIsOver = player;
                return;
            }
        } else if (IsSmallBoardDraw()) {
            this->smallBoards[smallBoardOfLastMove] = 2;
        }


        // if the small board in which the next player should move is already complete,
        // then the player will be able to move in any small board

        if (this->smallBoards[smallBoardSelected] != -1) smallBoardSelected = -1;
    }




    std::vector<int> GetAvailableMoves() {
        std::vector<int> moves;

        // if smallBoardSelected is -1 it means that the player can choose a move in every board

        if (smallBoardSelected == -1) {
            for (int i = 0; i < 81; i++) {
                if (board[i] == -1) moves.push_back(i);
            }
        } else {
            for (int i = smallBoardSelected * 9; i < (smallBoardSelected + 1) * 9; i++) {
                if (board[i] == -1) moves.push_back(i);
            }
        }

        return moves;
    }




    int MiniMax(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        EndGame endGame(board, smallBoards, lastMove, maximisingPlayer, move);


        // if search is over return evaluation

        if (endGame.gameIsOver != -1) {
            return (endGame.gameIsOver == 0) ? (WINVALUE + depth) : (-WINVALUE - depth);
        }

        bool tablecondition = endGame.smallBoardSelected == -1 && depth < TABLEDEPTH;

        if (tablecondition) {
            std::unordered_map<std::array<int, 81>, int, ArrayHasher>::iterator it = transpositionTable.find(board);

            if (it != transpositionTable.end()) {
                return it->second;
            }
        }

        // get available moves

        std::vector<int> possibleMoves = endGame.GetAvailableMoves();

        if (possibleMoves.size() == 0) return 0;
        if (possibleMoves.size() == 1) return endGame.MiniMax(possibleMoves[0], depth - 1, alpha, beta, !maximisingPlayer);


        // next MiniMax iteration
        // it loops through each move and calls MiniMax on the newly created instance of EndGame

        // since the program needs to create million of instances every computer move.
        // I believe more time could be saved doing something similar again,
        // either by making this class lighter or not creating new classes at all, rewriting entirely how this MiniMax works

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                maxEval = std::max(maxEval, endGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                alpha = std::max(alpha, maxEval);

                if (beta <= alpha) break;
            }

            if (tablecondition) transpositionTable[board] = maxEval;

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                minEval = std::min(minEval, endGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;
            }

            if (tablecondition) transpositionTable[board] = minEval;

            return minEval;
        }
    }




    bool IsSmallBoardWon(int playerSymbol) {
        // this loops through all possible small baords win combinations and checks whether the player has all three of the cells

        for (std::vector<int> winCombination : winMap) {
            if (
                board[winCombination[0] + 9 * smallBoardOfLastMove] == playerSymbol
             && board[winCombination[1] + 9 * smallBoardOfLastMove] == playerSymbol
             && board[winCombination[2] + 9 * smallBoardOfLastMove] == playerSymbol
            ) return true;
        }

        return false;
    }




    bool IsSmallBoardDraw() {
        // this simply checks if there are no more moves in the board in which the player just moved
        // if it finds one that is still -1, then the small board is still playable

        for (int i = smallBoardOfLastMove * 9; i < (smallBoardOfLastMove + 1) * 9; i++) {
            if (board[i] == -1) return false;
        }

        return true;
    }




    bool IsGameWon(int playerSymbol) {
        // this loops through all possible big board win combinations and checks whether the player has all three of the boards

        for (std::vector<int> winCombination : winMap) {
            if (
                smallBoards[winCombination[0]] == playerSymbol
             && smallBoards[winCombination[1]] == playerSymbol
             && smallBoards[winCombination[2]] == playerSymbol
            ) return true;
        }

        return false;
    }
};
