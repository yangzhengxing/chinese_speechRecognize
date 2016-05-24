/***************************************************************************
 * Copyright (C) 2016 by Yun Huang                                         *
 * email: huangyunict at gmail.com                                         *
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
#ifndef SND_FILE_XX_H_2016_05_04
#define SND_FILE_XX_H_2016_05_04

#include <iostream>
#include <fstream>
#include <sndfile.h>

class SndFileImpl;

struct SndFileInfo : public SF_INFO {
    explicit SndFileInfo() {
        frames     = 0;
        samplerate = 0;
        channels   = 0;
        format     = 0;
        sections   = 0;
        seekable   = 0;
    }
};

class SndFile {
private:
    SndFileImpl* m_pSndFileImpl;
public:
    explicit SndFile();
    ~SndFile();
    void open(const char* filename, int mode, SndFileInfo& info);
    void close();
    sf_count_t readf_short(short *buf, sf_count_t frames);
};

#endif

