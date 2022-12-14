#include "../../include/view/Window.h"
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

WINDOW*
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
  wmove(_win, at.y, at.x);
  waddch(_win, c);
}

void
Window::add_char_at_color(Point2i at, unsigned char c, int color)
{
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  init_pair(7, COLOR_BLACK, COLOR_BLACK);


  wmove(_win, at.y, at.x);
/*
  switch (color) {
    case 1:
      wattron(_win, COLOR_PAIR(1));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(1));
      break;
    case 2:
      wattron(_win, COLOR_PAIR(2));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(2));
      break;
    case 3:
      wattron(_win, COLOR_PAIR(3));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(3));
      break;
    case 4:
      wattron(_win, COLOR_PAIR(4));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(4));
      break;
    case 5:
      wattron(_win, COLOR_PAIR(5));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(5));
    case 6:
      wattron(_win, COLOR_PAIR(6));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(6));
    case 7:
      wattron(_win, COLOR_PAIR(7));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(7));
    default:
      wattron(_win, COLOR_PAIR(7));
      waddch(_win, c);
      wattroff(_win, COLOR_PAIR(7));
      break;
  }*/

  wattron(_win, COLOR_PAIR(color));
  waddch(_win, c);
  wattroff(_win, COLOR_PAIR(color));
}

void
Window::add_border()
{
  wborder(_win, 0, 0, 0, 0, 0, 0, 0, 0);
}

void
Window::remove_border()
{
  wborder(_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
}
