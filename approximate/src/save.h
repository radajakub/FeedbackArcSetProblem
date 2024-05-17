#ifndef __SAVE_H__
#define __SAVE_H__

#include <fstream>
#include <string>
#include <vector>

#include "graph.h"
#include "state.h"

namespace co {

    void save_solutions(std::vector<State> &solutions, std::vector<DGraph> &gs, std::string outpath);

};

#endif
