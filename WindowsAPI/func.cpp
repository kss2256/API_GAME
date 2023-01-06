#include "Common.h"
#include "func.h"



void SaveWString(const std::wstring& _str, FILE* _pFile)
{
    size_t iLen = _str.length();
    fwrite(&iLen, sizeof(size_t), 1, _pFile);
    fwrite(_str.c_str(), sizeof(wchar_t), iLen, _pFile);
}

void LoadWString(std::wstring& _str, FILE* _pFile)
{
    size_t iLen = 0;
    fread(&iLen, sizeof(size_t), 1, _pFile);

    wchar_t szBuff[256] = {};
    fread(szBuff, sizeof(wchar_t), iLen, _pFile);
    _str = szBuff;
}