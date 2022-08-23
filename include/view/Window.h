#ifndef WINDOW_H_
#define WINDOW_H_

#include <vector>

#include <ncurses.h>

#include "../model/Point2i.h"

class Window {
public:
  Window(int height, int width, int tl_y, int tl_x);

  Window(Point2i dimensions, Point2i top_left);

  ~Window();
   

  WINDOW * ptr();


  Point2i size();


  // call this to draw all changes to the window
  void refresh();
  
  // add char with attributes
  // 
  
  void add_char_at(Point2i at, unsigned char c);

  // default window border
  void add_border();

  // replace all border chars with spaces
  void remove_border();

private:
  WINDOW *_win;
};

#endif 
