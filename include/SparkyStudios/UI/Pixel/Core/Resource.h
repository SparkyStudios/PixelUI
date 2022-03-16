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

#ifndef PIXEL_UI_RESOURCE_H
#define PIXEL_UI_RESOURCE_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

#include <SparkyStudios/UI/Pixel/Graphics/Font.h>
#include <SparkyStudios/UI/Pixel/Graphics/Texture.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Generate paths to application resources.
     */
    class PI_EXPORT ResourcePaths
    {
    public:
        /**
         * @brief List of supported types of application resources.
         */
        enum class Type
        {
            /**
             * @brief Font resources.
             *
             * Only TTF files are supported.
             */
            Font,

            /**
             * @brief Image resources.
             */
            Texture,

            /**
             * @brief Other resources.
             *
             * May need a custom resource loader.
             */
            Other,

            MAX
        };

        virtual ~ResourcePaths()
        {}

        /**
         * @brief Get full path to a resource file.
         *
         * @param type File resource type.
         * @param name Relative path to file.
         *
         * @return Path relative to the resource directory.
         */
        virtual PiString GetPath(Type type, const PiString& name) const = 0;
    };

    /**
     * @brief Calculate application resources paths relatively to the executable path.
     */
    class PI_EXPORT RelativeToExecutableResourcePaths : public ResourcePaths
    {
    public:
        /**
         * @brief Constructor
         *
         * @param resourceDir Optional directory, relative to the executable.
         */
        explicit RelativeToExecutableResourcePaths(const PiString& resourceDir = PiString());

        PiString GetPath(Type type, const PiString& name) const final;

    private:
        PiString _resDir;
    };

    /**
     * @brief Base class for resource loading in platform specific
     * implementations.
     */
    class PI_EXPORT IResourceLoader
    {
    public:
        /**
         * @brief List of notification types used in the resource loader.
         */
        enum class NotificationType
        {
            /**
             * @brief Device lost, perhaps release resources.
             */
            DeviceLost,

            /**
             * @brief Device reappeared, perhaps reload resources.
             */
            DeviceAcquired,

            /**
             * @brief Release any resources held.
             */
            ReleaseResources,

            MAX,

            UserMessageStart = 100
        };

        /**
         * @brief The status of a load operation on a resource.
         */
        enum class LoadStatus
        {
            /**
             * @brief The resource was unloaded.
             */
            Unloaded,

            /**
             * @brief The resource was loaded.
             */
            Loaded,

            /**
             * @brief The resource file was not found while trying
             * to load it.
             */
            ErrorFileNotFound,

            /**
             * @brief The resource file contains bad data.
             */
            ErrorBadData,

            MAX
        };

        /**
         * @brief Loads a font resource.
         *
         * @param font The font resource to load.
         * @return The load status.
         */
        virtual LoadStatus LoadFont(const Font& font) = 0;

        /**
         * @brief Releases a font resource.
         *
         * @param font The font resource to release.
         */
        virtual void FreeFont(const Font& font) = 0;

        /**
         * @brief Loads a texture resource.
         *
         * @param texture The texture resource to load.
         * @return The load status.
         */
        virtual LoadStatus LoadTexture(const Texture& texture) = 0;

        /**
         * @brief Releases a texture resource.
         *
         * @param texture The texture resource to release.
         */
        virtual void FreeTexture(const Texture& texture) = 0;

        /**
         * @brief Get the texture data from a texture resource.
         *
         * @param texture The texture resource.
         * @return The texture data from the given texture resource.
         */
        virtual TextureData GetTextureData(const Texture& texture) const = 0;

        /**
         * @brief Notify the resource loader of an event.
         *
         * @param event The event to notify.
         */
        virtual void Notify(NotificationType event)
        {}
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_RESOURCE_H
