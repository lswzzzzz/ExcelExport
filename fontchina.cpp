#include "fontchina.h"
#include "Windows.h"

FontChina::FontChina(void)
{
}


FontChina::~FontChina(void)
{
}

const char* FontChina::G2U(const char* gb2312)
{
    int len = MultiByteToWideChar(0, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(0, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(65001, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(65001, 0, wstr, -1, str, len, NULL, NULL);
    if(wstr) delete[] wstr;
    return str;

}

bool FontChina::isAscII(const char* src)
{
    int i = 0;
    unsigned long asicount = 0;
    unsigned long slen = strlen(src);
    unsigned char chr;
    while (true) {
        chr = *(src + i);
        if (chr == '\0' || i >= slen) break;
        i++;
        if ((chr & 0x80) == 0)asicount++;
        else return false;
    }
    if (asicount >= slen) {
        return true;
    }
    return false;
}
