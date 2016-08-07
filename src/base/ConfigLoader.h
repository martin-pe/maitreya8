/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ConfigLoader.h
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

#ifndef _CONFIGLOADER_H_
#define _CONFIGLOADER_H_

#include <wx/string.h>

#define DECLARE_CONFIGLOADER( classname, configname ) \
public: \
	~classname(); \
	static classname *get(); \
	void cleanup(); \
	configname *getConfig( const int &i ); \
	vector<configname*> getConfigs(); \
	int getCount(); \
\
protected: \
	classname(); \
	classname( const classname& ) {} \
	void clear(); \
	void init(); \
	vector<configname*> defs; \
	class XmlWorker*worker; \
	static classname *ego;

#define IMPLEMENT_CONFIGLOADER( classname, configname, workername ) \
classname *classname::ego = 0; \
\
classname::classname() { worker = 0; } \
\
classname::~classname() { if ( worker ) delete worker; } \
\
classname *classname::get() \
{ \
	if ( ego == 0 ) ego = new classname; \
	return ego; \
} \
\
void classname::cleanup() { clear(); } \
\
int classname::getCount() { return (int)defs.size(); } \
\
configname *classname::getConfig( const int &i ) { \
	if ( defs.size() == 0 || worker->needsReload() ) init(); \
	assert( i >= 0 && i < (int)defs.size()); \
	return defs[i]; \
} \
\
vector<configname*> classname::getConfigs() { \
	if ( defs.size() == 0 || worker->needsReload() ) init(); \
	return defs; \
} \
\
void classname::clear() { \
	for( uint i = 0; i < defs.size(); i++ ) delete defs[i]; \
	defs.clear(); \
} \
\
void classname::init() { \
	clear(); \
	if ( ! worker ) worker = new workername; \
	((workername*)worker)->readConfig( defs ); \
}


#endif

