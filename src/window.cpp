#include "window.h"
#include <utility>
#include <vector>

//---- constructors and destructors -------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
window::window(int x, int y) : 
    active(false)
  , c_r(0x80)
  , c_g(0x80)
  , c_b(0xCC)
  , c_a(0x00)
  , mouse_x(0.0)
  , mouse_y(0.0)
  , mouse_down(false)
  , mouse_pulse(false)
  , q_min(false)
  , q_mov(false)
  , q_xit(false)
  , q_mov_last_x(0)
  , q_mov_last_y(0)
{
  //TODO: error handling for initializer
  SDL_CreateWindowAndRenderer("ghost", x, y, SDL_WINDOW_TRANSPARENT, &w, &r);
  id = SDL_GetWindowID(w);

  SDL_SetWindowBordered(w, false);
  SDL_SetWindowMouseGrab(w, false);
  SDL_SetWindowAlwaysOnTop(w, true);

  s = SDL_GetWindowSurface(w);
  
  //now, load the icons used by the window - these are hard-coded to always
  //use the same name
  std::vector<std::string> asset_names;
  asset_names.push_back("icon_idle_min"); //TODO: make these an enum or smth
  asset_names.push_back("icon_idle_mov");
  asset_names.push_back("icon_idle_xit");
  asset_names.push_back("icon_hovr_min");
  asset_names.push_back("icon_hovr_mov");
  asset_names.push_back("icon_hovr_xit");
  asset_names.push_back("icon_clic_min");
  asset_names.push_back("icon_clic_mov");
  asset_names.push_back("icon_clic_xit");

  for(std::string name : asset_names) {
    std::string path = "./assets/control/" + name + ".txt";
    img_assets.emplace(
        std::piecewise_construct
      , std::forward_as_tuple(name)
      , std::forward_as_tuple(path, r)
    );
  }

  // window icon caching. TODO: this, more intelligently
  w_w = x;
  w_h = y;
  offset = 4;
  icon_size = 32;
  icon_fullsize = icon_size + offset;
  i_xit_x = w_w - icon_fullsize * 1;
  i_xit_y = offset;
  i_mov_x = w_w - icon_fullsize * 2;
  i_mov_y = offset;
  i_min_x = w_w - icon_fullsize * 3;
  i_min_y = offset; 

  active = true;
}

//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
window::~window() {
  img_assets.clear();
  if(s != nullptr) { SDL_DestroySurface(s);  s = nullptr; }
  if(r != nullptr) { SDL_DestroyRenderer(r); r = nullptr; }
  if(w != nullptr) { SDL_DestroyWindow(w);   w = nullptr; }
}


//---- rendering the image on screen ------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// draw the window, and everything that's been queued for rendering - then,
// immediately wipe it all in preparation for next rendering cycle
void window::draw() {
  if(!active) { return; }
  SDL_UpdateWindowSurface(w);
  //SDL_RenderPresent(r);

  // wipe it out with a transparent background
  SDL_FillSurfaceRect(s, nullptr, SDL_MapSurfaceRGBA(s, c_r, c_g, c_b, c_a));
  //SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
  //SDL_RenderClear(r);
}

//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// blit one image to the window, by name
// ("internal" is for things like window icons and menus)
// NOT hardware accelerated, but preserves window transparency
void window::blit(const std::string name, int x, int y, bool internal=false) {
  if(!active) { return; }
  SDL_Surface *s = nullptr;

  if(internal && img_assets.find(name) != img_assets.end()) {
    s = img_assets.at(name).get_surface();
  }
  else { /*TODO*/ }

  if(s == nullptr) { return; /*TODO: error reporting, called a bad image*/ }

  SDL_Rect dst_r;
  SDL_zero(dst_r);
  dst_r.x = x;
  dst_r.y = y;
  dst_r.w = img_assets.at(name).get_w();//TODO: get window dims
  dst_r.h = img_assets.at(name).get_h(); 

  SDL_BlitSurface(s, NULL, this->s, &dst_r);
}


