#include "../../include/model/EntityTable.h"
#include <memory>

EntityTable::EntityTable(std::string pfile, Point2i dimensions) 
  : _battle_table(read_table_from_file(pfile)) 
  , _grid(dimensions.x, 
          std::vector<std::shared_ptr<Entity>>(dimensions.y, nullptr))
{ 
}

Entity&
EntityTable::put_entity(Point2i pos, EntityType type, Team team)
{
  _grid[pos.x][pos.y] = Entity(
      type, 
      team, 
      _battle_table.get_max_hp(type),
      _battle_table.get_max_stamina(type)
  );

  return _grid[pos.x][pos.y];
}

Entity&
EntityTable::get_entity(Point2i pos)
{
  return _grid[pos.x][pos.y];
}
