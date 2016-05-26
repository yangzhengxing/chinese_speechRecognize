#ifndef EKHO_UTILS_H_2016_05_04
#define EKHO_UTILS_H_2016_05_04

#include <vector>
#include <exception>
#include "StringHelper.h"
#include "sndfilexx.h"

namespace ekho {
namespace ekho_utils {
    std::vector<char> ReadSndFile(const char* filename, SndFileInfo& sfinfo);

}   //  end of namespace ekho_utils
}   //  end of namespace ekho

#endif

