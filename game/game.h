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

    bool computerIsMoving = false;

    int smallBoardSelected = -1;
    int smallBoardOfLastMove = -1;

    int lastMove = -1;

    std::array<int, 9> smallBoards = {-1, -1, -1, -1, -1, -1, -1, -1, -1};

    int gameIsOver = -1;
    int lastEval = 0;

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
            hoveredCell = ((mousey - bordery) / cellSize) * 9 + ((mousex - borderx) / cellSize);

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


        // highlight last computer move

        if (lastMove != -1) {
            DrawRectFromOrigin(
                borderx + cellSize * (lastMove % 9),
                bordery + cellSize * (lastMove / 9),
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

            rect.x = borderx + cellSize * (i % 9) + 5;
            rect.y = bordery + cellSize * (i / 9) + 5;
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

        /*COLORTYPE winningPlayerColor = (lastEval != 0) ? ((lastEval > 0) ? HRED : HBLUE) : AWHITE;
        SDL_Color winningPlayerSDLColor = (lastEval != 0) ? ((lastEval > 0) ? SDL_HRED : SDL_HBLUE) : SDL_AWHITE;

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << lastEval;
        const char *txt = (std::abs(lastEval) < WINVALUE)
            ? ss.str().c_str()
            : ("W" + std::to_string(MAXSEARCHDEPTH - std::abs(lastEval) + WINVALUE)).c_str();

        DisplayTextFromCenter(txt, winningPlayerSDLColor, tinyFont, -boardSize / 2 - 60, 0);
        DrawRectFromCenter(-boardSize / 2 - 40, 0, borderLineW + 2, boardSize + borderLineW, winningPlayerColor, true);*/


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


        // highlight last computer move

        if (turn == player && lastMove != -1) {
            DrawRectFromOrigin(
                borderx + cellSize * (lastMove % 9),
                bordery + cellSize * (lastMove / 9),
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

            rect.x = borderx + cellSize * (i % 9) + 5;
            rect.y = bordery + cellSize * (i / 9) + 5;
            rect.w = cellSize - 10;
            rect.h = cellSize - 10;

            SDL_RenderCopy(RENDERER, textures[board[i]], NULL, &rect);
        }


        // display final text with game results

        if (gameIsOver == player) {
            DisplayTextFromCenter("Player Won! :)", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        } else if (gameIsOver == 1 - player) {
            DisplayTextFromCenter("Computer Won... :(", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        } else {
            DisplayTextFromCenter("Draw :|", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        }


        // render everything

        SDL_RenderPresent(RENDERER);
    }




    bool PlayerIsHoveringValidCell() {
        return (smallBoardSelected != -1 && Contains(smallBoardsInverseMap[smallBoardSelected], hoveredCell))
                || smallBoardSelected == -1;
    }




    bool MouseIsInsideBoard() {
        SDL_GetMouseState(&mousex, &mousey);

        return (mousex > borderx && mousex < borderx + boardSize) && (mousey > bordery && mousey < bordery + boardSize);
    }




    int MouseButtonDown(SDL_Event event) {
        // PLAYER MOVE IS MADE HERE

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (turn == player && PlayerIsHoveringValidCell() && hoveredCell != -1 && board[hoveredCell] == -1) {
                MakeMove(player, hoveredCell);
                lastMove = hoveredCell;
                turn = 1 - turn;
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

        smallBoardSelected = smallBoardsNextMap[move];

        smallBoardOfLastMove = 3 * (move / 27) + (move % 9) / 3;


        // check if the small board is won by player

        if (IsSmallBoardWon(playerThatMoves)) {
            smallBoards[smallBoardOfLastMove] = playerThatMoves;

            // mark all cells in that board as not selectable

            for (int i : smallBoardsInverseMap[smallBoardOfLastMove]) {
                if (board[i] == -1) board[i] = 2;
            }
        } else if (IsSmallBoardDraw()) {
            smallBoards[smallBoardOfLastMove] = 2;

            for (int i : smallBoardsInverseMap[smallBoardOfLastMove]) {
                if (board[i] == -1) board[i] = 2;
            }
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




    int MiniMax(int move, int depth, int alpha, int beta, int maximisingPlayer) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read comments on the other one

        TempGame tempGame(board, smallBoards, lastMove, player);
        tempGame.MakeMove(maximisingPlayer, move);

        if (depth == 0 || tempGame.gameIsOver != -1) return tempGame.EvaluatePosition(depth);


        // check position in transposition table

        /*std::unordered_map<std::string, int>::iterator it = transpositionTable.find(BoardToString(tempGame.board));

        if (it != transpositionTable.end()) {
            return it->second;
        }*/


        std::vector<int> possibleMoves = tempGame.GetAvailableMoves();

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, 1 - maximisingPlayer);
                maxEval = std::max(maxEval, result);
                alpha = std::max(alpha, maxEval);

                if (beta <= alpha) break;
            }

            // add position to transposition table

            //transpositionTable[BoardToString(tempGame.board)] = maxEval;

            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, 1 - maximisingPlayer);
                minEval = std::min(minEval, result);
                beta = std::min(beta, minEval);

                if (beta <= alpha) break;

            }

            // add position to transposition table

            //transpositionTable[BoardToString(tempGame.board)] = minEval;

            return minEval;
        }
    }




    void PrintBoard() {
        std::cout << "\n" << std::endl;

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[9 * i + j] == -1 || board[9 * i + j] == 2) std::cout << "_ ";
                else if (board[9 * i + j] == 0) std::cout << "X ";
                else if (board[9 * i + j] == 1) std::cout << "O ";
            }

            std::cout << "\n";
        }
    }




    void PrintMap(std::vector< std::pair<int, int> > map) {
        std::cout << "\n" << std::endl;

        for (auto const &i : map) {
            std::cout << i.first << " " << i.second << std::endl;
        }
    }




    void CalculateMoveWOIterativeDeepening() {
        // this is the old version of the function, before implementing iterative deepening and sorting moves

        std::vector<int> possibleMoves = GetAvailableMoves();

        int bestMove;
        int bestMoveValue;

        if (player == 0) bestMoveValue = INFINITYVALUE;
        else bestMoveValue = -INFINITYVALUE;

        bestMove = possibleMoves[0];

        for (int i : possibleMoves) {
            int result = MiniMax(i, MAXSEARCHDEPTH, -INFINITYVALUE, INFINITYVALUE, player == 0);

            if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                bestMoveValue = result;
                bestMove = i;
            }
        }

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;

        computerIsMoving = false;
        turn = 1 - turn;
    }




    void CalculateMove() {
        // to measure improvements, I std::cout the time it took for the computer to move each time

        auto start = std::chrono::system_clock::now();

        std::vector<int> possibleMoves = GetAvailableMoves();

        // create map storing moves and evaluation of each move {move, eval}

        std::vector< std::pair<int, int> > movesMap;

        for (int i : possibleMoves) {
            movesMap.push_back({i, (player == 0) ? INFINITYVALUE : -INFINITYVALUE});
        }

        int bestMove;
        int bestMoveValue;

        // iterative deepening and move ordering: search with every depth from 1 and order moves

        for (int depth = 0; depth <= MAXSEARCHDEPTH; depth++) {
            std::vector< std::pair<int, int> > newMovesMap;

            bestMove = possibleMoves[0];
            if (player == 0) bestMoveValue = INFINITYVALUE;
            else bestMoveValue = -INFINITYVALUE;

            // call MiniMax for each possible move and store evaluations

            for (auto const &i : movesMap) {
                int result = MiniMax(i.first, depth, -INFINITYVALUE, INFINITYVALUE, player == 0);
                newMovesMap.push_back({i.first, result});

                if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                    bestMoveValue = result;
                    bestMove = i.first;
                }
            }

            // sort moves

            movesMap = newMovesMap;
            newMovesMap.clear();

            if (player == 0) {
                std::sort(movesMap.begin(), movesMap.end(), [](auto &left, auto &right) {return left.second < right.second;});
            } else {
                std::sort(movesMap.begin(), movesMap.end(), [](auto &left, auto &right) {return left.second > right.second;});
            }

            // clear transposition table

            //transpositionTable.clear();
        }

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;
        lastEval = bestMoveValue;


        // pass turn to player

        computerIsMoving = false;
        turn = 1 - turn;

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;

        std::cout << elapsed_seconds.count() << std::endl;
    }




    bool IsSmallBoardWon(int playerSymbol) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read description on the other one

        for (std::vector<int> winCombination : smallBoardWinMap) {
            if (
                board[winCombination[0] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
             && board[winCombination[1] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
             && board[winCombination[2] + 3 * (smallBoardOfLastMove % 3) + 27 * (smallBoardOfLastMove / 3)] == playerSymbol
            ) return true;
        }

        return false;
    }




    bool IsGameWon(int playerSymbol) {
        // maybe there is a clean way to merge the two functions in TempGame and this class?
        // they're exactly the same... read description on the other one

        for (std::vector<int> winCombination : bigBoardWinMap) {
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

        for (int i : smallBoardsInverseMap[smallBoardOfLastMove]) {
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

        while (true) {
            CLOCK.Tick();
            //std::cout << "\r" << CLOCK.GetFPSWithoutLimit() << "   ";
            //std::cout << "\r" << FormatTime(timeElapsed) << "    ";


            // update screen

            timeElapsed++;

            if (gameIsOver == -1) {
                Update();


                // start thread of computer move

                if (turn != player && !computerIsMoving) {
                    std::thread computerMove(CalculateMove, this);
                    computerMove.detach();
                    computerIsMoving = true;
                }


                // checks if the game is drawn

                if (GetAvailableMoves().size() == 0) {
                    gameIsOver = 2;
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