//---- window logic and event handling ----------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// handle window events.
void window::event_w(SDL_Event &e) {
  if(!active) { return; }
  // validate e is a windowevent, and targets this window
  if(e.window.windowID != id) { return; }

   switch(e.type) {
    case(SDL_EVENT_WINDOW_FOCUS_GAINED):
      win_focused = true;
      break;
    case(SDL_EVENT_WINDOW_FOCUS_LOST):
      win_focused = false;
      break;
    case(SDL_EVENT_WINDOW_MOUSE_ENTER):
      win_mouse_in_boundary = true;
      break;
    case(SDL_EVENT_WINDOW_MOUSE_LEAVE):
      win_mouse_in_boundary = false;
      break;
  }
 
}

//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// handle mouse events.
// mouse coordinates are window-based (tlc is 0,0).
// note that focusing a window does not fire a mouse event, for some reason.
// TODO: check multi-window stuff.
void window::event_m(SDL_Event &e) {
  if(!active) { return; }
  // validate this window is focused. note that window focus events
  // are handled before mouse events in the engine.
  if(!win_mouse_in_boundary) { return; }

  //get mouse position
  SDL_GetMouseState (&mouse_x, &mouse_y);

  //check for click
  
  if(e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) { 
    mouse_down = true; 
    mouse_pulse = true; 
  }
  if(e.type == SDL_EVENT_MOUSE_BUTTON_UP) { 
    mouse_down = false; 
    mouse_pulse = true;
  }

}

//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// handle window logic
void window::update() {
  if(!active) { return; }

  if(win_focused) { 
    //c_a = 0xFF;
    c_r = 0x40;
    c_g = 0x40;
    c_b = 0x90;  
  }
  else { 
    c_r = 0x00;
    c_g = 0x00;
    c_b = 0x00;
  }


  // now, check if the mouse is interacting with any icons
  int w_w, w_h = 0;
  SDL_GetWindowSize(this->w, &w_w, &w_h);
  int  top_offset = 4;
  int side_offset = 4;
  int icon_dims = 32; //TODO: grab this intelligently
  int icon_side_offset = icon_dims + side_offset;
  
  std::string min = "";
  std::string mov = "";
  std::string xit = "";

  //hovering over icon
  bool h_min = false;
  bool h_mov = false;
  bool h_xit = false;

  //is the mouse hovering over an icon? check min, mov, xit
  if(
    mouse_x > i_min_x && mouse_x < i_min_x + icon_size &&
    mouse_y > i_min_y && mouse_y < i_min_y + icon_size &&
    win_focused
  ) { h_min = true; }
  if(
    mouse_x > i_mov_x && mouse_x < i_mov_x + icon_size &&
    mouse_y > i_mov_y && mouse_y < i_mov_y + icon_size &&
    win_focused
  ) { h_mov = true; }
  if(
    mouse_x > i_xit_x && mouse_x < i_xit_x + icon_size &&
    mouse_y > i_xit_y && mouse_y < i_xit_y + icon_size &&
    win_focused
  ) { h_xit = true; }

  //check to see if the mouse did something this tick
  if(mouse_pulse) {
    if(mouse_down) {
      //clicked something - queue up actions, and then trigger them if the
      //user unclicks over that same button
      if(!q_min && !q_mov && !q_xit) {
        if(h_min) { q_min = true; }
        if(h_mov) { 
          q_mov = true; 
          q_mov_last_x = mouse_x;
          q_mov_last_y = mouse_y;
        }
        if(h_xit) { q_xit = true; }
      }
    }
    if(!mouse_down) {
      //stopped clicking something - check for queued events and execute them
      //if needed
      if(h_min && q_min) { SDL_MinimizeWindow(this->w); }
      if(q_mov) { 
        //TODO: snap so it's not dangling off the display. but... maybe not?
        //      don't know how to make this work with multiple displays.
      }
      if(h_xit && q_xit) { this->die(); }
      q_min = false;
      q_mov = false;
      q_xit = false;
    }
  }

  //if in move mode, set window to mouse position
  if(q_mov) {
    float m_gx = 0;
    float m_gy = 0;
    SDL_GetGlobalMouseState(&m_gx, &m_gy);

    SDL_SetWindowPosition(this->w, m_gx - q_mov_last_x, m_gy - q_mov_last_y);
  }



  // draw the window icons - this comes last, to make sure they're on top
  if(!win_focused && !win_mouse_in_boundary) {
    //not focused, mouse isn't here, don't draw any icons
    return;
  }

  //---- decide what "min" icon to draw --------------
  if(h_min) {
    //hovering over the icon!
    if(mouse_down && q_min) { min = "icon_clic_min"; }
    else {                    min = "icon_hovr_min"; }
  }
  else {
    if(q_min) {               min = "icon_hovr_min"; }
    else {                    min = "icon_idle_min"; } 
  }
  //---- decide what "mov" icon to draw --------------
  if(h_mov) {
    //hovering over the icon!
    if(mouse_down && q_mov) { mov = "icon_clic_mov"; }
    else {                    mov = "icon_hovr_mov"; }
  }
  else {
    if(q_mov) {               mov = "icon_hovr_mov"; }
    else {                    mov = "icon_idle_mov"; }
  }
  //---- decide what "xit" icon to draw --------------
  if(h_xit) {
    //hovering over the icon!
    if(mouse_down && q_xit) { xit = "icon_clic_xit"; }
    else {                    xit = "icon_hovr_xit"; }
  }
  else {
    if(q_xit) {               xit = "icon_hovr_xit"; }
    else {                    xit = "icon_idle_xit"; }
  }



  blit(min, w_w - icon_side_offset * 3, top_offset, true);
  blit(mov, w_w - icon_side_offset * 2, top_offset, true);
  blit(xit, w_w - icon_side_offset * 1, top_offset, true);

}


