#ifndef __UTIL_CONSTANTS_H_
#define __UTIL_CONSTANTS_H_

/// @file constants.h

// Texture ID's (in atlas)
const int TEXTURE_SAND = 0;
const int TEXTURE_WALL = 1;
const int TEXTURE_CRACKED_WALL = 2;
const int TEXTURE_ROUND_WALL = 3;
const int TEXTURE_CRACKED_ROUND_WALL = 4;
const int TEXTURE_ROCK = 5;
const int TEXTURE_EMERALD = 6;
const int TEXTURE_BOMB = 7;
const int TEXTURE_BAG = 8;
const int TEXTURE_DYNAMITE_0 = 9;
const int TEXTURE_DYNAMITE_1 = 10;
const int TEXTURE_DYNAMITE_2 = 11;
const int TEXTURE_DYNAMITE_3 = 12;
const int TEXTURE_DYNAMITE_4 = 13;
const int TEXTURE_DYNAMITE_5 = 14;
const int TEXTURE_DYNAMITE_6 = 15;
const int TEXTURE_DYNAMITE_7 = 16;
const int TEXTURE_EXPLOSION = 17;
const int TEXTURE_CLOSED_GOAL = 18;
const int TEXTURE_OPEN_GOAL = 19;
const int TEXTURE_PLAYER = 20;
const int TEXTURE_NOT_SET = 21;

// Block types
const int BLOCK_NONE = 0;
const int BLOCK_SAND = 1;
const int BLOCK_WALL = 2;
const int BLOCK_CRACKED_WALL = 3;
const int BLOCK_ROUND_WALL = 4;
const int BLOCK_CRACKED_ROUND_WALL = 5;
const int BLOCK_ROCK = 6;
const int BLOCK_EMERALD = 7;
const int BLOCK_BOMB = 8;
const int BLOCK_BAG = 9;
const int BLOCK_DYNAMITE = 10;
const int BLOCK_ACTIVE_DYNAMITE = 11;
const int BLOCK_EXPLOSION = 12;
const int BLOCK_CLOSED_GOAL = 13;
const int BLOCK_OPEN_GOAL = 14;
const int BLOCK_PLAYER = 15;

// Player move types
const int MOVE_NONE = 0;
const int MOVE_LEFT = 1;
const int MOVE_UP = 2;
const int MOVE_RIGHT = 3;
const int MOVE_DOWN = 4;
const int MOVE_FALL_BIT = 8;
const int MOVE_SLIDE_BIT = 16;
const int MOVE_GRAB_BIT = 32;
const int MOVE_PLACE_DYNAMITE_BIT = 64;

// Misc constants
const int RULES_EXPLOSION_DURATION = 3;
const int RULES_DYNAMITE_TIMER = 8;
const int DIR_LEFT = false;
const int DIR_RIGHT = true;

#endif  // __UTIL_CONSTANTS_H_
