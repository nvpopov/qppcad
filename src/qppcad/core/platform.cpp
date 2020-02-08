#include <qppcad/core/platform.hpp>
#include <sys/resource.h>

using namespace qpp;
using namespace qpp::cad;

//https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

int parse_line(char* line){

  int i = strlen(line);
  const char* p = line;
  while (*p <'0' || *p > '9') p++;
  line[i-3] = '\0';
  i = atoi(p);
  return i;

}

size_t platform_helper_t::get_mem_usage_mb() {

  FILE* file = fopen("/proc/self/status", "r");
  int result = -1;
  char line[128];

  while (fgets(line, 128, file) != NULL){
            if (strncmp(line, "VmRSS:", 6) == 0){
          result = parse_line(line);
          break;
        }
    }
  fclose(file);
  return result / 1024;

}
