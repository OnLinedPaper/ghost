#ifndef ENGINE_H_
#define ENGINE_H_

class engine {

public:
  engine();
 ~engine();

  int play();

private:
  engine(const engine&) = delete;
  engine &operator=(const engine &) = delete;
 
};

#endif
