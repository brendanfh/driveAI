/* Made by Brendan Hansen */

#ifndef __TRACK_LOADER_H__
#define __TRACK_LOADER_H__

#include <string>
#include <memory>

#include "../world/track.h"

auto LoadTrack(std::string filename) -> std::shared_ptr<Track>;

#endif
