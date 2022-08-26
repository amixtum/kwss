#include "../../include/model/EntitySpawner.h"

#include <random>

EntitySpawner::EntitySpawner(EntityTable* table, int respawn_interval)
  : _entity_table(table)
  , _respawn_interval(respawn_interval)
{
  _turns_since_respawn[static_cast<int>(Team::Left)] = _respawn_interval;
  _turns_since_respawn[static_cast<int>(Team::Right)] = _respawn_interval;
}

void
EntitySpawner::finish(Team team)
{
  _turns_since_respawn[static_cast<int>(team)] = 0;
}

void
EntitySpawner::tick()
{
  _turns_since_respawn[static_cast<int>(Team::Left)] += 1;
  _turns_since_respawn[static_cast<int>(Team::Right)] += 1;
}

bool
EntitySpawner::can_spawn(Team team)
{
  return _turns_since_respawn[static_cast<int>(team)] >= _respawn_interval;
}

Entity
EntitySpawner::spawn(EntityType type, Team team, int attempts)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  // if we're spawning a Wall make sure it doesn't interfere with
  // Soldier or Spy spawns
  int x = (gen() % (_entity_table->get_dimensions().x - 10) + 5);
  if (team == Team::Left) {
    x = 0;
  } else if (team == Team::Right) {
    x = _entity_table->get_dimensions().x - 1;
  }

  int attempts_left = attempts;
  auto try_spawn = EntitySpawner::spawn_helper(
    type, team, x, 0, _entity_table->get_dimensions().y);
  while (try_spawn.type() == EntityType::Size && attempts_left > 0) {
    if (team == Team::Size) {
      x = (gen() % (_entity_table->get_dimensions().x - 10) + 5);
    } else if (team == Team::Left) {
      x += 1;
    } else if (team == Team::Right) {
      x -= 1;
    }

    try_spawn = EntitySpawner::spawn_helper(
      type, team, x, 0, _entity_table->get_dimensions().y);

    attempts_left -= 1;
  }

  return try_spawn;
}

Entity
EntitySpawner::spawn_helper(EntityType type,
                            Team team,
                            int x,
                            int top_y,
                            int bottom_y)
{
  if (top_y >= bottom_y) {
    return Entity(EntityType::Size, Team::Size, 0, 0, Point2i(-1, -1));
  }
  std::random_device rd;
  std::mt19937 gen(rd());

  int mid = (gen() % (bottom_y - top_y)) + top_y;

  if (_entity_table->has_entity(Point2i(x, mid))) {
    if (gen() % 1000 < 500) {
      return EntitySpawner::spawn_helper(type, team, x, top_y, mid);
    }

    return EntitySpawner::spawn_helper(type, team, x, mid + 1, bottom_y);
  } else {
    return _entity_table->put_entity(Point2i(x, mid), type, team);
  }
}
