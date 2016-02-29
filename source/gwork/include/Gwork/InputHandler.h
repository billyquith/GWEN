/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUTHANDLER_H
#define GWK_INPUTHANDLER_H

#include <queue>
#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Controls
    {
        class Base;
    }

    namespace Key
    {
        enum KeyCode : char
        {
            Invalid = 0,
            Return = 1,
            Backspace = 2,
            Delete = 3,
            Left = 4,
            Right = 5,
            Shift = 6,
            Tab = 7,
            Space = 8,
            Home = 9,
            End = 10,
            Control = 11,
            Up = 12,
            Down = 13,
            Escape = 14,
            Alt = 15,
            KeysCount = 16  //!< number of keys
        };
    }

    namespace Input
    {
        namespace Message
        {
            enum
            {
                Copy,
                Paste,
                Cut,
                Undo,
                Redo,
                SelectAll
            };
        }

        // For use in panels
        bool GWK_EXPORT        IsKeyDown(int iKey);
        bool GWK_EXPORT        IsLeftMouseDown();
        bool GWK_EXPORT        IsRightMouseDown();
        Gwk::Point GWK_EXPORT GetMousePosition();

        inline bool IsShiftDown()
        {
            return IsKeyDown(Gwk::Key::Shift);
        }

        inline bool IsControlDown()
        {
            return IsKeyDown(Gwk::Key::Control);
        }

        // Does copy, paste etc
        bool GWK_EXPORT DoSpecialKeys(Controls::Base* pCanvas, Gwk::UnicodeChar chr);
        bool GWK_EXPORT HandleAccelerator(Controls::Base* pCanvas, Gwk::UnicodeChar chr);

        // Send input to canvas for study
        void GWK_EXPORT OnMouseMoved(Controls::Base* pCanvas, int x, int y, int deltaX,
                                      int deltaY);
        bool GWK_EXPORT OnMouseClicked(Controls::Base* pCanvas, int iButton, bool bDown);
        bool GWK_EXPORT OnKeyEvent(Controls::Base* pCanvas, int iKey, bool bDown);
        void GWK_EXPORT OnCanvasThink(Controls::Base* pControl);
    }
}
#endif // ifndef GWK_INPUTHANDLER_H