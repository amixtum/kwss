#ifndef ENTITY_SPAWNER_H_
#define ENTITY_SPAWNER_H_

#include <array>

#include "EntityTable.h"

class EntitySpawner
{
public:
  EntitySpawner(EntityTable* table, int respawn_interval);

  void finish(Team team);

  void tick();

  bool can_spawn(Team team);

  Entity spawn(EntityType type, Team team, int attempts);

private:
  Entity spawn_helper(EntityType type,
                      Team team,
                      int x,
                      int top_y,
                      int bottom_y);

private:
  EntityTable* _entity_table;

  int _respawn_interval = 0;

  std::array<int, static_cast<int>(Team::Size)> _turns_since_respawn;

private:
};

#endif // EntitySpawner.h included
