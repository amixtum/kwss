#include "../../include/model/Entity.h"


Entity::Entity(EntityType type, int hp, int stamina)
{
  _type = type;
  _hp = hp;
  _stamina = stamina;
}


EntityType Entity::type()
{
  return _type;
}

int Entity::hp()
{
  return _hp;
}

int Entity::stamina()
{
  return _stamina;
}

bool Entity::ability_active()
{
  return _ability_toggle;
}


void Entity::set_hp(int hp)
{
  _hp = hp;
}

void Entity::set_stamina(int stamina)
{
  _stamina = stamina;
}

void Entity::add_hp(int hp)
{
  _hp += hp;
}

void Entity::add_stamina(int stamina)
{
  _stamina += stamina;
}

void Entity::set_ability(bool on)
{
  _ability_toggle = on;
}

void Entity::toggle_ability() 
{
  _ability_toggle = !_ability_toggle;
}
