#ifndef __SAVE_H__
#define __SAVE_H__

#include <fstream>
#include <string>
#include <vector>

#include "graph.h"
#include "state.h"

namespace co {

    void save_solutions(std::vector<State> &small_solutions, std::vector<DGraph> &small_gs, std::vector<State> &big_solutions, std::vector<DGraph> &big_gs, std::string outpath);

};

#endif
