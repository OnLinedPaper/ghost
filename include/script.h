#ifndef SCRIPT_H_
#define SCRIPT_H_

/*
the script class is designed to keep track of "animation scripts" - preset 
sets and timings of images to render. it might be changed later on.

the animation frames are tied to a global time state.
*/

#include <map>
#include <string>

class script {
public:
  script(const std::string, const std::string);

  //get the name of the image to draw
  std::string get_iname() const;
private:
  std::map<int, std::string> frame_data;
  int total_frames;
};

#endif
