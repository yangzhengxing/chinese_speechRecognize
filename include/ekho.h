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
#ifndef EKHO_H
#define EKHO_H

#include "config.h"

#ifdef HAVE_PULSEAUDIO
#include <pulse/simple.h>
#include <pulse/error.h>
#endif

#ifdef ANDROID
#include "flite.h"
#define ENABLE_ENGLISH
#endif

#ifdef DEBUG_ANDROID
#define LOG_TAG "Ekho Engine"
#include "Log.h"
#endif

using namespace std;

namespace ekho {

    class EkhoImpl;

    class Ekho {
    private:
        EkhoImpl* m_pImpl;
    public:
        /* Constructors.
        */
        explicit Ekho();
        explicit Ekho(const string& voice);

        /* Destructor.
        */
        ~Ekho();

        /* Set voice 
         * voice is the name of voice, which is a directory name under
         * ekho-data and should be begun with jyutping-, pinyin- or 
         * hangul-. Cantonese, Mandarin, Korean are alias to jyutping, 
         * pinyin, hangul.
         */
        int setVoice(string voice);
        /* Get current voice */
        string getVoice(void);

        /* no pause is allowed
         * it will return after all sound is played
         */
        int blockSpeak(string text);

        /* output text to WAVE file */
        int saveWav(string text, string filename);

        /* output to OGG file */
        int saveOgg(string text, string filename);

        /* output to MP3 file */
        int saveMp3(string text, string filename);

        /* Set tempo delta
         * Parameter:
         *    tempo_delta (-50 .. 100, in percent)
         *    If input out of range, tempo_delta will restore to 0
         */
        void setSpeed(int tempo_delta);
        int getSpeed(void);

        /* Set pitch delta
         * Parameter:
         *    pitch_delta (-100 .. 100, in percent)
         *    If input out of range, pitch_delta will restore to 0
         */
        void setPitch(int pitch_delta);
        int getPitch(void);

        /* Set volume delta
         * Parameter:
         *    volume_delta (-100 .. 100, in percent)
         *    If input out of range, volume_delta will restore to 0
         */
        void setVolume(int volume_delta);
        int getVolume(void);

        /* Set rate delta
         * Parameter:
         *    rate_delta (-50 .. 100, in percent)
         *    If input out of range, rate_delta will restore to 0
         * Return: 0
         */
        void setRate(int rate_delta);
        int getRate(void);
    };
}

#endif

