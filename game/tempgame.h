/**
 * @file game.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the TempGame class, which is used for games created for MiniMax
 * @copyright Copyright (c) 2023
 */



class TempGame {
    public:

    std::array<int, 81> board;
    int player;

    int smallBoardSelected = -1;
    int smallBoardOfLastMove = -1;

    int lastMove;

    std::array<int, 9> smallBoards;

    int gameIsOver = -1;



    TempGame(std::array<int, 81> board, std::array<int, 9> smallBoards, int lastMove, int player) {
        this->board = board;
        this->smallBoards = smallBoards;
        this->lastMove = lastMove;
        this->player = player;
    }




    std::vector<int> GetAvailableMoves() {
        std::vector<int> moves;

        // if smallBoardSelected is -1 it means that the player can choose a move in every board

        if (smallBoardSelected != -1) {
            for (int i = smallBoardSelected * 9; i < (smallBoardSelected + 1) * 9; i++) {
                if (board[i] == -1) moves.push_back(i);
            }
        } else {
            for (int i = 0; i < 81; i++) {
                if (board[i] == -1) moves.push_back(i);
            }
        }

        return moves;
    }




    std::vector<int> GetSmallBoardValues() {
        std::vector<int> newSmallBoardsPoints = smallBoardPointsBase;

        for (std::vector<int> comb : winMap) {
            if (smallBoards[comb[0]] + smallBoards[comb[1]] + smallBoards[comb[2]] < 0) {
                for (int i = 0; i < 3; i++) {
                    newSmallBoardsPoints[i] += 10;
                }
            }
        }

        return newSmallBoardsPoints;
    }




    int EvaluatePosition(int depth) {
        int positionJudgement = 0;

        // if game is won by any side, directly return the value + the depth
        // (a win in 1 move is to prefer to a win in 3, even if still forced, and it is more reasonable)

        if (gameIsOver == 0) return WINVALUE + depth;
        if (gameIsOver == 1) return -WINVALUE - depth;

        // add points based on boards completed and big board win possibilities

        for (std::vector<int> comb : winMap) {
            int ar = GetBigCombination(comb);

            if (ar == 0) positionJudgement += BIGWINCOMVALUE;
            else if (ar == 1) positionJudgement -= BIGWINCOMVALUE;
        }

        for (int i = 0; i < 9; i++) {
            if (smallBoards[i] == 0) positionJudgement += smallBoardPointsBase[i];
            else if (smallBoards[i] == 1) positionJudgement -= smallBoardPointsBase[i];
            /*else {
                int winChecks = IsBoardWinnable(i);

                if (winChecks == 0) positionJudgement += BIGWINCOMVALUE;
                if (winChecks == 1) positionJudgement -= BIGWINCOMVALUE;
            }*/


            // add points based on moves position and possibilities of completing a small board

            if (smallBoards[i] == -1) {
                int xcount = 0;
                int ocount = 0;

                int start = i * 9;


                // add points based on where the moves are placed

                for (int j = start; j < start + 9; j++) {
                    if (board[j] == 0) {
                        positionJudgement++;
                        xcount++;
                    } else if (board[j] == 1) {
                        positionJudgement--;
                        ocount++;
                    }
                }

                if (xcount + ocount < 3) continue;


                // add points based on how many winning possibilities each side has

                for (std::vector<int> comb : winMap) {
                    int ar = GetSmallCombination(comb, start);

                    if (ar == 0) {
                        positionJudgement += SMALLWINCOMBVALUE;
                    } else if (ar == 1) {
                        positionJudgement -= SMALLWINCOMBVALUE;
                    }
                }
            }
        }

        /*for (int i = 0; i < 81; i++) {
            if (board[i] == 0) positionJudgement += pointsMap[i];
            else if (board[i] == 1) positionJudgement -= pointsMap[i];
        }*/

        return positionJudgement;
    }




    int GetSmallCombination(std::vector<int> comb, int start) {
        std::array<int, 3> ar = {board[comb[0] + start], board[comb[1] + start], board[comb[2] + start]};

        if (ar[0] == -1 && ar[1] == ar[2]) return ar[1];
        if (ar[1] == -1 && ar[0] == ar[2]) return ar[0];
        if (ar[2] == -1 && ar[0] == ar[1]) return ar[0];

        return -1;
    }




