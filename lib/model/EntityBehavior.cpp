#include "../../include/model/EntityBehavior.h"
#include "../../include/util/DebugLog.h"
#include <climits>
#include <cmath>
#include <random>

Point2i
EntityBehavior::spy_behavior(EntityTable& table,
                             Point2i center,
                             int sight_radius,
                             Neighborhood n,
                             float distance_threshold)
{
  DebugLog logger;

  std::random_device rd;
  std::mt19937 gen(rd());

  auto entity = table.get_entity(center);
  if (entity.type() != EntityType::Spy) {
    logger.write("Wrong entity type in spy_behavior at position ");
    logger.write(center.to_string());
    logger.write("\n");
  }

  auto enemies = EntityBehavior::count_enemy(table, center, sight_radius, n);

  if (enemies != 0) {
    auto friendly =
      EntityBehavior::count_friendly(table, center, sight_radius, n);

    auto nearest_distance =
      EntityBehavior::nearest_enemy_distance(table, center, sight_radius, n);

    if (friendly <= enemies - 1) {
      if (entity.state() == AbilityState::On) {
        if (nearest_distance <= distance_threshold) {
          return EntityBehavior::move_to_nearest_enemy(
            table, center, sight_radius, n);
        } else {
          return EntityBehavior::move_to_nearest_enemy(
            table, center, sight_radius, n);
        }
      }

      if (nearest_distance <= distance_threshold) {
        if (table.get_entity(center).has_stamina()) {
          return center;
        } else {
          return EntityBehavior::runaway_from_nearest_enemy(
            table, center, sight_radius, n);
        }
      }

      return EntityBehavior::runaway_from_nearest_enemy(
        table, center, sight_radius, n);
    } else {
      if (entity.state() == AbilityState::On) {
        return EntityBehavior::move_to_nearest_enemy(
          table, center, sight_radius, n);
      } else {
        if (nearest_distance <= distance_threshold) {
          return EntityBehavior::move_to_nearest_of_type_enemy(
            table, center, sight_radius, n, EntityType::Spy);
        } else {
          return EntityBehavior::attack_move(table, center, sight_radius, n);
        }
      }
    }
  } else {
    return EntityBehavior::random_move(table, center, sight_radius, n);
  }
}

Point2i
EntityBehavior::soldier_behavior(EntityTable& table,
                                 Point2i center,
                                 int sight_radius,
                                 Neighborhood n,
                                 float distance_threshold)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto enemies = EntityBehavior::count_enemy(table, center, sight_radius, n);

  if (enemies != 0) {
    auto friendly =
      EntityBehavior::count_friendly(table, center, sight_radius, n);

    auto nearest_distance =
      EntityBehavior::nearest_enemy_distance(table, center, sight_radius, n);

    if (friendly <= enemies - 1) {
      if (nearest_distance <= distance_threshold) {
        return EntityBehavior::runaway_from_nearest_enemy(
          table, center, sight_radius, n);
      }

      if (table.get_entity(center).has_stamina()) {
        return center;
      } else {
        return EntityBehavior::runaway_move(table, center, sight_radius, n);
      }
    } else {
      if (nearest_distance <= distance_threshold) {
        return EntityBehavior::move_to_nearest_enemy(
          table, center, sight_radius, n);
      }

      return EntityBehavior::attack_move(table, center, sight_radius, n);
    }
  } else {
    return EntityBehavior::random_move(table, center, sight_radius, n);
  }
}

Point2i
EntityBehavior::random_move(EntityTable& table,
                            Point2i center,
                            int sight_radius,
                            Neighborhood n)
{

  std::random_device rd;
  std::mt19937 gen(rd());
  auto entity = table.get_entity(center);
  auto valid_moves = table.empty_in_radius(center, 1, n);

  if (valid_moves.size() > 0) {
    std::vector<Point2i> advancing_moves;
    for (auto& p : valid_moves) {
      if (p.first != center) {
        if (entity.team() == Team::Left) {
          if (p.first.x >= center.x) {
            advancing_moves.push_back(p.first);
          }
        } else if (entity.team() == Team::Right) {
          if (p.first.x <= center.x) {
            advancing_moves.push_back(p.first);
          }
        }
      }
    }
    if (advancing_moves.size() > 0) {
      return advancing_moves[gen() % advancing_moves.size()];
    } else {
      auto iter = valid_moves.begin();
      std::advance(iter, gen() % valid_moves.size());
      return iter->first;
    }
  } else {
    if (table.get_entity(center).type() == EntityType::Soldier) {
      auto try_wall = EntityBehavior::move_to_nearest_of_type(
        table, center, sight_radius, n, EntityType::Wall);
      return try_wall;
    } else {
      return center;
    }
  }
}

float
EntityBehavior::nearest_enemy_distance(EntityTable& table,
                                       Point2i center,
                                       int sight_radius,
                                       Neighborhood n)
{
  Point2i move_to(center.x, center.y);
  Point2i nearest = move_to;
  auto enemies = table.enemies_in_radius(center, sight_radius, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float min_dist = MAXFLOAT;

    for (auto& enemy : enemies) {
      auto diff = center - enemy.first;
      float dist = diff.length();
      if (dist < min_dist) {
        min_dist = dist;
        nearest = enemy.first;
      }
    }

    return (nearest - center).length();
  }

  return MAXFLOAT;
}

int
EntityBehavior::count_type(EntityTable& table,
                           Point2i center,
                           int sight_radius,
                           Neighborhood n,
                           EntityType type)
{
  return table.in_radius(center, sight_radius, type, n).size();
}

int
EntityBehavior::count_type_enemy(EntityTable& table,
                                 Point2i center,
                                 int sight_radius,
                                 Neighborhood n,
                                 EntityType type)
{
  return table.enemies_in_radius(center, sight_radius, type, n).size();
}

