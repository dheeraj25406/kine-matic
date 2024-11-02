#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>
#include <cmath>
#include<iostream>
#include "window.hpp"
#include<complex>
#include<SDL2/SDL2_gfxPrimitives.h>
#include <iterator>

typedef std::complex<double> cmplx;

using namespace std::complex_literals;


class Arm{
public:
    double length, angle;
    cmplx end;
    cmplx start;
    static Uint32 armColor;
    Arm *parent = NULL;
    int thickness;
    Arm(Arm* parent_, double length, double angle){
        this->length = length;
        this->angle = angle;
        this->start = 0.0;
        this->end = 0.0;
        this->parent = parent_;
        this->thickness = 1;
    }

    cmplx calculate(){
        this->end = this->start + length*exp(
            1i*(angle)
        );
        return this->end;
    }

    cmplx render(Window &window, cmplx& dimVector){
        this->calculate();
        int start_x = start.real() + window.width/2.;
        int start_y = start.imag() + window.height/2.;
        int end_x = end.real() + window.width/2.;
        int end_y = end.imag() + window.height/2.;
        filledCircleColor(window.renderer, end_x , window.height - end_y , thickness/2 , 0xffffffff);
        thickLineColor(
            window.renderer,
            start_x,
            window.height - start_y,
            end_x,
            window.height - end_y,
            this->thickness,
            this->armColor
        );
        // if (this->parent) this->parent->render(window, dimVector);
        return end;
    }

    void follow(cmplx cursor){
        cmplx diff = cursor - start;
        angle = atan2(diff.imag(), diff.real());
        start = cursor - length*exp(1i*angle);
        this->calculate();
        if (this->parent) this->parent->follow(start);
    }
};




class Plane{
public:
    cmplx origin = 0.0 + 0.0i;
    static Uint32 originColor;
    Plane(Window& window) : parent(window){
        this->setOrigin(window.width/2., window.height/2.);
    }

    void setOrigin(double x, double y){
        origin.imag(y);
        origin.real(x);
    }

    void drawOrigin(){
        thickLineColor(
            parent.renderer,
            0 ,
            parent.height/2 ,
            parent.width ,
            parent.height/2,
            1,
            originColor
        );
        thickLineColor(
            parent.renderer,
            parent.width/2,
            0 ,
            parent.width/2,
            parent.height,
            1,
            originColor
        );
    }

private:
    Window &parent;
};

//origin color is in form of 0xaabbggrr
Uint32 Plane::originColor = 0xff555555;
Uint32 Arm::armColor = 0xffffffff;
const Uint8 *keyboard;


int main(int argc, char *argv[]){

    Window window{800, 600, "new window"};
    Plane plane{window};

    int totalArms = 100;
    cmplx base{0.0, -window.height/2.0};
    bool locked = false;

    Arm *arms = static_cast<Arm *>(operator new[](sizeof(Arm)*totalArms));

    for(int i=0;i<totalArms;i++){
        new(arms+i)Arm(i>0?arms+i-1:NULL, 4.0, 0.0);
        arms[i].thickness = (i)*0.3 + 1;
    }

    window.setBlendMode(true);
    window.runningWindow = true;
    keyboard = SDL_GetKeyboardState(NULL);

    int mouse_x = 0, mouse_y = 0;
    cmplx mouse_vctr;
    cmplx window_vctr{window.width*1.0, window.height*1.0};
    while(window.runningWindow){
        while(SDL_PollEvent(&window.event)){
            if(window.event.type == SDL_QUIT) window.runningWindow = false;
            if (window.event.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mouse_x, &mouse_y);
            }
        }
        locked = keyboard[SDL_SCANCODE_L];
        mouse_vctr.real(mouse_x);
        mouse_vctr.imag(window.height-mouse_y);
        mouse_vctr -= window_vctr/2.0;
        window.clear(0x00000000);
        plane.drawOrigin();

        boxColor(window.renderer, 0 ,0  , 40 , 40 , 0xffff0000^(locked*0x00ff00ff));
        arms[totalArms-1].follow(mouse_vctr);
        if (locked) arms[0].start = base;
        else base = arms[0].start;
        arms[0].render(window, window_vctr);
        for(int i=1;i<totalArms;i++) {
            if (locked) arms[i].start = arms[i-1].end;
            arms[i].render(window, window_vctr);
        };
        window.update();
    }
    for(int i=0;i<totalArms;i++){
        arms[i].~Arm();
    }
    operator delete[](arms);
    return 0;
}
