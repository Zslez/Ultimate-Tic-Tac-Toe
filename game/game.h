/**
 * @file game.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the Game class, which is called whenever a game is started
 * @copyright Copyright (c) 2023
 */



class Game {
    public:

    // graphics variables

    COLORTYPE BGCOLOR = ABLACK;

    int bordery = 90;
    int boardSize = HEIGHT - 2 * bordery;
    int borderx = (WIDTH - boardSize) / 2;

    int cellSize = boardSize / 9;
    int tripleSize = cellSize * 3;
    int borderLineW = 4;
    int innerLineW = 1;

    int mousex, mousey;

    SDL_Rect rect;

    TTF_Font *tinyFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 14);
    TTF_Font *smallFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 28);
    TTF_Font *font = TTF_OpenFont("data/fonts/SugarSnow.ttf", 40);


    // game variables

    // MEANING: -1 => free cell or unset value; 0 => X value; 1 => O value

    int hoveredCell = -1;
    std::array<int, 81> board;
    int player = 1;
    int turn = 0;

    bool soloGame = false; // used to build the .exe that plays against itself
    double totalTimeX = 0.0;
    double totalTimeO = 0.0;

    bool computerIsMoving = false;

    int smallBoardSelected = -1;
    int smallBoardOfLastMove = -1;

    int lastMove = -1;

    std::array<int, 9> smallBoards = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    int gameIsOver = -1;
    int lastEval = 0;
    int movingTime = 0;
    int currentMove = 0;

    int freeCells = 81;
    std::vector<int> allAvailableMoves;

    // transposition table: {board string, value}
    // poor failed attempt, I sadly couldn't figure out how to make it work properly
    // it was either not improving time at all, or zeroing it making the Bot play badly even by increasing the search depth

    //std::unordered_map<std::string, int> transpositionTable;


    // main methods

    void Update() {
        ColorBackground(BGCOLOR);

        // display title

        DisplayTextFromCenter("ULTIMATE TIC-TAC-TOE", SDL_HYELLOW, font, 0, -boardSize / 2 - 40);


        // highlight in which small board the player has to make a move

        if (smallBoardSelected != -1) {
            DrawRectFromOrigin(
                borderx + tripleSize * (smallBoardSelected % 3),
                bordery + tripleSize * (smallBoardSelected / 3),
                tripleSize, tripleSize, LAVENDER, true
            );
        } else {
            DrawRectFromOrigin(
                borderx,
                bordery,
                boardSize, boardSize, LAVENDER, true
            );
        }


        // cover completer small boards

        for (int i = 0; i < 9; i++) {
            if (smallBoards[i] == 0) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HRED, true
                );
            } else if (smallBoards[i] == 1) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HBLUE, true
                );
            } else if (smallBoards[i] == 2) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HYELLOW, true
                );
            }
        }


        // highlight the hovered cell

        if (MouseIsInsideBoard()) {
            hoveredCell = boardMap[((mousey - bordery) / cellSize) * 9 + ((mousex - borderx) / cellSize)];

            if (turn == player && board[hoveredCell] == -1) {
                if (PlayerIsHoveringValidCell()) {
                    DrawRectFromOrigin(
                        mousex - ((mousex - borderx) % cellSize),
                        mousey - ((mousey - bordery) % cellSize),
                        cellSize, cellSize, HBLUE, true
                    );
                }
            }
        } else {
            hoveredCell = -1;
        }


        // highlight last move

        if (lastMove != -1) {
            int cell = boardMap[lastMove];

            DrawRectFromOrigin(
                borderx + cellSize * (cell % 9),
                bordery + cellSize * (cell / 9),
                cellSize, cellSize, HCYAN, true
            );
        }


        // display board

        for (int i = 0; i < 4; i++) {
            DrawRectFromCenter(-boardSize / 2 + i * boardSize / 3, 0, borderLineW, boardSize + borderLineW, AWHITE, true);
            DrawRectFromCenter(0, -boardSize / 2 + i * boardSize / 3, boardSize + borderLineW, borderLineW, AWHITE, true);
        }

        for (int i = 0; i < 3; i++) {
            DrawRectFromCenter(-7 * boardSize / 18 + i * boardSize / 3, 0, innerLineW, boardSize, AWHITE, true);
            DrawRectFromCenter(-5 * boardSize / 18 + i * boardSize / 3, 0, innerLineW, boardSize, AWHITE, true);

            DrawRectFromCenter(0, -7 * boardSize / 18 + i * boardSize / 3, boardSize, innerLineW, AWHITE, true);
            DrawRectFromCenter(0, -5 * boardSize / 18 + i * boardSize / 3, boardSize, innerLineW, AWHITE, true);
        }


        // draw X and O

        for (int i = 0; i < 81; i++) {
            if (board[i] == -1) continue;

            int cell = boardMap[i];

            rect.x = borderx + cellSize * (cell % 9) + 5;
            rect.y = bordery + cellSize * (cell / 9) + 5;
            rect.w = cellSize - 10;
            rect.h = cellSize - 10;

            SDL_RenderCopy(RENDERER, textures[board[i]], NULL, &rect);
        }


        // display text if computer is choosing its move

        if (turn != player) {
            DisplayTextFromCenter("Computer is choosing a move...", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        }


        // display evaluation points and bar
        // tried to make something like the chess.com evaluation bar (💀) but dropped it,
        // nonetheless it was helpful to see in real time how the Bot was evaluating the position to spot bugs

        COLORTYPE winningPlayerColor = (lastEval != 0) ? ((lastEval > 0) ? HRED : HBLUE) : AWHITE;
        SDL_Color winningPlayerSDLColor = (lastEval != 0) ? ((lastEval > 0) ? SDL_HRED : SDL_HBLUE) : SDL_AWHITE;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << (double) lastEval / 20;
        const char *txt = (std::abs(lastEval) < WINVALUE)
            ? ss.str().c_str()
            : ("W" + std::to_string(((endgame) ? ENDGAMEMOVES : MAXSEARCHDEPTH) - std::abs(lastEval) + WINVALUE)).c_str();

        DisplayTextFromCenter(txt, winningPlayerSDLColor, tinyFont, -boardSize / 2 - 60, 0);
        DrawRectFromCenter(-boardSize / 2 - 40, 0, borderLineW + 2, boardSize + borderLineW, winningPlayerColor, true);


        // render everything

        SDL_RenderPresent(RENDERER);
    }




    void UpdateOver() {
        ColorBackground(BGCOLOR);

        // display title

        DisplayTextFromCenter("ULTIMATE TIC-TAC-TOE", SDL_HYELLOW, font, 0, -boardSize / 2 - 40);


        // cover completer small boards

        for (int i = 0; i < 9; i++) {
            if (smallBoards[i] == 0) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HRED, true
                );
            } else if (smallBoards[i] == 1) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HBLUE, true
                );
            } else if (smallBoards[i] == 2) {
                DrawRectFromOrigin(
                    borderx + tripleSize * (i % 3),
                    bordery + tripleSize * (i / 3),
                    tripleSize, tripleSize, HYELLOW, true
                );
            }
        }


        // highlight last move

        int cell = boardMap[lastMove];

        DrawRectFromOrigin(
            borderx + cellSize * (cell % 9),
            bordery + cellSize * (cell / 9),
            cellSize, cellSize, HCYAN, true
        );


        // display board

        for (int i = 0; i < 4; i++) {
            DrawRectFromCenter(-boardSize / 2 + i * boardSize / 3, 0, borderLineW, boardSize + borderLineW, AWHITE, true);
            DrawRectFromCenter(0, -boardSize / 2 + i * boardSize / 3, boardSize + borderLineW, borderLineW, AWHITE, true);
        }

        for (int i = 0; i < 3; i++) {
            DrawRectFromCenter(-7 * boardSize / 18 + i * boardSize / 3, 0, innerLineW, boardSize, AWHITE, true);
            DrawRectFromCenter(-5 * boardSize / 18 + i * boardSize / 3, 0, innerLineW, boardSize, AWHITE, true);

            DrawRectFromCenter(0, -7 * boardSize / 18 + i * boardSize / 3, boardSize, innerLineW, AWHITE, true);
            DrawRectFromCenter(0, -5 * boardSize / 18 + i * boardSize / 3, boardSize, innerLineW, AWHITE, true);
        }


        // draw X and O

        for (int i = 0; i < 81; i++) {
            if (board[i] == -1) continue;

            int cell = boardMap[i];

            rect.x = borderx + cellSize * (cell % 9) + 5;
            rect.y = bordery + cellSize * (cell / 9) + 5;
            rect.w = cellSize - 10;
            rect.h = cellSize - 10;

            SDL_RenderCopy(RENDERER, textures[board[i]], NULL, &rect);
        }


        // display final text with game results

        if (!soloGame)
            if (gameIsOver == player) {
                DisplayTextFromCenter("Player Won! :)", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
            } else if (gameIsOver == 1 - player) {
                DisplayTextFromCenter("Computer Won... :(", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
            } else {
                DisplayTextFromCenter("Draw :|", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
            }
        else {
            if (gameIsOver == 0)
                DisplayTextFromCenter("X Won", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
            else if (gameIsOver == 1)
                DisplayTextFromCenter("O Won", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
            else
                DisplayTextFromCenter("Draw", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        }


        // render everything

        SDL_RenderPresent(RENDERER);
    }




    bool PlayerIsHoveringValidCell() {
        return (smallBoardSelected != -1 && hoveredCell >= smallBoardSelected * 9 && hoveredCell < (smallBoardSelected + 1) * 9)
                || smallBoardSelected == -1;
    }




    bool MouseIsInsideBoard() {
        SDL_GetMouseState(&mousex, &mousey);

        return (mousex > borderx && mousex < borderx + boardSize) && (mousey > bordery && mousey < bordery + boardSize);
    }




    int MouseButtonDown(SDL_Event event) {
        // PLAYER MOVE IS MADE HERE
        // ========================
        // ========================
        // ========================
        // ========================
        // ========================
        // PLAYER MOVE IS MADE HERE

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (turn == player && PlayerIsHoveringValidCell() && board[hoveredCell] == -1) {
                MakeMove(player, hoveredCell);
                lastMove = hoveredCell;
                turn = 1 - turn;
                currentMove++;
                freeCells--;

                // checks if the game is drawn

                if (GetAvailableMoves().size() == 0 && gameIsOver == -1) {
                    gameIsOver = 2;
                }
            }
        } else if (event.button.button == SDL_BUTTON_RIGHT) {
            // --
        }

        return DO_NOTHING;
    }




    void MakeMove(int playerThatMoves, int move) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read comments on the other one

        board[move] = playerThatMoves;
        allAvailableMoves.erase(std::find(allAvailableMoves.begin(), allAvailableMoves.end(), move));

        smallBoardSelected = move % 9;
        smallBoardOfLastMove = move / 9;


        // check if the small board is won by player

        if (IsSmallBoardWon(playerThatMoves)) {
            smallBoards[smallBoardOfLastMove] = playerThatMoves;

            // mark all cells in that board as not selectable

            for (int i = smallBoardOfLastMove * 9; i < (smallBoardOfLastMove + 1) * 9; i++) {
                if (board[i] == -1) {
                    board[i] = 2;
                    freeCells--;
                    allAvailableMoves.erase(std::find(allAvailableMoves.begin(), allAvailableMoves.end(), i));
                }
            }
        } else if (IsSmallBoardDraw()) {
            smallBoards[smallBoardOfLastMove] = 2;
        }


        // check if game is won by player

        if (IsGameWon(playerThatMoves)) {
            gameIsOver = playerThatMoves;
        }


        if (smallBoards[smallBoardSelected] != -1) smallBoardSelected = -1;
    }




    std::vector<int> GetAvailableMoves() {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read comments on the other one

        std::vector<int> moves;

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




    int MiniMaxEndGame(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        std::cout << "\r" << movesChecked++ << "   ";
        EndGame endGame(board, smallBoards, lastMove, player, allAvailableMoves);
        endGame.MakeMove(maximisingPlayer, move);

        if (endGame.gameIsOver != -1) {
            return (endGame.gameIsOver == 0) ? (WINVALUE + depth) : (-WINVALUE - depth);
        }

        std::vector<int> possibleMoves = endGame.GetAvailableMoves();

        if (possibleMoves.size() == 0) return 0;

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                maxEval = std::max(maxEval, endGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                alpha = std::max(alpha, maxEval);

                if (beta <= alpha) break;
            }

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                minEval = std::min(minEval, endGame.MiniMax(i, depth - 1, alpha, beta, !maximisingPlayer));
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;
            }

            return minEval;
        }
    }




    int MiniMax(int move, int depth, int alpha, int beta, int maximisingPlayer) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read comments on the other one

        TempGame tempGame(board, smallBoards, lastMove, player);
        tempGame.MakeMove(maximisingPlayer, move);

        if (depth == 0 || tempGame.gameIsOver != -1) return tempGame.EvaluatePosition(depth);


        std::vector<int> possibleMoves = tempGame.GetAvailableMoves();

        if (possibleMoves.size() == 0) return 0;

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, 1 - maximisingPlayer);
                maxEval = std::max(maxEval, result);
                alpha = std::max(alpha, maxEval);

                if (beta <= alpha) break;
            }

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, 1 - maximisingPlayer);
                minEval = std::min(minEval, result);
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;

            }

            return minEval;
        }
    }




    void CalculateMove() {
        // COMPUTER MOVE IS MADE HERE
        // ==========================
        // ==========================
        // ==========================
        // ==========================
        // ==========================
        // COMPUTER MOVE IS MADE HERE

        auto start = std::chrono::system_clock::now();

        // play instantly the first move

        if (currentMove == 0) {
            MakeMove(1 - player, 0);
            lastMove = 0;
            lastEval = 0;
            currentMove++;

            computerIsMoving = false;
            turn = 1 - turn;
            if (soloGame) player = 1 - player;
            return;
        }

        // main algorithm

        std::vector<int> possibleMoves = GetAvailableMoves();

        int bestMove;
        int bestMoveValue;

        if (player == 0) bestMoveValue = INFINITYVALUE;
        else bestMoveValue = -INFINITYVALUE;

        bestMove = possibleMoves[0];
        movesChecked = 0;

        for (int i : possibleMoves) {
            int result;
            if (endgame) result = MiniMaxEndGame(i, ENDGAMEMOVES, -INFINITYVALUE, INFINITYVALUE, player == 0);
            else result = MiniMax(i, MAXSEARCHDEPTH, -INFINITYVALUE, INFINITYVALUE, player == 0);

            if (!soloGame) {
                if (result >= WINVALUE && player == 1) {
                    bestMove = i;
                    bestMoveValue = result;
                    ENDGAMEMOVES = std::abs(result) - WINVALUE - 2;
                    break;
                } else if (result <= -WINVALUE && player == 0) {
                    bestMove = i;
                    bestMoveValue = result;
                    ENDGAMEMOVES = std::abs(result) - WINVALUE - 2;
                    break;
                }
            }

            if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                bestMoveValue = result;
                bestMove = i;
            }
        }

        // make move and pass turn

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;
        lastEval = bestMoveValue;
        currentMove++;
        freeCells--;

        computerIsMoving = false;
        turn = 1 - turn;
        if (soloGame) player = 1 - player;

        // print time needed to move

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        if (player == 1) totalTimeX += elapsed_seconds.count();
        else totalTimeO += elapsed_seconds.count();

        if (!soloGame) std::cout << elapsed_seconds.count() << std::endl;
        else std::cout << "\r" << totalTimeX << "  " << totalTimeO << "   ";
    }




    void CalculateMoveWOEndGame() {
        // COMPUTER MOVE IS MADE HERE
        // ==========================
        // ==========================
        // ==========================
        // ==========================
        // ==========================
        // COMPUTER MOVE IS MADE HERE

        auto start = std::chrono::system_clock::now();

        // play instantly the first move

        if (currentMove == 0) {
            MakeMove(1 - player, 0);
            lastMove = 0;
            lastEval = 0;
            currentMove++;

            computerIsMoving = false;
            turn = 1 - turn;
            if (soloGame) player = 1 - player;
            return;
        }

        // main algorithm

        std::vector<int> possibleMoves = GetAvailableMoves();

        int bestMove;
        int bestMoveValue;

        if (player == 0) bestMoveValue = INFINITYVALUE;
        else bestMoveValue = -INFINITYVALUE;

        bestMove = possibleMoves[0];
        movesChecked = 0;

        for (int i : possibleMoves) {
            int result = MiniMax(i, MAXSEARCHDEPTH, -INFINITYVALUE, INFINITYVALUE, player == 0);

            if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                bestMoveValue = result;
                bestMove = i;
            }
        }

        // make move and pass turn

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;
        lastEval = bestMoveValue;
        currentMove++;
        freeCells--;

        computerIsMoving = false;
        turn = 1 - turn;
        if (soloGame) player = 1 - player;

        // print time needed to move

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;

        if (player == 1) totalTimeX += elapsed_seconds.count();
        else totalTimeO += elapsed_seconds.count();

        if (!soloGame) std::cout << elapsed_seconds.count() << std::endl;
        else std::cout << "\r" << totalTimeX << "  " << totalTimeO << "   ";
    }




    void CalculateMoveIterativeDeepening() {
        // to measure improvements, I std::cout the time it took for the computer to move each time

        auto start = std::chrono::system_clock::now();

        // play instantly the first move

        if (currentMove == 0) {
            MakeMove(1 - player, 0);
            lastMove = 0;
            lastEval = 0;
            currentMove++;

            computerIsMoving = false;
            turn = 1 - turn;
            return;
        }

        std::vector<int> possibleMoves = GetAvailableMoves();

        // create map storing moves and evaluation of each move {move, eval}

        std::vector< std::pair<int, int> > movesMap;

        for (int i : possibleMoves) {
            movesMap.push_back({i, (player == 0) ? INFINITYVALUE : -INFINITYVALUE});
        }

        int bestMove;
        int bestMoveValue;

        // iterative deepening and move ordering: search with every depth from 1 and order moves

        for (int depth = 4; depth <= MAXSEARCHDEPTH; depth++) {
            std::vector< std::pair<int, int> > newMovesMap;

            bestMove = possibleMoves[0];
            if (player == 0) bestMoveValue = INFINITYVALUE;
            else bestMoveValue = -INFINITYVALUE;

            // call MiniMax for each possible move and store evaluations

            int currentMove = 0;

            for (auto const &i : movesMap) {
                int depthIncrement = (depth > 4) ? ((currentMove == 0) ? 1 : -2) : 0;
                int result = MiniMax(i.first, depth + depthIncrement, -INFINITYVALUE, INFINITYVALUE, player == 0);
                newMovesMap.push_back({i.first, result});

                if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                    bestMoveValue = result;
                    bestMove = i.first;
                }

                currentMove++;
            }

            // sort moves

            movesMap = newMovesMap;
            newMovesMap.clear();

            if (player == 0) {
                std::sort(movesMap.begin(), movesMap.end(), [](auto &left, auto &right) {return left.second < right.second;});
            } else {
                std::sort(movesMap.begin(), movesMap.end(), [](auto &left, auto &right) {return left.second > right.second;});
            }

            PrintMap(movesMap);
        }

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;
        lastEval = bestMoveValue;
        currentMove++;
        freeCells--;


        // pass turn to player

        computerIsMoving = false;
        turn = 1 - turn;
        if (soloGame) player = 1 - player;

        // print time needed to move

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;

        if (player == 1) totalTimeX += elapsed_seconds.count();
        else totalTimeO += elapsed_seconds.count();
        if (!soloGame) std::cout << elapsed_seconds.count() << std::endl;
        else std::cout << "\r" << totalTimeX << "  " << totalTimeO << "   ";
    }




    bool IsSmallBoardWon(int playerSymbol) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read description on the other one

        for (std::vector<int> winCombination : winMap) {
            if (
                board[winCombination[0] + 9 * smallBoardOfLastMove] == playerSymbol
             && board[winCombination[1] + 9 * smallBoardOfLastMove] == playerSymbol
             && board[winCombination[2] + 9 * smallBoardOfLastMove] == playerSymbol
            ) return true;
        }

        return false;
    }




    bool IsGameWon(int playerSymbol) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read description on the other one

        for (std::vector<int> winCombination : winMap) {
            if (
                smallBoards[winCombination[0]] == playerSymbol
             && smallBoards[winCombination[1]] == playerSymbol
             && smallBoards[winCombination[2]] == playerSymbol
            ) return true;
        }

        return false;
    }




    bool IsSmallBoardDraw() {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read description on the other one

        for (int i = smallBoardOfLastMove * 9; i < (smallBoardOfLastMove + 1) * 9; i++) {
            if (board[i] == -1) return false;
        }

        return true;
    }




    int Run() {
        // main loop

        SDL_Event event;

        int insertBallsNum;
        int aliveFlows;

        board.fill(-1);

        for (int i = 0; i < 81; i++) {
            allAvailableMoves.push_back(i);
        }

        while (true) {
            CLOCK.Tick();
            //std::cout << "\r" << CLOCK.GetFPSWithoutLimit() << "   ";
            //std::cout << "\r" << FormatTime(timeElapsed) << "    ";
            //std::cout << "\r" << lastMove << "    ";


            // update screen

            if (gameIsOver == -1) {
                Update();


                // checks if the game is drawn

                if (GetAvailableMoves().size() == 0) {
                    gameIsOver = 2;
                    continue;
                } else if (
                    (freeCells <= ENDGAMEMOVES && smallBoardSelected != -1) ||
                    (freeCells <= ENDGAMEMOVES - 4)) endgame = true;


                // start thread of computer move

                if (turn != player && !computerIsMoving) {
                    std::thread computerMove;

                    if (soloGame) {
                        if (player == 0) computerMove = std::thread(CalculateMove, this);
                        else computerMove = std::thread(CalculateMoveWOEndGame, this);
                    } else {
                        computerMove = std::thread(CalculateMove, this);
                    }

                    computerMove.detach();
                    computerIsMoving = true;
                    movingTime = 0;
                    abortSearch = false;
                } else if (turn != player && computerIsMoving) {
                    movingTime++;

                    // for now this variable doesn't really do anything.
                    // I added this system to detect when the time limit is reached,
                    // but the algorithm still has nothing related to the variable

                    if (movingTime == maxTime * SECOND) abortSearch = true;
                }
            } else {
                UpdateOver();
            }


            // event loop

            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    break;
                }

                // handle key and mouse press

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int result = MouseButtonDown(event);
                }

                if (event.type == SDL_KEYDOWN) {}

                else if (event.type == SDL_KEYUP) {
                    if (event.key.keysym.sym == 1073741894) SaveScreenshotBMP("screenshot.png", TRUESCREEN, RENDERER);
                    if (event.key.keysym.sym == SDLK_ESCAPE) return PREVIOUS_WIN;
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        // --
                    }
                }
            }
        }

        return CLOSE_WINDOW;
    }




    // other useful functions

    void WaitTime(int frames) {
        SDL_Event event;

        while (frames != 0) {
            CLOCK.Tick();

            ColorBackground(BGCOLOR);

            // draw stuff

            SDL_RenderPresent(RENDERER);
            SDL_PollEvent(&event);

            frames--;
        }
    }
};