//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// destroy the window and set it to inactive
// this function is called when the user clicks the "x" on the window. it
// technically leaves the window object alive until it's cleaned up by the
// program closing - theoretically this could tie up RAM, but i'm pretty sure
// that after the heavy hitter data structures are freed this doesn't become
// an issue i realistically need to worry about. 
void window::die() {
  active = false;
  img_assets.clear();
  if(s != nullptr) { SDL_DestroySurface(s);  s = nullptr; }
  if(r != nullptr) { SDL_DestroyRenderer(r); r = nullptr; }
  if(w != nullptr) { SDL_DestroyWindow(w);   w = nullptr; }
}


//---- DEPRECATED functions ---------------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// render one image to the window, by name
// ("internal" is for things like window icons and menus)
// hardware accelerated, but does NOT preserve transparency, and is therefore
// DEPRECATED and should not be used
void window::render(const std::string name, int x, int y, bool internal=false) {
  if(!active) { return; }
  SDL_Texture *t = nullptr;

  if(internal && img_assets.find(name) != img_assets.end()) { 
    t = img_assets.at(name).get_texture(); 
  }
  else { /*TODO*/ }

  if(t == nullptr) { return; /*TODO: error reporting, called a bad image */ }

  SDL_FRect dst_r;
  SDL_zero(dst_r);
  dst_r.x = 0;
  dst_r.y = 0;
  dst_r.w = img_assets.at(name).get_w();
  dst_r.h = img_assets.at(name).get_h(); 

  SDL_FRect src_r;
  SDL_zero(src_r);
  src_r.x = x;
  src_r.y = y;
  src_r.w = img_assets.at(name).get_w();
  src_r.h = img_assets.at(name).get_h();

  SDL_RenderTexture(r, t, &src_r, &dst_r);
}


