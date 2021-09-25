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

#ifndef PIXEL_UI_CONFIG_H
#define PIXEL_UI_CONFIG_H

#if defined(PI_COMPILE_SHARED)

#if defined(_WIN32)
#if defined(__GNUC__)
#define PI_EXPORT __attribute__((dllexport))
#else
#define PI_EXPORT __declspec(dllexport)
#endif // defined(__GNUC__)
#endif // defined(_WIN32)

#elif defined(PI_COMPILE_STATIC)

#define PI_EXPORT

#elif defined(PI_DLL)

#if defined(_WIN32)
#if defined(__GNUC__)
#define PI_EXPORT __attribute__((dllimport))
#else
#define PI_EXPORT __declspec(dllimport)
#endif // defined(__GNUC__)
#endif // defined(_WIN32)

// - We're using PixelUI through the static library
#else

#define PI_EXPORT

#endif // defined(PI_COMPILE_SHARED)

#ifndef PI_EXPORT
#define PI_EXPORT
#endif

// Enable animation functions.
#define PI_ANIMATE 1

// Check if wchar_t is supported
#if defined(_WIN32) || defined(_WIN64) || defined(WINAPI_FAMILY) || defined(__linux__)
#define PI_WCHAR_SUPPORTED
#endif

#endif // PIXEL_UI_CONFIG_H
