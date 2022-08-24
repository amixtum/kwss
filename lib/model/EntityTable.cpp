#include "../../include/model/EntityTable.h"
#include "../../include/model/AbilityState.h"
#include "../../include/model/EntityType.h"

#include <map>

EntityTable::EntityTable(std::string pfile, Point2i dimensions)
  : _battle_table(read_table_from_file(pfile))
  , _grid(dimensions.x,
          std::vector<Entity>(dimensions.y,
                              Entity(EntityType::Size, Team::Size, 0, 0)))
  , _dimensions(dimensions.x, dimensions.y)
{
}

Entity&
EntityTable::put_entity(Point2i pos, EntityType type, Team team)
{
  _grid[pos.x][pos.y] = Entity(type,
                               team,
                               _battle_table.get_max_hp(type),
                               _battle_table.get_max_stamina(type));

  return _grid[pos.x][pos.y];
}

Entity&
EntityTable::put_entity(Point2i pos, Entity& entity)
{
  _grid[pos.x][pos.y] = entity;

  return _grid[pos.x][pos.y];
}

Entity&
EntityTable::get_entity(Point2i pos)
{
  return _grid[pos.x][pos.y];
}

bool
EntityTable::has_entity(Point2i pos)
{
  return _grid[pos.x][pos.y].type() != EntityType::Size;
}

void
EntityTable::move_entity(Point2i from, Point2i to)
{
  _grid[to.x][to.y] = _grid[from.x][from.y];
  _grid[from.x][from.y] = Entity(EntityType::Size, Team::Size, 0, 0);
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  in_radius_helper(center, radius, neighbors, n);

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::in_radius(Point2i center,
                       int radius,
                       EntityType type,
                       Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  in_radius_helper_type(center, radius, neighbors, type, n);

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::empty_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  empty_in_radius_helper(center, radius, neighbors, n);

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::enemies_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  enemies_in_radius_helper(
    center, radius, neighbors, get_entity(center).team(), n);

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::friendly_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  friendly_in_radius_helper(
    center, radius, neighbors, get_entity(center).team(), n);

  return neighbors;
}

void
EntityTable::in_radius_helper(Point2i center,
                              int radius,
                              std::map<Point2i, Entity, PointCompare>& to_fill,
                              Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if ((e_at.type() == EntityType::Spy && e_at.state() == AbilityState::On)) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    in_radius_helper(neighbor, radius - 1, to_fill, n);
  }
}

void
EntityTable::in_radius_helper_type(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  EntityType type,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() != type ||
        (e_at.type() == EntityType::Spy && e_at.state() == AbilityState::On)) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    in_radius_helper_type(neighbor, radius - 1, to_fill, type, n);
  }
}

void
EntityTable::enemies_in_radius_helper(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  Team team,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.team() == team ||
        (e_at.team() != team && e_at.type() == EntityType::Spy &&
         e_at.state() == AbilityState::On)) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    enemies_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
  }
}

void
EntityTable::friendly_in_radius_helper(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  Team team,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.team() != team) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    friendly_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
  }
}

void
EntityTable::enemies_in_radius_helper_type(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  Team team,
  EntityType type,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.team() == team || e_at.type() != type ||
        (e_at.team() != team && e_at.type() == EntityType::Spy &&
         e_at.state() == AbilityState::On)) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    enemies_in_radius_helper_type(neighbor, radius - 1, to_fill, team, type, n);
  }
}

void
EntityTable::friendly_in_radius_helper_type(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  Team team,
  EntityType type,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.team() != team || e_at.type() != type) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    friendly_in_radius_helper_type(
      neighbor, radius - 1, to_fill, team, type, n);
  }
}

void
EntityTable::empty_in_radius_helper(
  Point2i center,
  int radius,
  std::map<Point2i, Entity, PointCompare>& to_fill,
  Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() != EntityType::Size) {
      continue;
    }

    to_fill.insert(std::make_pair(neighbor, get_entity(neighbor)));

    in_radius_helper(neighbor, radius - 1, to_fill, n);
  }
}

bool
EntityTable::PointCompare::operator()(Point2i& lhs, Point2i& rhs)
{
  return (lhs.x * lhs.y) + lhs.x == (rhs.x * rhs.y) + rhs.x;
}
