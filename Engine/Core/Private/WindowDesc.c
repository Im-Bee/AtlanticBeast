#include "WindowDesc.h"

#include "AbLimits.h"

WindowDesc CreateWindowDesc(const wchar_t* pwszName, size_t uNameLen, int32_t width, int32_t height)
{ 
    WindowDesc wd;

    wd.uNameLen = uNameLen;
    wd.Width = width;
    wd.Height = height;
    wd.uLastMessage = 0;

    if (wd.uNameLen >= AB_SMALLS_STRING) {
        wd.uNameLen = AB_SMALLS_STRING;
    }
    
    wd.Name = (wchar_t*)malloc(wd.uNameLen * sizeof(wchar_t));
    for (size_t i = 0; i < wd.uNameLen - 1; ++i) {
        wd.Name[i] = pwszName[i];
    }
    wd.Name[wd.uNameLen - 1] = L'\0';

    return wd;
}

