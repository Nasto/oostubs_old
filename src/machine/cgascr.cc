/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                 Technische Informatik II                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 *                                    C G A _ S C R E E N                                        *
 *                                                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
#include "machine/cgascr.h"
#include "machine/io_port.h"

CGA_Screen::CGA_Screen(){
    this->attribute = DEFAULT_SCREEN_ATTRIB;
    clear();
}

CGA_Screen::~CGA_Screen(){

}

void CGA_Screen::setpos (unsigned short x, unsigned short y) {

    IO_Port indexreg(0x3d4);                //indirect access
    IO_Port datareg(0x3d5);

    int pos = (x+(y*80));                 //calculate the position

    indexreg.outb(15);
    datareg.outb(pos & 0xff);         //write first half (8bit)
    indexreg.outb(14);
    datareg.outb((pos >> 8) & 0xff);       //wirte second half (3bit)
}

void CGA_Screen::getpos (unsigned short& x, unsigned short& y) const{

    IO_Port indexreg(0x3d4);                //indirect access
    IO_Port datareg(0x3d5);

    indexreg.outb(15);
    char cursor1 = datareg.inb();           //fetch first half
    indexreg.outb(14);
    char cursor2 = datareg.inb();           //fetch second half

    int pos = (cursor1 & 0xff) | ((cursor2 & 0xff) << 8);    //put them together to one position

    x = (pos)%80;                         //write the positionparameters back to the given addresses
    y = (pos)/80;
}

void CGA_Screen::show (unsigned short x, unsigned short y, char c, unsigned char attrib) {
    char* CGA = (char*) 0xb8000;
    x = x%80;
    y= y%25;
    int pos = 2*((y*80)+x);
    CGA[pos] = c;
    CGA[pos+1] = attrib;
}

void CGA_Screen::print (const char* string, unsigned int n) {
    unsigned short curpos_x;
    unsigned short curpos_y;
    getpos(curpos_x, curpos_y);                                 //here I am right now
    for (unsigned int i = 0;i<n;i++){
        if(string[i]=='\n'){
            curpos_x=0;
            ++curpos_y;
        } else {
            show(curpos_x, curpos_y, string[i], this->attribute);   //each char for it's own
            ++curpos_x;
        }
        if(curpos_x>=79){                                       //don't go over the edge
            ++curpos_y;                                         //next line
            curpos_x=0;                                         //start from the beginning
        }
        if(curpos_y>24){
            scrollup();
            curpos_y=24;
        }
    }
    setpos(curpos_x,curpos_y);
}

void CGA_Screen::scrollup () {
    char* pos = (char*) 0xB80A0;            //start in the second line
    for(unsigned int i=0;i<24;i++){         //cycle through lines
        for(unsigned int j=0;j<80;j++){     //cycle thorugh columns
            show(j,i,*pos,*(pos+1));
            pos+=2;
        }
    }
    char att = this->attribute;
    for(int k=0;k<80;k++){
        show(k,24,' ',att);                 //fill the last line with spaces
    }
    setpos(0,24);
}

void CGA_Screen::clear () {
    char* CGA = (char*) 0xb8000;
    char att = this->attribute;
    for(unsigned int i=0;i<80*2*25;i++){
        CGA[i]=' ';                 //set space
        i++;
        CGA[i]=att;                 //set attribute
    }
    setpos(0,0);                    //start from the beginning
}

void CGA_Screen::setAttributes(int fgColor, int bgColor, bool blink){
    fgColor= fgColor%16;

    bgColor = bgColor%8;

    if(blink){
        blink = 1;
    }

    this->attribute = (char)(fgColor | (bgColor<<4) | (blink<<7));
}
