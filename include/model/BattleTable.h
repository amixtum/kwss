#ifndef BATTLE_TABLE_H_
#define BATTLE_TABLE_H_

#include <array>
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

  std::function<void(Entity&, Entity&)> get_fn(Entity& attacker,
                                               Entity& defender);

  int get_dmg(Entity& attacker, Entity& defender);

  int get_max_hp(EntityType entity);

  int get_max_stamina(EntityType entity);

  void battle(Entity& attacker, Entity& defender);

private:
  FnTable _fn_table;
  DmgTable _dmg_table;
  HPTable _hp_table;
  StaminaTable _stamina_table;
};

#endif // BattleTable.h included
