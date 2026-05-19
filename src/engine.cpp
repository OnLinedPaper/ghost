#include "engine.h"
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_Main.h>

int engine::play() {
  if(SDL_Init(SDL_INIT_VIDEO) == false) { 
    std::cerr << SDL_GetError() << std::endl; 
    return -1; 
  }
  SDL_Window *w{SDL_CreateWindow("test window", 400, 400, 400)};
  if(w == nullptr) {
    std::cerr << SDL_GetError() << std::endl; 
    return -1; 
  }
  SDL_SetWindowBordered(w, true);
  SDL_SetWindowMouseGrab(w, false);
  SDL_Surface *s = SDL_GetWindowSurface(w);

  const bool *keystate = SDL_GetKeyboardState(NULL);
  bool quit = false;
  SDL_Event e;
  SDL_zero(e);
  int red = 0xAA;
  int green = 0xAA;
  int blue = 0xFF;
  while(!quit) {
    //handle SDL events
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_EVENT_QUIT) { quit = true; }
      else if(e.type == SDL_EVENT_WINDOW_MOUSE_ENTER) {
        red = 0;
      }
      else if(e.type == SDL_EVENT_WINDOW_MOUSE_LEAVE) {
        red = 0xAA;
      }
      else if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        green = 0;
      }
      else if(e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        green = 0xAA;
      }
    }
    //handle key events
    SDL_PumpEvents();
    if(keystate[SDL_SCANCODE_ESCAPE]) { quit = true; }

    SDL_FillSurfaceRect(s, nullptr, SDL_MapSurfaceRGB(s, red, green, blue));
    SDL_UpdateWindowSurface(w);

    //blue += 10;
    //blue %= 0xFF;
    // sleep 50 ms
    SDL_Delay(50);
  }

  SDL_DestroySurface(s);
  SDL_DestroyWindow(w);
  SDL_Quit();

  std::cout << "engine ran alright! (press any key and enter to terminate)" << std::endl;
  char wait;
  std::cin >> wait;

  return 0;
}

engine::engine() { }

engine::~engine() { }
