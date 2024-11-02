#include "window.hpp"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <exception>


void dummyCallback(SDL_Event &, void *);
class NullResultPointer: public std::exception {
public:
    const char *message;
    NullResultPointer(const char *msg){
        this->message = msg;
    }
    const char* what() const noexcept override {
        return message;
    }
};


Window::Window(int width, int height,char const *caption) {
        this->width = width;
        this->height = height;
        this->event_callback = dummyCallback;
        this->caption = caption;
        SDL_Init(SDL_INIT_VIDEO);
        this->window = SDL_CreateWindow(
            this->caption,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            this->width,
            this->height, 0
        );
        if (!this->window) throw NullResultPointer("Error while initalizing SDL Window");
        this->renderer = SDL_CreateRenderer(this->window,-1 , this->RENDER_FLAGS);
        if (!this->renderer) throw NullResultPointer("Error cannot initalize renderer");
}

bool Window::resize(int width, int height){
    return true;
}

void Window::poll_events(){
    while(SDL_PollEvent(&this->event)){
        if(event.type == SDL_QUIT) this->runningWindow = false;
        if (event.type == SDL_WINDOWEVENT){
            // this->resize(event.window.data1, event.window.data2);
        }
        this->event_callback(event, this->userPointer);
    }
}


void Window::clear(Uint32 color){
    u_char a = color&0xff;
    u_char g = (color >> 8)&0xff;
    u_char b = (color >> 16)&0xff;
    u_char r = (color >> 24)&0xff;
    SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    SDL_RenderClear(this->renderer);
}

void Window::update(){
    SDL_RenderPresent(this->renderer);
}

void dummyCallback(SDL_Event &event, void *userPointer){

}

void Window::setBlendMode(bool blend){
    SDL_SetRenderDrawBlendMode(
        this->renderer,
        blend ? SDL_BLENDMODE_BLEND :SDL_BLENDMODE_NONE
    );
}

Window::~Window(){
        if (this->window) SDL_DestroyWindow(this->window);
        if (this->renderer) SDL_DestroyRenderer(this->renderer);
}
