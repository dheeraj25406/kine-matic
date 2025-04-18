#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include<SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>


typedef unsigned char u_char;

class Window{
public:
        SDL_Window* window = nullptr;
        Uint32 RENDER_FLAGS = SDL_RENDERER_ACCELERATED;
        SDL_Renderer* renderer = nullptr;
        SDL_Event event;
        void (*event_callback)(SDL_Event &event, void *userPointer);
        void (*event_renderloop)(void *userPointer);
        Window(int width, int height, const char *caption);
        void init();
        bool resize(int width, int height);
        void poll_events();
        bool runningWindow = false;
        void *userPointer = nullptr;
        ~Window();
        void clear(Uint32 color);
        void update();
        void setBlendMode(bool blend);
        int width, height;

private:
    char const *caption;
};


#endif // _WINDOW_HPP
