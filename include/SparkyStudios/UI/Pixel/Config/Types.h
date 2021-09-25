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

#ifndef PIXEL_UI_TYPES_H
#define PIXEL_UI_TYPES_H

#include <SparkyStudios/UI/Pixel/Config/Config.h>

#include <cstdint>
#include <string>

namespace SparkyStudios::UI::Pixel
{
    typedef int8_t PiInt8;
    typedef int16_t PiInt16;
    typedef int32_t PiInt32;
    typedef int64_t PiInt64;

    typedef uint8_t PiUInt8;
    typedef uint16_t PiUInt16;
    typedef uint32_t PiUInt32;
    typedef uint64_t PiUInt64;

    typedef float PiReal32;
    typedef double PiReal64;

    typedef double PiTime;

    typedef void* PiVoidPtr;

    typedef std::string PiString;

#if defined(PI_WCHAR_SUPPORTED)
    typedef std::wstring PiPlatformString;
#else
    typedef std::string PiPlatformString;
#endif

} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_TYPES_H
