/**
 * @file funcutils.h
 * @author Cristiano Sansò (cristiano.sanso.04@gmail.com)
 * @brief This file contains the Game class, which is called whenever a game is started
 * @copyright Copyright (c) 2023
 */



#define PATHTYPE std::vector<DOUBLEPOS>




// templates

template <typename T1, typename T2>
bool Contains(T1 vector, T2 element) {
    return std::find(vector.begin(), vector.end(), element) != vector.end();
}

template <typename T>
bool IsSubset(std::vector<T> A, std::vector<T> B) {
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    return std::includes(A.begin(), A.end(), B.begin(), B.end());
}




// functions

std::string BoardToString(std::array<int, 81> position) {
    std::string result;

    for (int i : position) {
        result += std::to_string(i);
    }

    return result;
}




void PrintMap(std::vector< std::pair<int, int> > map) {
    std::cout << "\n" << std::endl;

    for (auto const &i : map) {
        std::cout << i.first << " " << i.second << std::endl;
    }
}




std::string StringToHex(const std::string& input) {
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);

    for (unsigned char c : input) {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }

    return output;
}




std::string HexToString(const std::string& input) {
    std::string res;
    res.reserve(input.size() / 2);

    for (int i = 0; i < input.size(); i += 2) {
        std::istringstream iss(input.substr(i, 2));
        int temp;
        iss >> std::hex >> temp;
        res += static_cast<char>(temp);
    }

    return res;
}




std::string FormatTime(int frames) {
    int sec = frames / FPS % 60;
    int min = frames / FPS / 60;

    std::string seconds = (sec < 10) ? "0" + std::to_string(sec) : std::to_string(sec);
    std::string minutes = std::to_string(min);

    return minutes + ":" + seconds;
}




bool SaveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
    SDL_Surface* saveSurface = NULL;
    SDL_Surface* infoSurface = NULL;
    infoSurface = SDL_GetWindowSurface(SDLWindow);

    if (infoSurface != NULL)  {
        unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];

        if (pixels == 0) {
            return false;
        } else {
            if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
                delete[] pixels;
                return false;
            } else {
                saveSurface = SDL_CreateRGBSurfaceFrom(
                    pixels,
                    infoSurface->w,
                    infoSurface->h,
                    infoSurface->format->BitsPerPixel,
                    infoSurface->w * infoSurface->format->BytesPerPixel,
                    infoSurface->format->Rmask,
                    infoSurface->format->Gmask,
                    infoSurface->format->Bmask,
                    infoSurface->format->Amask
                );

                if (saveSurface == NULL) {
                    delete[] pixels;
                    return false;
                }

                SDL_SaveBMP(saveSurface, filepath.c_str());
                SDL_FreeSurface(saveSurface);
                saveSurface = NULL;
            }

            delete[] pixels;
        }

        SDL_FreeSurface(infoSurface);
        infoSurface = NULL;
    }

    return true;
};
