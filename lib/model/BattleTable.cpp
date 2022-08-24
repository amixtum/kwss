#include <random>

#include "../../include/model/BattleTable.h"

BattleTable::BattleTable()
{
  auto basic_attack = [this](Entity& att, Entity& def) {
    def.add_hp(get_dmg(att, def));
  };

  auto basic_attack_reverse = [this](Entity& att, Entity& def) {
    att.add_hp(get_dmg(def, att));
  };

  auto uncloak_spy_def = [](Entity&, Entity& def) {
    def.set_ability_state(AbilityState::Off);
  };

  auto uncloak_spy_att = [](Entity& att, Entity&) {
    att.set_ability_state(AbilityState::Off);
  };

  auto try_turn_def = [this](Entity& att, Entity& def) {
    std::random_device rd;
    std::mt19937 gen(rd());

    // in the case of spy vs. spy get_dmg is in [0, 100]
    // which represents the probability of turning another spy
    if (static_cast<int>(gen() % 100) < get_dmg(att, def)) {
      def.set_team(att.team());
    }
  };

  auto turn_def = [](Entity& att, Entity& def) { def.set_team(att.team()); };

  auto nothing = [](Entity&, Entity&) { return; };

  set_fn(EntityType::Soldier,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::On,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Soldier,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::On,
         basic_attack_reverse);

  set_fn(EntityType::Soldier,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::On,
         uncloak_spy_def);

  set_fn(EntityType::Spy,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack_reverse);

  set_fn(EntityType::Spy,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::On,
         basic_attack_reverse);

  set_fn(EntityType::Spy,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Spy,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::On,
         uncloak_spy_att);

  set_fn(EntityType::Spy,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::Off,
         try_turn_def);

  set_fn(EntityType::Spy,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::Off,
         turn_def);

  set_fn(EntityType::Spy,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Soldier,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::On,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Soldier,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Leader,
         EntityType::Spy,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Leader,
         EntityType::Spy,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack_reverse);

  set_fn(EntityType::Spy,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::On,
         basic_attack_reverse);

  set_fn(EntityType::Spy,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Spy,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Leader,
         EntityType::Soldier,
         AbilityState::Off,
         AbilityState::On,
         basic_attack);

  set_fn(EntityType::Leader,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Leader,
         EntityType::Soldier,
         AbilityState::On,
         AbilityState::On,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::Off,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::On,
         basic_attack);

  set_fn(EntityType::Soldier,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Soldier,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Leader,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Wall,
         EntityType::Leader,
         AbilityState::On,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Wall,
         AbilityState::Off,
         AbilityState::On,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::Off,
         nothing);

  set_fn(EntityType::Leader,
         EntityType::Wall,
         AbilityState::On,
         AbilityState::On,
         nothing);
}

void
BattleTable::set_fn(EntityType attack_type,
                    EntityType defend_type,
                    AbilityState attack_state,
                    AbilityState defend_state,
                    std::function<void(Entity&, Entity&)> fn)
{
  _fn_table[static_cast<int>(attack_type)][static_cast<int>(defend_type)]
           [static_cast<int>(attack_state)][static_cast<int>(defend_state)] =
             fn;
}

void
BattleTable::set_dmg(EntityType attack_type,
                     EntityType defend_type,
                     AbilityState attack_state,
                     AbilityState defend_state,
                     int dmg)
{
  _dmg_table[static_cast<int>(attack_type)][static_cast<int>(defend_type)]
            [static_cast<int>(attack_state)][static_cast<int>(defend_state)] =
              dmg;
}

void
BattleTable::set_max_hp(EntityType entity, int hp)
{
  _hp_table[static_cast<int>(entity)] = hp;
}

void
BattleTable::set_max_stamina(EntityType entity, int stamina)
{
  _stamina_table[static_cast<int>(entity)] = stamina;
}

std::function<void(Entity&, Entity&)>
BattleTable::get_fn(Entity& attacker, Entity& defender)
{
  return _fn_table[static_cast<int>(attacker.type())][static_cast<int>(
    defender.type())][static_cast<int>(attacker.state())]
                  [static_cast<int>(defender.state())];
}

int
BattleTable::get_dmg(Entity& attacker, Entity& defender)
{
  return _dmg_table[static_cast<int>(attacker.type())][static_cast<int>(
    defender.type())][static_cast<int>(attacker.state())]
                   [static_cast<int>(defender.state())];
}

int
BattleTable::get_max_hp(EntityType entity)
{
  return _hp_table[static_cast<int>(entity)];
}

int
BattleTable::get_max_stamina(EntityType entity)
{
  return _stamina_table[static_cast<int>(entity)];
}

void
BattleTable::battle(Entity& attacker, Entity& defender)
{
  if (attacker.type() == EntityType::Size ||
      defender.type() == EntityType::Size) {
    return;
  }

  _fn_table[static_cast<int>(attacker.type())][static_cast<int>(
    defender.type())][static_cast<int>(attacker.state())]
           [static_cast<int>(defender.state())](attacker, defender);
}
