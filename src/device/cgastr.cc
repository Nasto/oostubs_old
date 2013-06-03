/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                 Technische Informatik II                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 *                                    C G A _ S T R E A M                                        *
 *                                                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "device/cgastr.h"

void CGA_Stream::flush () {
    setAttributes(O_Stream::fgCol, O_Stream::bgCol, O_Stream::blink);
    print(buffer, pos);
    pos=0;
}

void CGA_Stream::setAttributes(int fgColor, int bgColor, bool blink){
    CGA_Screen::setAttributes(fgColor, bgColor, blink);
}
