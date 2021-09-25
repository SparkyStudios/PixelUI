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

#ifndef PIXEL_UI_TEXTURE_H
#define PIXEL_UI_TEXTURE_H

#include <SparkyStudios/UI/Pixel/Config/Config.h>
#include <SparkyStudios/UI/Pixel/Config/Types.h>

#include <list>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Texture
    {
        typedef std::list<Texture*> List;

        //! Status of load operation.
        enum class Status
        {
            Unloaded,
            Loaded,
            ErrorFileNotFound,
            ErrorBadData,
            MAX
        };

        Texture()
            : readable(false)
        {}

        Texture(const Texture&) = default;
        Texture& operator=(const Texture&) = default;

        inline bool operator==(const Texture& rhs) const
        {
            return name == rhs.name;
        }
        inline bool operator!=(const Texture& rhs) const
        {
            return name != rhs.name;
        }

        PiString name;
        bool readable;
    };

    struct TextureData
    {
        TextureData()
            : width(0.f)
            , height(0.f)
        {}

        float width;
        float height;
        bool readable;
    };
} // namespace SparkyStudios::UI::Pixel

namespace std
{
    template<>
    struct hash<SparkyStudios::UI::Pixel::Texture>
    {
        typedef SparkyStudios::UI::Pixel::Texture argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& f) const noexcept
        {
            return std::hash<decltype(f.name)>{}(f.name);
        }
    };
} // namespace std

#endif // PIXEL_UI_TEXTURE_H
