#include "script.h"
#include "timekeep.h"
#include <fstream>

script::script(const std::string dir, const std::string scriptname) :
    total_frames(0)
{
  std::ifstream f("./assets/" + dir + "/script/" + scriptname + ".txt", std::ios::in);
  if(!f) { return; }

  int temp_i = 0;
  std::string temp_s = "";
  f >> temp_s >> total_frames; //get the total frames

  //assemble the map of timestamps and the frames to draw when it's their turn
  while(f && f.peek() != EOF) {
    f >> temp_i >> temp_s;
    frame_data.insert(std::pair{temp_i, temp_s});
  }
}

std::string script::get_iname() const {
  int tick = time::get().get_tick() % total_frames;
  std::string name = "";

  for(auto const& [key, value] : frame_data) {
    if(key > tick) { break; }
    name = value;
  }

  return name;
}
