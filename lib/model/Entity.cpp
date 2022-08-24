#include "../../include/model/Entity.h"

Entity::Entity(EntityType type, Team team, int hp, int stamina)
{
  _type = type;
  _team = team;
  _hp = hp;
  _stamina = stamina;
  _state = AbilityState::Off;
}

EntityType
Entity::type()
{
  return _type;
}

Team
Entity::team()
{
  return _team;
}

int
Entity::hp()
{
  return _hp;
}

int
Entity::stamina()
{
  return _stamina;
}

AbilityState
Entity::state()
{
  return _state;
}

bool
Entity::alive()
{
  return hp() > 0;
}

bool
Entity::has_stamina()
{
  return stamina() > 0;
}

void
Entity::set_team(Team team)
{
  _team = team;
}

void
Entity::set_hp(int hp)
{
  _hp = hp;
}

void
Entity::set_stamina(int stamina)
{
  _stamina = stamina;
}

void
Entity::add_hp(int hp)
{
  _hp += hp;
}

void
Entity::add_stamina(int stamina)
{
  _stamina += stamina;
}

void
Entity::set_ability_state(AbilityState state)
{
  _state = state;
}

// only use if ability is toggleable
void
Entity::toggle_ability_state()
{
  if (_state == AbilityState::On) {
    _state = AbilityState::Off;
  } else if (_state == AbilityState::Off) {
    _state = AbilityState::On;
  }
}

Entity&
Entity::operator=(const Entity& other)
{
  _team = other._team;
  _type = other._type;
  _hp = other._hp;
  _stamina = other._stamina;
  _state = other._state;

  return *this;
}
