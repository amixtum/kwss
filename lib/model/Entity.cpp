#include "../../include/model/Entity.h"
#include <string>

Entity::Entity(EntityType type, Team team, int hp, int stamina, Point2i pos)
  : _team(team)
  , _type(type)
  , _hp(hp)
  , _stamina(stamina)
  , _state(AbilityState::Off)
  , _position(pos.x, pos.y)
{
}

Entity::Entity(const Entity& other)
  : _team(other.team())
  , _type(other.type())
  , _hp(other.hp())
  , _stamina(other.stamina())
  , _state(AbilityState::Off)
  , _position(other.position().x, other.position().y)
{
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
Entity::set_type(EntityType type)
{
  _type = type;
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
  _team = other.team();
  _type = other.type();
  _hp = other.hp();
  _stamina = other.stamina();
  _state = other.state();
  _position = other.position();

  return *this;
}

std::string
Entity::to_string()
{
  std::string s;
  s.append("Type: ");

  switch (_type) {
    case EntityType::Soldier:
      s.append("Soldier, ");
      break;
    case EntityType::Spy:
      s.append("Spy, ");
      break;
    case EntityType::Wall:
      s.append("Wall, ");
      break;
    case EntityType::Leader:
      s.append("Leader, ");
      break;
    case EntityType::Size:
      s.append("No Type, ");
      break;
  }

  s.append("Team: ");

  switch (_team) {
    case Team::Left:
      s.append("Left, ");
      break;
    case Team::Right:
      s.append("Right, ");
      break;
    case Team::Size:
      s.append("No Team, ");
      break;
  }

  s.append("State:");

  switch (_state) {
    case AbilityState::Off:
      s.append("Off, ");
      break;
    case AbilityState::On:
      s.append("On, ");
      break;
    case AbilityState::Size:
      s.append("No State, ");
      break;
  }

  s.append("HP: ");
  s.append(std::to_string(_hp));
  s.append(", ");

  s.append("Stamina: ");
  s.append(std::to_string(_stamina));
  s.append(", ");

  s.append("Position: ");
  s.append(_position.to_string());

  return s;
}
