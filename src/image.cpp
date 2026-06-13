#include "image.h"
#include <fstream>
#include <SDL3_image/SDL_image.h>

//TODO: failure reporting
image::image(const std::string dir, const std::string pngname, SDL_Renderer *r) :
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
  std::ifstream f("./assets/" + dir + "/control/" + pngname + ".txt", std::ios::in);
  if(!f) { return; }

  std::string filename = "";
  f >> filename;
  f >> name;
  f >> width;
  f >> height;
  f >> frames;
  f >> fps;
  
  //TODO: validate inputs later

  //TODO: check for bad image and load "missing texture" as failsafe
  t = IMG_LoadTexture(r, ("./assets/" + dir + "/png/" + filename).c_str());
  if(t == nullptr) { return; }

  s = IMG_Load(("./assets/" + dir + "/png/" + filename).c_str());

  loaded = true;
}

image::~image() {
  SDL_DestroySurface(s);
  SDL_DestroyTexture(t);
}
