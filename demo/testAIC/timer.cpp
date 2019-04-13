/*  This file is part of CORE-CAM Driver.
	Copyright 2018-2019 Manuele Turini

	CORE-CAM Driver is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	CORE-CAM Driver is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with CORE-CAM Driver.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <windows.h>

static LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
static LARGE_INTEGER Frequency;


void StartTimer()
{
	QueryPerformanceFrequency(&Frequency); 
	QueryPerformanceCounter(&StartingTime);
}

unsigned long long EndTimer()
{
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;


	//
	// We now have the elapsed number of ticks, along with the
	// number of ticks-per-second. We use these values
	// to convert to the number of elapsed microseconds.
	// To guard against loss-of-precision, we convert
	// to microseconds *before* dividing by ticks-per-second.
	//

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	return ElapsedMicroseconds.QuadPart;
}