/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PrintoutSheetCreator.h
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

#ifndef _PRINTOUTSHEETCREATOR_H_
#define _PRINTOUTSHEETCREATOR_H_

class ChartProperties;
class Horoscope;
class Sheet;

/*************************************************//**
*
*  
*
******************************************************/
class PrintoutSheetCreator
{
public:
	PrintoutSheetCreator( Horoscope*, ChartProperties*, Sheet* );

	void write( const int &id );

protected:

	Horoscope *horoscope;
	ChartProperties *chartprops;
	Sheet *sheet;
};


#endif


