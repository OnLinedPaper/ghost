#ifndef WINDOW_H_
#define WINDOW_H_

/*
  the window class is an individual GUI window, and comes with a set of
  predefined icons and behaviors. it is transparent, and should eventually
  be able to either adapt to the shape of the images it contains, or pass
  clicks straight through (the latter is in an SDL PR).
  
  as a departure from my other projects, it will NOT be a singleton, and the
  eventual goal is to spawn a good deal of them as needed. 
*/

#include <SDL3/SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "image.h"
#include "script.h"

class window {

public:
  window(int, int);
  ~window();

  int get_id() const { return id; }
  bool is_active() { return active; }

  void add_ghost(const std::string);

  void render(const std::string, int, int, bool); /*DEPRECATED*/
  void blit(const std::string, int, int, bool=false);
  void draw();

  void event_w(SDL_Event &);
  void event_m(SDL_Event &);
  void update();
private:
  bool active;
  int dim_x, dim_y;
  int c_r, c_g, c_b, c_a; //the background surface RGBA
  bool win_focused, win_mouse_in_boundary; 
  SDL_WindowID id;
  SDL_Renderer *r;
  SDL_Window *w;
  SDL_Surface *s;
  SDL_Surface *w_shape;

  // cached window and button positions. TODO: this, more intelligently
  int w_w, w_h; // window width and height
  int offset; // spacing between buttons and borders
  int icon_size; // size of icon buttons
  int icon_fullsize; // size of icon buttons, plus offset
  int i_min_x, i_min_y, i_mov_x, i_mov_y, i_xit_x, i_xit_y; // icon tlc corners

  float mouse_x, mouse_y;
  bool mouse_down, mouse_pulse;
  bool h_min, h_mov, h_xit;
  bool q_min, q_mov, q_xit;
  float q_mov_last_x, q_mov_last_y;

  std::unordered_map<std::string, image> img_assets; //icons, menus, etc
  std::unordered_map<std::string, image> img_ghosts; //characters and such
  std::vector<script> scr_ghosts; //characters and such

  void die();
  window() = delete;
  window(const window&) = delete;
  window &operator=(const window &) = delete;
};

#endif
