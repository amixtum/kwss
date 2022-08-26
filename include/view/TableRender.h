#ifndef TABLE_RENDER_H_
#define TABLE_RENDER_H_

#include <vector>

#include <ncurses.h>

#include "../model/EntityTable.h"
#include "../model/EntityType.h"
#include "../model/Team.h"
#include "Window.h"

using Symbol = std::pair<char, int>;

using SymbolTable = std::array<std::array<Symbol, static_cast<int>(Team::Size)>,
                               static_cast<int>(EntityType::Size)>;

class TableRender
{
public:
  TableRender();

  void render(Window& window,
              EntityTable& table,
              Point2i top_left,
              Point2i bottom_right);

  Symbol symbol(Entity entity);

  void set_symbol(EntityType type, Team team, Symbol sym);

public:
  static Symbol default_symbol();

private:
  SymbolTable _symbols;
};

#endif // TableRender.h
