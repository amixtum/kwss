#include <cstring>
#include <cwchar>
#include <iostream>
#include <string>

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

  Window win(height, width, 0, 0);

  win.add_border();
  win.refresh();

  TableRender render;

  render.render(win, gm.grid(), Point2i(0, 0), gm.grid().get_dimensions());
  win.refresh();
  wgetch(win.ptr());

  int left_wins = 0;
  int right_wins = 0;

  for (int i = 0; i < 100; i += 1) {
    while (gm.tick()) {
      render.render(win, gm.grid(), Point2i(0, 0), gm.grid().get_dimensions());
      win.refresh();
      // wait for a keypress
      // wgetch(win.ptr());
    }
    if (gm.winner() == Team::Left) {
      left_wins += 1;
    } else {
      right_wins += 1;
    }

    gm.reset();

    std::string winner;
    winner.append("Left wins: ");
    winner.append(std::to_string(left_wins));
    winner.append(", Right Wins: ");
    winner.append(std::to_string(right_wins));

    wmove(win.ptr(), height / 2, width / 2);
    waddstr(win.ptr(), winner.c_str());
  }

  std::string winner;
  winner.append("Left wins: ");
  winner.append(std::to_string(left_wins));
  winner.append(", Right Wins: ");
  winner.append(std::to_string(right_wins));

  wmove(win.ptr(), height / 2, width / 2);
  waddstr(win.ptr(), winner.c_str());

  while (wgetch(win.ptr()) != 'q') {
  }

  // end curses
  endwin();

  return 0;
}
