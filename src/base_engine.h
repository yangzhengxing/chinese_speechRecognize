/***************************************************************************
 * Copyright (C) 2016-2016 by Yun Huang                                    *
 * email: huangyunict at gmail.com                                         *
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
#ifndef BASE_ENGINE_H_2016_05_01
#define BASE_ENGINE_H_2016_05_01

#include <vector>
#include <string>

namespace ekho {

using namespace std;

//  abstract base class for ekho engine
class BaseEngine {
    //  initialize engine
    virtual bool init() = 0;
    //  generate Pulse-code modulation (PCM) bytes vector from text
    virtual const vector<char>& getPcm(const string& text);
};

}   //  namespace ekho

#endif

