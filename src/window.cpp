#include "window.h"

window::window(int x, int y) : 
    c_r(0x80)
  , c_g(0x80)
  , c_b(0xCC)
  , c_a(0x00)
{
  //TODO: error handling for initializer
  w = SDL_CreateWindow("ghost", x, y, SDL_WINDOW_TRANSPARENT);
  id = SDL_GetWindowID(w);

  SDL_SetWindowBordered(w, false);
  SDL_SetWindowMouseGrab(w, false);
  SDL_SetWindowAlwaysOnTop(w, true);

  s = SDL_GetWindowSurface(w);
}

window::~window() {
  SDL_DestroySurface(s);
  SDL_DestroyWindow(w);
}

// draw the window, and everything that's been queued for rendering - then,
// immediately wipe it all in preparation for next rendering cycle
void window::draw() {
  SDL_UpdateWindowSurface(w);
  //wipe it out with a transparent background
  SDL_FillSurfaceRect(s, nullptr, SDL_MapSurfaceRGBA(s, c_r, c_g, c_b, c_a));
}

//handle window events.
void window::event(SDL_Event &e) {
  //validate e is a windowevent, and targets this window
  if(e.window.windowID != id) { return; }

   switch(e.type) {
    case(SDL_EVENT_WINDOW_FOCUS_GAINED):
      c_a = 0xFF;
      break;
    case(SDL_EVENT_WINDOW_FOCUS_LOST):
      c_a = 0x00;
      break;
    case(SDL_EVENT_WINDOW_MOUSE_ENTER):
      c_r = 0x90;
      c_g = 0x90;
      break;
    case(SDL_EVENT_WINDOW_MOUSE_LEAVE):
      c_r = 0x80;
      c_g = 0x80;
      break;
  }
 
}

void window::update() {
  /*TODO:
    - determine whether window is focused or not
    - determine if the mouse is in the window, and where
    - determine if the mouse clicked / held / dragged, and what to do
    - set the icons to be drawn accordingly
  */
}
