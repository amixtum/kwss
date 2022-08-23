#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>

#include "AbilityState.h"
#include "EntityType.h"
#include "Team.h"

class Entity
{
public:
  Entity(EntityType type, Team team, int hp, int stamina);

  EntityType type();

  Team team();

  int hp();

  int stamina();

  AbilityState state();

  bool alive();

  bool has_stamina();

  void set_team(Team team);

  void set_hp(int hp);

  void add_hp(int hp);

  void set_stamina(int stamina);

  void add_stamina(int stamina);

  void set_ability_state(AbilityState state);

  void toggle_ability_state();

private:
  Team _team;
  EntityType _type;

  int _hp;
  int _stamina;

  AbilityState _state;
};

#endif // Entity.h included
