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
#include "config.h"
#include "ekho.h"
#include "StringHelper.h"

using namespace std;
using namespace ekho;

int main(int argc, char* argv[]) {
    string line;
    vector<string> vWord;
    while (getline(cin, line)) {
        //  split sentence to words by space
        StringHelper::SplitSpace<string>(line, back_inserter(vWord));
        if (vWord.empty()) {
            continue;
        }
    }
    return 0;
}

//static Ekho *ekho_g = NULL;
//static bool isDebugging = false;


//static void show_help(void) {
//    printf("\
//Ekho text-to-speech engine.\n\
//Version: %s\n\n\
//Syntax: ekho [option] [text]\n\
//-v, --voice=VOICE\n\
//        Specified language or voice. ('Cantonese', 'Mandarin', 'Hakka', 'Tibetan', 'Ngangien' and 'Hangul' are available now. Mandarin is the default language.)\n\
//-l, --symbol\n\
//        List phonetic symbol of text. Characters' symbols are splited by space.\n\
//-f, --file=FILE\n\
//        Speak text file. ('-' for stdin)\n\
//-o, --output=FILE\n\
//        Output to file.\n\
//-t, --type=OUTPUT_TYPE\n\
//        Output type: wav(default), ogg or mp3\n\
//-p, --pitch=PITCH_DELTA\n\
//        Set delta pitch. Value range from -100 to 100 (percent)\n\
//-a, --volume=VOLUME_DELTA\n\
//        Set delta volume. Value range from -100 to 100 (percent)\n\
//-r, --rate=RATE\n\
//        Set delta rate (this scales pitch and speed at the same time). Value range from -50 to 100 (percent)\n\
//-s, --speed=SPEED\n\
//        Set delta speed. Value range from -50 to 300 (percent)\n\
//--server\n\
//        Start Ekho TTS server.\n\
//--request=TEXT\n\
//        Send request to Ekho TTS server.\n\
//--port\n\
//        Set server port. Default is 2046.\n\
//--version\n\
//        Show version number.\n\
//-h, --help\n\
//        Display this help message.\n\n\
//Please report bugs to Cameron Wong (hgneng at gmail.com)\n",
//PACKAGE_VERSION);
//}
//
//int main(int argc, char *argv[]) {
//  struct option opts[] = {
//    {"help", 0, NULL, 'h'},
//    {"voice", 1, NULL, 'v'},
//    {"file", 1, NULL, 'f'},
//    {"output", 1, NULL, 'o'},
//    {"type", 1, NULL, 't'},
//    {"pitch", 1, NULL, 'p'},
//    {"volume", 1, NULL, 'a'},
//    {"rate", 1, NULL, 'r'},
//    {"speed", 1, NULL, 's'},
//    {"port", 1, NULL, '1'},
//    {"server", 0, NULL, 'e'},
//    {"request", 1, NULL, 'q'},
//    {"symbol", 0, NULL, 'l'},
//    {"debug", 0, NULL, 'd'},
//    {"version", 0, NULL, 'n'},
//    {NULL, 0, NULL, 0}
//  };
//  /* set locale to zh_CN.UTF-8 */
//  //  setlocale(LC_ALL, "zh_CN.UTF-8");
//
//  /* get arguments */
//  int opt;
//  int optidx;
//  string language = "Mandarin";
//#define NORMAL_MODE 0
//#define SERVER_MODE 2
//#define REQUEST_MODE 3
//  int mode = NORMAL_MODE;
//  int text_buffer_size = 256;
//  char *text = (char*)malloc(text_buffer_size);
//  text[0] = 0;
//  const char *text_filename = NULL;
//  const char *save_filename = NULL;
//  char *save_type = NULL;
//  int pitch_delta = 0;
//  int volume_delta = 0;
//  int rate_delta = 0;
//  int tempo_delta = 0;
//  extern char *optarg;
//  extern int optind, optopt;
//  bool is_listing_symbols = false;
//  bool is_listing_word = false;
//  int server_port = 2046;
//
//  while ((opt = getopt_long(argc, argv, ":hgv:n:f:o:t:p:r:a:s:eq:lwd1:", opts, &optidx)) != -1 ) {
//    switch (opt) {
//      case 'd':
//        isDebugging = true;
//        break;
//      case 'l':
//        is_listing_symbols = true;
//        break;
//      case 'w':
//        is_listing_word = true;
//        break;
//      case 'h':
//        show_help();
//        return 0;
//      case 'v':
//        language = optarg;
//        break;
//      case 'f':
//        text_filename = optarg;
//        if (!text_filename) {
//          cerr << "no filename" << endl;
//          show_help();
//          exit(-1);
//        }
//        break;
//      case 'o':
//        save_filename = optarg;
//        if (!save_filename) {
//          show_help();
//          exit(-1);
//        }
//        break;
//      case 't':
//        save_type = optarg;
//        if (!save_type) {
//          show_help();
//          exit(-1);
//        }
//        break;
//      case 'p':
//        pitch_delta = atof(optarg);
//        break;
//      case 'r':
//        rate_delta = atoi(optarg);
//        break;
//      case 's':
//        tempo_delta = atoi(optarg);
//        break;
//      case 'a':
//        volume_delta = atoi(optarg);
//        break;
//      case 'e':
//        mode = SERVER_MODE;
//        break;
//      case '1':
//        server_port = atoi(optarg);
//        break;
//      case 'q':
//        mode = REQUEST_MODE;
//        if (text_buffer_size < strlen(optarg) + 1) {
//          do {
//            text_buffer_size *= 2;
//          } while (text_buffer_size < strlen(optarg) + 1);
//          text = (char*)realloc(text, text_buffer_size);
//        }
//         
//        strcpy(text, optarg);
//        break;
//      case 'n':
//        printf("%s\n", PACKAGE_VERSION);
//        return 0;
//      case '?' :
//        fprintf(stderr, "Invalid option: -%c\n", optopt);
//        return -1;
//    }
//  }
//
//  if (text_filename) {
//    if (strcmp(text_filename, "-") == 0) {
//      read_stdin(&text);
//    } else {
//      read_textfile(text_filename, &text);
//    }
//  } else {
//    bool is_first_text = true;
//    for ( ; optind < argc; optind++) {
//      if (access(argv[optind], R_OK)) {
//        if (strlen(text) + strlen(argv[optind]) + 2 > text_buffer_size) {
//          do {
//            text_buffer_size *= 2;
//          } while (strlen(text) + strlen(argv[optind]) + 2 > text_buffer_size);
//          text = (char*)realloc(text, text_buffer_size);
//        }
//        if (is_first_text)
//          is_first_text = false;
//        else
//          strcat(text, " ");
//        strcat(text, argv[optind]);
//      }
//    }
//  }
//
//  if (mode != REQUEST_MODE && is_listing_symbols) {
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
//  } else if (mode != REQUEST_MODE && is_listing_word) {
//    Ekho::debug(isDebugging);
//    Dict dict(MANDARIN);
//    list<Word> word_list = dict.lookupWord(text);
//    list<Word>::iterator word = word_list.begin();
//    for (; word != word_list.end(); word++) {
//      cout << word->text;
//      cout << "/";
//    }
//  } else if (mode == SERVER_MODE) {
//    Ekho::debug(isDebugging);
//    ekho_g = new Ekho(language);
//    ekho_g->startServer(server_port);
//  } else if (mode == REQUEST_MODE) {
//    Ekho::debug(isDebugging);
//    ekho_g = new Ekho();
//    ekho_g->setSpeed(tempo_delta);
//    ekho_g->setPitch(pitch_delta);
//    ekho_g->setVolume(volume_delta);
//    
//    Command cmd;
//    if (is_listing_symbols) {
//      cmd = GETPHONSYMBOLS;
//    } else if (save_type && strcmp(save_type, "ogg") == 0) {
//      cmd = SAVEOGG;
//    } else {
//      cmd = SAVEMP3;
//    }
//
//    if (!save_filename) {
//      if (cmd == GETPHONSYMBOLS) {
//        save_filename = "output.sym";
//      } else if (cmd == SAVEOGG) {
//        save_filename = "output.ogg";
//      } else {
//        save_filename = "output.mp3";
//      }
//    }
//
//    ekho_g->request("127.0.0.1", server_port, cmd, text, save_filename);
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
//  if (text)
//    free(text);
//
//  return 0;
//}

