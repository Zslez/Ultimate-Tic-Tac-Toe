/**
 * @file eval.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the function used to evaluate a static position of the board
 * @copyright Copyright (c) 2023
 */



int EvaluatePosition(std::array<int, 81> position, int depth) {
        int positionJudgement = 0;

        // if game is won by any side, directly return the value + the depth
        // (a win in 1 move is to prefer to a win in 3, even if still forced, and it is more reasonable)

        if (gameIsOver == 0) return WINVALUE + depth;
        if (gameIsOver == 1) return -WINVALUE - depth;

        for (int i = 0; i < 9; i++) {
            // add points based on boards completed and big board win possibilities

            for (std::vector<int> comb : winMap) {
                std::array<int, 3> ar = {smallBoards[comb[0]], smallBoards[comb[1]], smallBoards[comb[2]]};
                std::sort(ar.begin(), ar.end());

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


                // add points based on how many winning possibilities each side has

                for (std::vector<int> comb : winMap) {
                    std::array<int, 3> ar = {board[comb[0] + start], board[comb[1] + start], board[comb[2] + start]};
                    std::sort(ar.begin(), ar.end());

                    if (ar == XWIN) {
                        positionJudgement += SMALLWINCOMBVALUE;
                    } else if (ar == OWIN) {
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