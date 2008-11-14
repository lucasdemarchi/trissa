/*
 * common.h
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
#ifndef COMMON_H__
#define COMMON_H__

#include <vector>

namespace trissa{
	class Player;
	
	typedef Player * (*function_creator_ptr)();
	typedef struct Move {
		int x;
		int y;
		int z;
	} Move;
		
	typedef std::vector<std::vector<std::vector<int> > > Cube;

#define PLAYER_NONE 0

}

#endif /* COMMON_H__ */
