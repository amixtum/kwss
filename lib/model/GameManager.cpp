#include "../../include/model/GameManager.h"

#include "../../include/util/TypeParse.h"

#include <random>
#include <string>

#include "../../include/util/DebugLog.h"

GameManager::GameManager(std::string pfile)
  : _battle_table(read_table_from_file(pfile))
  , _entity_table(&_battle_table)
  , _spawner(&_entity_table, _battle_table.get_respawn_interval())
{
  setup();
}

void
GameManager::reset()
{
  _ordering = nullptr;
  _entity_table.reset();
  while (!_spawner.can_spawn(Team::Left)) {
    _spawner.tick();
  }
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

  cleanup();

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
  Point2i right_leader_pos(_entity_table.get_dimensions().x - 1,
                           _entity_table.get_dimensions().y / 2);

  _entity_table.put_entity(left_leader_pos, EntityType::Leader, Team::Left);
  _entity_table.put_entity(right_leader_pos, EntityType::Leader, Team::Right);

  spawn_walls();
}

void
GameManager::cleanup()
{
  auto key_fn = [](Point2i p) { return p.x * p.y + p.x; };

  for (int x = 0; x < _entity_table.get_dimensions().x; x += 1) {
    for (int y = 0; y < _entity_table.get_dimensions().y; y += 1) {
      Point2i pos(x, y);
      auto in_tree = Tree::Search(_ordering, key_fn(pos));
      if (!_entity_table.get_entity(pos).alive()) {
        _entity_table.put_entity(pos, EntityType::Size, Team::Size);
      } else if (in_tree == nullptr) {
        if (_entity_table.get_entity(pos).type() == EntityType::Spy ||
            _entity_table.get_entity(pos).type() == EntityType::Soldier) {
          Tree::InsertNode(
            _ordering, key_fn(pos), _entity_table.get_entity(pos));
        }
      } else {
        // spies get evac'd at the other side of the grid
        if (_entity_table.get_entity(pos).type() == EntityType::Spy) {
          switch (_entity_table.get_entity(pos).team()) {
            case Team::Left:
              if (pos.x >= _entity_table.get_dimensions().x - 1 - (_entity_table.get_dimensions().x / 4)) {
                _entity_table.put_entity(
                  pos, Entity(EntityType::Size, Team::Size, 0, 0, pos));
                auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(pos));
                if (new_root_maybe->parent() == nullptr) {
                  _ordering = new_root_maybe;
                }
              }
              break;
            case Team::Right:
              if (pos.x <= _entity_table.get_dimensions().x / 4) {
                _entity_table.put_entity(
                  pos, Entity(EntityType::Size, Team::Size, 0, 0, pos));
                auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(pos));
                if (new_root_maybe->parent() == nullptr) {
                  _ordering = new_root_maybe;
                }
              }
              break;
            default:
              break;
          }
        } else if (_entity_table.get_entity(pos).type() ==
                   EntityType::Soldier) {
          switch (_entity_table.get_entity(pos).team()) {
            case Team::Left:
              if (pos.x >= _entity_table.get_dimensions().x - 4 &&
                  (pos.y >= _entity_table.get_dimensions().y - 4 ||
                   pos.y <= 3)) {
                _entity_table.put_entity(
                  pos, Entity(EntityType::Size, Team::Size, 0, 0, pos));
                auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(pos));
                if (new_root_maybe->parent() == nullptr) {
                  _ordering = new_root_maybe;
                }
              }
              break;
            case Team::Right:
              if (pos.x <= 3 &&
                  (pos.y >= _entity_table.get_dimensions().y - 4 ||
                   pos.y <= 3)) {
                _entity_table.put_entity(
                  pos, Entity(EntityType::Size, Team::Size, 0, 0, pos));
                auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(pos));
                if (new_root_maybe->parent() == nullptr) {
                  _ordering = new_root_maybe;
                }
              }
              break;
            default:
              break;
          }
        }
      }
    }
  }
}

void
GameManager::spawn_units()
{
  std::random_device rd;
  std::mt19937 gen(rd());

  for (int i = 0; i < _battle_table.get_wave_size(); i += 1) {
    spawn(EntityType::Soldier, Team::Right);
    spawn(EntityType::Soldier, Team::Left);

    spawn(EntityType::Spy, Team::Right);
    spawn(EntityType::Spy, Team::Left);
  }

  _spawner.finish(Team::Left);
  _spawner.finish(Team::Right);
}

void
GameManager::spawn_walls()
{
  for (int _ = 0;
       _ < _entity_table.get_dimensions().y * _battle_table.get_wall_factor();
       _ += 1) {
    spawn(EntityType::Wall, Team::Size);
  }
}

