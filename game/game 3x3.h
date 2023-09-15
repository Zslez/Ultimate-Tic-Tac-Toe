/**
 * @file game.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the Game class, which is called whenever a game is started
 * @copyright Copyright (c) 2023
 */



class Game3x3 {
    public:

    // graphics variables

    COLORTYPE BGCOLOR = ABLACK;

    int bordery = 90;
    int boardSize = HEIGHT - 2 * bordery;
    int borderx = (WIDTH - boardSize) / 2;

    int cellSize = boardSize / 3;
    int borderLineW = 4;

    int mousex, mousey;

    SDL_Rect rect;

    TTF_Font *smallFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 28);
    TTF_Font *font = TTF_OpenFont("data/fonts/SugarSnow.ttf", 40);


    // game variables

    // MEANING: -1 => free cell or unset value; 0 => X value; 1 => O value

    int hoveredCell = -1;
    std::array<int, 9> board = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int player = 0;
    int turn = 0;

    std::thread computerMove;
    bool computerIsMoving = false;

    int lastMove = -1;

    int gameIsOver = -1;


    // main methods

    void Update() {
        ColorBackground(BGCOLOR);

        // display title

        DisplayTextFromCenter("TIC-TAC-TOE", SDL_HYELLOW, font, 0, -boardSize / 2 - 40);


        // highlight the hovered cell

        if (MouseIsInsideBoard()) {
            hoveredCell = ((mousey - bordery) / cellSize) * 3 + ((mousex - borderx) / cellSize);

            if (turn == player && board[hoveredCell] == -1) {
                DrawRectFromOrigin(
                    mousex - ((mousex - borderx) % cellSize),
                    mousey - ((mousey - bordery) % cellSize),
                    cellSize, cellSize, HBLUE, true
                );
            }
        } else {
            hoveredCell = -1;
        }


        // highlight last move

        if (lastMove != -1) {
            DrawRectFromOrigin(
                borderx + cellSize * (lastMove % 3),
                bordery + cellSize * (lastMove / 3),
                cellSize, cellSize, HCYAN, true
            );
        }


        // display board

        for (int i = 0; i < 4; i++) {
            DrawRectFromCenter(-boardSize / 2 + i * boardSize / 3, 0, borderLineW, boardSize + borderLineW, AWHITE, true);
            DrawRectFromCenter(0, -boardSize / 2 + i * boardSize / 3, boardSize + borderLineW, borderLineW, AWHITE, true);
        }


        // draw X and O

        for (int i = 0; i < 9; i++) {
            if (board[i] == -1) continue;

            rect.x = borderx + cellSize * (i % 3) + 20;
            rect.y = bordery + cellSize * (i / 3) + 20;
            rect.w = cellSize - 40;
            rect.h = cellSize - 40;

            SDL_RenderCopy(RENDERER, textures[board[i]], NULL, &rect);
        }


        // display text if computer is choosing its move

        if (turn != player) {
            DisplayTextFromCenter("Computer is choosing a move...", SDL_AWHITE, smallFont, 0, boardSize / 2 + 40);
        }


        // render everything

        SDL_RenderPresent(RENDERER);
    }




    void UpdateOver() {
        ColorBackground(BGCOLOR);

        // display title

        DisplayTextFromCenter("TIC-TAC-TOE", SDL_HYELLOW, font, 0, -boardSize / 2 - 40);


        // display board

        for (int i = 0; i < 4; i++) {
            DrawRectFromCenter(-boardSize / 2 + i * boardSize / 3, 0, borderLineW, boardSize + borderLineW, AWHITE, true);
            DrawRectFromCenter(0, -boardSize / 2 + i * boardSize / 3, boardSize + borderLineW, borderLineW, AWHITE, true);
        }


        // draw X and O

        for (int i = 0; i < 9; i++) {
            if (board[i] == -1) continue;

            rect.x = borderx + cellSize * (i % 3) + 20;
            rect.y = bordery + cellSize * (i / 3) + 20;
            rect.w = cellSize - 40;
            rect.h = cellSize - 40;

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




    bool MouseIsInsideBoard() {
        SDL_GetMouseState(&mousex, &mousey);

        return (mousex > borderx && mousex < borderx + boardSize) && (mousey > bordery && mousey < bordery + boardSize);
    }




    int MouseButtonDown(SDL_Event event) {
        // PLAYER MOVE IS MADE HERE

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (turn == player && hoveredCell != -1 && board[hoveredCell] == -1) {
                MakeMove(player, hoveredCell);
                lastMove = hoveredCell;
                turn = 1 - turn;
            }
        }

        return DO_NOTHING;
    }




    std::vector<int> MakeMove(int playerThatMoves, int move) {
        board[move] = playerThatMoves;
        std::vector<int> moves = GetAvailableMoves();

        // check if game is won by player

        if (IsGameWon(playerThatMoves)) {
            gameIsOver = playerThatMoves;
        } else if (moves.size() == 0) {
            gameIsOver = 2;
        }

        return moves;
    }




    void SetVariables(std::array<int, 9> bigBoard) {
        board = bigBoard;
    }




    std::vector<int> GetAvailableMoves() {
        std::vector<int> moves;

        for (int i = 0; i < 9; i++) {
            if (board[i] == -1) moves.push_back(i);
        }

        return moves;
    }




    int MiniMax(int move, int depth, int alpha, int beta, bool maximisingPlayer) {
        Game3x3 tempGame;

        tempGame.SetVariables(board);
        std::vector<int> possibleMoves = tempGame.MakeMove(1 - maximisingPlayer, move);

        // if the search or the game are over, evaluate the current state of the board

        if (tempGame.gameIsOver != -1 || depth == 0) return tempGame.EvaluatePosition(depth);

        maximisingPlayer = !maximisingPlayer;

        // minimax

        if (maximisingPlayer) {
            int maxEval = -INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, true);
                maxEval = std::max(maxEval, result);
                alpha = std::max(alpha, result);

                if (beta <= alpha) break;
            }


            return maxEval;
        } else {
            int minEval = INFINITYVALUE;

            for (int i : possibleMoves) {
                int result = tempGame.MiniMax(i, depth - 1, alpha, beta, false);
                minEval = std::min(minEval, result);
                beta = std::min(beta, result);

                if (beta <= alpha) break;
            }

            return minEval;
        }
    }




    int EvaluatePosition(int depth) {
        int positionJudgement = 0;

        if (gameIsOver == 0) positionJudgement += 100 + 10 * depth;
        else if (gameIsOver == 1) positionJudgement -= 100 + 10 * depth;

        return positionJudgement;
    }




    void PrintBoard() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[3 * i + j] == -1) std::cout << "_ ";
                else if (board[3 * i + j] == 0) std::cout << "X ";
                else if (board[3 * i + j] == 1) std::cout << "O ";
            }

            std::cout << "\n";
        }
    }




    void PrintComputerMove(int move, int value) {
        Game3x3 tempGame;

        tempGame.SetVariables(board);
        tempGame.MakeMove(1 - player, move);

        std::cout << "\nMove: " << move << std::endl;

        std::cout << "Value: " << value << std::endl;
    }




    void CalculateMove() {
        std::vector<int> possibleMoves = GetAvailableMoves();
        int bestMove = possibleMoves[0];
        int bestMoveValue;

        if (player == 0) bestMoveValue = INFINITYVALUE;
        else bestMoveValue = -INFINITYVALUE;

        for (int i : possibleMoves) {
            int result = MiniMax(i, 10, -INFINITYVALUE, INFINITYVALUE, false);

            if ((player == 0 && result < bestMoveValue) || (player == 1 && result > bestMoveValue)) {
                bestMoveValue = result;
                bestMove = i;
            }

            PrintComputerMove(i, result);
        }

        MakeMove(1 - player, bestMove);
        lastMove = bestMove;


        // pass turn to player

        computerIsMoving = false;
        turn = 1 - turn;
    }




    bool IsGameWon(int playerSymbol) {
        for (std::vector<int> winCombination : bigBoardWinMap) {
            if (
                board[winCombination[0]] == playerSymbol
                && board[winCombination[1]] == playerSymbol
                && board[winCombination[2]] == playerSymbol
                ) return true;
        }

        return false;
    }




    int Run() {
        // main loop

        SDL_Event event;

        int insertBallsNum;
        int aliveFlows;

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
                    computerMove = std::thread(CalculateMove, this);
                    computerMove.detach();
                    computerIsMoving = true;
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
