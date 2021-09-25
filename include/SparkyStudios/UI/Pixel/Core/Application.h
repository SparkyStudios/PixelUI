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

/**
 * @brief The current instance of the Pixel application.
 */
#define piApp SparkyStudios::UI::Pixel::Application::Instance()

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Application
    {
    public:
        static Application* Instance();

        bool Init(MainWindow* mainWindow);
        int Run();

    private:
        Application();

        bool _initialized;
        MainWindow* _mainWindow;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_APPLICATION_H
