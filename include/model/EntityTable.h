#ifndef ENTITY_TABLE_H_
#define ENTITY_TABLE_H_

#include <algorithm>
#include <vector>
#include <memory>

#include "Entity.h"
#include "BattleTable.h"
#include "Point2i.h"

#include "../util/TypeParse.h"

using Grid = std::vector<std::vector<Entity>>;

class EntityTable 
{
public:
  EntityTable(std::string pfile, Point2i dimensions);

  Entity & put_entity(Point2i pos, 
                      EntityType type, 
                      Team team);

  Entity & put_entity(Point2i pos,
                      Entity &entity);

  Entity & get_entity(Point2i pos);

private:
  BattleTable _battle_table;

  Grid _grid; 
};

#endif // EntityTable.h included
