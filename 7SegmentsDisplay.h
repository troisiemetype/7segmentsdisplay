/*
 * This Arduino library is for driving a 7 segments display
 * Copyright (C) 2016  Pierre-Loup Martin
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
 */

#ifndef SEVEN_SEGMENTS_H
#define SEVEN_SEGMENTS_H

#include <Arduino.h>
#include <stdint.h>


class SevenSegments{
public:
	enum mode_t{
		CC = 0,
		AC,
	};

	void init(uint8_t shiftPin, uint8_t clockPin, mode = AC);

	void update();

	void setValue(uint8_t value);
	void setPoint(bool value = true);
	void clrPoint();

protected:
	uint8_t _pin;
	uint8_t _clockPin;
	uint8_t _value;
	bool _point;

	mode_t _mode;

	// TODO: see if it works that way, or if it should be defined elsewere.
	static uint8_t table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

};

#endif