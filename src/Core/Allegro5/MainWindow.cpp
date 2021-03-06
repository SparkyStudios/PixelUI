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

#include <SparkyStudios/UI/Pixel/Core/MainWindow.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>

namespace SparkyStudios::UI::Pixel
{
    static int TranslateToAllegroFlags(int flags)
    {
        int alFlags = 0;

        if ((flags & MAIN_WINDOW_WINDOWED) == MAIN_WINDOW_WINDOWED)
        {
            alFlags |= ALLEGRO_WINDOWED;
        }

        if ((flags & MAIN_WINDOW_FULLSCREEN) == MAIN_WINDOW_FULLSCREEN)
        {
            alFlags |= ALLEGRO_FULLSCREEN;
        }

        if ((flags & MAIN_WINDOW_RESIZABLE) == MAIN_WINDOW_RESIZABLE)
        {
            alFlags |= ALLEGRO_RESIZABLE;
        }

        if ((flags & MAIN_WINDOW_FRAMELESS) == MAIN_WINDOW_FRAMELESS)
        {
            alFlags |= ALLEGRO_FRAMELESS;
        }

        if ((flags & MAIN_WINDOW_MINIMIZED) == MAIN_WINDOW_MINIMIZED)
        {
            alFlags |= ALLEGRO_MINIMIZED;
        }

        if ((flags & MAIN_WINDOW_MAXIMIZED) == MAIN_WINDOW_MAXIMIZED)
        {
            alFlags |= ALLEGRO_MAXIMIZED;
        }

        return alFlags;
    }

    MainWindow::MainWindow(PiInt32 x, PiInt32 y, PiUInt32 width, PiUInt32 height, const PiString& title, int flags)
        : _nativeHandle(nullptr)
        , _flags(flags)
        , _title(title)
        , _position(x, y)
        , _size(width, height)
        , _cursor(new Cursor(this))
        , _defaultCursorStyle(CursorStyle::Default)
    {}

    MainWindow::MainWindow(PiUInt32 width, PiUInt32 height, const PiString& title, int flags)
        : MainWindow(-1, -1, width, height, title, flags)
    {}

    MainWindow::MainWindow(const Rect& rect, const PiString& title, int flags)
        : MainWindow(rect.x, rect.y, rect.w, rect.h, title, flags)
    {}

    MainWindow::MainWindow(const Size& size, const PiString& title, int flags)
        : MainWindow(size.w, size.h, title, flags)
    {}

    MainWindow::~MainWindow()
    {
        al_destroy_display(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle));
    }

    void MainWindow::CreateNativeWindow()
    {
        if (_position.x >= 0 && _position.y >= 0)
        {
            al_set_new_window_position(_position.x, _position.y);
        }

        al_set_new_display_flags(TranslateToAllegroFlags(_flags) | ALLEGRO_GENERATE_EXPOSE_EVENTS);

        _nativeHandle = al_create_display(_size.w, _size.h);

        if (!_nativeHandle)
            return;

        SetTitle(_title);
    }

    PiVoidPtr MainWindow::GetNativeHandle() const
    {
        return _nativeHandle;
    }

    void MainWindow::CreateRootCanvas(Skin* skin)
    {
        _rootCanvas = std::make_unique<Canvas>(this, skin);
        _rootCanvas->SetSize(_size);
        _rootCanvas->SetDrawBackground(true);
        _rootCanvas->SetName("@@root");
    }

    const std::unique_ptr<Canvas>& MainWindow::GetRootCanvas() const
    {
        return _rootCanvas;
    }

    void MainWindow::SetTitle(const PiString& title)
    {
        al_set_window_title(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), title.c_str());
    }

    const PiString& MainWindow::GetTitle() const
    {
        return _title;
    }

    void MainWindow::Maximize()
    {
        al_set_display_flag(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), ALLEGRO_MAXIMIZED, true);
    }

    void MainWindow::Minimize()
    {
        al_set_display_flag(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), ALLEGRO_MINIMIZED, true);
    }

    void MainWindow::SetSize(const Size& size)
    {
        al_resize_display(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), size.w, size.h);
        _size = size;
    }

    const Size& MainWindow::GetSize() const
    {
        return _size;
    }

    PiInt32 MainWindow::GetWidth() const
    {
        return _size.w;
    }

    PiInt32 MainWindow::GetHeight() const
    {
        return _size.h;
    }

    void MainWindow::SetPosition(const Point& position)
    {
        al_set_window_position(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), position.x, position.y);
        _position = position;
    }

    const Point& MainWindow::GetPosition() const
    {
        return _position;
    }

    Cursor* MainWindow::GetCursor() const
    {
        return _cursor;
    }

    void MainWindow::SetDefaultCursorStyle(CursorStyle style)
    {
        _defaultCursorStyle = style;
    }

    CursorStyle MainWindow::GetDefaultCursorStyle() const
    {
        return _defaultCursorStyle;
    }

    void MainWindow::ApplyCursorStyle(CursorStyle style) const
    {
        _cursor->ApplyStyle(style);
    }

    void MainWindow::ApplyDefaultCursorStyle() const
    {
        _cursor->ApplyStyle(_defaultCursorStyle);
    }

    bool MainWindow::FileOpen(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut)
    {
        ALLEGRO_FILECHOOSER* chooser =
            al_create_native_file_dialog(startPath.c_str(), name.c_str(), extension.c_str(), ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);

        if (al_show_native_file_dialog(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), chooser))
        {
            if (al_get_native_file_dialog_count(chooser) != 0)
            {
                filePathOut = al_get_native_file_dialog_path(chooser, 0);
            }
        }

        al_destroy_native_file_dialog(chooser);
        return true;
    }

    bool MainWindow::FileSave(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut)
    {
        ALLEGRO_FILECHOOSER* chooser =
            al_create_native_file_dialog(startPath.c_str(), name.c_str(), extension.c_str(), ALLEGRO_FILECHOOSER_SAVE);

        if (al_show_native_file_dialog(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), chooser))
        {
            if (al_get_native_file_dialog_count(chooser) != 0)
            {
                filePathOut = al_get_native_file_dialog_path(chooser, 0);
            }
        }

        al_destroy_native_file_dialog(chooser);
        return true;
    }

    bool MainWindow::FolderOpen(const PiString& name, const PiString& startPath, PiString& filePathOut)
    {
        ALLEGRO_FILECHOOSER* chooser = al_create_native_file_dialog(
            startPath.c_str(), name.c_str(),
            "*.*", // extension.c_str(),
            ALLEGRO_FILECHOOSER_FOLDER);

        if (al_show_native_file_dialog(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle), chooser))
        {
            if (al_get_native_file_dialog_count(chooser) != 0)
            {
                filePathOut = al_get_native_file_dialog_path(chooser, 0);
            }
        }

        al_destroy_native_file_dialog(chooser);
        return true;
    }

    void MainWindow::OnResize(const Size& newSize)
    {
        al_acknowledge_resize(static_cast<ALLEGRO_DISPLAY*>(_nativeHandle));
        _rootCanvas->SetSize(newSize);
    }

    void MainWindow::OnExpose()
    {}

    void MainWindow::Paint(Skin* skin)
    {
        if (skin == nullptr)
            return;

        skin->GetRenderer()->BeginContext(this);
        {
            _rootCanvas->RenderCanvas();
        }
        skin->GetRenderer()->PresentContext(this);
        skin->GetRenderer()->EndContext(this);
    }
} // namespace SparkyStudios::UI::Pixel
