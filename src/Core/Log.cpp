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

#include <cstdlib>

#include <SparkyStudios/UI/Pixel/Core/Log.h>
#include <SparkyStudios/UI/Pixel/Core/Utility.h>

namespace SparkyStudios::UI::Pixel::Log
{
    static LogListener gLogListener = nullptr;

    void Write(Level level, const char* format, ...)
    {
        if (gLogListener == nullptr)
            return;

        char buff[512];
        const char* logLevel[] = { "INFO", "WARNING", "ERROR", "FATAL" };
        int slen = Utility::snprintf(buff, sizeof(buff), "[%s] ", logLevel[(int)level]);

        va_list argList;
        va_start(argList, format);
        const int mlen = Utility::vsnprintf(buff + slen, sizeof(buff) - slen, format, argList);
        va_end(argList);

        if (slen + mlen < sizeof(buff) - 2)
        {
            buff[slen + mlen] = '\n';
            buff[slen + mlen + 1] = '\0';
        }

        gLogListener(level, buff);
    }

    void SetLogListener(LogListener listener)
    {
        gLogListener = listener;
    }
} // namespace SparkyStudios::UI::Pixel::Log