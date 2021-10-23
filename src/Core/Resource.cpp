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

#include <SparkyStudios/UI/Pixel/Core/Platform.h>
#include <SparkyStudios/UI/Pixel/Core/Resource.h>

namespace SparkyStudios::UI::Pixel
{
    RelativeToExecutableResourcePaths::RelativeToExecutableResourcePaths(const PiString& resourceDir)
        : _resDir(Platform::GetExecutableDir() + resourceDir)
    {}

    PiString RelativeToExecutableResourcePaths::GetPath(Type type, const PiString& name)
    {
        PiString filePath(_resDir + name);

        // Only support TTF files for fonts
        if (type == Type::Font)
        {
            if (filePath.find(".ttf") == PiString::npos)
                filePath += ".ttf";
        }

        return filePath;
    }
} // namespace SparkyStudios::UI::Pixel
