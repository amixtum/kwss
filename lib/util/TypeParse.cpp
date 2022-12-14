#include <fstream>
#include <iostream>
#include <vector>

#include "../../include/util/TypeParse.h"

EntityType
parse_type(std::string s)
{
  if (s.compare("Soldier") == 0) {
    return EntityType::Soldier;
  } else if (s.compare("Spy") == 0) {
    return EntityType::Spy;
  } else if (s.compare("Wall") == 0) {
    return EntityType::Wall;
  } else if (s.compare("Leader") == 0) {
    return EntityType::Leader;
  }

  return EntityType::Size;
}

AbilityState
parse_ability_state(std::string s)
{
  if (s.compare("On") == 0) {
    return AbilityState::On;
  } else if (s.compare("Off") == 0) {
    return AbilityState::Off;
  }

  return AbilityState::Size;
}

BattleTable
read_table_from_file(std::string fname)
{
  BattleTable table;

  std::string line;

  std::ifstream file;
  file.open(fname, std::ios::in);

  if (file.is_open()) {
    auto first_section = false;

    while (getline(file, line)) {
      if (line.compare("MaxValues") == 0) {
        first_section = true;
        continue;
      } else if (line.compare("DmgValues") == 0) {
        first_section = false;
        continue;
      } else if (line.at(0) == '#' || line.at(0) == '\n') {
        continue;
      }

      std::vector<std::string> words;
      int index = 0;
      int last_index = 0;
      while (index != -1) {
        index = line.find(' ', last_index);
        words.push_back(line.substr(last_index, index - last_index));
        last_index = index + 1;
      }

      if (first_section && words.size() == 5) {
        auto type = parse_type(words[0]);
        auto hp = atoi(words[1].c_str());
        auto stamina = atoi(words[2].c_str());
        auto sight_radius = atoi(words[3].c_str());
        auto distance_threshold = atof(words[4].c_str());

        table.set_max_hp(type, hp);
        table.set_max_stamina(type, stamina);
        table.set_sight_radius(type, sight_radius);
        table.set_distance_threshold(type, distance_threshold);

      } else if (!first_section && words.size() == 5) {
        auto att_type = parse_type(words[0]);
        auto def_type = parse_type(words[1]);
        auto att_state = parse_ability_state(words[2]);
        auto def_state = parse_ability_state(words[3]);
        auto dmg = atoi(words[4].c_str());

        table.set_dmg(att_type, def_type, att_state, def_state, dmg);
      } else if (!first_section && words.size() == 3 &&
                 words[0].compare("Dimensions") == 0) {
        auto width = atoi(words[1].c_str());
        auto height = atoi(words[2].c_str());

        table.set_dimensions(Point2i(width, height));
      } else if (!first_section && words.size() == 4 &&
                 words[0].compare("Respawn") == 0) {
        auto interval = atoi(words[1].c_str());
        auto attempts = atoi(words[2].c_str());
        auto wave_size = atoi(words[3].c_str());

        table.set_respawn_interval(interval);
        table.set_spawn_attempts(attempts);
        table.set_wave_size(wave_size);
      } else if (!first_section && words.size() == 2 &&
                 words[0].compare("WallFactor") == 0) {
        auto wall_factor = atof(words[1].c_str());

        table.set_wall_factor(wall_factor);
      }
    }

    file.close();
  }

  return table;
}
