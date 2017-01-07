/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/SingletonLifeline.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2017 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "SingletonLifeline.h"

#include "MultipleViewConfig.h"
#include "PrintoutConfig.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"
#include "Yoga.h"
#include "UranianHelper.h"

/*****************************************************
**
**   SingletonLifeline   ---   cleanup
**
******************************************************/
void SingletonLifeline::cleanup()
{
	//PrintoutConfigLoader::get()->clear();
	delete PrintoutConfigLoader::get();
	delete WesternChartConfigLoader::get();
	delete VedicChartConfigLoader::get();
	YogaExpert::get()->clear();
	delete MultipleViewConfigLoader::get();
	delete UranianTextLoader::get();
}
