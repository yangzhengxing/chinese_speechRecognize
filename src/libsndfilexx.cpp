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

#include <sndfile.h>
#include "libsndfilexx.h"

//  class SndFileImpl
class SndFileImpl {
private:
    SNDFILE* m_pSndFile;
public:
    explicit SndFileImpl() {
        m_pSndFile = nullptr;
    }
    ~SndFileImpl() {
        close();
    }
    void open(const char* filename, std::ios_base::openmode mode) {
    }
    void close() {
        if (m_pSndFile) {
            //  ignore return value
            sf_close(m_pSndFile) ;
        }
        m_pSndFile = nullptr;
    }
};

/////////////////////////////////////////////////////////////////////////////////
//  class SndFile

SndFile::SndFile() {
    m_pSndFileImpl = new SndFileImpl();
}

SndFile::~SndFile() {
    delete m_pSndFileImpl;
    m_pSndFileImpl = nullptr;
}

void SndFile::open(const char* filename, std::ios_base::openmode mode) {
    this->m_pSndFileImpl->open(filename, mode);
}

void SndFile::close() {
    this->m_pSndFileImpl->close();
}


int main(int argc, char* argv[]) {
    SndFile sf;
    sf.open("a.wav");
    return 0;
}

