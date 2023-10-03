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

        for (int i = 0; i < 9; i++) {
            if (smallBoards[i] == 0) positionJudgement += smallBoardPointsBase[i];
            else if (smallBoards[i] == 1) positionJudgement -= smallBoardPointsBase[i];

            // add points based on boards completed and big board win possibilities

            for (std::vector<int> comb : winMap) {
                std::array<int, 3> ar = {smallBoards[comb[0]], smallBoards[comb[1]], smallBoards[comb[2]]};
                //std::sort(ar.begin(), ar.end());
                // I literally don't know why, but commenting out this sort seems to make the bot stronger...

                if (ar == XWIN) positionJudgement += BIGWINCOMVALUE;
                else if (ar == OWIN) positionJudgement -= BIGWINCOMVALUE;
            }


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
                xcount = 0;
                ocount = 0;


                // add points based on how many winning possibilities each side has

                for (std::vector<int> comb : winMap) {
                    std::array<int, 3> ar = {board[comb[0] + start], board[comb[1] + start], board[comb[2] + start]};
                    //std::sort(ar.begin(), ar.end());
                    // I literally don't know why, but commenting out this sort seems to make the bot stronger...

                    if (ar == XWIN && xcount < SMALLWINCOMBVALUE) {
                        positionJudgement += SMALLWINCOMBVALUE - xcount;
                        xcount++;
                    } else if (ar == OWIN && ocount < SMALLWINCOMBVALUE) {
                        positionJudgement -= SMALLWINCOMBVALUE - ocount;
                        ocount++;
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




    int MiniMax(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        TempGame tempGame(board, smallBoards, lastMove, player);
        tempGame.MakeMove(maximisingPlayer, move);


        // if search is over return evaluation

        if (depth == 0 || tempGame.gameIsOver != -1) return tempGame.EvaluatePosition(depth);


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

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                minEval = std::min(minEval, tempGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;
            }

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