int
EntityBehavior::count_type_friendly(EntityTable& table,
                                    Point2i center,
                                    int sight_radius,
                                    Neighborhood n,
                                    EntityType type)
{
  return table.friendly_in_radius(center, sight_radius, type, n).size();
}

int
EntityBehavior::count_enemy(EntityTable& table,
                            Point2i center,
                            int sight_radius,
                            Neighborhood n)
{
  return table.enemies_in_radius(center, sight_radius, n).size();
}

int
EntityBehavior::count_friendly(EntityTable& table,
                               Point2i center,
                               int sight_radius,
                               Neighborhood n)
{
  return table.friendly_in_radius(center, sight_radius, n).size();
}

Point2i
EntityBehavior::move_to_nearest_of_type(EntityTable& table,
                                        Point2i center,
                                        int sight_radius,
                                        Neighborhood n,
                                        EntityType type)
{
  Point2i move_to(center.x, center.y);
  Point2i nearest = move_to;
  auto enemies = table.in_radius(center, sight_radius, type, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float min_dist = MAXFLOAT;

    for (auto& enemy : enemies) {
      auto diff = center - enemy.first;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist < min_dist) {
        min_dist = dist;
        nearest = enemy.first;
      }
    }

    if ((nearest - center).length() < 2) {
      return nearest;
    }

    min_dist = MAXFLOAT;
    for (auto& pos : valid_moves) {
      auto diff = pos.first - nearest;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist < min_dist) {
        min_dist = dist;
        move_to = pos.first;
      }
    }
  }

  return move_to;
}

Point2i
EntityBehavior::move_to_nearest_of_type_enemy(EntityTable& table,
                                              Point2i center,
                                              int sight_radius,
                                              Neighborhood n,
                                              EntityType type)
{
  Point2i move_to(center.x, center.y);
  Point2i nearest = move_to;
  auto enemies = table.enemies_in_radius(center, sight_radius, type, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float min_dist = MAXFLOAT;

    for (auto& enemy : enemies) {
      auto diff = center - enemy.first;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist < min_dist) {
        min_dist = dist;
        nearest = enemy.first;
      }
    }

    if ((nearest - center).length() < 2) {
      return nearest;
    }

    min_dist = MAXFLOAT;
    for (auto& pos : valid_moves) {
      auto diff = pos.first - nearest;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist < min_dist) {
        min_dist = dist;
        move_to = pos.first;
      }
    }
  }

  return move_to;
}

Point2i
EntityBehavior::move_to_nearest_enemy(EntityTable& table,
                                      Point2i center,
                                      int sight_radius,
                                      Neighborhood n)
{
  Point2i move_to(center.x, center.y);
  Point2i nearest = move_to;
  auto enemies = table.enemies_in_radius(center, sight_radius, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float min_dist = MAXFLOAT;

    for (auto& enemy : enemies) {
      auto diff = center - enemy.first;
      float dist = diff.length();
      if (dist < min_dist) {
        min_dist = dist;
        nearest = enemy.first;
      }
    }

    if ((nearest - center).length() < 2) {
      return nearest;
    }

    min_dist = MAXFLOAT;
    for (auto& pos : valid_moves) {
      auto diff = pos.first - nearest;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist < min_dist) {
        min_dist = dist;
        move_to = pos.first;
      }
    }
  }

  return move_to;
}

Point2i
EntityBehavior::runaway_from_nearest_enemy(EntityTable& table,
                                           Point2i center,
                                           int sight_radius,
                                           Neighborhood n)
{
  Point2i move_to(center.x, center.y);
  Point2i nearest = move_to;
  auto enemies = table.enemies_in_radius(center, sight_radius, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float min_dist = MAXFLOAT;
    float max_dist = 0;

    for (auto& enemy : enemies) {
      auto diff = center - enemy.first;
      float dist = diff.length();
      if (dist < min_dist) {
        min_dist = dist;
        nearest = enemy.first;
      }
    }

    for (auto& pos : valid_moves) {
      auto diff = pos.first - nearest;
      float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
      if (dist > max_dist) {
        max_dist = dist;
        move_to = pos.first;
      }
    }
  }

  return move_to;
}

Point2i
EntityBehavior::attack_move(EntityTable& table,
                            Point2i center,
                            int sight_radius,
                            Neighborhood n)
{
  Point2i move_to(center.x, center.y);
  auto enemies = table.enemies_in_radius(center, sight_radius, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float avg_dist = 0;
    float min_dist = MAXFLOAT;

    for (auto& pos : valid_moves) {
      for (auto& enemy : enemies) {
        auto diff = pos.first - enemy.first;
        float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));

        avg_dist += dist;
      }

      avg_dist /= enemies.size();

      if (avg_dist < min_dist) {
        min_dist = avg_dist;
        move_to = pos.first;
      }

      avg_dist = 0;
    }
  }

  return move_to;
}

Point2i
EntityBehavior::runaway_move(EntityTable& table,
                             Point2i center,
                             int sight_radius,
                             Neighborhood n)
{
  Point2i move_to(center.x, center.y);
  auto enemies = table.enemies_in_radius(center, sight_radius, n);

  if (enemies.size() != 0) {
    auto valid_moves = table.empty_in_radius(center, 1, n);
    float avg_dist = 0;
    float max_dist = 0;

    for (auto& pos : valid_moves) {
      for (auto& enemy : enemies) {
        auto diff = pos.first - enemy.first;
        float dist = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));

        avg_dist += dist;
      }

      avg_dist /= enemies.size();

      if (avg_dist > max_dist) {
        max_dist = avg_dist;
        move_to = pos.first;
      }

      avg_dist = 0;
    }
  }

  return move_to;
}
