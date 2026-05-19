#ifndef ENGINE_H_
#define ENGINE_H_

class engine {

public:
  engine();
  engine(const engine&) = delete;
  engine &operator=(const engine &) = delete;
  ~engine();

  int play();

private:

};

#endif
