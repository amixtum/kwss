#ifndef DEBUG_LOG_H_
#define DEBUG_LOG_H_

#include <fstream>
#include <string>

class DebugLog
{
public:
  DebugLog();

  ~DebugLog();

  void write(const std::string& s);
private:
  std::ofstream file;
  std::string filename = "/home/ganiparrott/src/projects/kwss/log/log.txt";
};

#endif // DebugLog.h included
