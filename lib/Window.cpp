#include "../include/Window.h"
#include <curses.h>

Window::Window(int height, int width, int tl_y, int tl_x)
{
  _win = newwin(height, width, tl_y, tl_x);
}

Window::Window(Point2i dimensions, Point2i top_left)
{
  _win = newwin(dimensions.y, dimensions.x, top_left.y, top_left.x);
}

Window::~Window()
{
  delwin(_win);
}


WINDOW * 
Window::ptr()
{
  return _win;
}

Point2i 
Window::size()
{
  int width, height;  
  getmaxyx(_win, height, width);
  return Point2i(width, height);
}
 
void 
Window::refresh()
{
  wrefresh(_win);
}

void 
Window::add_char_at(Point2i at, unsigned char c)
{
  mvwaddch(_win, at.y, at.x, c);
}

void 
Window::add_border()
{
  wborder(_win, 0, 0, 0, 0, 0, 0, 0, 0);
  this->refresh();
}

void 
Window::remove_border()
{
  wborder(_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
  this->refresh();
}
