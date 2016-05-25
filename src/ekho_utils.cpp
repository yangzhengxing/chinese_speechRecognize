#include <vector>
#include <exception>
#include "ekho_utils.h"

namespace ekho {
namespace ekho_utils {
    std::vector<char> ReadSndFile(const char* filename) {
        std::vector<char> vBuf;
        vBuf.clear();
        SndFileInfo sfinfo;
        SndFile sndfile;
        sndfile.open(filename, SFM_READ, sfinfo);   //  exception could be thrown here
        //  channels are corrupted
        if (sfinfo.channels > 2) {
            throw std::runtime_error(("Invalid channels: " + StringHelper::Other2String(sfinfo.channels)).c_str());
        }
        sfinfo.channels = 1;
        int samples = 0;

        /* sfinfo.channels has not been taken into account .... */
        switch (sfinfo.format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_VORBIS:
        case SF_FORMAT_GSM610:
        case SF_FORMAT_PCM_16:
            vBuf.resize(sfinfo.frames * 2 * sfinfo.channels);
            samples = sndfile.readf_short(reinterpret_cast<short int*>(&vBuf[0]), sfinfo.frames);
            break;
        case SF_FORMAT_PCM_S8:
        case SF_FORMAT_PCM_U8:
        default:
            throw std::runtime_error(("Unknown sound file format: " + StringHelper::Other2String(sfinfo.format & SF_FORMAT_SUBMASK)).c_str());
        }
        //  final check
        if (samples != sfinfo.frames) {
            throw std::runtime_error(("Fail to read : " + StringHelper::Other2String(samples) +
                    " frames out of " + StringHelper::Other2String(sfinfo.frames) + " have been read.").c_str());
        }
        sndfile.close();
    }

}   //  end of namespace ekho_utils
}   //  end of namespace ekho

