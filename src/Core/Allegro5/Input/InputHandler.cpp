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

#include <Core/Allegro5/Input/InputHandler.h>

namespace SparkyStudios::UI::Pixel
{
    static Key TranslateKeyCode(PiInt32 iKeyCode)
    {
        switch (iKeyCode)
        {
        case ALLEGRO_KEY_BACKSPACE:
            return Key::Backspace;

        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_PAD_ENTER:
            return Key::Return;

        case ALLEGRO_KEY_ESCAPE:
            return Key::Escape;

        case ALLEGRO_KEY_TAB:
            return Key::Tab;

        case ALLEGRO_KEY_SPACE:
            return Key::Space;

        case ALLEGRO_KEY_UP:
            return Key::Up;

        case ALLEGRO_KEY_DOWN:
            return Key::Down;

        case ALLEGRO_KEY_LEFT:
            return Key::Left;

        case ALLEGRO_KEY_RIGHT:
            return Key::Right;

        case ALLEGRO_KEY_HOME:
            return Key::Home;

        case ALLEGRO_KEY_END:
            return Key::End;

        case ALLEGRO_KEY_DELETE:
            return Key::Delete;

        case ALLEGRO_KEY_LSHIFT:
        case ALLEGRO_KEY_RSHIFT:
            return Key::Shift;

        case ALLEGRO_KEY_LCTRL:
        case ALLEGRO_KEY_RCTRL:
            return Key::Control;

        case ALLEGRO_KEY_ALT:
        case ALLEGRO_KEY_ALTGR:
            return Key::Alt;

        case ALLEGRO_KEY_PGUP:
            return Key::PageUp;

        case ALLEGRO_KEY_PGDN:
            return Key::PageDown;

        case ALLEGRO_KEY_PAUSE:
            return Key::Pause;

        case ALLEGRO_KEY_INSERT:
            return Key::Insert;

        case ALLEGRO_KEY_COMMAND:
            return Key::Command;

        default:
            return Key::Invalid;
        }
    }

    InputHandler_Allegro::InputHandler_Allegro()
        : m_eventListener(nullptr)
        , m_mouseX(0)
        , m_mouseY(0)
    {}

    void InputHandler_Allegro::Initialize(IInputEventListener* el)
    {
        m_eventListener = el;
    }

    bool InputHandler_Allegro::ProcessMessage(const ALLEGRO_EVENT& event)
    {
        if (!m_eventListener)
            return false;

        switch (event.type)
        {
        case ALLEGRO_EVENT_MOUSE_AXES:
            {
                const PiInt32 dx = event.mouse.dx;
                const PiInt32 dy = event.mouse.dy;

                if (event.mouse.dz != 0 || event.mouse.dw != 0)
                    return m_eventListener->OnMouseWheel(-event.mouse.dw, event.mouse.dz);

                m_mouseX = event.mouse.x;
                m_mouseY = event.mouse.y;
                return m_eventListener->OnMouseMove(m_mouseX, m_mouseY, dx, dy);
            }

        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            return m_eventListener->OnMouseButton(
                static_cast<MouseButton>(event.mouse.button - 1),
                event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ? MouseButtonPressMode::Pressed : MouseButtonPressMode::Released);

        case ALLEGRO_EVENT_KEY_CHAR:
            return m_eventListener->OnCharacter(event.keyboard.unichar);

        case ALLEGRO_EVENT_KEY_DOWN:
        case ALLEGRO_EVENT_KEY_UP:
            {
                const bool pressed = (event.type == ALLEGRO_EVENT_KEY_DOWN);

                if (event.keyboard.keycode != 0 && pressed && event.keyboard.keycode >= ALLEGRO_KEY_A &&
                    event.keyboard.keycode <= ALLEGRO_KEY_Z)
                {
                    return m_eventListener->OnCharacter(event.keyboard.unichar);
                }

                const Key key = TranslateKeyCode(event.keyboard.keycode);
                return key != Key::Invalid && m_eventListener->OnSpecialKey(key, pressed ? KeyPressMode::Pressed : KeyPressMode::Released);
            }
        }

        return false;
    }
} // namespace SparkyStudios::UI::Pixel
