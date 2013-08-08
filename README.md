has_char
========

Small tool to determine whether a certain font contains a certain characters. Usage is:

    ./has_char font_name < characters_to_be_tested.txt

Example:

    $ echo 'I❤U' | ./has_char 'Bitstream Vera Sans'
    Missing char: ❤

Font name should be exact, otherwise fontconfig will default to some other font and the program will fail:

    $ ./has_char 'DejaVuSans' < file.txt
    Couldn't find font DejaVuSans, aborting

    $ ./has_char 'DejaVu Sans' < file.txt
    Missing char: 한
    Missing char: 글

To figure out the font's name, use any program that allows you to select fonts, like [gtk2fontsel](http://gtk2fontsel.sourceforge.net/).

The program will return a zero status if all characters were found, and a non-zero status if any characters were not found:

    $ echo '☺' | ./has_char 'Bitstream Vera Sans' && echo 'success' || echo 'failure'
    Missing char: ☺
    failure

    $ echo ':)' | ./has_char 'Bitstream Vera Sans' && echo 'success' || echo 'failure'
    success
