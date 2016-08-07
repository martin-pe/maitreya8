/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/PrintoutTextHelper.h
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

#ifndef _PRINTOUTTEXTHELPER_H_
#define _PRINTOUTTEXTHELPER_H_

class ChartProperties;
class Horoscope;
class Sheet;

#include "maitreya.h"

/*************************************************//**
*
*  
*
******************************************************/
class PrintoutTextHelper
{
public:

	PrintoutTextHelper( Horoscope*, const ChartProperties*, Sheet* );
	~PrintoutTextHelper();

	void setShowHeader( const bool b = true ) { show_header = b; }

	void writePrintoutHeader( const int &mode, const bool &vedic );

private:
	void writeBaseData( Tc& );

	Horoscope *horoscope;
	ChartProperties *chartprops;
	Sheet *sheet;
	bool show_header;
};

#endif

