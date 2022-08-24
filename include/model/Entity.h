#ifndef ENTITY_H_
#define ENTITY_H_

#include <memory>

#include "AbilityState.h"
#include "EntityType.h"
#include "Point2i.h"
#include "Team.h"

class Entity
{
public:
  Entity(EntityType type, Team team, int hp, int stamina, Point2i pos);

  Entity(const Entity& other);

  EntityType type() const;

  Team team() const;

  int hp() const;

  int stamina() const;

  AbilityState state() const;

  Point2i position() const;

  bool alive() const;

  bool has_stamina() const;

  void set_team(Team team);

  void set_hp(int hp);

  void add_hp(int hp);

  void set_stamina(int stamina);

  void add_stamina(int stamina);

  void set_ability_state(AbilityState state);

  void toggle_ability_state();

  void set_position(Point2i pos);

  Entity& operator=(const Entity& other);

protected:
  Team _team;
  EntityType _type;

  int _hp;
  int _stamina;

  AbilityState _state;

  Point2i _position;
};

#endif // Entity.h included
