#include "engine.h"
#include "window.h"
#include "timekeep.h"
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


  const bool *keystate = SDL_GetKeyboardState(NULL);
  bool quit = false;
  SDL_Event e;
  SDL_zero(e);

  std::list<SDL_EventType> WINDOW_EVENTS = {
      SDL_EVENT_WINDOW_MOUSE_ENTER
    , SDL_EVENT_WINDOW_MOUSE_LEAVE
    , SDL_EVENT_WINDOW_FOCUS_GAINED
    , SDL_EVENT_WINDOW_FOCUS_LOST
  };

  std::list<SDL_EventType> MOUSE_EVENTS = {
      SDL_EVENT_MOUSE_MOTION
    , SDL_EVENT_MOUSE_BUTTON_DOWN
    , SDL_EVENT_MOUSE_BUTTON_UP
  };

  time::get();

  window win(400, 400);
  win.add_ghost("them");
  while(!quit) {
    //handle SDL events
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_EVENT_QUIT) { quit = true; }
/*
      else if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        green = 0;
      }
      else if(e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
        green = 0xAA;
      }
*/  
      //pass window events to the appropriate window
      if(
        std::find(
            WINDOW_EVENTS.begin()
          , WINDOW_EVENTS.end()
          , e.type
        ) != WINDOW_EVENTS.end()
      ) {
        //TODO: multi-window
        win.event_w(e);
      }

      //pass mouse events to the appropriate window
      if(
        std::find(
            MOUSE_EVENTS.begin()
          , MOUSE_EVENTS.end()
          , e.type
        ) != MOUSE_EVENTS.end()
      ) { 
        //TODO: multi-window
        win.event_m(e);
      }
    }
    //handle key events
    SDL_PumpEvents();
    if(keystate[SDL_SCANCODE_ESCAPE]) { quit = true; }

    win.update();
    win.draw();

    //TODO: check all windows
    if(!win.is_active()) { quit = true; }

    //TODO: timescale from qdbp to keep clock consistent
    SDL_Delay(time::get().get_wait());
    time::get().update();
  }

  SDL_Quit();

  //std::cout << "engine ran alright! (press any key and enter to terminate)" << std::endl;
  //char wait;
  //std::cin >> wait;

  return 0;
}

engine::engine() { }

engine::~engine() { }
