#pragma once

struct Input
{
	Input() : left(false), right(false), up(false), down(false) {};
	void Reset() { left = right = up = down = false; }

	bool left;
	bool right;
	bool up;
	bool down;
};