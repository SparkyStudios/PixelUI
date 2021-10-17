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

#ifndef PIXEL_UI_MAINWINDOW_H
#define PIXEL_UI_MAINWINDOW_H

#include <SparkyStudios/UI/Pixel/Config/Types.h>

#include <SparkyStudios/UI/Pixel/Core/Input/Mouse.h>

#include <SparkyStudios/UI/Pixel/Graphics/Point.h>
#include <SparkyStudios/UI/Pixel/Graphics/Rect.h>

namespace SparkyStudios::UI::Pixel
{
    enum MAIN_WINDOW_FLAGS
    {
        MAIN_WINDOW_WINDOWED,
        MAIN_WINDOW_FULLSCREEN,
        MAIN_WINDOW_RESIZABLE,
        MAIN_WINDOW_FRAMELESS,
        MAIN_WINDOW_MINIMIZED,
        MAIN_WINDOW_MAXIMIZED,
    };

    class PI_EXPORT MainWindow
    {
    public:
        MainWindow(PiInt32 x, PiInt32 y, PiUInt32 width, PiUInt32 height, PiString title, int flags);
        MainWindow(PiUInt32 width, PiUInt32 height, PiString title, int flags);
        MainWindow(const Rect& rect, PiString title, int flags);
        MainWindow(const Size& size, PiString title, int flags);

        ~MainWindow();

        void CreateNativeWindow();
        PiVoidPtr GetNativeHandle() const;

        void SetTitle(const PiString& title);
        const PiString& GetTitle() const;

        void Maximize();
        void Minimize();

        void SetSize(const Size& size);
        const Size& GetSize() const;

        void SetPosition(const Point& position);
        const Point& GetPosition() const;

        Cursor* GetCursor() const;
        void SetCursorStyle(CursorStyle style);

        PiString GetClipboardText() const;
        bool SetClipboardText(const PiString& str);

        bool FileOpen(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut);
        bool FileSave(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut);
        bool FolderOpen(const PiString& name, const PiString& startPath, PiString& filePathOut);

    private:
        PiVoidPtr _nativeHandle;

        Cursor* _cursor;
        Point _position;
        Size _size;
        PiString _title;
        int _flags;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_MAINWINDOW_H
