#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <fontconfig/fontconfig.h>

#define MAX_NAME_SIZE 1024

int main(int argc, char* argv[]) {
    FcConfig *config;
    FcPattern *pattern;
    FcResult result;
    FcCharSet *charset;
    char font_name[MAX_NAME_SIZE];
    FcChar8 *font_chosen;
    wchar_t ch;
    int status;

    // I'll never understand why you have to unset locale to use wide chars
    setlocale(LC_CTYPE, "");
    
    // treating command line
    if (argc != 2) {
        fwprintf(stderr, L"Usage: %s font_name < text\n", argv[0]);
        return 255;
    }
    strncpy(font_name, argv[1], MAX_NAME_SIZE);

    // initializing fontconfig
    if (FcInit() != FcTrue) {
        fwprintf(stderr, L"Couldn't initialize fontconfig\n");
        return 255;
    }
    config = FcInitLoadConfigAndFonts();

    // trying to select desired font
    pattern = FcNameParse((FcChar8*)font_name);
    FcDefaultSubstitute(pattern);
    FcConfigSubstitute(config, pattern, FcMatchFont);
    pattern = FcFontMatch(config, pattern, &result);

    // checking if we selected the right font (if the font is missing, fontconfig
    // will select "the closest match" (which actually means "one completely
    // different"))
    FcPatternGetString(pattern, "family", 0, &font_chosen);
    if (strlen(font_name) != strlen((char*)font_chosen) || \
        (strncmp(font_name, (char*)font_chosen, strlen(font_name)) != 0)) {
        fwprintf(stderr, L"Couldn't find font %s, aborting\n", font_name);
        return 255;
    }

    // checking all characters in stdin against the font's character set
    FcPatternGetCharSet(pattern, "charset", 0, &charset);
    status = 0;
    while (1) {
        // read one char from stdin
        ch = fgetwc(stdin);
        // if eof, exit
        if (ch == WEOF)
            break;
        // skip linebreak and bom (ideally you should skip all non-printing
        // chars your input might contain)
        else if (ch == L'\n' || ch == L'\r' || ch == 0xfeff)
            continue;
        // if font is missing char, print it and return non-zero status at the
        // end
        else if (!FcCharSetHasChar(charset, (FcChar32)ch) == FcTrue) {
            status = 1;
            wprintf(L"Missing char: %lc\n", ch);
        }
    }

    return status;
}
