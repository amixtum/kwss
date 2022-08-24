#ifndef ENTITY_BEHAVIOR_H_
#define ENTITY_BEHAVIOR_H_

#include "Entity.h"
#include "EntityTable.h"
#include "EntityType.h"
#include "Point2i.h"

class EntityBehavior
{
public:
  static Point2i spy_behavior(EntityTable& table,
                              Point2i center,
                              int sight_radius,
                              Neighborhood n,
                              float distance_threshold);

  static Point2i soldier_behavior(EntityTable& table,
                                  Point2i center,
                                  int sight_radius,
                                  Neighborhood n,
                                  float distance_threshold);

  static Point2i random_move(EntityTable& table,
                             Point2i center,
                             int sight_radius,
                             Neighborhood n);

  static float nearest_enemy_distance(EntityTable& table,
                                      Point2i center,
                                      int sight_radius,
                                      Neighborhood n);

  static int count_type(EntityTable& table,
                        Point2i center,
                        int sight_radius,
                        Neighborhood n,
                        EntityType type);

  static int count_type_enemy(EntityTable& table,
                              Point2i center,
                              int sight_radius,
                              Neighborhood n,
                              EntityType type);

  static int count_type_friendly(EntityTable& table,
                                 Point2i center,
                                 int sight_radius,
                                 Neighborhood n,
                                 EntityType type);

  static int count_enemy(EntityTable& table,
                         Point2i center,
                         int sight_radius,
                         Neighborhood n);

  static int count_friendly(EntityTable& table,
                            Point2i center,
                            int sight_radius,
                            Neighborhood n);

  // finds the nearest Entity of EntityType type and
  // returns the nearest adjacent empy space to that Entity
  static Point2i move_to_nearest_of_type(EntityTable& table,
                                         Point2i center,
                                         int sight_radius,
                                         Neighborhood n,
                                         EntityType type);

  // finds the nearest enemy of EntityType type and returns the nearest
  // adjacent empty space to that enemy
  static Point2i move_to_nearest_of_type_enemy(EntityTable& table,
                                               Point2i center,
                                               int sight_radius,
                                               Neighborhood n,
                                               EntityType type);

  static Point2i move_to_nearest_enemy(EntityTable& table,
                                       Point2i center,
                                       int sight_radius,
                                       Neighborhood n);

  static Point2i runaway_from_nearest_enemy(EntityTable& table,
                                            Point2i center,
                                            int sight_radius,
                                            Neighborhood n);

  // gets the adjacent Point2i from center
  // that represents moving "into the fray"
  // i.e. minimizing the averge distance
  // from all visible enemies
  static Point2i attack_move(EntityTable& table,
                             Point2i center,
                             int sight_radius,
                             Neighborhood n);

  // gets the adjacent Point2i from center
  // that represents moving away from danger
  // i.e. maximizing the averge distance
  // from all visible enemies
  static Point2i runaway_move(EntityTable& table,
                              Point2i center,
                              int sight_radius,
                              Neighborhood n);
};

#endif // EntityBehavior.h included
