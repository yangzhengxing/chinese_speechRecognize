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

#include <iostream>
#include <string>
#include <getopt.h>
#include "config.h"
#include "ekho.h"
#include "StringHelper.h"

using namespace std;
using namespace ekho;

//  global options
bool isDebugging = false;
string language = "Mandarin";
string text_filename = "-";
string save_filename;
char *save_type = NULL;
int pitch_delta = 0;
int volume_delta = 0;
int rate_delta = 0;
int tempo_delta = 0;
bool is_listing_symbols = false;
bool is_listing_word = false;

void show_help() {
    cerr << "Ekho text-to-speech engine." << endl;
    cerr << "Version: " << PACKAGE_VERSION << endl;
    cerr << endl;
    cerr << "Syntax: ekho [option] [file]" << endl;
    cerr << "file" << endl;
    cerr << "        Input file, if not given or given '-', read from standard input" << endl;
    cerr << "-v, --voice=VOICE" << endl;
    cerr << "        Specified language or voice. ('Cantonese', 'Mandarin', 'Hakka', 'Tibetan', 'Ngangien' and 'Hangul' are available now. Mandarin is the default language.)" << endl;
    cerr << "-l, --symbol" << endl;
    cerr << "        List phonetic symbol of text. Characters' symbols are splited by space." << endl;
    cerr << "-o, --output=FILE" << endl;
    cerr << "        Output to file." << endl;
    cerr << "-t, --type=OUTPUT_TYPE" << endl;
    cerr << "        Output type: wav(default), ogg or mp3" << endl;
    cerr << "-p, --pitch=PITCH_DELTA" << endl;
    cerr << "        Set delta pitch. Value range from -100 to 100 (percent)" << endl;
    cerr << "-a, --volume=VOLUME_DELTA" << endl;
    cerr << "        Set delta volume. Value range from -100 to 100 (percent)" << endl;
    cerr << "-r, --rate=RATE" << endl;
    cerr << "        Set delta rate (this scales pitch and speed at the same time). Value range from -50 to 100 (percent)" << endl;
    cerr << "-s, --speed=SPEED" << endl;
    cerr << "        Set delta speed. Value range from -50 to 300 (percent)" << endl;
    cerr << "--version" << endl;
    cerr << "        Show version number." << endl;
    cerr << "-h, --help" << endl;
    cerr << "        Display this help message." << endl;
    cerr << endl;
    cerr << "Please report bugs to Cameron Wong (hgneng at gmail.com)" << endl;
    exit(1);
}

void parse_options(int argc, char* argv[]) {
    struct option opts[] = {
        {"help", 0, NULL, 'h'},
        {"voice", 1, NULL, 'v'},
        {"output", 1, NULL, 'o'},
        {"type", 1, NULL, 't'},
        {"pitch", 1, NULL, 'p'},
        {"volume", 1, NULL, 'a'},
        {"rate", 1, NULL, 'r'},
        {"speed", 1, NULL, 's'},
        {"symbol", 0, NULL, 'l'},
        {"debug", 0, NULL, 'd'},
        {"version", 0, NULL, 'n'},
        {NULL, 0, NULL, 0}
    };
    extern char *optarg;
    extern int optind, optopt;
    int opt;
    int optidx;

    while ((opt = getopt_long(argc, argv, ":hgv:n:o:t:p:r:a:s:lwd", opts, &optidx)) != -1 ) {
        switch (opt) {
        case 'd':
            isDebugging = true;
            break;
        case 'l':
            is_listing_symbols = true;
            break;
        case 'w':
            is_listing_word = true;
            break;
        case 'h':
            show_help();
            break;
        case 'v':
            language = optarg;
            break;
        case 'o':
            save_filename = optarg;
            break;
        case 't':
            save_type = optarg;
            break;
        case 'p':
            pitch_delta = atof(optarg);
            break;
        case 'r':
            rate_delta = atoi(optarg);
            break;
        case 's':
            tempo_delta = atoi(optarg);
            break;
        case 'a':
            volume_delta = atoi(optarg);
            break;
        case 'n':
            cerr << PACKAGE_VERSION << endl;
            exit(1);
        case '?' :
            cerr << "Invalid option: -" << static_cast<char>(optopt) << endl;
            exit(1);
        }
    }
    if (optind < argc) {
        text_filename = argv[optind];
    }
}

