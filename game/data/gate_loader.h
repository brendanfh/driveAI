/* Made by Brendan Hansen */

#ifndef __GATE_LOADER_N__
#define __GATE_LOADER_H__

#include <string>
#include <memory>

#include "../world/gates.h"

auto LoadGates(std::string filename) -> std::shared_ptr<Gates>;

#endif