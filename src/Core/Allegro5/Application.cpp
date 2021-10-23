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

#include <SparkyStudios/UI/Pixel/Core/Application.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

namespace SparkyStudios::UI::Pixel
{
    static Application* gApplication = nullptr;

    static ALLEGRO_DISPLAY* gDisplay = nullptr;
    static ALLEGRO_EVENT_QUEUE* gEventQueue = nullptr;

    Application::~Application()
    {
        delete _mainWindow;

        al_destroy_event_queue(gEventQueue);
    }

    bool Application::Init(MainWindow* mainWindow)
    {
        if (!_initialized)
        {
            if (!al_init())
                return EXIT_FAILURE;

            // Create the main window
            mainWindow->CreateNativeWindow();
            gDisplay = static_cast<ALLEGRO_DISPLAY*>(mainWindow->GetNativeHandle());

            if (!gDisplay)
                return EXIT_FAILURE;

            gEventQueue = al_create_event_queue();

            if (!gEventQueue)
                return EXIT_FAILURE;

            al_init_image_addon();
            al_init_font_addon();
            al_init_primitives_addon();
            al_init_ttf_addon();
            al_install_mouse();
            al_install_keyboard();
            al_register_event_source(gEventQueue, al_get_display_event_source(gDisplay));
            al_register_event_source(gEventQueue, al_get_mouse_event_source());
            al_register_event_source(gEventQueue, al_get_keyboard_event_source());

            _mainWindow = mainWindow;
            _initialized = true;
        }

        return _initialized;
    }

    int Application::Run()
    {
        if (!_initialized)
            return EXIT_FAILURE;

        ALLEGRO_EVENT ev;
        bool haveQuit = false;
        while (!haveQuit)
        {
            while (al_get_next_event(gEventQueue, &ev))
            {
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    haveQuit = true;
            }

            al_rest(0.001);
        }

        delete this;

        return EXIT_SUCCESS;
    }

    Application::Application()
        : _initialized(false)
        , _mainWindow(nullptr)
    {}

    Application* Application::Instance()
    {
        return gApplication ? gApplication : gApplication = new Application();
    }
} // namespace SparkyStudios::UI::Pixel