#include "image.h"
#include <fstream>
#include <SDL3_Image/SDL_Image.h>

//TODO: failure reporting
image::image(const std::string c_file, SDL_Renderer *r) :
    t(nullptr)
  , s(nullptr)
  , frames(0)
  , fps(0)
  , width(0)
  , height(0)
  , name("")
  , loaded(false)
{
  // open the file and get the data
  std::ifstream f(c_file, std::ios::in);
  if(!f) { return; }

  std::string filepath = "";
  f >> filepath;
  f >> name;
  f >> width;
  f >> height;
  f >> frames;
  f >> fps;
  
  //TODO: validate inputs later

  //TODO: check for bad image and load "missing texture" as failsafe
  t = IMG_LoadTexture(r, filepath.c_str());
  if(t == nullptr) { return; }

  s = IMG_Load(filepath.c_str());

  loaded = true;
}

image::~image() {
  SDL_DestroySurface(s);
  SDL_DestroyTexture(t);
}
