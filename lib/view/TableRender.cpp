#include "../../include/view/TableRender.h"

#include <iostream>
#include <random>

TableRender::TableRender()
{
  start_color();
  char soldier = '%';
  char spy = '$';
  char leader = '@';
  char wall = ' ';

  auto sol_left = std::make_pair(soldier, 1);
  auto sol_left_on = std::make_pair(soldier, 5);
  auto sol_right = std::make_pair(soldier, 2);
  auto sol_right_on = std::make_pair(soldier, 6);

  auto spy_left = std::make_pair(spy, 5);
  auto spy_left_on = std::make_pair(spy, 5);
  auto spy_right = std::make_pair(spy, 6);
  auto spy_right_on = std::make_pair(spy, 6);

  auto leader_left = std::make_pair(leader, 1);
  auto leader_right = std::make_pair(leader, 2);

  auto wall_left = std::make_pair(wall, 3);
  auto wall_right = std::make_pair(wall, 3);

  set_symbol(EntityType::Soldier, Team::Left, AbilityState::Off, sol_left);
  set_symbol(EntityType::Soldier, Team::Right, AbilityState::Off, sol_right);
  set_symbol(EntityType::Soldier, Team::Left, AbilityState::On, sol_left_on);
  set_symbol(EntityType::Soldier, Team::Right, AbilityState::On, sol_right_on);

  set_symbol(EntityType::Spy, Team::Left, AbilityState::Off, spy_left);
  set_symbol(EntityType::Spy, Team::Right, AbilityState::Off, spy_right);
  set_symbol(EntityType::Spy, Team::Left, AbilityState::On, spy_left_on);
  set_symbol(EntityType::Spy, Team::Right, AbilityState::On, spy_right_on);

  set_symbol(EntityType::Leader, Team::Left, AbilityState::Off, leader_left);
  set_symbol(EntityType::Leader, Team::Right, AbilityState::Off, leader_right);
  set_symbol(EntityType::Leader, Team::Left, AbilityState::On, leader_left);
  set_symbol(EntityType::Leader, Team::Right, AbilityState::On, leader_right);

  set_symbol(EntityType::Wall, Team::Left, AbilityState::Off, wall_left);
  set_symbol(EntityType::Wall, Team::Right, AbilityState::Off, wall_right);
  set_symbol(EntityType::Wall, Team::Left, AbilityState::On, wall_left);
  set_symbol(EntityType::Wall, Team::Right, AbilityState::On, wall_right);
}

void
TableRender::render(Window& window,
                    EntityTable& table,
                    Point2i top_left,
                    Point2i bottom_right)
{
  if (top_left.x < 0 || top_left.y < 0 ||
      top_left.x >= table.get_dimensions().x ||
      top_left.y >= table.get_dimensions().y || bottom_right.x < 0 ||
      bottom_right.y < 0 || bottom_right.x > table.get_dimensions().x ||
      bottom_right.y > table.get_dimensions().y ||
      top_left.x >= bottom_right.x || top_left.y >= bottom_right.y ||
      (bottom_right.x - top_left.x) > window.size().x - 1 ||
      (bottom_right.y - top_left.y) > window.size().y - 1) {
    std::cout << window.size().x << " " << window.size().y << "\n";
    return;
  }

  int w_x = 1;
  int w_y = 1;
  for (int x = top_left.x; x < bottom_right.x; x += 1) {
    for (int y = top_left.y; y < bottom_right.y; y += 1) {
      Point2i window_pos(w_x, w_y);
      auto sym = symbol(table.get_entity(Point2i(x, y)));
      window.add_char_at_color(window_pos, sym.first, sym.second);
      w_y += 1;
    }
    w_y = 1;
    w_x += 1;
  }

  window.refresh();
}

Symbol
TableRender::symbol(Entity entity)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  if (entity.type() == EntityType::Size) {
    return TableRender::default_symbol();
  }

  auto team = entity.team();

  if (team == Team::Size) {
    team = Team::Left;
  }

  return _symbols[static_cast<int>(entity.type())][static_cast<int>(team)][static_cast<int>(entity.state())];
}

void
TableRender::set_symbol(EntityType type, Team team, AbilityState state, Symbol sym)
{
  _symbols[static_cast<int>(type)][static_cast<int>(team)][static_cast<int>(state)] = sym;
}

Symbol
TableRender::default_symbol()
{
  return std::make_pair('.', 7);
}
