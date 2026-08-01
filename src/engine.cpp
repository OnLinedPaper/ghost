#include "engine.h"
#include "window.h"
#include "timekeep.h"
#include <string>
#include <iostream>
#include <SDL3/SDL.h>
#include <list>
#include <algorithm>
#include <filesystem>

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

  //load all ghosts in ./assets/ghosts
  std::list<window> windows;
  for(const auto &c_dname : std::filesystem::directory_iterator("./assets/ghosts/")) {
    windows.emplace_back(400, 400);
    windows.back().add_ghost(c_dname.path().stem().string());
  }

  while(!quit) {
    //handle SDL events
    while(SDL_PollEvent(&e)) {
      if(e.type == SDL_EVENT_QUIT) { quit = true; }

      //pass window events to the appropriate window
      if(
        std::find(
            WINDOW_EVENTS.begin()
          , WINDOW_EVENTS.end()
          , e.type
        ) != WINDOW_EVENTS.end()
      ) 
      { for(window &w : windows) { w.event_w(e); } }

      //pass mouse events to the appropriate window
      if(
        std::find(
            MOUSE_EVENTS.begin()
          , MOUSE_EVENTS.end()
          , e.type
        ) != MOUSE_EVENTS.end()
      ) 
      { for(window &w : windows) { w.event_m(e); } }
    }
    //handle key events
    SDL_PumpEvents();
    if(keystate[SDL_SCANCODE_ESCAPE]) { quit = true; }

    //update all windows, and if NONE of them are active, quit
    bool all_windows_inactive = true;
    for(window &w : windows) { 
      w.update(); 
      w.draw(); 
      if(w.is_active()) { all_windows_inactive = false; }
    }; 
    if(all_windows_inactive) { quit = true; }

    //scaling delay to ensure consistent anim speed, if not framerate
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
