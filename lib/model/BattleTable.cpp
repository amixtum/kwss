#include <random>

#include "../../include/model/BattleTable.h"

BattleTable::BattleTable()
{
  auto basic_attack = [this](Entity att, Entity def) {
    auto def_c = def;
    def_c.add_hp(-get_dmg(att, def));
    return std::make_pair(att, def_c);
  };

  auto basic_attack_reverse = [this](Entity att, Entity def) {
    auto att_c = att;
    att_c.add_hp(-get_dmg(def, att));
    return std::make_pair(att_c, def);
  };

  auto uncloak_spy_def = [](Entity att, Entity def) {
    auto def_c = def;
    def_c.set_ability_state(AbilityState::Off);
    return std::make_pair(att, def_c);
  };

  auto uncloak_spy_att = [](Entity att, Entity def) {
    auto att_c = att;
    att_c.set_ability_state(AbilityState::Off);
    return std::make_pair(att_c, def);
  };

  auto try_turn_def = [this](Entity att, Entity def) {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto def_c = def;
    // in the case of spy vs-> spy get_dmg is in [0, 100]
    // which represents the probability of turning another spy
    if (static_cast<int>(gen() % 100) < get_dmg(att, def)) {
      def_c.set_team(att.team());
    }

    return std::make_pair(att, def_c);
  };

  auto turn_def = [](Entity att, Entity def) {
    auto def_c = def;
    def_c.set_team(att.team());
    return std::make_pair(att, def_c);
  };

  auto nothing = [](Entity att, Entity def) {
    return std::make_pair(att, def);
  };

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
                    Fn fn)
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

void
BattleTable::set_sight_radius(EntityType type, int radius)
{
  _sight_table[static_cast<int>(type)] = radius;
}

void
BattleTable::set_distance_threshold(EntityType type, float distance_threshold)
{
  _threshold_table[static_cast<int>(type)] = distance_threshold;
}

void
BattleTable::set_dimensions(Point2i dimensions)
{
  _dimensions = dimensions;
}

void
BattleTable::set_respawn_interval(int n)
{
  _respawn_interval = n;
}

void
BattleTable::set_spawn_attempts(int n)
{
  _spawn_attempts = n;
}

void
BattleTable::set_wave_size(int n)
{
  _wave_size = n;
}

void
BattleTable::set_wall_factor(float n)
{
  _wall_factor = n;
}

Fn
BattleTable::get_fn(Entity attacker, Entity defender)
{
  return _fn_table[static_cast<int>(attacker.type())][static_cast<int>(
    defender.type())][static_cast<int>(attacker.state())]
                  [static_cast<int>(defender.state())];
}

int
BattleTable::get_dmg(Entity attacker, Entity defender)
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

int
BattleTable::get_sight_radius(EntityType type)
{
  return _sight_table[static_cast<int>(type)];
}

float
BattleTable::get_distance_threshold(EntityType type)
{
  return _threshold_table[static_cast<int>(type)];
}

Point2i
BattleTable::get_dimensions()
{
  return _dimensions;
}

int
BattleTable::get_respawn_interval()
{
  return _respawn_interval;
}

int
BattleTable::get_spawn_attempts()
{
  return _spawn_attempts;
}

int
BattleTable::get_wave_size()
{
  return _wave_size;
}

float
BattleTable::get_wall_factor()
{
  return _wall_factor;
}

std::pair<Entity, Entity>
BattleTable::battle(Entity attacker, Entity defender)
{
  if (attacker.type() == EntityType::Size ||
      defender.type() == EntityType::Size) {
    return std::make_pair(attacker, defender);
  }

  return _fn_table[static_cast<int>(attacker.type())][static_cast<int>(
    defender.type())][static_cast<int>(attacker.state())]
                  [static_cast<int>(defender.state())](attacker, defender);
}
