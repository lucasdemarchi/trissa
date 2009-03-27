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
#ifndef _COMMON_H_
#define _COMMON_H_ 1
#ifndef TRISSA_VERSION
#define TRISSA_VERSION 0.5
#endif

#ifndef TRISSA_CONF_UI3D
#define TRISSA_CONF_UI3D 0
#endif

#include <vector>

namespace trissa
{
class Player;
enum PlayerType {PLAYER_BLANK, PLAYER_CROSS, PLAYER_CIRCLE};

typedef Player * (*function_creator_ptr)(int, PlayerType);
class Move
{
public:
    int x;
    int y;
    int z;
    Move(int x, int y, int z) : x(x), y(y), z(z) {}
    //Move(){}
    void operator-=(Move a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;
    }
    void operator+=(Move a) {
        x += a.x;
        y += a.y;
        z += a.z;
    }
};

typedef std::vector<std::vector<std::vector<PlayerType> > > Cube;


}

#endif /* _COMMON_H_ */
