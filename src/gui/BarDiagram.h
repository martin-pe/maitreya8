/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BarDiagram.h
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

#ifndef _BARDIAGRAM_H_
#define _BARDIAGRAM_H_

#include "BasicWidget.h"
#include "Sheet.h"

#include <list>

using namespace std;

class AshtakavargaExpert;
class BarDiagramConfig;
class BarDiagramStyle;
class ChartProperties;
class Horoscope;

/*************************************************//**
*
*
*
******************************************************/
class BarDiagramWriter
{
public:
	BarDiagramWriter( Sheet*, ChartProperties*, Horoscope* );

	void writeVargaDiagrams();

	void writeAvRasiDiagrams( AshtakavargaExpert* );
	void writeAvPindaDiagrams( AshtakavargaExpert* );

private:
	Sheet *sheet;
	ChartProperties *props;
	Horoscope *horoscope;
};

/*************************************************//**
*
* \brief information about a single item in BarDiagram
*
******************************************************/
class BarDiagramItem : public SheetItem
{
  DECLARE_CLASS( BarDiagramItem )

public:
	BarDiagramItem( const int id, const double v, BarDiagramConfig* = (BarDiagramConfig*)NULL );

	virtual SheetItem *cloneClean();

	virtual MRect getRefreshRect( const bool ismarked = false, const int selectionMode = 0 );

	/**
	 * \brief id of the object (planet or sign)
	 */
	int objectId;

	/**
	 * \brief value of the object (planet or sign) in diagram
	 */
	double value;

private:

	BarDiagramConfig *barconfig;
};

#define BARDIAGRAM_XMIN 300
#define BARDIAGRAM_XMAX 600
#define BARDIAGRAM_RATIO .7

/*************************************************//**
*
* 
*
******************************************************/
class BarDiagramWidget :  public SheetWidgetItem
{
  DECLARE_CLASS( BarDiagramWidget )

public:
  BarDiagramWidget(
		ChartProperties*,
		wxString title,
		const bool planetmode = true,
		const double xmin = BARDIAGRAM_XMIN,
		const double xmax = BARDIAGRAM_XMAX,
		const double ratio = BARDIAGRAM_RATIO,
		BarDiagramConfig *barconfig = (BarDiagramConfig*)NULL
	);
  ~BarDiagramWidget();

  virtual SheetItem *cloneClean();

	void clear();
	void addItem( BarDiagramItem* );

	virtual SheetItem *getSubitem4Point( const wxPoint& );

	virtual void doPaint( Painter *painter, const MRect &refreshRect );

	virtual bool mouseHasMoved( const wxPoint&, const bool &outside );

protected:

	void drawBar( Painter *painter, BarDiagramItem* );

	const wxString title;

	list<BarDiagramItem*> subitems;
	double maxvalue, minvalue;
	double text_height;

	const bool planetmode;
	const int deltasteps;
  const double xmargin;
	const double xdmargin;
	const double ymargin;
	const double ydmargin;
	BarDiagramConfig *barconfig;
};

#endif

