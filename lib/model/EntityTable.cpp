#include "../../include/model/EntityTable.h"
#include "../../include/model/AbilityState.h"
#include "../../include/model/EntityType.h"
#include "../../include/util/DebugLog.h"

#include <map>

EntityTable::EntityTable(BattleTable* battle_table)
  : _battle_table(battle_table)
  , _grid(battle_table->get_dimensions().x,
          std::vector<Entity>(
            battle_table->get_dimensions().y,
            Entity(EntityType::Size, Team::Size, 0, 0, Point2i(0, 0))))
  , _dimensions(battle_table->get_dimensions())
  , left_leader(-1, -1)
  , right_leader(-1, -1)
{
  for (auto i = 0; i < _dimensions.x; i += 1) {
    for (auto k = 0; k < _dimensions.y; k += 1) {
      _grid[i][k].set_position(Point2i(i, k));
    }
  }
}

void
EntityTable::reset()
{
  for (auto i = 0; i < _dimensions.x; i += 1) {
    for (auto k = 0; k < _dimensions.y; k += 1) {
      auto blank = Entity(EntityType::Size, Team::Size, 0, 0, Point2i(i, k));
      put_entity(Point2i(i, k), blank);
    }
  }
  left_leader = Point2i(-1, -1);
  right_leader = Point2i(-1, -1);
}

void
EntityTable::battle(Point2i attacker, Point2i defender)
{
  auto new_entities =
    _battle_table->battle(get_entity(attacker), get_entity(defender));
  put_entity(attacker, new_entities.first);
  put_entity(defender, new_entities.second);
}

Team
EntityTable::get_winner()
{
  if (!get_entity(left_leader).alive() ||
      get_entity(left_leader).type() != EntityType::Leader) {
    return Team::Right;
  } else if (!get_entity(right_leader).alive() ||
             get_entity(left_leader).type() != EntityType::Leader) {
    return Team::Left;
  }

  return Team::Size;
}

Point2i
EntityTable::get_dimensions()
{
  return _dimensions;
}

Entity
EntityTable::put_entity(Point2i pos, EntityType type, Team team)
{
  if (type == EntityType::Size) {
    _grid[pos.x][pos.y] = Entity(type, team, 0, 0, pos);
    return _grid[pos.x][pos.y];
  }
  _grid[pos.x][pos.y] = Entity(type,
                               team,
                               _battle_table->get_max_hp(type),
                               _battle_table->get_max_stamina(type),
                               pos);

  if (type == EntityType::Leader) {
    if (team == Team::Left) {
      left_leader = pos;
    } else if (team == Team::Right) {
      right_leader = pos;
    }
  }

  return _grid[pos.x][pos.y];
}

Entity
EntityTable::put_entity(Point2i pos, Entity entity)
{
  auto e = get_entity(pos);
  e.set_ability_state(AbilityState::Off);
  e.set_type(entity.type());
  e.set_team(entity.team());
  e.set_hp(entity.hp());
  e.set_stamina(entity.stamina());
  e.set_position(pos);

  _grid[pos.x][pos.y] = e;

  return _grid[pos.x][pos.y];
}

Entity
EntityTable::get_entity(Point2i pos)
{
  return _grid[pos.x][pos.y];
}

Point2i 
EntityTable::get_leader(Team team)
{
  switch (team)
  {
    case Team::Left:
      return left_leader;
    case Team::Right:
      return right_leader;
    default:
      return Point2i(-1, -1);
  }
}

bool
EntityTable::has_entity(Point2i pos) const
{
  return _grid[pos.x][pos.y].type() != EntityType::Size;
}

void
EntityTable::move_entity(Point2i from, Point2i to)
{
  auto blank = Entity(EntityType::Size, Team::Size, 0, 0, from);
  put_entity(to, _grid[from.x][from.y]);
  put_entity(from, blank);
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

  auto insert_fn = [&](Entity e) {
    return !(e.type() == EntityType::Spy && e.state() == AbilityState::On);
  };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
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

  auto insert_fn = [&](Entity e) {
    return e.type() == type &&
           !(e.type() == EntityType::Spy && e.state() == AbilityState::On);
  };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
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

  auto insert_fn = [&](Entity e) {
    return e.team() != team && e.team() != Team::Size &&
           !(e.type() == EntityType::Spy && e.state() == AbilityState::On);
  };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
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

  auto insert_fn = [&](Entity e) { return e.team() == team; };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
}

void
EntityTable::enemies_in_radius_helper_type(Point2i center,
                                           int radius,
                                           Nbrs& to_fill,
                                           Team team,
                                           EntityType type,
                                           Neighborhood n)
{
  if (radius <= 0) {
    return;
  }

  auto insert_fn = [&](Entity e) {
    return e.team() != team && e.type() == type && e.team() != Team::Size &&
           !(e.type() == EntityType::Spy && e.state() == AbilityState::On);
  };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
}

void
EntityTable::friendly_in_radius_helper_type(Point2i center,
                                            int radius,
                                            Nbrs& to_fill,
                                            Team team,
                                            EntityType type,
                                            Neighborhood n)
{
  if (radius <= 0) {
    return;
  }

  auto insert_fn = [&](Entity e) {
    return e.team() == team && e.type() == type;
  };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
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
  */
}

void
EntityTable::empty_in_radius_helper(Point2i center,
                                    int radius,
                                    Nbrs& to_fill,
                                    Neighborhood n)
{
  if (radius <= 0) {
    return;
  }

  auto insert_fn = [&](Entity e) { return e.type() == EntityType::Size; };

  in_radius_helper(center, radius, to_fill, n, insert_fn, center);

  /*
  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
    auto e_at = get_entity(neighbor);

    if (e_at.type() == EntityType::Wall) {
      continue;
    } else if (e_at.type() != EntityType::Size) {
      empty_in_radius_helper(neighbor, radius - 1, to_fill, n);
      continue;
    }

    to_fill.insert({ neighbor, e_at });
    empty_in_radius_helper(neighbor, radius - 1, to_fill, n);
  }*/
}

void
EntityTable::in_radius_helper(Point2i center,
                              int radius,
                              Nbrs& to_fill,
                              Neighborhood n,
                              std::function<bool(Entity)> insert_fn,
                              Point2i exclude)
{
  if (radius <= 0) {
    return;
  }

  for (auto& neighbor :
       neighbors(n, center, 0, 0, _dimensions.x - 1, _dimensions.y - 1)) {
    if (neighbor == exclude) {
      in_radius_helper(neighbor, radius - 1, to_fill, n, insert_fn, exclude);
      continue;
    }

    auto e_at = get_entity(neighbor);

    if (insert_fn(e_at)) {
      to_fill.insert({ neighbor, e_at });
      in_radius_helper(neighbor, radius - 1, to_fill, n, insert_fn, exclude);
      continue;
    } else if (e_at.type() == EntityType::Wall) {
      continue;
    }

    in_radius_helper(neighbor, radius - 1, to_fill, n, insert_fn, exclude);
  }
}

bool
EntityTable::PointCompare::operator()(const Point2i& lhs,
                                      const Point2i& rhs) const
{
  return (lhs.x * lhs.y) + lhs.x < (rhs.x * rhs.y) + rhs.x;
}
