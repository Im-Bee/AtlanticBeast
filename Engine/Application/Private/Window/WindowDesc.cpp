#include "Core.h"
#include "Window/WindowDesc.hpp"

#include "AbLimits.h"

WindowDesc CreateWindowDesc(const wchar_t* pwszName, 
                            size_t uNameLen, 
                            int32_t width, 
                            int32_t height)
{ 
    WindowDesc wd = { 0 };

    wd.uNameLen = uNameLen;
    wd.pwszClassName = NULL;
    wd.Width = width;
    wd.Height = height;
    wd.LastEvent = NothingNew;

    if (wd.uNameLen >= AB_SMALL_STRING) 
        wd.uNameLen = AB_SMALL_STRING;
    
    wd.Name = (wchar_t*)malloc(wd.uNameLen * sizeof(wchar_t));
    for (size_t i = 0; i < wd.uNameLen; ++i) 
        wd.Name[i] = pwszName[i];

    wd.Name[wd.uNameLen - 1] = L'\0';

    return wd;
}

