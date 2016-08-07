/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/GraphConfig.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/

#include "GraphConfig.h"
#include "ColorConfig.h"
#include "Conf.h"

#include <wx/log.h>

extern Config *config;

/*****************************************************
**
**   GcRegion   ---   Constructor
**
******************************************************/
GcRegion::GcRegion()
{
	show = true;
}

/*****************************************************
**
**   GcChartFrame   ---   Constructor
**
******************************************************/
GcChartFrame::GcChartFrame( const double radius )
 : GcRegion()
{
	this->radius = radius;
	frameType = CF_INHERITED;
}

/*****************************************************
**
**   GcRectangle   ---   Constructor
**
******************************************************/
GcRectangle::GcRectangle()
 : GcRegion()
{
	doubleOuterLine = false;
	radius = 100.0;
	cornerRadius = 0;
}

/*****************************************************
**
**   GcArrow   ---   Constructor
**
******************************************************/
GcArrow::GcArrow()
{
	innerFrame.radius = 50;
	innerFrame.show = false;

	outerFrame.radius = 97;
	outerFrame.show = false;
}

