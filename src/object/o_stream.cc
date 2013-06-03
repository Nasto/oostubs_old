/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                 Technische Informatik II                                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                                               *
 *                                      O _ S T R E A M                                          *
 *                                                                                               *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "object/o_stream.h"

O_Stream::O_Stream() : Stringbuffer(){
    this->base = dec;     //initialise with decimal
    this->fgCol = 15;
    this->bgCol = 0;
    this->blink = false;
}


O_Stream::~O_Stream(){

}


O_Stream& O_Stream::operator << (char value) {
    put(value);             //chars are alright
    return *this;
}


O_Stream& O_Stream::operator << (unsigned char value) {
    put(value);             //not sure about the unsigned ones
    return *this;
}


O_Stream& O_Stream::operator << (char* value) {
    while(*value!= '\0'){   //this is where I want to stop
        put(*value);
        ++value;            //move on
    }
  return *this;
}

O_Stream& O_Stream::operator << (const char* value) {
    while(*value!= '\0'){   //this is where I want to stop
        put(*value);
        ++value;            //move on
    }
  return *this;
}


O_Stream& O_Stream::operator << (short value) {
    *this << ((long) value);    //cast and use what I've already got
  return *this;
}

O_Stream& O_Stream::operator << (unsigned short value) {
    *this << ((unsigned long) value);
  return *this;
}

O_Stream& O_Stream::operator << (int value) {
    *this << ((long) value);
  return *this;
}

O_Stream& O_Stream::operator << (unsigned int value) {
    *this << ((unsigned long) value);
  return *this;

}

O_Stream& O_Stream::operator << (long value) {
    if(value<0){
        value*=(-1);            //be positive
        *this << '-';           //mark the change
    }
    *this << ((unsigned long) value);
  return *this;
}

O_Stream& O_Stream::operator << (unsigned long value) {
    if(base==hex){
        unsigned long value_copy = value;
        int counter=(value > base) ? 1 : 0;
        while(value_copy > base){
            value_copy = value_copy/base;
            counter++;
        }
        for(int i=7-counter;i>0;i--){
            *this<<'0';
        }
    }
    return putToBuff(value);
}

O_Stream& O_Stream::putToBuff(unsigned long value){

    if (value < base) {         //One char is enough
        if (value < 10) {       //this is where I can use numbers
            put((char)('0' + value));
        } else {                //I'll need characters here
            put((char)('A' + value-10));
        }
    } else {                    //I'll need mutiple chars
        putToBuff(value/base);  //recursion
        putToBuff(value%base);
    }

  return *this;
}

O_Stream& O_Stream::operator << (void* value) {
    Base oldbase = base;
    base = hex;
    *this << ((unsigned long) value);
    base = oldbase;
    return *this;
}

O_Stream& O_Stream::operator << (FGColor fgColor){
    this->flush();
    this->fgCol = fgColor.color;
    return *this;
}

O_Stream& O_Stream::operator << (BGColor bgColor){
    this->flush();
    this->bgCol = bgColor.color;
    return *this;
}

O_Stream& O_Stream::operator << (Blink blink){
    this->flush();
    this->blink = blink.blink;
    return *this;
}


O_Stream& endl (O_Stream& os) {
    os << '\n';                 //just send an endline there
    os.flush();
    return os;
}

O_Stream& bin (O_Stream& os) {
    os.base = O_Stream::bin;
    os << "0b";
    return os;
}

O_Stream& oct (O_Stream& os) {
    os.base = O_Stream::oct;
    return os;
}

O_Stream& dec (O_Stream& os) {
    os.base = O_Stream::dec;
    return os;
}

O_Stream& hex (O_Stream& os) {
    os.base = O_Stream::hex;
    os << "0x";
    return os;
}

O_Stream& O_Stream::operator << (O_Stream& (*f) (O_Stream&)) {
  return f(*this);
}
