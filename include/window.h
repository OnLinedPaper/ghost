#ifndef WINDOW_H_
#define WINDOW_H_
#include <SDL3/SDL.h>

/*
  the window class is an individual GUI window, and comes with a set of
  predefined icons and behaviors. it is transparent, and should eventually
  be able to either adapt to the shape of the images it contains, or pass
  clicks straight through (the latter is in an SDL PR).
  
  as a departure from my other projects, it will NOT be a singleton, and the
  eventual goal is to spawn a good deal of them as needed. 
*/

class window {

public:
  window(int, int);
  ~window();

  int get_id() const { return id; }

  //TODO: render an image
  //void render();
  void draw();

  void event(SDL_Event &);
  void update();
private:
  int dim_x, dim_y;
  int c_r, c_g, c_b, c_a;
  SDL_WindowID id;
  SDL_Window *w;
  SDL_Surface *s;

  window() = delete;
  window(const window&) = delete;
  window &operator=(const window &) = delete;
};

#endif
