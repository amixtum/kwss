#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>

#include "EntityType.h"

class Entity 
{
public:
  Entity(EntityType type, int hp, int stamina);


  EntityType type();

  int hp();

  int stamina();

  bool ability_active();


  void set_hp(int hp);

  void add_hp(int hp);

  void set_stamina(int stamina);

  void add_stamina(int stamina);

  void set_ability(bool on);

  void toggle_ability();

private:
  EntityType _type;

  int _hp;
  int _stamina;

  bool _ability_toggle;
};

#endif // Entity.h included
