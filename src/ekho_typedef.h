#ifndef EKHO_TYPEDEF_H
#define EKHO_TYPEDEF_H

namespace ekho {

enum Command {
    SPEAK,
    SAVEMP3,
    SAVEOGG,
    GETPHONSYMBOLS,
};

enum EkhoPuncType {
    EKHO_PUNC_NONE = 1,
    EKHO_PUNC_SOME,
    EKHO_PUNC_ALL,
};

enum Language {
    CANTONESE = 1,
    MANDARIN = 2,
    ENGLISH = 3,
    KOREAN = 4,
    HAKKA = 5,
    TIBETAN = 6,
    NGANGIEN = 7,
};

}

#endif

