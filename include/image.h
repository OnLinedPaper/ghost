#ifndef IMAGE_H_
#define IMAGE_H_

/*
in order to support multi-window, each window will have its own set of images
(a necessity since each window has its own renderer). outside of that, the
images will operate similarly to how they do in qdbp and p_lite_tuber, using
the txt file control format from p_lite_tuber to keep things simple. 

the control file format is:
  <relative filepath to image file, including name of file>
  <name of image - anything descriptive, must be unique>
  <width of ONE frame, in pixels>
  <height of ONE frame, in pixels>
  <total frame count, 1 for still images>
  <animation frames per second, 0 is un-animated>

a notable DIFFERENCE is that the image will NOT be responsible for drawing
itself anymore. instead, the image will pass up its texture if requested. the
window itself will be responsible for drawing the image. this saves the 
trouble of keeping a copy of the renderer in the image class, but also means
the parent window must be sure to free its images if it also frees its
renderer!
*/

#include <string>
#include <SDL3/SDL.h>
#include "image.h"

class image {
public:
  // control file path and renderer to be rendered to
  image(const std::string, const std::string, SDL_Renderer *);
  image(const image &) = delete;
  image &operator=(const image &) = delete;

  ~image();

  SDL_Texture *get_texture() const { return t; } /*DEPRECATED*/
  SDL_Surface *get_surface() const { return s; }
  int get_w() const { return width; };
  int get_h() const { return height; };

private:

  // the actual texture of the image to be rendered
  SDL_Texture *t;
  SDL_Surface *s;

  // image data
  int frames, fps, width, height;
  //TODO: move to image handler later?
  std::string name;

  bool loaded;

  
};

#endif
