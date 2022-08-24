#include "../../include/model/Entity.h"

Entity::Entity(EntityType type, Team team, int hp, int stamina, Point2i pos)
  : _position(pos.x, pos.y)
{
  _type = type;
  _team = team;
  _hp = hp;
  _stamina = stamina;
  _state = AbilityState::Off;
}

Entity::Entity(const Entity& other)
  : _position(other.position().x, other.position().y)
{
  _type = other.type();
  _team = other.team();
  _hp = other.hp();
  _stamina = other.stamina();
  _state = other.state();
}

EntityType
Entity::type() const
{
  return _type;
}

Team
Entity::team() const
{
  return _team;
}

int
Entity::hp() const
{
  return _hp;
}

int
Entity::stamina() const
{
  return _stamina;
}

AbilityState
Entity::state() const
{
  return _state;
}

Point2i
Entity::position() const
{
  return _position;
}

bool
Entity::alive() const
{
  return hp() > 0;
}

bool
Entity::has_stamina() const
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

void
Entity::set_position(Point2i pos)
{
  _position = pos;
}

Entity&
Entity::operator=(const Entity& other)
{
  _team = other._team;
  _type = other._type;
  _hp = other._hp;
  _stamina = other._stamina;
  _state = other._state;
  _position = other._position;

  return *this;
}
