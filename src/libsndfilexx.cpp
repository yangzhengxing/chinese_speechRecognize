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
#include <string.h>
#include "libsndfilexx.h"

using namespace std;

//  class SndFileImpl
class SndFileImpl {
private:
    SNDFILE* m_pSndFile;
    SF_INFO m_SFInfo;
private:
    void init() {
        m_pSndFile = nullptr;
        memset(&m_SFInfo, 0, sizeof(SF_INFO));
    }
    void throwSndFileError() {
        throw runtime_error(sf_strerror(NULL));
    }
public:
    explicit SndFileImpl() {
        init();
    }
    ~SndFileImpl() {
        close();
    }
    void open(const char* filename, ios_base::openmode mode) {
        close();
        int sndmode = 0;
        if ((mode & ios_base::in) && (mode & ios_base::out)) {
            sndmode = SFM_RDWR;
        }
        else if (mode & ios_base::in) {
            sndmode = SFM_READ;
        }
        else if (mode & ios_base::out) {
            sndmode = SFM_WRITE; 
        }
        else {
            throw runtime_error("Unknown open mode");
        }
        m_pSndFile = sf_open(filename, sndmode, &m_SFInfo);
        if (!m_pSndFile) {
            throwSndFileError();
        }
    }
    void close() {
        if (m_pSndFile) {
            //  ignore return value
            sf_close(m_pSndFile) ;
        }
        init();
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

void SndFile::open(const char* filename, ios_base::openmode mode) {
    this->m_pSndFileImpl->open(filename, mode);
}

void SndFile::close() {
    this->m_pSndFileImpl->close();
}

