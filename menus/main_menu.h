/**
 * @file main_menu.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the MainMenu class of the game, accessed when the game is opened
 * @copyright Copyright (c) 2023
 */



// CLASS

class MainMenu {
    public:

    int numButtons = 2;
    SDL_Rect buttons[2];

    int (MainMenu::*onclicks[2])() = {Play, Quit};

    COLORTYPE BGCOLOR = ABLACK;

    TTF_Font *tinyFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 18);
    TTF_Font *smallFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 28);
    TTF_Font *font = TTF_OpenFont("data/fonts/SugarSnow.ttf", 40);
    TTF_Font *bigFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 65);
    TTF_Font *gigaFont = TTF_OpenFont("data/fonts/SugarSnow.ttf", 90);


    // methods

    int Quit() {
        return CLOSE_WINDOW;
    }



    int Back() {
        return PREVIOUS_WIN;
    }



    int DoNothing() {
        return DO_NOTHING;
    }



    int Play() {
        endgame = false;
        Game game;
        return game.Run();
    }



    void DrawScreen() {
        // draw bg

        SetColor(BGCOLOR);
        SDL_RenderClear(RENDERER);

        // PLAY BUTTON

        buttons[0] = DrawRectFromCenter(0, 0, 260, 80, LAVENDER, true);
        DrawRectFromCenter(0, 0, 250, 70, LPURPLE, true);
        DisplayTextFromCenter("NEW GAME", SDL_LAVENDER, font, 0, 0);

        // QUIT BUTTON

        buttons[1] = DrawRectFromOrigin(WIDTH - 100, HEIGHT - 100, 60, 60, HHGREEN, true);
        DrawRectFromOrigin(WIDTH - 95, HEIGHT - 95, 50, 50, HGREEN, true);
        DisplayCenteredText("Q", SDL_HHGREEN, font, WIDTH - 70, HEIGHT - 70);

        // render everything

        SDL_RenderPresent(RENDERER);
    }



    int MouseButtonDown(SDL_Event event) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            // loop through all the buttons

            for (int i = 0; i < numButtons; i++) {
                // if it is clicked

                if (IsMouseInsideRect(buttons[i])) {
                    if ((this->*(onclicks[i]))() == CLOSE_WINDOW) return CLOSE_WINDOW;

                    return DO_NOTHING;
                }
            }
        }

        return DO_NOTHING;
    }



    void Run() {
        SDL_RenderSetLogicalSize(RENDERER, WIDTH, HEIGHT);

        // main loop

        SDL_Event event;

        while (true) {
            CLOCK.Tick();

            // update screen

            DrawScreen();

            // event loop

            if (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) break;

                // handle key and mouse press

                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int result = MouseButtonDown(event);

                    if (result == CLOSE_WINDOW) break;

                    BGCOLOR = ABLACK;
                }
            }
        }
    }
};