void
GameManager::move_entities()
{
  auto key_fn = [](Point2i p) { return p.x * p.y + p.x; };

  std::vector<Node> random_traverse;
  Tree::Inorder(_ordering, random_traverse);
  auto rng = std::default_random_engine {};
  std::shuffle(std::begin(random_traverse), std::end(random_traverse), rng);

  for (auto& node : random_traverse) {
    auto center = node->value().position();

    // we already deleted this entity when we battled it
    // so just continue
    if (!_entity_table.get_entity(center).alive()) {
      continue;
    }

    auto entity = _entity_table.get_entity(center);
    auto next_pos = center;

    // compute stamina and AbilityState values
    if (entity.state() == AbilityState::Off) {
      if (entity.stamina() < _battle_table.get_max_stamina(entity.type())) {
        auto c = _entity_table.get_entity(center);
        c.add_stamina(1);
        _entity_table.put_entity(center, c);
      }
    } else if (entity.state() == AbilityState::On) {
      auto c = _entity_table.get_entity(center);
      c.add_stamina(-1);
      _entity_table.put_entity(center, c);

      if (!c.has_stamina()) {
        c = _entity_table.get_entity(center);
        c.set_ability_state(AbilityState::Off);
        _entity_table.put_entity(center, c);
      }
    }

    // may have updated entity in the table
    entity = _entity_table.get_entity(center);

    // compute next move
    if (entity.type() == EntityType::Spy) {
      next_pos = EntityBehavior::spy_behavior(
        _entity_table,
        center,
        _battle_table.get_sight_radius(EntityType::Spy),
        Neighborhood::Moore,
        _battle_table.get_distance_threshold(EntityType::Spy));
    } else if (entity.type() == EntityType::Soldier) {
      next_pos = EntityBehavior::soldier_behavior(
        _entity_table,
        center,
        _battle_table.get_sight_radius(EntityType::Soldier),
        Neighborhood::Moore,
        _battle_table.get_distance_threshold(EntityType::Soldier));
    }

    // computed an activate ability/wait
    if (next_pos == center) {
      if (entity.state() == AbilityState::Off && entity.has_stamina()) {
        auto c = _entity_table.get_entity(center);
        c.set_ability_state(AbilityState::On);
        c.add_stamina(-1);
        _entity_table.put_entity(center, c);
        //_entity_table.get_entity(center)->set_ability_state(AbilityState::On);
      }

      // attacking another entity
    } else if (_entity_table.get_entity(next_pos).type() != EntityType::Size) {
      // don't attack the same Team
      if (_entity_table.get_entity(next_pos).team() ==
          _entity_table.get_entity(center).team()) {
        continue;
      }

      if (_entity_table.get_entity(next_pos).state() == AbilityState::On &&
          !_entity_table.get_entity(next_pos).has_stamina()) {
        auto e = _entity_table.get_entity(next_pos);
        e.set_ability_state(AbilityState::Off);
        _entity_table.put_entity(next_pos, e);
      }

      // battle
      _entity_table.battle(center, next_pos);

      if (_entity_table.get_entity(next_pos).type() == EntityType::Leader ||
          (_entity_table.get_entity(next_pos).type() == EntityType::Soldier &&
           _entity_table.get_entity(next_pos).state() == AbilityState::On)) {
        _entity_table.battle(next_pos, center);
      } 

      auto e = _entity_table.get_entity(center);
      e.set_ability_state(AbilityState::Off);
      e.add_stamina(-1);
      _entity_table.put_entity(center, e);

      auto attacker = _entity_table.get_entity(center);
      auto defender = _entity_table.get_entity(next_pos);

      // remove this Entity if it died in battle
      if (!attacker.alive()) {
        auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(center));
        if (new_root_maybe != nullptr) {
          if (new_root_maybe->parent() == nullptr) {
            _ordering = new_root_maybe;
          }
        }

        _entity_table.put_entity(center, EntityType::Size, Team::Size);
      }

      // remove other Entity if it died in battle
      if (!defender.alive() && defender.type() != EntityType::Wall) {
        auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(next_pos));
        if (new_root_maybe != nullptr) {
          if (new_root_maybe->parent() == nullptr) {
            _ordering = new_root_maybe;
          }
        }

        _entity_table.put_entity(next_pos, EntityType::Size, Team::Size);
      } else if (!defender.alive() && defender.type() == EntityType::Wall) {
        _entity_table.put_entity(next_pos, EntityType::Size, Team::Size);
      }

      // move Entity to empty space
    } else {
      auto new_root_maybe = Tree::DeleteNode(_ordering, key_fn(center));
      if (new_root_maybe != nullptr) {
        if (new_root_maybe->parent() == nullptr) {
          _ordering = new_root_maybe;
        }
      }

      new_root_maybe = Tree::DeleteNode(_ordering, key_fn(next_pos));
      if (new_root_maybe != nullptr) {
        if (new_root_maybe->parent() == nullptr) {
          _ordering = new_root_maybe;
        }
      }

      _entity_table.move_entity(center, next_pos);

      auto e = _entity_table.get_entity(next_pos);

      if (_entity_table.get_entity(next_pos).type() == EntityType::Soldier) {
        e.set_ability_state(AbilityState::Off);
        _entity_table.put_entity(next_pos, e);
      }

      Tree::InsertNode(
        _ordering, key_fn(next_pos), _entity_table.get_entity(next_pos));
    }
  }
}

bool
GameManager::spawn(EntityType type, Team team)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto key_fn = [](Point2i p) { return p.x * p.y + p.x; };

  auto spawned = _spawner.spawn(type, team, _battle_table.get_spawn_attempts());

  if (spawned.type() == type) {
    if (type == EntityType::Spy || type == EntityType::Soldier) {
      if (_ordering == nullptr) {
        _ordering = Tree::MakeNode(key_fn(spawned.position()), spawned);
      } else {
        Tree::InsertNode(_ordering, key_fn(spawned.position()), spawned);
      }
    }

    auto e = _entity_table.get_entity(spawned.position());
    e.set_ability_state(AbilityState::Off);
    e.set_type(type);
    e.set_team(team);
    e.set_hp(_battle_table.get_max_hp(type));
    e.set_stamina(_battle_table.get_max_stamina(type));
    e.set_position(spawned.position());
    _entity_table.put_entity(spawned.position(), e);
  }

  return spawned.type() == type;
}
