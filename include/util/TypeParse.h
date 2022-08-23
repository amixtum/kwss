#ifndef TYPE_PARSE_H_
#define TYPE_PARSE_H_

#include <string>

#include "../model/AbilityState.h"
#include "../model/EntityType.h"

#include "../model/BattleTable.h"

EntityType
parse_type(std::string s);

AbilityState
parse_ability_state(std::string s);

BattleTable
read_table_from_file(std::string fname);

#endif
