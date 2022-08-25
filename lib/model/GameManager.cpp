#include "../../include/model/GameManager.h"

#include "../../include/util/TypeParse.h"

#include <random>

GameManager::GameManager(std::string pfile)
  : _battle_table(read_table_from_file(pfile))
  , _entity_table(&_battle_table)
  , _spawner(&_entity_table, _battle_table.get_respawn_interval())
{
}

void
GameManager::move_entities()
{
  auto key_fn = [](const Entity& e) {
    return e.position().x * e.position().y + e.position().x;
  };

  std::vector<Node> random_ordering;

  BinarySearchTree<int, Entity>::RandomTraverse(_move_tree, random_ordering);

  for (auto& entity : random_ordering) {
    // Manage stamina
    if (entity->value().state() == AbilityState::On) {
      _entity_table.get_entity(entity->value().position()).add_stamina(-1);

      if (!_entity_table.get_entity(entity->value().position()).has_stamina()) {
        _entity_table.get_entity(entity->value().position())
          .set_ability_state(AbilityState::Off);
      }
    } else if (entity->value().state() == AbilityState::Off) {
      _entity_table.get_entity(entity->value().position()).add_stamina(1);
    }

    // Delete Entitys that are not alive
    if (!entity->value().alive()) {
      Tree::DeleteNode(entity);
      _entity_table.put_entity(
        entity->value().position(), EntityType::Size, Team::Size);
      continue;
    }

    Point2i next_pos = entity->value().position();

    // Compute next position
    if (entity->value().type() == EntityType::Spy) {
      next_pos = EntityBehavior::spy_behavior(
        _entity_table,
        entity->value().position(),
        _battle_table.get_sight_radius(EntityType::Spy),
        Neighborhood::Moore,
        _battle_table.get_distance_threshold(EntityType::Spy));

    } else if (entity->value().type() == EntityType::Soldier) {
      next_pos = EntityBehavior::soldier_behavior(
        _entity_table,
        entity->value().position(),
        _battle_table.get_sight_radius(EntityType::Soldier),
        Neighborhood::Moore,
        _battle_table.get_distance_threshold(EntityType::Soldier));
    }

    auto at_pos = _entity_table.get_entity(next_pos);

    // Activate ability
    if (next_pos == entity->value().position()) {
      if (at_pos.state() == AbilityState::Off && at_pos.has_stamina()) {
        _entity_table.get_entity(next_pos).set_ability_state(AbilityState::On);
      }

      // Attack Entity at next position
    } else if (at_pos.type() != EntityType::Size) {
      _battle_table.battle(_entity_table.get_entity(entity->value().position()),
                           _entity_table.get_entity(next_pos));
      Tree::DeleteNode(_move_tree, key_fn(entity->value()));
      Tree::DeleteNode(_move_tree, key_fn(at_pos));

      if (_entity_table.get_entity(next_pos).alive()) {
        Tree::InsertNode(
          _move_tree, key_fn(at_pos), _entity_table.get_entity(next_pos));
      } else {
        _entity_table.put_entity(next_pos, EntityType::Size, Team::Size);
      }

      if (_entity_table.get_entity(entity->value().position()).alive()) {
        Tree::InsertNode(_move_tree,
                         key_fn(at_pos),
                         _entity_table.get_entity(entity->value().position()));
      } else {
        _entity_table.put_entity(
          entity->value().position(), EntityType::Size, Team::Size);
      }

      // Move Entity to empty space
    } else {

      Tree::DeleteNode(_move_tree, key_fn(entity->value()));

      _entity_table.move_entity(entity->value().position(), next_pos);

      Tree::InsertNode(_move_tree,
                       key_fn(_entity_table.get_entity(next_pos)),
                       _entity_table.get_entity(next_pos));
    }
  }
}

bool
GameManager::spawn(EntityType type, Team team)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  auto key_fn = [](Entity& e) {
    return e.position().x * e.position().y + e.position().x;
  };

  auto spawned = _spawner.spawn(type, team, _battle_table.get_spawn_attempts());

  if (spawned.type() == type &&
      (type == EntityType::Spy || type == EntityType::Soldier)) {
    if (_move_tree == nullptr) {
      _move_tree = Tree::MakeNode(key_fn(spawned), spawned);
    } else {
      auto new_node = Tree::MakeNode(key_fn(spawned), spawned);
      Tree::InsertNode(_move_tree, new_node);
    }
  }

  return spawned.type() == type;
}
