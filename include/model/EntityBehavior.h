#ifndef ENTITY_BEHAVIOR_H_
#define ENTITY_BEHAVIOR_H_

#include "Entity.h"
#include "EntityTable.h"
#include "EntityType.h"
#include "Point2i.h"

using Nbrs = std::map<Point2i, Entity, EntityTable::PointCompare>;

class EntityBehavior
{
public:
  static Point2i runaway_direction(Nbrs nbrs);
};

#endif // EntityBehavior.h included
