/**
 * @file game.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the Clock class, which is used to cap the frame rate
 * @copyright Copyright (c) 2023
 */


class Clock {
    public:
        float GetCurrentlySetFPS() {
            return _fps;
        }

        int GetFPSWithoutLimit() {
            return (float) _delay / _frameTime * _fps;
        }

        void SetFPS(int fps) {
            _fps = fps;
            _delay = 1000.0 / fps;
        }

        void Tick() {
            _frameTime = SDL_GetTicks64() - _lastTime;

            if (_delay > _frameTime) SDL_Delay(_delay - _frameTime);

            _lastTime = SDL_GetTicks64();
        }

    private:
        float _lastTime = SDL_GetTicks64();
        float _frameTime;
        int _fps = 60;
        float _delay = 1000.0 / _fps;
};
