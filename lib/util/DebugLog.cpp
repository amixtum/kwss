#include "../../include/util/DebugLog.h"

DebugLog::DebugLog()
{
  file.open(filename, std::ios::app);
}

DebugLog::~DebugLog()
{
  file.close();
}

void
DebugLog::write(const std::string& s)
{
  if (file.is_open()) {
    file << s;
  }
}
