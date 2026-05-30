#include "window.h"
#include <utility>
#include <vector>

//---- constructors and destructors -------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
window::window(int x, int y) : 
    c_r(0x80)
  , c_g(0x80)
  , c_b(0xCC)
  , c_a(0x00)
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
}

//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
window::~window() {
  img_assets.clear();
  SDL_DestroySurface(s);  s = nullptr;
  SDL_DestroyRenderer(r); r = nullptr;
  SDL_DestroyWindow(w);   w = nullptr;
}


//---- rendering the image on screen ------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// draw the window, and everything that's been queued for rendering - then,
// immediately wipe it all in preparation for next rendering cycle
void window::draw() {
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
void window::event(SDL_Event &e) {
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
// handle window logic
void window::update() {
  /*TODO:
    - determine if the mouse is in the window, and where
    - determine if the mouse clicked / held / dragged, and what to do
    - set the icons to be drawn accordingly
  */
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

  if(win_mouse_in_boundary) {
    //TODO: get mouse dims

  }
  else {
  }


  // draw the window icons - this comes last, to make sure they're on top
  int w_w, w_h = 0;
  SDL_GetWindowSize(this->w, &w_w, &w_h);
  int  top_offset = 4;
  int side_offset = 4;
  int icon_dims = 32; //TODO: grab this intelligently
  int icon_side_offset = icon_dims + side_offset;
  
  std::string min = "";
  std::string mov = "";
  std::string xit = "";

  if(!win_focused && !win_mouse_in_boundary) {
    //not focused, mouse isn't here, don't draw anything
    return;
  }
  else if(!win_focused && win_mouse_in_boundary) {
    //hovering but hasn't clicked in, draw the min forms
    min = "icon_idle_min";
    mov = "icon_idle_mov";
    xit = "icon_idle_xit";
  }
  else if(win_focused && !win_mouse_in_boundary) {
    //focused but not hovering in
    //TODO: change to "window focused and hovering over icon"
    min = "icon_hovr_min";
    mov = "icon_hovr_mov";
    xit = "icon_hovr_xit";
  }
  else if(win_focused && win_mouse_in_boundary) {
    //focused and hovering in
    //TODO: change per-icon to "clicking on icon"
    min = "icon_clic_min";
    mov = "icon_clic_mov";
    xit = "icon_clic_xit";
  }

  blit(min, w_w - icon_side_offset * 3, top_offset, true);
  blit(mov, w_w - icon_side_offset * 2, top_offset, true);
  blit(xit, w_w - icon_side_offset * 1, top_offset, true);

}


//---- DEPRECATED functions ---------------------------------------------------
//  -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   - 
// render one image to the window, by name
// ("internal" is for things like window icons and menus)
// hardware accelerated, but does NOT preserve transparency, and is therefore
// DEPRECATED and should not be used
void window::render(const std::string name, int x, int y, bool internal=false) {
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


