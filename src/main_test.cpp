#include <cstring>

#include <ncurses.h>

#include "../include/Window.h"

int main(int argc, char **argv) 
{
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
  
  Window win(height, width, 0, 0);

  win.add_border();

  int mid_x = (width - strlen("hello world")) / 2;
  int mid_y = height / 2;

  wmove(win.ptr(), mid_y, mid_x);

  // print hellow rold
  wprintw(win.ptr(), "Hello world");

  // refresh the window so we can see it
  win.refresh();

  // wait for a keypress
  wgetch(win.ptr());

  win.remove_border();

  //win.refresh();

  wgetch(win.ptr());
  
  // end curses
  endwin();

  return 0;  
}
