#include "../../include/model/GameManager.h"

#include "../../include/util/TypeParse.h"

#include <random>

#include "../../include/util/DebugLog.h"

GameManager::GameManager(std::string pfile)
  : _battle_table(read_table_from_file(pfile))
  , _entity_table(&_battle_table)
  , _spawner(&_entity_table, _battle_table.get_respawn_interval())
{
  setup();
}

bool
GameManager::tick()
{
  move_entities();

  auto winner = _entity_table.get_winner();
  if (winner != Team::Size) {
    return false;
  }

  _spawner.tick();

  if (_spawner.can_spawn(Team::Left) && _spawner.can_spawn(Team::Right)) {
    spawn_units();
  }

  return true;
}

Team
GameManager::winner()
{
  return _entity_table.get_winner();
}

EntityTable&
GameManager::grid()
{
  return _entity_table;
}

void 
GameManager::setup()
{
  Point2i left_leader_pos(0, _entity_table.get_dimensions().y / 2);
  Point2i right_leader_pos(
      _entity_table.get_dimensions().x - 1, 
      _entity_table.get_dimensions().y / 2);


  _entity_table.put_entity(left_leader_pos, 
                           EntityType::Leader, 
                           Team::Left);
  _entity_table.put_entity(right_leader_pos, 
                           EntityType::Leader, 
                           Team::Right);

  spawn_walls();
}

void
GameManager::spawn_units()
{
 for (int i = 0; i < _battle_table.get_wave_size(); i += 1) {
    spawn(EntityType::Soldier, Team::Left);

    spawn(EntityType::Soldier, Team::Right);

    spawn(EntityType::Spy, Team::Left);

    spawn(EntityType::Soldier, Team::Right);

    spawn(EntityType::Soldier, Team::Left);

    spawn(EntityType::Spy, Team::Right);
  }

  _spawner.finish(Team::Left);
  _spawner.finish(Team::Right);
}

void
GameManager::spawn_walls()
{
  for (int _ = 0; _ < _entity_table.get_dimensions().y * _battle_table.get_wall_factor(); _ += 1) {
    spawn(EntityType::Wall, Team::Size);
  }
}

void
GameManager::move_entities()
{
  DebugLog logger;
  for (int i = 0; i < (int)_ordering.size(); i += 1) {
    auto entity = _ordering[i];
    auto center = entity->position();
    auto next_pos = center;

    // compute stamina and AbilityState values
    if (entity->state() == AbilityState::Off) {
      if (entity->stamina() < 
          _battle_table.get_max_stamina(entity->type())) {
        entity->add_stamina(1);
      }
    } else if (entity->state() == AbilityState::On) {
      entity->add_stamina(-1);

      if (!entity->has_stamina()) {
        entity->set_ability_state(AbilityState::Off);
      }
    } else {
      logger.write("Moveable Entity does not have a state");
    }
    
    // compute next move
    if (entity->type() == EntityType::Spy) {
      next_pos = EntityBehavior::spy_behavior(
          _entity_table,
          center,
          _battle_table.get_sight_radius(EntityType::Spy),
          Neighborhood::Moore,
          _battle_table.get_distance_threshold(EntityType::Spy)
      );
    } else if (entity->type() == EntityType::Soldier) {
      next_pos = EntityBehavior::soldier_behavior(
          _entity_table,
          center,
          _battle_table.get_sight_radius(EntityType::Soldier),
          Neighborhood::Moore,
          _battle_table.get_distance_threshold(EntityType::Soldier)
      );
    }

    // computed an activate ability/wait
    if (next_pos == center) {
      if (entity->state() == AbilityState::Off &&
          entity->has_stamina()) {
        entity->set_ability_state(AbilityState::On);
      }

    // attacking another entity
    } else if (_entity_table.get_entity(next_pos).type() != 
               EntityType::Size) {
      if (_entity_table.get_entity(next_pos).team() == 
          entity->team()) {
        continue;
      }

      _battle_table.battle(
          _entity_table.get_entity(entity->position()),
          _entity_table.get_entity(next_pos)
      );

      if (!_entity_table.get_entity(entity->position()).alive()) {
        if (!entity->alive()) {
          logger.write("Pointers working as planned");
        }

        to_remove[i] = true;
      } else if (!_entity_table.get_entity(next_pos).alive() &&
                  _entity_table.get_entity(next_pos).type() != 
                  EntityType::Wall) {
        for (int k = 0; k < (int)_ordering.size(); k += 1) {
          if (_ordering[k]->position() == next_pos) {
            to_remove[k] = true;
            break;
          }
        }       
      }
    } else {
      _entity_table.move_entity(entity->position(), next_pos);
    }
  }
}

bool
GameManager::spawn(EntityType type, Team team)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto swp_last = [] (std::vector<Entity*>& e, int i) {
    auto temp = e[e.size() - 1];
    e[e.size() - 1] = e[i];
    e[i] = temp;
  };

  auto spawned = _spawner.spawn(type, team, _battle_table.get_spawn_attempts());

  if (spawned.type() == type &&
      (type == EntityType::Spy || type == EntityType::Soldier)) {
  }

  _entity_table.get_entity(spawned.position()).set_ability_state(AbilityState::Off);

  return spawned.type() == type;
}
