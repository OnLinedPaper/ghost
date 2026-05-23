#include "engine.h"
#include "window.h"
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <list>
#include <algorithm>

int engine::play() {
  if(SDL_Init(SDL_INIT_VIDEO) == false) { 
    std::cerr << SDL_GetError() << std::endl; 
    return -1; 
  }
/*  SDL_Window *w{SDL_CreateWindow("test window", 400, 400, SDL_WINDOW_TRANSPARENT)};
  if(w == nullptr) {
    std::cerr << SDL_GetError() << std::endl; 
    return -1; 
  }
  SDL_SetWindowBordered(w, false);
  SDL_SetWindowMouseGrab(w, false);
  SDL_SetWindowAlwaysOnTop(w, true);
  //SDL_SetWindowFocusable(w, false);
  SDL_Surface *s = SDL_GetWindowSurface(w);
*/
  const bool *keystate = SDL_GetKeyboardState(NULL);
  bool quit = false;
  SDL_Event e;
  SDL_zero(e);
  /*
  int red = 0x00;
  int green = 0x00;
  int blue = 0x00;
  int alpha = 0x00;
  */

  std::list<SDL_EventType> WINDOW_EVENTS = {
      SDL_EVENT_WINDOW_MOUSE_ENTER
    , SDL_EVENT_WINDOW_MOUSE_LEAVE
    , SDL_EVENT_WINDOW_FOCUS_GAINED
    , SDL_EVENT_WINDOW_FOCUS_LOST
  };

  window win(400, 400);
  while(!quit) {
    //handle SDL events
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_EVENT_QUIT) { quit = true; }
/*
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
      else if(e.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
        red = 0xFF; green = 0xFF; blue = 0xFF; alpha = 0xFF;
      }
      else if(e.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
        red = 0; green = 0; blue = 0; alpha = 0;
      }
*/  
      //pass window events to the appropriate window
      else if(std::find(
          WINDOW_EVENTS.begin()
        , WINDOW_EVENTS.end()
        , e.type
      ) != WINDOW_EVENTS.end()) {
        //TODO: multi-window
        win.event(e);
      }
    }
    //handle key events
    SDL_PumpEvents();
    if(keystate[SDL_SCANCODE_ESCAPE]) { quit = true; }

    //SDL_FillSurfaceRect(s, nullptr, SDL_MapSurfaceRGBA(s, red, green, blue, alpha));
    //SDL_UpdateWindowSurface(w);

    win.draw();

    //blue += 10;
    //blue %= 0xFF;
    // sleep 50 ms
    //TODO: timescale from qdbp to keep clock consistent
    SDL_Delay(50);
  }

//  SDL_DestroySurface(s);
  //SDL_DestroyWindow(w);
  SDL_Quit();

  std::cout << "engine ran alright! (press any key and enter to terminate)" << std::endl;
  char wait;
  std::cin >> wait;

  return 0;
}

engine::engine() { }

engine::~engine() { }