int main(int argc, char* argv[]) {
    //  parse command line options
    parse_options(argc, argv);
    //  open file
    istream* pIS = &cin;
    ifstream fin;
    if (!text_filename.empty() && text_filename != "-") {
        fin.open(text_filename.c_str());
        if (!fin.is_open()) {
            cerr << argv[0] << ": open file failed: " << text_filename << endl;
            exit(1);
        }
        pIS = &fin;
    }
    //  init ekho
    Ekho ekho(language);
    ekho.setPitch(pitch_delta);
    ekho.setSpeed(tempo_delta);
    ekho.setVolume(volume_delta);
    ekho.setRate(rate_delta);
    //  process
    string line;
    vector<string> vWord;
    while (getline(*pIS, line)) {
        //  split sentence to words by space
        StringHelper::SplitSpace<string>(line, back_inserter(vWord));
        if (vWord.empty()) {
            continue;
        }
        ekho.blockSpeak(line);
    }
    //  close
    if (fin.is_open()) {
        fin.close();
    }
    pIS = NULL;
    return 0;
}

//int main(int argc, char *argv[]) {
//  /* set locale to zh_CN.UTF-8 */
//  //  setlocale(LC_ALL, "zh_CN.UTF-8");
//
//
//  if (is_listing_symbols) {
//    Language lang = ENGLISH;
//    if (language.compare("Cantonese") == 0) {
//      lang = CANTONESE;
//    } else if (language.compare("Mandarin") == 0) {
//      lang = MANDARIN;
//    } else if (language.compare("Korean") == 0) {
//      lang = KOREAN;
//    } else if (language.compare("Hakka") == 0) {
//      lang = HAKKA;
//    } else if (language.compare("Tibetan") == 0) {
//      lang = TIBETAN;
//    } else if (language.compare("Ngangien") == 0) {
//      lang = NGANGIEN;
//    }
//    Dict dict(lang);
//
//    list<Word> wordlist = dict.lookupWord(text);
//    for (list<Word>::iterator word = wordlist.begin();
//        word != wordlist.end(); word++) {
//      list<PhoneticSymbol*> phonList = word->symbols;
//
//      list<PhoneticSymbol*>::iterator phonItor = phonList.begin();
//      for (; phonItor != phonList.end(); ++phonItor) {
//        cout << (*phonItor)->symbol << " ";
//      }
//    }
//
//    cout << endl;
//  } else if (is_listing_word) {
//    Ekho::debug(isDebugging);
//    Dict dict(MANDARIN);
//    list<Word> word_list = dict.lookupWord(text);
//    list<Word>::iterator word = word_list.begin();
//    for (; word != word_list.end(); word++) {
//      cout << word->text;
//      cout << "/";
//    }
//  } else {
//    Ekho::debug(isDebugging);
//    ekho_g = new Ekho(language);
//    ekho_g->setPitch(pitch_delta);
//    ekho_g->setSpeed(tempo_delta);
//    ekho_g->setVolume(volume_delta);
//    ekho_g->setRate(rate_delta);
//
//    if (save_filename) {
//      if (save_type && strcmp(save_type, "ogg") == 0) {
//        ekho_g->saveOgg(text, save_filename);
//      } else if (save_type && strcmp(save_type, "mp3") == 0) {
//#ifdef HAVE_MP3LAME
//        ekho_g->saveMp3(text, save_filename);
//#else
//        printf("mp3 type is not supported!\n");
//#endif
//      } else {
//        ekho_g->saveWav(text, save_filename);
//      }
//    } else if (strlen(text) > 0) {
//      ekho_g->blockSpeak(text);
//    } else {
//        show_help();
//    }
//
//    delete(ekho_g);
//    ekho_g = 0;
//  }
//
//}

