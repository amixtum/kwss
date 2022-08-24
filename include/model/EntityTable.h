#ifndef ENTITY_TABLE_H_
#define ENTITY_TABLE_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "BattleTable.h"
#include "Entity.h"
#include "Point2i.h"
#include "Team.h"

#include "../generic/Graph.h"

#include "../util/Neighborhood.h"
#include "../util/TypeParse.h"

using Grid = std::vector<std::vector<Entity>>;

class EntityTable
{
public:
  struct PointCompare
  {
    bool operator()(const Point2i& lhs, const Point2i& rhs) const;
  };

  using Nbrs = std::map<Point2i, Entity, PointCompare>;

public:
  EntityTable(BattleTable* battle_table);

  Point2i get_dimensions();

  Entity& put_entity(Point2i pos, EntityType type, Team team);

  Entity& put_entity(Point2i pos, Entity& entity);

  void move_entity(Point2i from, Point2i to);

  Entity& get_entity(Point2i pos);

  bool has_entity(Point2i pos) const;

  Nbrs in_radius(Point2i center, int radius, Neighborhood n);

  Nbrs in_radius(Point2i center, int radius, EntityType type, Neighborhood n);

  Nbrs enemies_in_radius(Point2i center, int radius, Neighborhood n);

  Nbrs enemies_in_radius(Point2i center,
                         int radius,
                         EntityType type,
                         Neighborhood n);

  Nbrs friendly_in_radius(Point2i center, int radius, Neighborhood n);

  Nbrs friendly_in_radius(Point2i center,
                          int radius,
                          EntityType type,
                          Neighborhood n);

  Nbrs empty_in_radius(Point2i center, int radius, Neighborhood n);

private:
  void in_radius_helper(Point2i center,
                        int radius,
                        Nbrs& to_fill,
                        Neighborhood n);

  void in_radius_helper_type(Point2i center,
                             int radius,
                             Nbrs& to_fill,
                             EntityType type,
                             Neighborhood n);

  void enemies_in_radius_helper(Point2i center,
                                int radius,
                                Nbrs& to_fill,
                                Team team,
                                Neighborhood n);

  void friendly_in_radius_helper(Point2i center,
                                 int radius,
                                 Nbrs& to_fill,
                                 Team team,
                                 Neighborhood n);

  void enemies_in_radius_helper_type(Point2i center,
                                     int radius,
                                     Nbrs& to_fill,
                                     Team team,
                                     EntityType type,
                                     Neighborhood n);

  void friendly_in_radius_helper_type(Point2i center,
                                      int radius,
                                      Nbrs& to_fill,
                                      Team team,
                                      EntityType type,
                                      Neighborhood n);

  void empty_in_radius_helper(Point2i center,
                              int radius,
                              Nbrs& to_fill,
                              Neighborhood n);

private:
  BattleTable* _battle_table = nullptr;

  Grid _grid;

  Point2i _dimensions;
};

#endif // EntityTable.h included
