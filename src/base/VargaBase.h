/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VargaBase.h
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

#ifndef _VARGABASE_H_
#define _VARGABASE_H_

#include <wx/string.h>
#include <vector>

#include "maitreya.h"

using namespace std;

/*************************************************//**
*
* 
*
******************************************************/
class VargaConfigLoader
{
	DECLARE_SINGLETON( VargaConfigLoader )

public:

	void init();

	wxString getVargaName( const Varga& );

	wxString getVargaSignification( const Varga& );

	int getVargaDivision( const Varga& ) const;

	Varga getVargaIndexByDivision( const int& ) const;

	wxString getDefaultVargaSignification( const Varga& );

private:

	const static int vargaDivisions[NB_VARGAS];
};


#endif


