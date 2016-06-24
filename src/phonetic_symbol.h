/***************************************************************************
 * Copyright (C) 2008-2013 by Cameron Wong                                 *
 * name in passport: HUANG GUANNENG                                        *
 * email: hgneng at gmail.com                                              *
 * website: http://www.eguidedog.net                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU General Public License             *
 * as published by the Free Software Foundation; either version 2          *
 * of the License, or any later version.                                   *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License       *
 * along with this program; if not, write to the Free Software             *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,              *
 * MA  02110-1301, USA.                                                    *
 **************************************************************************/
#ifndef PHONETIC_SYMBOL
#define PHONETIC_SYMBOL

//#define DEBUG_ANDROID
#ifdef DEBUG_ANDROID
#include "Log.h"
#define LOG_TAG "Ekho Engine"
#endif

#include <iostream>
#include <sndfile.h>
#include <string.h>
#include "ekho_utils.h"

using namespace std;

namespace ekho {
  typedef struct SymbolCode {
    const char *name;
    unsigned short code;
    void print(string prefix) {
        cerr << prefix << ": SymbolCode.code=" << code << ", SymbolCode.name=" << name << endl;
    }
  } SymbolCode;

  class PhoneticSymbol {
    public:
      PhoneticSymbol(void): offset(0), bytes(0) {};
      PhoneticSymbol(const char *sym):
        symbol(sym), offset(0), bytes(0) {};
      PhoneticSymbol(const char *sym, unsigned int off, unsigned short b):
        symbol(sym), offset(off), bytes(b) {};

      ~PhoneticSymbol(void) {
      }

      void setPcm(char *pcm, const int size) {
        mPcm.clear();
        mPcm.reserve(size);
        std::copy(pcm, pcm + size, back_inserter(mPcm));
      }

      inline const char* getPcm(int &size) {
        return getPcm("", "wav", size);
      }

      inline const char* getPcm(const char *wavDir, const char *postfix, int &size) {
        SndFileInfo sfinfo;
        return getPcm(wavDir, postfix, size, sfinfo);
      }

      const char* getPcm(FILE *file, int &size) {
#ifdef DEBUG_ANDROID
        LOGV("getPcm(%p, %d) offset=%d bytes=%d", file, size, offset, bytes);
#endif
        if (mPcm.empty() && fseek(file, offset, SEEK_SET) == 0) {
#ifdef ANDROID
          FILE *gsmfile = fopen("/data/data/net.eguidedog.ekho.cantonese/cache/tmpfile", "wb+");
          if (!gsmfile)
            gsmfile = fopen("/data/data/net.eguidedog.ekho.cantonese/tmpfile", "wb+");
#else
          FILE *gsmfile = tmpfile();
#endif

          if (!gsmfile) {
#ifdef DEBUG_ANDROID
            LOGV("Fail to open /data/data/net.eguidedog.ekho.cantonese/cache/tmpfile");
#endif
            cerr << "Fail to create temparary file." << endl;
            size = 0;
            return 0;
          }
          
          char buffer[128000];
          int b = bytes;
          while (b > 0) {
            if (b <= 128000) {
              fread(buffer, 1, b, file);
              fwrite(buffer, 1, b, gsmfile);
              b = 0;
            } else {
              b -= 128000;
              fread(buffer, 128000, b, file);
              fwrite(buffer, 128000, b, gsmfile);
            }
          }

#ifdef DEBUG_ANDROID
          LOGV("finish writting gsmfile");
#endif

          rewind(gsmfile);
          SndFileInfo sfinfo;
          SNDFILE *sndfile = sf_open_fd(fileno(gsmfile), SFM_READ, &sfinfo, 1);
          readSndfile(sndfile, sfinfo);
        }

        size = mPcm.size();
        return &mPcm[0];
      }

      const char* getPcm(const char *wavDir, const char *postfix, int &size, SndFileInfo& sfinfo) {
        if (mPcm.empty()) {
          sfinfo = SndFileInfo();
          string wav_file = wavDir;
          // char | 32 means get lower case
          if (this->symbol[0] == '\\')
          {
            char c = this->symbol[1] | 32;
            if (c >= 'a' && c <= 'z') {
              wav_file += "/../alphabet/";
              wav_file += c;
              wav_file += ".wav";
            } else {
              return 0;
            }
          } else {
            wav_file += "/";
            wav_file += this->symbol;
            wav_file += ".";
            wav_file += postfix;
          }

          try {
              mPcm = ekho_utils::ReadSndFile(wav_file.c_str(), sfinfo);
          }
          catch (std::runtime_error e) {
              std::cerr << e.what() << endl;
              return NULL;
          }

          //SNDFILE *sndfile = sf_open(wav_file.c_str(), SFM_READ, &sfinfo);
          //readSndfile(sndfile, sfinfo);
        }

        size = mPcm.size();
        return &mPcm[0];
      }

      void readSndfile(SNDFILE *sndfile, SndFileInfo& sfinfo) {
#ifdef DEBUG_ANDROID
                LOGV("readSndfile(%p, %p)", sndfile, &sfinfo);
#endif
          if (!sndfile) {
//            cerr << "Fail to open file " << wav_file << " at " << __LINE__ <<
  //              " of " << __FILE__ << endl;                
          } else {
//            sfinfo.channels = 1; // this->sfinfo.channels is corrupted
            if (sfinfo.channels > 2) {
              static bool show_channel_error = true;
              if (show_channel_error) {
                cerr << "Invalid channels: " << sfinfo.channels << endl;
                show_channel_error = false;
              }

              sfinfo.channels = 1;
            }
            int samples = 0;

            /* sfinfo.channels has not been taken into account .... */
            switch (sfinfo.format & SF_FORMAT_SUBMASK) {
              case SF_FORMAT_VORBIS:
              case SF_FORMAT_GSM610:
              case SF_FORMAT_PCM_16:
                mPcm.resize((int)sfinfo.frames * 2 * sfinfo.channels);
                samples = (int)sf_readf_short(sndfile, (short int*)&mPcm[0], sfinfo.frames);
#ifdef DEBUG_ANDROID
                LOGV("read samples: %d, %p", samples, mPcm);
#endif
                break;
              case SF_FORMAT_PCM_S8:
              case SF_FORMAT_PCM_U8:
              default:
                cerr << "Unknown soundfile format: " << (sfinfo.format & SF_FORMAT_SUBMASK) << endl;
            }

            if (samples != sfinfo.frames) {
              cerr << "Fail to read : " << samples <<
                " frames out of " << sfinfo.frames << " have been read." << endl;
            }

            sf_close(sndfile);
          }
      }
 
      static PhoneticSymbol* getUnknownPhoneticSymbol() {
        static PhoneticSymbol *ps = new PhoneticSymbol(" ");
        return ps;
      }

    public:
      string symbol;
      unsigned int offset; // bytes' offset
      unsigned short bytes;
    private:
      vector<char> mPcm;
  };
}

#endif

