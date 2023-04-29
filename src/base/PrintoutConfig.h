/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfig.h
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
#ifndef _PRINTOUTCONFIG_H_
#define _PRINTOUTCONFIG_H_

#include <list>
#include <map>
#include <vector>

#include "ConfigListLoader.h"
#include "PrintoutConfigBase.h"
#include "maitreya.h"

class wxJSONValue;

enum PD_ITEM_TYPE { PD_TITLE = 0, PD_HEADER, PD_DASA_SUMMARY, PD_CHART, PD_WIDGET_GRID, PD_COLUMN_SET, PD_SBC,
	PD_ROW_SET, PD_GRID, PD_WESTERN_CHART, PD_VEDIC_CHART_PAIR,
	PD_ASPECTARIUM, PD_ASHTAKAVARGA, PD_URANIAN, PD_YOGAS, PD_VARGA_DIAGRAMS,
	PD_EMPTY, PD_ERROR };

/*************************************************//**
*
* 
*
******************************************************/
class PrintoutItem //  : public wxObject
{
public:
	PrintoutItem( const PD_ITEM_TYPE &type ) : type( type ) {}

	const PD_ITEM_TYPE type;
};

class PrintoutItemHeader  : public PrintoutItem
{
public:
	PrintoutItemHeader( const int &headerType, const bool vedic )
		: PrintoutItem( PD_HEADER ), headerType( headerType ), vedic( vedic ) {}
	const int headerType;
	const bool vedic;
};

class PrintoutItemChart  : public PrintoutItem
{
public:
	PrintoutItemChart( const bool &vedic, const int &skin, const Varga varga = V_NONE )
		: PrintoutItem( PD_CHART ),
		vedic( vedic ),
		skin( skin ),
		varga( varga )
		{}

	const bool vedic;
	const uint skin;
	const Varga varga;
};

class PrintoutItemSbc  : public PrintoutItem
{
public:
	PrintoutItemSbc( const int &skin )
		: PrintoutItem( PD_SBC ),
		skin( skin )
		{}

	const int skin;
};

class PrintoutItemContainer : public PrintoutItem
{
public:
	PrintoutItemContainer( const PD_ITEM_TYPE &type )
		: PrintoutItem( type )
		{
			ratio = 0;
		}

	~PrintoutItemContainer()
	{
		for( std::list<PrintoutItem*>::iterator iter = children.begin(); iter != children.end(); iter++ ) { delete *iter; }
	}

	std::list<PrintoutItem*> children;
	double ratio;
};

class PrintoutItemColumnSet  : public PrintoutItemContainer
{
public:
	PrintoutItemColumnSet() : PrintoutItemContainer( PD_COLUMN_SET ) {}
};

class PrintoutItemRowSet  : public PrintoutItemContainer
{
public:
	PrintoutItemRowSet() : PrintoutItemContainer( PD_ROW_SET ) {}
};

class PrintoutItemGrid  : public PrintoutItemContainer
{
public:
	PrintoutItemGrid( const int &cols )
		: PrintoutItemContainer( PD_GRID ),
		nb_cols( cols )
		{}

	const int nb_cols;
};

class PrintoutItemDasaSummary  : public PrintoutItem
{
public:
	PrintoutItemDasaSummary( const DasaId &dasaId, const int &tableType )
		: PrintoutItem( PD_DASA_SUMMARY ),
		dasaId( dasaId ),
		tableType( tableType )
		{}

	const DasaId dasaId;
	const int tableType;
};

class PrintoutItemError  : public PrintoutItem
{
public:
	PrintoutItemError( wxString message )
		: PrintoutItem( PD_ERROR ),
		message( message )
		{}

	wxString message;
};

/*************************************************//**
*
* 
*
******************************************************/
class PrintoutConfig : public ConfigBase
{
public:

	PrintoutConfig( wxString name = wxEmptyString );
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	std::vector<PrintoutItem*> items;
	wxString name;
	wxString description;
	bool vedic;
};

/*************************************************//**
*
* \brief reads and contains configs for printouts
*
******************************************************/
class PrintoutConfigLoader : public ConfigListLoader<PrintoutConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( PrintoutConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );

private:

	PrintoutConfigLoader();

	PrintoutItem *loadItem( wxJSONValue& );
	void loadContainer( wxJSONValue&, PrintoutItemContainer* );

	std::map<wxString, PD_ITEM_TYPE> pd_map;

};

#endif

