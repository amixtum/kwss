#include <cstring>
#include <iostream>

#include <ncurses.h>

#include "../include/view/TableRender.h"
#include "../include/view/Window.h"

#include "../include/model/GameManager.h"

int
main(int argc, char** argv)
{
  GameManager gm("/home/gani/src/kwss/data/properties.txt");

  // call this before using ncurses
  initscr();

  // get single characters e.g. dont wait for enter key
  cbreak();

  // don't echo typed characters
  noecho();

  // enable special keystorkes
  // keypad(stdscr, TRUE);

  int height, width;
  getmaxyx(stdscr, height, width);

  std::cout << "width, height: " << width << ", " << height << std::endl;

  Window win(height, width, 0, 0);
  TableRender render;

  render.render(win, gm.grid(), Point2i(0, 0), Point2i(80, 20));
  win.refresh();
  wgetch(win.ptr());
  while (gm.tick()) {
    render.render(win, gm.grid(), Point2i(0, 0), Point2i(80, 20));
    win.refresh();
    // wait for a keypress
    wgetch(win.ptr());
  }

  // end curses
  endwin();

  return 0;
}
