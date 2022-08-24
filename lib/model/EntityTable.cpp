#include "../../include/model/EntityTable.h"
#include "../../include/model/AbilityState.h"
#include "../../include/model/EntityType.h"

#include <map>

EntityTable::EntityTable(BattleTable* battle_table)
  : _battle_table(battle_table)
  , _grid(battle_table->get_dimensions().x,
          std::vector<Entity>(
            battle_table->get_dimensions().y,
            Entity(EntityType::Size, Team::Size, 0, 0, Point2i(0, 0))))
  , _dimensions(battle_table->get_dimensions())
{
  for (auto i = 0; i < _dimensions.x; i += 1) {
    for (auto k = 0; k < _dimensions.y; k += 1) {
      _grid[i][k].set_position(Point2i(i, k));
    }
  }
}

Point2i
EntityTable::get_dimensions()
{
  return _dimensions;
}

Entity&
EntityTable::put_entity(Point2i pos, EntityType type, Team team)
{
  _grid[pos.x][pos.y] = Entity(type,
                               team,
                               _battle_table->get_max_hp(type),
                               _battle_table->get_max_stamina(type),
                               pos);

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
EntityTable::has_entity(Point2i pos) const
{
  return _grid[pos.x][pos.y].type() != EntityType::Size;
}

void
EntityTable::move_entity(Point2i from, Point2i to)
{
  _grid[to.x][to.y] = _grid[from.x][from.y];
  _grid[from.x][from.y] = Entity(EntityType::Size, Team::Size, 0, 0, from);
  _grid[to.x][to.y].set_position(to);
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    in_radius_helper(center, radius, neighbors, n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::in_radius(Point2i center,
                       int radius,
                       EntityType type,
                       Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    in_radius_helper_type(center, radius, neighbors, type, n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::empty_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    empty_in_radius_helper(center, radius, neighbors, n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::enemies_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    enemies_in_radius_helper(
      center, radius, neighbors, get_entity(center).team(), n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::friendly_in_radius(Point2i center, int radius, Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    friendly_in_radius_helper(
      center, radius, neighbors, get_entity(center).team(), n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::enemies_in_radius(Point2i center,
                               int radius,
                               EntityType type,
                               Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    enemies_in_radius_helper_type(
      center, radius, neighbors, get_entity(center).team(), type, n);
  }

  return neighbors;
}

std::map<Point2i, Entity, EntityTable::PointCompare>
EntityTable::friendly_in_radius(Point2i center,
                                int radius,
                                EntityType type,
                                Neighborhood n)
{
  std::map<Point2i, Entity, PointCompare> neighbors;

  if (has_entity(center)) {
    friendly_in_radius_helper_type(
      center, radius, neighbors, get_entity(center).team(), type, n);
  }

  return neighbors;
}

void
EntityTable::in_radius_helper(Point2i center,
                              int radius,
                              Nbrs& to_fill,
                              Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() == EntityType::Wall) {
      to_fill.insert({ neighbor, e_at });
      continue;
    } else if ((e_at.type() == EntityType::Spy &&
                e_at.state() == AbilityState::On)) {
      in_radius_helper(neighbor, radius - 1, to_fill, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    in_radius_helper(neighbor, radius - 1, to_fill, n);
  }
}

void
EntityTable::in_radius_helper_type(Point2i center,
                                   int radius,
                                   Nbrs& to_fill,
                                   EntityType type,
                                   Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() == EntityType::Wall) {
      if (type == EntityType::Wall) {
        to_fill.insert({ neighbor, e_at });
      }
      continue;
    } else if (e_at.type() != type || (e_at.type() == EntityType::Spy &&
                                       e_at.state() == AbilityState::On)) {
      in_radius_helper_type(neighbor, radius - 1, to_fill, type, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    in_radius_helper_type(neighbor, radius - 1, to_fill, type, n);
  }
}

void
EntityTable::enemies_in_radius_helper(Point2i center,
                                      int radius,
                                      Nbrs& to_fill,
                                      Team team,
                                      Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.team() == team || e_at.team() == Team::Size ||
               (e_at.team() != team && e_at.type() == EntityType::Spy &&
                e_at.state() == AbilityState::On)) {
      enemies_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    enemies_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
  }
}

void
EntityTable::friendly_in_radius_helper(Point2i center,
                                       int radius,
                                       Nbrs& to_fill,
                                       Team team,
                                       Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);
    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.team() != team) {
      friendly_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    friendly_in_radius_helper(neighbor, radius - 1, to_fill, team, n);
  }
}

void
EntityTable::enemies_in_radius_helper_type(Point2i center,
                                           int radius,
                                           Nbrs& to_fill,
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
    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.team() == team || e_at.type() != type ||
               e_at.team() == Team::Size ||
               (e_at.team() != team && e_at.type() == EntityType::Spy &&
                e_at.state() == AbilityState::On)) {
      enemies_in_radius_helper_type(
        neighbor, radius - 1, to_fill, team, type, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    enemies_in_radius_helper_type(neighbor, radius - 1, to_fill, team, type, n);
  }
}

void
EntityTable::friendly_in_radius_helper_type(Point2i center,
                                            int radius,
                                            Nbrs& to_fill,
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
    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.team() != team || e_at.type() != type) {
      friendly_in_radius_helper_type(
        neighbor, radius - 1, to_fill, team, type, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });

    friendly_in_radius_helper_type(
      neighbor, radius - 1, to_fill, team, type, n);
  }
}

void
EntityTable::empty_in_radius_helper(Point2i center,
                                    int radius,
                                    Nbrs& to_fill,
                                    Neighborhood n)
{
  if (radius == 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x, _dimensions.y)) {
    auto e_at = get_entity(neighbor);

    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.type() != EntityType::Size) {
      in_radius_helper(neighbor, radius - 1, to_fill, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });
    in_radius_helper(neighbor, radius - 1, to_fill, n);
  }
}

bool
EntityTable::PointCompare::operator()(const Point2i& lhs,
                                      const Point2i& rhs) const
{
  return (lhs.x * lhs.y) + lhs.x == (rhs.x * rhs.y) + rhs.x;
}
