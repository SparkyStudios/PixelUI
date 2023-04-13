// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdio>

#include <SparkyStudios/UI/Pixel/Core/Utility.h>

// codecvt causes problems due to different standard libraries implementing at different times.
//  - libstdc++ looks like it didn't support codecvt until at least GCC 5.2.
//  - Apple has good support (but defined __GNUC__ ?!)
#if (defined(__GNUC__) && !(__GNUC__ >= 5 && __GNUC_MINOR__ > 1)) && !defined(__APPLE__)
#define AVOID_CPP11_CODECVT
#endif

// For Unicode support.
#include <locale> // Narrow/widen
// Note: <codecvt> is C++11. It exists in libc++ (LLVM) but is patchy in libstdc++ pre-GCC 5
#ifndef AVOID_CPP11_CODECVT
#include <codecvt> // Narrow/widen - C++11
#else
#include <iconv.h>
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h> // alloca
#else
#include <alloca.h> // alloca
#endif
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace SparkyStudios::UI::Pixel::Utility
{
    //! Cross platform implementation of vsnprintf that returns number of
    //! characters which would have been written to the final string if
    //! enough space had been available.
    int vsnprintf(char* _str, size_t _count, const char* _format, va_list _argList)
    {
#if defined(_MSC_VER)
        const int len = ::vsnprintf_s(_str, _count, _count, _format, _argList);
        return -1 == len ? ::_vscprintf(_format, _argList) : len;
#else
        return ::vsnprintf(_str, _count, _format, _argList);
#endif
    }

    //! Cross platform implementation of vsnwprintf that returns number of
    //! characters which would have been written to the final string if
    //! enough space had been available.
    int vsnwprintf(wchar_t* _str, size_t _count, const wchar_t* _format, va_list _argList)
    {
#if defined(_MSC_VER)
        const int len = ::_vsnwprintf_s(_str, _count, _count, _format, _argList);
        return -1 == len ? ::_vscwprintf(_format, _argList) : len;
#elif defined(__MINGW32__)
        return ::vsnwprintf(_str, _count, _format, _argList);
#else
        return ::vswprintf(_str, _count, _format, _argList);
#endif
    }

    int snprintf(char* _str, size_t _count, const char* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        const int len = vsnprintf(_str, _count, _format, argList);
        va_end(argList);
        return len;
    }

    int swnprintf(wchar_t* _out, size_t _count, const wchar_t* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        const int len = vsnwprintf(_out, _count, _format, argList);
        va_end(argList);
        return len;
    }

    void PrintfVargs(std::string& _out, const char* _format, va_list _argList)
    {
        char temp[2048];

        char* out = temp;
        int len = Utility::vsnprintf(out, sizeof(temp), _format, _argList);
        if (int(sizeof(temp)) < len)
        {
            out = (char*)alloca(len + 1);
            len = Utility::vsnprintf(out, len, _format, _argList);
        }
        out[len] = '\0';
        _out.append(out);
    }

    void Printf(std::string& _out, const char* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        PrintfVargs(_out, _format, argList);
        va_end(argList);
    }

    PiString Format(const char* _format, ...)
    {
        va_list argList;
        va_start(argList, _format);
        PiString out;
        PrintfVargs(out, _format, argList);
        va_end(argList);
        return out;
    }

    std::wstring Widen(const PiString& nstr)
    {
#ifndef AVOID_CPP11_CODECVT
        // UTF-8 to UTF-16 (C++11)
        // See: http://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
        // See: http://www.cplusplus.com/reference/codecvt/codecvt_utf8_utf16/

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
        const std::wstring wstr(conversion.from_bytes(nstr.c_str()));
        return wstr;
#else
        static iconv_t cd = iconv_open("WCHAR_T", "UTF-8"); // TODO - iconv_close() on exit
        // size_t iconv(iconv_t cd,
        //     const char* * inbuf, size_t * inbytesleft, char* * outbuf, size_t * outbytesleft);
        char* inbuf = (char*)nstr.data();
        size_t inbytesleft = std::distance(nstr.begin(), nstr.end()) * sizeof(char);
        size_t outbytesleft = inbytesleft * 2 + inbytesleft / 8; // assumed 2*narrow length + pad
        char *outbuf = (char*)alloca(outbytesleft), *retbuf = outbuf;
        size_t numCvt = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
        *outbuf = L'\0'; // terminate
        return std::wstring((wchar_t*)retbuf);
#endif
    }

    PiString Narrow(const std::wstring& wstr)
    {
#ifndef AVOID_CPP11_CODECVT
        // wide to UTF-8 (C++11)
        // See: http://en.cppreference.com/w/cpp/locale/wstring_convert/to_bytes

        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
        PiString u8str = conv1.to_bytes(wstr);

        return u8str;
#else
        static iconv_t cd = iconv_open("UTF-8", "WCHAR_T"); // TODO - iconv_close() on exit
        // size_t iconv(iconv_t cd,
        //     const char* * inbuf, size_t * inbytesleft, char* * outbuf, size_t * outbytesleft);
        char* inbuf = (char*)wstr.data();
        size_t inbytesleft = std::distance(wstr.begin(), wstr.end()) * sizeof(wchar_t);
        size_t outbytesleft = inbytesleft;
        char *outbuf = (char*)alloca(outbytesleft), *retbuf = outbuf;
        size_t numCvt = iconv(cd, &inbuf, &inbytesleft, &outbuf, &outbytesleft);
        *outbuf = 0; // terminate
        return String(retbuf);
#endif
    }

    void Replace(PiString& str, const PiString& strFind, const PiString& strReplace)
    {
        size_t pos = 0;

        while ((pos = str.find(strFind, pos)) != PiString::npos)
        {
            str.replace(pos, strFind.length(), strReplace);
            pos += strReplace.length();
        }
    }

    void Strings::Split(const PiString& str, const PiString& seperator, Strings::List& outbits, bool bLeave)
    {
        size_t iOffset = 0;
        size_t iLength = str.length();
        size_t iSepLen = seperator.length();
        size_t i = str.find(seperator, 0);

        while (i != std::string::npos)
        {
            outbits.push_back(str.substr(iOffset, i - iOffset));
            iOffset = i + iSepLen;
            i = str.find(seperator, iOffset);

            if (bLeave)
                iOffset -= iSepLen;
        }

        outbits.push_back(str.substr(iOffset, iLength - iOffset));
    }

    int Strings::To::Int(const PiString& str)
    {
        if (str.empty())
            return 0;

        return std::atoi(str.c_str());
    }

    float Strings::To::Float(const PiString& str)
    {
        if (str.empty())
            return 0.0f;

        return std::atof(str.c_str());
    }

    bool Strings::To::Bool(const PiString& str)
    {
        if (str.empty())
            return false;

        const char first = tolower(str[0]);

        switch (first)
        {
        case 't':
        case 'y':
            return true;

        case 'f':
        case 'n':
            return false;

        default:
            break;
        }

        return To::Int(str) != 0;
    }

    bool Strings::To::Floats(const PiString& str, float* f, size_t iCount)
    {
        Strings::List lst;
        Strings::Split(str, " ", lst);

        if (lst.size() != iCount)
            return false;

        for (size_t i = 0; i < iCount; i++)
        {
            f[i] = Strings::To::Float(lst[i]);
        }

        return true;
    }

    bool Strings::Wildcard(const PiString& strWildcard, const PiString& strHaystack)
    {
        const PiString& W = strWildcard;
        const PiString& H = strHaystack;

        if (strWildcard == "*")
            return true;

        size_t iPos = W.find('*', 0);

        if (iPos == PiString::npos)
            return strWildcard == strHaystack;

        // First half matches
        if (iPos > 0 && W.substr(0, iPos) != H.substr(0, iPos))
            return false;

        // Second half matches
        if (iPos != W.length() - 1)
        {
            PiString strEnd = W.substr(iPos + 1, W.length());

            if (strEnd != H.substr(H.length() - strEnd.length(), H.length()))
                return false;
        }

        return true;
    }

    void Strings::ToUpper(PiString& str)
    {
        transform(str.begin(), str.end(), str.begin(), toupper);
    }

    void Strings::Strip(PiString& str, const PiString& chars)
    {
        PiString Source = str;
        str = "";

        for (const char& i : Source)
        {
            if (chars.find(i) != PiString::npos)
                continue;

            str += i;
        }
    }
} // namespace SparkyStudios::UI::Pixel::Utility
