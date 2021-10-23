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

#pragma once

#ifndef PIXEL_UI_UTILITY_H
#define PIXEL_UI_UTILITY_H

#include <algorithm>
#include <sstream>
#include <stdarg.h>
#include <vector>

#include <SparkyStudios/UI/Pixel/Config/Types.h>

/**
 * @brief Get the number of items in a static array.
 *
 * @param A The array to get the number of items.
 */
#define PI_ARRAY_COUNT(A) (sizeof(A) / sizeof(A[0]))

namespace SparkyStudios::UI::Pixel
{
    namespace Utility
    {
        PI_EXPORT int vsnprintf(char* _str, size_t _count, const char* _format, va_list _argList);
        PI_EXPORT int vsnwprintf(wchar_t* _str, size_t _count, const wchar_t* _format, va_list _argList);
        PI_EXPORT int snprintf(char* _str, size_t _count, const char* _format, ...);
        PI_EXPORT int swnprintf(wchar_t* _out, size_t _count, const wchar_t* _format, ...);

        PI_EXPORT void PrintfVargs(PiString& _out, const char* _format, va_list _argList);
        PI_EXPORT void Printf(PiString& _out, const char* _format, ...);
        PI_EXPORT PiString Format(const char* fmt, ...);

        PI_EXPORT std::wstring Widen(const PiString& nstr);
        PI_EXPORT PiString Narrow(const std::wstring& wstr);

        template<typename TYPE>
        inline PiString ToString(const TYPE& object)
        {
            std::ostringstream os;
            os << object;
            return os.str();
        }

        PI_EXPORT void Replace(PiString& str, const PiString& strFind, const PiString& strReplace);

        template<class T, class U>
        static inline std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<U>& r) noexcept
        {
            if (auto p = dynamic_cast<typename std::shared_ptr<T>::element_type*>(r.get()))
                return std::shared_ptr<T>(r, p);
            else
                return std::shared_ptr<T>();
        }

        namespace Strings
        {
            typedef std::vector<PiString> List;

            PI_EXPORT void Split(const PiString& str, const PiString& seperator, Strings::List& outbits, bool bLeaveSeperators = false);
            PI_EXPORT bool Wildcard(const PiString& strWildcard, const PiString& strHaystack);

            PI_EXPORT void ToUpper(PiString& str);
            PI_EXPORT void Strip(PiString& str, const PiString& chars);

            template<typename T>
            T TrimLeft(const T& str, const T& strChars)
            {
                T outstr = str;
                outstr.erase(0, outstr.find_first_not_of(strChars));
                return outstr;
            }

            /**
             * @brief Widen a UTF8 code to a wide encoding.
             *
             * Source: https://stackoverflow.com/a/148766/3233
             */
            static inline wchar_t utf8_to_wchart(char*& in)
            {
#if defined(__clang__) && defined(__apple_build_version__) && __apple_build_version__ >= 8
                // thread_local not supported on Xcode versions below 8
                static unsigned int next = 0x10000;
#else
                thread_local static unsigned int next = 0x10000;
#endif

                if (next != 0x10000)
                {
                    wchar_t ret = static_cast<wchar_t>(next);
                    next = 0x10000;
                    return ret;
                }
                unsigned int codepoint;

                while (*in != 0)
                {
                    unsigned char ch = static_cast<unsigned char>(*in);
                    if (ch <= 0x7f)
                        codepoint = ch;
                    else if (ch <= 0xbf)
                        codepoint = (codepoint << 6) | (ch & 0x3f);
                    else if (ch <= 0xdf)
                        codepoint = ch & 0x1f;
                    else if (ch <= 0xef)
                        codepoint = ch & 0x0f;
                    else
                        codepoint = ch & 0x07;
                    ++in;
                    if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
                    {
                        if (sizeof(wchar_t) > 2)
                            return static_cast<wchar_t>(codepoint);
                        else if (codepoint > 0xffff)
                        {
                            next = static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff));
                            return static_cast<wchar_t>(0xd800 + (codepoint >> 10));
                        }
                        else if (codepoint < 0xd800 || codepoint >= 0xe000)
                            return static_cast<wchar_t>(codepoint);
                    }
                }
                return 0;
            }

            namespace To
            {
                PI_EXPORT bool Bool(const PiString& str);
                PI_EXPORT int Int(const PiString& str);
                PI_EXPORT float Float(const PiString& str);
                PI_EXPORT float Float(const PiString& str);
                PI_EXPORT bool Floats(const PiString& str, float* f, size_t iCount);
            } // namespace To
        } // namespace Strings
    } // namespace Utility

    template<typename T>
    inline T Clamp(T current, T vmin, T vmax)
    {
        return current < vmin ? vmin : (current > vmax ? vmax : current);
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_UTILITY_H
