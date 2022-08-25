#ifndef BATTLE_TABLE_H_
#define BATTLE_TABLE_H_

#include <array>
#include <climits>
#include <functional>

#include "AbilityState.h"
#include "Entity.h"
#include "EntityType.h"

using FnTable = std::array<
  std::array<std::array<std::array<std::function<void(Entity&, Entity&)>,
                                   static_cast<int>(AbilityState::Size)>,
                        static_cast<int>(AbilityState::Size)>,
             static_cast<int>(EntityType::Size)>,
  static_cast<int>(EntityType::Size)>;

using DmgTable = std::array<
  std::array<std::array<std::array<int, static_cast<int>(AbilityState::Size)>,
                        static_cast<int>(AbilityState::Size)>,
             static_cast<int>(EntityType::Size)>,
  static_cast<int>(EntityType::Size)>;

using HPTable = std::array<int, static_cast<int>(EntityType::Size)>;

using StaminaTable = std::array<int, static_cast<int>(EntityType::Size)>;

using SightTable = std::array<int, static_cast<int>(EntityType::Size)>;

using ThresholdTable = std::array<float, static_cast<int>(EntityType::Size)>;

class BattleTable
{
public:
  BattleTable();

  void set_fn(EntityType attack_type,
              EntityType defend_type,
              AbilityState attack_state,
              AbilityState defend_state,
              std::function<void(Entity&, Entity&)> fn);

  void set_dmg(EntityType attack_type,
               EntityType defend_type,
               AbilityState attack_state,
               AbilityState defend_state,
               int dmg);

  void set_max_hp(EntityType entity, int hp);

  void set_max_stamina(EntityType entity, int stamina);

  void set_sight_radius(EntityType entity, int sight_radius);

  void set_distance_threshold(EntityType type, float threshold);

  void set_dimensions(Point2i dimensions);

  void set_respawn_interval(int n);

  void set_spawn_attempts(int n);

  void set_wave_size(int n);

  std::function<void(Entity&, Entity&)> get_fn(Entity& attacker,
                                               Entity& defender);

  int get_dmg(Entity& attacker, Entity& defender);

  int get_max_hp(EntityType entity);

  int get_max_stamina(EntityType entity);

  int get_sight_radius(EntityType type);

  float get_distance_threshold(EntityType type);

  Point2i get_dimensions();

  int get_respawn_interval();

  int get_spawn_attempts();

  int get_wave_size();

  void battle(Entity& attacker, Entity& defender);

private:
  FnTable _fn_table;
  DmgTable _dmg_table;
  HPTable _hp_table;
  StaminaTable _stamina_table;
  SightTable _sight_table;
  ThresholdTable _threshold_table;
  Point2i _dimensions = Point2i(-1, -1);
  int _respawn_interval = INT_MAX;
  int _spawn_attempts = 0;
  int _wave_size = 0;
};

#endif // BattleTable.h included
