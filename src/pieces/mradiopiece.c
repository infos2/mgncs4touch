/*
 *   This file is part of mGNCS4Touch, a component for MiniGUI.
 *
 *   Copyright (C) 2008~2018, Beijing FMSoft Technologies Co., Ltd.
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *   Or,
 *
 *   As this program is a library, any link to this program must follow
 *   GNU General Public License version 3 (GPLv3). If you cannot accept
 *   GPLv3, you need to be licensed from FMSoft.
 *
 *   If you have got a commercial license of this program, please use it
 *   under the terms and conditions of the commercial license.
 *
 *   For more information about the commercial license, please refer to
 *   <http://www.minigui.com/en/about/licensing-policy/>.
 */
#include <string.h>
#include <assert.h>

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>

#include <mgplus/mgplus.h>
#include <mgeff/mgeff.h>
#include <mgncs/mgncs.h>

#include "mgncs4touch.h"

static void mRadioPiece_construct (mRadioPiece *self, DWORD add_data)
{
    Class(mStaticPiece).construct ((mStaticPiece*)self, add_data);

    self->state = NCSN_RADIOPIECE_OFF;
    self->color = 0xFF000000;

    self->hgs   = MP_INV_HANDLE;
}

static void mRadioPiece_destroy (mRadioPiece *self)
{
    MGPlusGraphicDelete (self->hgs);

    Class(mStaticPiece).destroy ((mStaticPiece*)self);
}

static int mRadioPiece_getState (mRadioPiece *self)
{
    return self->state;
}

static void setStateWithUpdatePiece (mRadioPiece *self, enum mRadioPieceState state)
{
    _c(self)->setState(self, state);

    //TODO: why PanelPiece_invalidatePiece not work?
    PanelPiece_update ((mHotPiece*)self, TRUE);
}

static void mRadioPiece_setState (mRadioPiece *self, enum mRadioPieceState state)
{
    if (self->state == state) return;

    self->state = state;
}

static void mRadioPiece_resetState (mRadioPiece *self)
{
    setStateWithUpdatePiece (self, NCSN_RADIOPIECE_OFF);
}

static void mRadioPiece_changeState (mRadioPiece* self)
{
    enum mRadioPieceState state =
        (self->state == NCSN_RADIOPIECE_OFF) ? NCSN_RADIOPIECE_ON : NCSN_RADIOPIECE_OFF;

    setStateWithUpdatePiece (self, state);
}

#if 0
static int mRadioPiece_processMessage (mRadioPiece *self, int message, WPARAM wParam, LPARAM lParam, mObject *owner)
{
    if (message == MSG_LBUTTONDOWN) {
    } else if (message == MSG_LBUTTONUP) {
        _c(self)->changeState(self);
        ncsRaiseEvent((mObject*)self, self->state, 0);
    }
    return 0;
}
#endif

static void mRadioPiece_paint (mRadioPiece *self, HDC hdc, mObject *owner, DWORD add_data)
{
    HGRAPHICS hgs;
    HPEN      pen;

    int w;
    int h;

    if (self->state == NCSN_RADIOPIECE_ON) {
        hgs = self->hgs;
        w   = self->hgs_width;
        h   = self->hgs_height;

        MGPlusGraphicClear (hgs, 0);

        pen = MGPlusPenCreate (2, self->color);

        MGPlusDrawLine (hgs, pen, w / 4, h / 2, w / 2, h * 3 / 4);
        MGPlusDrawLine (hgs, pen, w / 2, h * 3 / 4, w * 6 / 7, h / 4);

        MGPlusPenDelete (pen);

        BitBlt (MGPlusGetGraphicDC(hgs), 0, 0, 0, 0, hdc, 0, 0, 0);
    }
}

static BOOL mRadioPiece_setProperty (mRadioPiece *self, int id, DWORD value)
{
    switch (id) {
        case NCSP_RADIO_COLOR:
            self->color = (ARGB)value;
            break;
        default:
            return Class(mStaticPiece).setProperty ((mStaticPiece*)self, id, value);
    }

    return TRUE;
}

static DWORD mRadioPiece_getProperty(mRadioPiece *self, int id)
{
    switch (id) {
        case NCSP_RADIO_COLOR:
            return (DWORD)self->color;
        default:
            return Class(mStaticPiece).getProperty ((mStaticPiece*)self, id);
    }
}

static BOOL mRadioPiece_setRect(mRadioPiece *self, const RECT *prc)
{
    BOOL ret = Class(mStaticPiece).setRect ((mStaticPiece*)self, prc);

    if (ret) {
        self->hgs_width = RECTWP(prc);
        self->hgs_height = RECTHP(prc);

        if (self->hgs != MP_INV_HANDLE) {
            MGPlusGraphicDelete (self->hgs);
        }

        self->hgs = MGPlusGraphicCreate (self->hgs_width, self->hgs_height);
    }

    return ret;
}

BEGIN_MINI_CLASS(mRadioPiece, mStaticPiece)
    CLASS_METHOD_MAP(mRadioPiece, construct   )
    CLASS_METHOD_MAP(mRadioPiece, destroy     )
    CLASS_METHOD_MAP(mRadioPiece, getState    )
    CLASS_METHOD_MAP(mRadioPiece, setState    )
    CLASS_METHOD_MAP(mRadioPiece, changeState )
    CLASS_METHOD_MAP(mRadioPiece, resetState  )
    CLASS_METHOD_MAP(mRadioPiece, paint       )
    CLASS_METHOD_MAP(mRadioPiece, setProperty )
    CLASS_METHOD_MAP(mRadioPiece, getProperty )
    CLASS_METHOD_MAP(mRadioPiece, setRect     )
END_MINI_CLASS
