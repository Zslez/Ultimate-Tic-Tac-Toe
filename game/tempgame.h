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
            for (int i : smallBoardsInverseMap[smallBoardSelected]) {
                if (board[i] == -1) moves.push_back(i);
            }
        } else {
            for (int i = 0; i < 81; i++) {
                if (board[i] == -1) moves.push_back(i);
            }
        }

        return moves;
    }




    std::vector<int> GetSmallBoardValues(int movingPlayer) {
        std::vector<int> newSmallBoardsPoints = smallBoardPointsBase;

        // for each small board in the game

        for (int i = 0; i < 9; i++) {

            // if the small board is completed by the moving player

            if (smallBoards[i] == movingPlayer) {

                // find the other small boards that player needs to make a tris and win

                for (std::vector<int> combination : bigBoardWinMap) {
                    if (Contains(combination, i)) {
                        bool isBadCombination = false;

                        // for each combination check if at least one of its boards is already taken by the opponent
                        // and if so, mark the combination as bad, as the moving player cannot win through it

                        for (int j : combination) {
                            if (smallBoards[j] == 1 - movingPlayer) {
                                isBadCombination = true;
                                break;
                            }
                        }

                        // increase or decrease the value of each board based on whether the combination is good or bad

                        newSmallBoardsPoints[i] += 10 * (isBadCombination ? -1 : 1);
                    }
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

        // this edits the boards values based on whether the opponent has already completed another board
        // that is needed to win with that board

        // EXAMPLE:
        // if the situation is
        //
        // X _ O
        // X O _
        // _ _ _

        // the board in the top center will lose value because neither X nor O can win by completing it
        // (though it regains part of the lost value because O can still win vertically with it)
        // whereas the value of the board in the bottom left will increase, since both X and O will win the game completing it

        // ----------------------------------------------------------------

        // I don't know why, but with the +1 it works and plays better 💀
        // but I'm still not sure if this is regardless of whether the computer is X or O
        // I just know that when it is X it works

        std::vector<int> smallBoardValues = GetSmallBoardValues((MAXSEARCHDEPTH - depth) % 2 + 1);

        for (int i = 0; i < 9; i++) {
            if (smallBoards[i] == 0) positionJudgement += smallBoardValues[i];
            else if (smallBoards[i] == 1) positionJudgement -= smallBoardValues[i];
        }

        // check if last played move blocked a tris
        // this slows down the algorithm a lot, and I'm not even sure whether it actually improves the evaluation...

        /*for (std::vector<int> winCombination : getSmallWinMap(smallBoardOfLastMove)) {
            if (Contains(winCombination, lastMove)) {
                if (
                    board[winCombination[0]] == -1
                 || board[winCombination[1]] == -1
                 || board[winCombination[2]] == -1
                ) continue;

                int sum = board[winCombination[0]] + board[winCombination[1]] + board[winCombination[2]];
                if (sum == 2) positionJudgement += 5;
                if (sum == 1) positionJudgement -= 5;
            }
        }*/

        // adds points based on where the last move was made
        // this slows down the algorithm a little, and I'm not even sure whether it actually improves the evaluation...

        /*int pos = smallBoardsNextMap[lastMove];

        if (!(isPlayerTurn && player)) {
            std::cout << "asjhdjfhsdkfsdj" << std::endl;
            if (pos == 0 || pos == 2 || pos == 6 || pos == 8) positionJudgement += 3;
            else if (pos == 1 || pos == 3 || pos == 5 || pos == 7) positionJudgement += 2;
            else positionJudgement += 4;
        } else {
            std::cout << "sudysysad" << std::endl;
            if (pos == 0 || pos == 2 || pos == 6 || pos == 8) positionJudgement -= 3;
            else if (pos == 1 || pos == 3 || pos == 5 || pos == 7) positionJudgement -= 2;
            else positionJudgement -= 4;
        }*/

        for (int i = 0; i < 81; i++) {
            if (board[i] == 0) positionJudgement += pointsMap[i];
            else if (board[i] == 1) positionJudgement -= pointsMap[i];
        }

        return positionJudgement;
    }




    int MiniMax(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        TempGame tempGame(board, smallBoards, lastMove, player);
        tempGame.MakeMove(maximisingPlayer, move);

        // I thought it was better to check for draws, but I can't figure out why doing this makes the Bot lose
        // I leave the commented code just in case...
        // NOTE: std::vector<int> possibleMoves = tempGame.GetAvailableMoves(); needs to be called before this code, in case

        /*if (possibleMoves.size() == 0) {
            tempGame.gameIsOver = 2;
        }*/


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

        smallBoardSelected = smallBoardsNextMap[move];

        // this stores the small board in which the current player just moved
        // I tried to improve this calculation, but I just made the game not recognise completed boards anymore...

        smallBoardOfLastMove = smallBoardsPrevMap[move];


        // check if the small board is won by player

        if (IsSmallBoardWon(playerThatMoves)) {
            smallBoards[smallBoardOfLastMove] = playerThatMoves;

            // mark all cells in that board as not selectable

            for (int i : smallBoardsInverseMap[smallBoardOfLastMove]) {
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
        // maybe this can be improved?

        for (std::vector<int> winCombination : smallBoardWinMap) {
            if (
                board[winCombination[0] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
             && board[winCombination[1] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
             && board[winCombination[2] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
            ) return true;
        }

        return false;
    }




    bool IsSmallBoardDraw() {
        // this simply checks if there are no more moves in the board in which the player just moved
        // if it finds one that is still -1, then the small board is still playable

        for (int i : smallBoardsInverseMap[smallBoardOfLastMove]) {
            if (board[i] == -1) return false;
        }

        return true;
    }




    bool IsGameWon(int playerSymbol) {
        // this loops through all possible big board win combinations and checks whether the player has all three of the boards

        for (std::vector<int> winCombination : bigBoardWinMap) {
            if (
                smallBoards[winCombination[0]] == playerSymbol
             && smallBoards[winCombination[1]] == playerSymbol
             && smallBoards[winCombination[2]] == playerSymbol
            ) return true;
        }

        return false;
    }
};