    int GetBigCombination(std::vector<int> comb) {
        std::array<int, 3> ar = {smallBoards[comb[0]], smallBoards[comb[1]], smallBoards[comb[2]]};

        if (ar[0] == -1 && ar[1] == ar[2]) {
            if (IsBoardWinnable(comb[0]) == 1 - ar[1]) return 1 - ar[1];
            return ar[1];
        }

        if (ar[1] == -1 && ar[0] == ar[2]) {
            if (IsBoardWinnable(comb[1]) == 1 - ar[0]) return 1 - ar[0];
            return ar[1];
        }

        if (ar[2] == -1 && ar[0] == ar[1]) {
            if (IsBoardWinnable(comb[2]) == 1 - ar[0]) return 1 - ar[0];
            return ar[1];
        }

        return -1;
    }




    int IsBoardWinnable(int smallBoard) {
        // -1: no side can win that small board, it's drawn no matter what
        //  0: X only can win that small board
        //  1: O only can win that small board
        //  2: both sides can win that small board

        std::array<bool, 9> fullSmallBoard; // bool because it can only contain 0s and 1s
        int start = 9 * smallBoard;
        bool Xcan = false;
        bool Ocan = false;

        // check if X can win

        for (int i = 0; i < 9; i++) {
            fullSmallBoard[i] = (board[i + start] == -1) ? 0 : (bool) board[i + start];
        }

        // maybe doing this with a bitboard would be faster...

        for (std::vector<int> comb : winMap) {
            if (fullSmallBoard[comb[0]] == 0 && fullSmallBoard[comb[1]] == 0 && fullSmallBoard[comb[2]] == 0) {
                Xcan = true;
                break;
            }
        }

        // check if O can win

        for (int i = 0; i < 9; i++) {
            fullSmallBoard[i] = (bool) board[i + start];
        }

        for (std::vector<int> comb : winMap) {
            if (fullSmallBoard[comb[0]] == 1 && fullSmallBoard[comb[1]] == 1 && fullSmallBoard[comb[2]] == 1) {
                Ocan = true;
                break;
            }
        }

        if (Xcan && Ocan) return 2;
        if (Xcan) return 0;
        if (Ocan) return 1;
        return -1;
    }




    int MiniMax(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        TempGame tempGame(board, smallBoards, lastMove, player);
        tempGame.MakeMove(maximisingPlayer, move);


        // if search is over return evaluation

        if (depth == 0 || tempGame.gameIsOver != -1) return tempGame.EvaluatePosition(depth);


        bool tablecondition = tempGame.smallBoardSelected == -1;

        /*if (tablecondition) {
            std::unordered_map<std::array<int, 81>, int, ArrayHasher>::iterator it = smallTranspTable.find(board);

            if (it != smallTranspTable.end()) {
                return it->second;
            }
        }*/


        // get available moves

        std::vector<int> possibleMoves = tempGame.GetAvailableMoves();

        // maybe this correctly checks for draws? I haven't noticed any improvement though...

        if (possibleMoves.size() == 0) return 0;


        // next MiniMax iteration
        // it loops through each move and calls MiniMax on the newly created instance of TempGame
        // I saved a lot of time already by making this a separate class,
        // since the program needs to create million of instances every computer move.

        // I believe more time could be saved doing something similar again,
        // either by making this class lighter or not creating new classes at all, rewriting entirely how this MiniMax works

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                maxEval = std::max(maxEval, tempGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                alpha = std::max(alpha, maxEval);

                if (beta <= alpha) break;
            }

            //if (tablecondition) smallTranspTable[board] = maxEval;

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                minEval = std::min(minEval, tempGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;
            }

            //if (tablecondition) smallTranspTable[board] = minEval;

            return minEval;
        }
    }




    void MakeMove(int playerThatMoves, int move) {
        // this simply updates the full board with the new move

        board[move] = playerThatMoves;

        // this stores the small board in which the next player will have to move

        smallBoardSelected = move % 9;

        // this stores the small board in which the current player just moved

        smallBoardOfLastMove = move / 9;


        // check if the small board is won by player

        if (IsSmallBoardWon(playerThatMoves)) {
            smallBoards[smallBoardOfLastMove] = playerThatMoves;

            // mark all cells in that board as not selectable

            for (int i = smallBoardOfLastMove * 9; i < (smallBoardOfLastMove + 1) * 9; i++) {
                if (board[i] == -1) board[i] = 2;
            }
        } else if (IsSmallBoardDraw()) {
            smallBoards[smallBoardOfLastMove] = 2;
        }


        // check if game is won by player

        if (IsGameWon(playerThatMoves)) {
            gameIsOver = playerThatMoves;
        }


        // if the small board in which the next player should move is already complete,
        // then the player will be able to move in any small board

        if (smallBoards[smallBoardSelected] != -1) smallBoardSelected = -1;
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
