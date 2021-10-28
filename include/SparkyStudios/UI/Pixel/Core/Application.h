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

#ifndef PIXEL_UI_APPLICATION_H
#define PIXEL_UI_APPLICATION_H

#include <SparkyStudios/UI/Pixel/Core/MainWindow.h>
#include <SparkyStudios/UI/Pixel/Core/Resource.h>

#include <SparkyStudios/UI/Pixel/Core/Renderer/Skin.h>

/**
 * @brief The current instance of the Pixel application.
 */
#define piApp SparkyStudios::UI::Pixel::Application::Instance()

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Application
    {
    public:
        /**
         * @brief Get the application instance.
         */
        static Application* Instance();

        /**
         * @brief Set the path to the application resources directory.
         *
         * This directory path is relative to the application executable path.
         *
         * @param path The path to the application resources directory.
         */
        static void SetAppResourcesDirectoryPath(const PiString& path);

        /**
         * @brief Get the path to the application resources directory.
         *
         * @return The path to the application resources directory
         * relative to the executable path.
         */
        static const PiString& GetAppResourcesDirectoryPath();

        ~Application();

        /**
         * @brief Initializes the application with the given main window.
         *
         * @param mainWindow The application main window.
         * @param skinData The application skin data.
         *
         * @return Whether the initialization was successful.
         */
        bool Init(MainWindow* mainWindow, const Skin::Data& skinData = Skin::Data::Default);

        /**
         * @brief Starts and runs the application.
         *
         * This method will block until the application has finished running
         * (the main window has been closed).
         *
         * @return int The exit code of the application.
         */
        int Run();

        /**
         * @brief Get the application skin.
         *
         * @return The application skin.
         */
        Skin* GetSkin() const;

    private:
        Application();

        bool _initialized;
        MainWindow* _mainWindow;
        RelativeToExecutableResourcePaths _paths;

        Skin* _skin;
        BaseRenderer* _renderer;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_APPLICATION_H
