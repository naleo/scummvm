
/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef BAGEL_BAGLIB_CREDITS_DIALOG_H
#define BAGEL_BAGLIB_CREDITS_DIALOG_H

#include "bagel/boflib/gui/dialog.h"
#include "bagel/boflib/gfx/text.h"

namespace Bagel {

class CBagCreditsDialog : public CBofDialog {
public:
	CBagCreditsDialog();

	virtual void OnInitDialog();

protected:
	virtual void OnPaint(CBofRect *pRect);
	virtual void onClose();

	virtual void onKeyHit(uint32 lKey, uint32 lRepCount);
	virtual void OnLButtonDown(uint32 nFlags, CBofPoint *pPoint, void * = nullptr);

	virtual void OnMainLoop();

	ErrorCode NextScreen();
	ErrorCode DisplayCredits();
	ErrorCode LoadNextTextFile();
	int LinesPerPage();
	void NextLine();
	ErrorCode PaintLine(int nLine, char *pszText);

	CBofBitmap *m_pCreditsBmp;
	CBofBitmap *m_pSaveBmp;

	char *m_pszNextLine;
	char *m_pszEnd;
	char *m_pszText;

	int m_nLines;
	int m_nNumPixels;
	int m_iScreen;

	bool m_bDisplay;
};

} // namespace Bagel

#endif
