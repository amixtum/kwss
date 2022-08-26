#ifndef GAME_MANGER_H_
#define GAME_MANGER_H_

#include <vector>

#include "../generic/BinarySearchTree.h"

#include "BattleTable.h"
#include "EntityBehavior.h"
#include "EntitySpawner.h"
#include "EntityTable.h"

using Tree = BinarySearchTree<int, Entity>;
using Node = Tree::Node;

class GameManager
{
public:
  GameManager(std::string pfile);

  bool tick();

  Team winner();

  EntityTable& grid();

private:
  void setup();

  void spawn_units();

  void spawn_walls();

  void move_entities();

  bool spawn(EntityType type, Team team);

private:
  BattleTable _battle_table;

  EntityTable _entity_table;

  EntitySpawner _spawner;

  Node _ordering = nullptr;
};

#endif // GameManager.h included
