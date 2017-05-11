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

#include "SevenSegmentsDisplay.h"


//init the object.
//Set the values to default, set pin direction.
void SevenSegments::init(uint8_t shiftPin, uint8_t clockPin, mode_t mode){
	_pin = shiftPin;
	_clockPin = clockPin;
	_mode = mode;
	_value = 0;
	_point = false;

	pinMode(_pin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
}

//Shift out the value to the shift register
void SevenSegments::update(){
	uint8_t value = _value;
	if(_point){
		if(_mode == AC){
			value &= ~(1 << 7);
		} else {
			value |= (1 << 7);
		}
	}

	shiftOut(_pin, _clockPin, MSBFIRST, value);
}

//Set the current value
void SevenSegments::setValue(uint8_t value){
	//If the value is more than a digit, clear the display
	if(value > 9){
		_value = 0;
	//Else get the right byte value for this digit.
	} else {
		_value = _table[value];
	}

	//Invert bits if display is common anode.
	if(_mode == AC){
		_value = ~_value;
	}
}

//Clear the value: set all bits to 0
void SevenSegments::clrValue(){
	_value = 0;
}

//Set / unset point
void SevenSegments::setPoint(bool value){
	_point = value;
}

//Convenience function to unset point.
void SevenSegments::clrPoint(){
	setPoint(false);
}

FourDigits::~FourDigits(){
	for(uint8_t i = 0; i < 4; i++){
		delete _digits[i];
	}
}

void FourDigits::init(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4){
	_minutes = 0;
	_seconds = 0;
	_points = 0;
	_current = 0;
	_enable = true;

	_enablePins[0] = pin1;
	_enablePins[1] = pin2;
	_enablePins[2] = pin3;
	_enablePins[3] = pin4;

	for(uint8_t i = 0; i < 4; i++){
		SevenSegments *digit = new SevenSegments();
		digit->init(4, 5, SevenSegments::AC);
		_digits[i] = digit;

		pinMode(_enablePins[i], OUTPUT);
	}

}

void FourDigits::update(){
	_digits[0]->setValue(_seconds%10);
	_digits[1]->setValue(_seconds/10);
	_digits[2]->setValue(_minutes%10);
	_digits[3]->setValue(_minutes/10);

	for(uint8_t i = 0; i < 4; i++){
		digitalWrite(_enablePins[i], LOW);
	}

	_digits[_current]->update();
	if(_enable)	digitalWrite(_enablePins[_current], HIGH);

	if(++_current > 3) _current = 0;

}

void FourDigits::setMinutes(uint8_t minutes){
	_minutes = minutes;
}

void FourDigits::setSeconds(uint8_t seconds){
	_seconds = seconds;
}

void FourDigits::setPoint(bool value){
		_digits[2]->setPoint(value);
		_points = value;
}

void FourDigits::clrPoint(){
	setPoint(false);
}

void FourDigits::enable(bool value){
	_enable = value;
}
