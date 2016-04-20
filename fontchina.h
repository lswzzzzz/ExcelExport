#ifndef FONTCHINA_H
#define FONTCHINA_H
class FontChina
{
public:
    FontChina(void);
    ~FontChina(void);
    static const char* G2U(const char* gb2312);
    static bool isAscII(const char* src);
};
#endif // FONTCHINA_H
