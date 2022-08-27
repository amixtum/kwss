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
  if (argc != 2) {
    std::cout << "Usage: KWSS <path to properties file>\n";
    return -1;
  }

  GameManager gm(argv[1]);


  // call this before using ncurses
  initscr();

  // no cursor
  curs_set(0);

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

  //int left_wins = 0;
  //int right_wins = 0;

  std::string current_score = "";

  while (gm.tick()) {
    render.render(win, gm.grid(), Point2i(0, 0), gm.grid().get_dimensions());
    win.refresh();

    // wait for a keypress
    // wgetch(win.ptr());

    current_score.append("Left HP: ");
    current_score.append(std::to_string(gm.grid().get_entity(gm.grid().get_leader(Team::Left)).hp()));
    current_score.append(", ");
    current_score.append("Right HP: ");
    current_score.append(std::to_string(gm.grid().get_entity(gm.grid().get_leader(Team::Right)).hp()));
    current_score.append("");
    wmove(win.ptr(), height - 2, (width / 2) - (strlen(current_score.c_str()) / 2));
    waddstr(win.ptr(), current_score.c_str());
    win.refresh();
    current_score.clear();
  }

  std::string winner;
  if (gm.winner() == Team::Left) {
    winner = "Left Wins";
  } else {
    winner = "Right Wins";
  }

  winner.append(". Press q to quit");

  wmove(win.ptr(), height - 2, width / 2 - (strlen(winner.c_str()) / 2));
  waddstr(win.ptr(), winner.c_str());

  win.refresh();

  while (wgetch(win.ptr()) != 'q') {
  }

  // end curses
  endwin();

  return 0;
}
