/*
 * UIText.h
 * This file is part of Trissa
 *
 * Copyright (C) 2008 - Lucas De Marchi
 *
 * Trissa is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * Trissa is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Trissa; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */
#ifndef _UITEXT_H_
#define _UITEXT_H_ 1

#include "UI.h"

namespace trissa
{
class UIText : public UI
{
public:
	UIText(ConfigManager* cm, StateManager* sm, PlayerFactory const* pf) : UI(cm, sm, pf) { }
	~UIText() {}
	
	void configure();
	void start(Cube const& board);
	void start_thread();
	void setPos(Move const &m, PlayerType player);
	
	void gameOver(Move const& startPosition, Move const& direction);
	void gameOver();


	Move getUserInput();
	void printWinnerMessage(std::string msg) { }
	void printLooserMessage(std::string msg) { }
private:
	Cube const* mBoard;
	bool confirmConfig();
	unsigned int getDimension();
	std::string getPlayer(std::string player);

	void refresh();
};
}

#endif /*_UITEXT_H_*/
