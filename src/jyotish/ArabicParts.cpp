/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/ArabicParts.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "ArabicParts.h"

#include "astrobase.h"
#include "maitreya.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Sheet.h"
#include "Table.h"
#include "VedicPlanet.h"
#include "Varga.h"

#include <assert.h>
#include <wx/log.h>
#include <wx/string.h>

/**********************************************************
***
*
* Arabic parts calculation parameters.
* "Planets" may be Ascendant/Meridian or Nodes.
*
* Each parameter must have a reference object
* (planet, house, other Arabic part or fixed longitude).
*
* Attention: lords are different in Vedic and western
* astrology (Uranus, Neptune and Pluto for Aq, Pi and Sc)
*
***
***********************************************************/

enum
{
	// longitude of a planet (0..MAX_EPHEM_OBJECTS)
	AR_PLANET_LEN = 0,

	// longitude of other arabic part (only Fortune and Spirit used)
	AR_ARABIC_LEN,

	// fixed zodiacal longitude (0..360) : ! NO AYANAMSA APPLIED!
	AR_FIXED_LEN,

	// longitude of house cusp (0..11) = h
	AR_HOUSECUSP_LEN,

	// like above plus 10 deg (0..11) = j
	AR_HOUSECUSP_LEN_10,

	// longitude of lord of house cusp (0..11) = r
	AR_HOUSECUSP_LORD_LEN,

	// longitude of Dispistor (0..MAX_EPHEM_OBJECTS) = D
	AR_PLANETS_DISPOSITOR_LEN,

	// planet's house cusp (like AR_HOUSECUSP_LEN, but reference to planet instead of house number), (0..MAX_EPHEM_OBJECTS) = H
	AR_PLANETS_HOUSECUSP_LEN,

	// longitude of lord of a planet's house cusp (0..MAX_EPHEM_OBJECTS) = R
	AR_PLANETS_HOUSECUSP_LORD_LEN
};

/*****************************************************
**
**   ArabicPartsExpert   ---   Constructor
**
******************************************************/
ArabicPartsExpert::ArabicPartsExpert( Horoscope *horoscope )
		: h ( horoscope )
{
	for ( int i = 0; i < MAX_ARABIC_OBJECTS; i++ ) planets[i] = new VedicPlanet( h, (ObjectId)(OFORTUNE + i ));
	vayanamsa = wayanamsa = 0;
	dirty = true;
}

/*****************************************************
**
**   ArabicPartsExpert   ---   Destructor
**
******************************************************/
ArabicPartsExpert::~ArabicPartsExpert()
{
	for ( int i = 0; i < MAX_ARABIC_OBJECTS; i++ ) delete planets[i];
}

/*****************************************************
**
**   ArabicPartsExpert   ---   getPart
**
******************************************************/
ArabicPartDefinition ArabicPartsExpert::getPart( const int &i )
{
	assert( i >= 0 && i < MAX_ARABIC_OBJECTS );
	const static ArabicPartDefinition parts[MAX_ARABIC_OBJECTS] =
	{
		// from Astrolog
		// start of generated code
		{ _( "Fortune"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMOON}, true },
		{ _( "Spirit"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OSUN}, true },
		{ _( "Victory"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE+1, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Valor and Bravery"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_ARABIC_LEN, OFORTUNE}, true },
		{ _( "Mind and Administrators"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMARS}, true },
		{ _( "Property and Goods"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LORD_LEN, HOUSE2, AR_HOUSECUSP_LEN, HOUSE2}, true },
		{ _( "Siblings"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Death of Siblings"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_HOUSECUSP_LEN_10, HOUSE3}, true },
		{ _( "Death of Parents"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Grandparents"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LEN, HOUSE2, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Real Estate"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Children and Life"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Expected Birth"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANETS_HOUSECUSP_LORD_LEN, OMOON}, false },
		{ _( "Disease and Defects (1)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMARS}, false },
		{ _( "Disease and Defects (2)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, false },
		{ _( "Captivity"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANETS_HOUSECUSP_LORD_LEN, OSATURN}, false },
		{ _( "Servants"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMOON}, false },
		{ _( "Partners"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_HOUSECUSP_LEN, HOUSE7}, false },
		{ _( "Death"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_HOUSECUSP_LEN, HOUSE8}, false },
		{ _( "Sickness and Murder"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, true },
		{ _( "Danger, Violence and Debt"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Journeys"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LORD_LEN, HOUSE9, AR_HOUSECUSP_LEN, HOUSE9}, false },
		{ _( "Travel by Water"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_FIXED_LEN, 105}, true },
		{ _( "Faith, Trust and Belief"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Deep Reflection"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMOON}, true },
		{ _( "Understanding and Wisdom"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OSUN}, true },
		{ _( "Fame and Recognition"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Rulers and Disassociation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OMOON}, true },
		{ _( "Father, Fate and Karma"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Sudden Advancement"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_ARABIC_LEN, OFORTUNE}, true },
		{ _( "Celebrity of Rank"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OSUN}, false },
		{ _( "Surgery and Accident"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Merchants and Their Work"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Merchandise (Exchange)"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE+1, AR_ARABIC_LEN, OFORTUNE}, true },
		{ _( "Mother"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OMOON}, true },
		{ _( "Glory and Constancy"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_ARABIC_LEN, OFORTUNE+1}, true },
		{ _( "Honorable Acquaintances"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_PLANET_LEN, OSUN}, true },
		{ _( "Success"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Worldliness"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Acquaintances"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Violence"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE+1, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Liberty of Person"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSUN}, true },
		{ _( "Praise and Acceptance"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Enmity"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LORD_LEN, HOUSE12, AR_HOUSECUSP_LEN, HOUSE12}, false },
		{ _( "Bad Luck"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE+1, AR_ARABIC_LEN, OFORTUNE}, false },
		{ _( "Debilitated Bodies"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_PLANET_LEN, OMARS}, true },
		{ _( "Boldness and Violence"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANETS_DISPOSITOR_LEN, OASCENDANT, AR_PLANET_LEN, OMOON}, true },
		{ _( "Trickery and Deceit"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_ARABIC_LEN, OFORTUNE+1}, true },
		{ _( "Necessities"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_HOUSECUSP_LEN, HOUSE3}, false },
		{ _( "Realization of Needs"  ), { AR_PLANET_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Retribution"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OSUN}, true },
		{ _( "Children (Male)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Children (Female)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Play and Variety"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OMARS}, false },
		{ _( "Stability"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Speculation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_HOUSECUSP_LEN, HOUSE5}, true },
		{ _( "Art"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Sexual Attraction"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LORD_LEN, HOUSE5, AR_HOUSECUSP_LEN, HOUSE5}, false },
		{ _( "Sex Drive and Stimulation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_FIXED_LEN, 10}, false },
		{ _( "Passion"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMARS}, false },
		{ _( "Emotion and Affection"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OMARS}, false },
		{ _( "Most Perilous Year"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_HOUSECUSP_LORD_LEN, HOUSE8}, false },
		{ _( "Peril"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_HOUSECUSP_LEN, HOUSE8}, false },
		{ _( "Occultism"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Commerce"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Marriage Contracts"  ), { AR_HOUSECUSP_LEN, HOUSE9, AR_PLANET_LEN, OVENUS, AR_HOUSECUSP_LEN, HOUSE3}, false },
		{ _( "Travel by Land"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LORD_LEN, HOUSE9, AR_HOUSECUSP_LEN, HOUSE9}, false },
		{ _( "Travel by Air"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANETS_HOUSECUSP_LEN, OURANUS, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Destiny"  ), { AR_PLANET_LEN, OMERIDIAN, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OSUN}, true },
		{ _( "Vocation and Status"  ), { AR_PLANET_LEN, OMERIDIAN, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMOON}, true },
		{ _( "Honor, Nobility (Day)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_FIXED_LEN, 19}, false },
		{ _( "Honor, Nobility (Night)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_FIXED_LEN, 33}, false },
		{ _( "Organization"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_FIXED_LEN, 10}, false },
		{ _( "Divorce"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LEN, HOUSE7, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Ostracism and Loss"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Friends"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OMOON}, true },
		{ _( "Tragedy and Brethren"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Race (Consciousness)"  ), { AR_PLANET_LEN, OASCENDANT, AR_FIXED_LEN, 10, AR_PLANET_LEN, OMOON}, true },
		{ _( "Bondage and Slavery"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANETS_DISPOSITOR_LEN, HOUSE2, AR_PLANET_LEN, OMOON}, true },
		{ _( "Imprisonment and Sorrow"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_ARABIC_LEN, OFORTUNE}, false },
		{ _( "Perversion"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Self-Undoing"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_HOUSECUSP_LEN, HOUSE12}, false },
		{ _( "Treachery and Entrapment"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Bereavement"  ), { AR_HOUSECUSP_LEN, HOUSE12, AR_PLANET_LEN, ONEPTUNE, AR_HOUSECUSP_LORD_LEN, HOUSE12}, false },
		{ _( "Suicide (Yang)"  ), { AR_PLANET_LEN, OASCENDANT, AR_HOUSECUSP_LEN, HOUSE12, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Suicide (Yin)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_HOUSECUSP_LEN, HOUSE8}, false },
		{ _( "Depression"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Assassination (Yang)"  ), { AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Assassination (Yin)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_HOUSECUSP_LORD_LEN, HOUSE12}, false },
		{ _( "Cancer (Disease)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Catastrophe"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Foolhardiness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Release and Luck"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Benevolence and Assurance"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Hope and Sensitivity"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Aptness and Aloofness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Charm and Personality"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Faith and Good Manners"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMOON}, true },
		{ _( "Temperament"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSUN}, false },
		{ _( "Security and Treasure"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Originality"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Eccentricity, Astrology"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Divination"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Intrusion"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Negotiation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OMARS}, false },
		{ _( "Discord and Controversy"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Coincidence"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OMARS}, false },
		{ _( "Unpreparedness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Popularity"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OMARS}, false },
		{ _( "Misunderstanding"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Sentiment and Marriage"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Loneliness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Success in Investment"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Frugality and Labor"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Wastefulness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Vanity"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Corruptness"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Initiative"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OSUN}, false },
		{ _( "Memory"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMARS}, true },
		{ _( "Love, Beauty and Peace"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Disinterest and Boredom"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OSUN}, false },
		{ _( "Accomplishment"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OSUN}, false },
		{ _( "Influence"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Increase and Impression"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Caution"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, ONEPTUNE}, false },
		{ _( "Timidity"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Entertainment"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OURANUS}, false },
		{ _( "Bequest"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Genius"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OSUN}, false },
		{ _( "Revelation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, ONEPTUNE, AR_PLANET_LEN, OMOON}, true },
		{ _( "Delusion"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, ONEPTUNE}, true },
		{ _( "Misinterpretation"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OURANUS}, true },
		{ _( "Intellectuality"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OURANUS, AR_PLANET_LEN, OSUN}, false },
		{ _( "Earth"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Water"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OVENUS}, false },
		{ _( "Air and Wind"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_HOUSECUSP_LORD_LEN, HOUSE4}, false },
		{ _( "Fire"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMARS}, false },
		{ _( "Clouds"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Rains"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Cold"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Wheat"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Barley and Meats"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Rice and Millet"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Corn"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Lentils, Iron, Pungents"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Beans and Onions"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, true },
		{ _( "Chick Peas"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSUN}, true },
		{ _( "Sesame and Grapes"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Sugar and Legumes"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Honey"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OSUN}, true },
		{ _( "Oils"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OMOON}, true },
		{ _( "Nuts and Flax"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Olives"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMOON}, true },
		{ _( "Apricots and Peaches"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, true },
		{ _( "Melons"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Salt"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMARS}, true },
		{ _( "Sweets"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMERCURY}, true },
		{ _( "Astrigents and Purgatives"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OSATURN}, true },
		{ _( "Silk and Cotton"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OVENUS}, true },
		{ _( "Purgatives (Bitter)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, true },
		{ _( "Purgatives (Acid)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OJUPITER}, true },
		{ _( "Secrets"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANETS_DISPOSITOR_LEN, OASCENDANT, AR_PLANET_LEN, OMERIDIAN}, false },
		{ _( "Information True/False"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OMOON}, true },
		{ _( "Injury to Business"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANETS_DISPOSITOR_LEN, OASCENDANT, AR_ARABIC_LEN, OFORTUNE}, true },
		{ _( "Freedmen and Servants"  ), { AR_PLANET_LEN, OMERCURY, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Employers"  ), { AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OJUPITER, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Marriage"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OVENUS, AR_HOUSECUSP_LEN, HOUSE7}, false },
		{ _( "Time for Action/Success"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Time Occupied in Action"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Dismissal and Resignation"  ), { AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OJUPITER}, false },
		{ _( "Life/Death of Absentee"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMARS}, false },
		{ _( "Lost Animal (Light)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSUN, AR_PLANET_LEN, OMARS}, false },
		{ _( "Lost Animal (Dark)"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OSATURN, AR_PLANET_LEN, OMARS}, false },
		{ _( "Lawsuit"  ), { AR_PLANET_LEN, OASCENDANT, AR_PLANET_LEN, OMARS, AR_PLANET_LEN, OMERCURY}, false },
		{ _( "Decapitation"  ), { AR_HOUSECUSP_LEN, HOUSE8, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OMARS}, false },
		{ _( "Torture"  ), { AR_PLANET_LEN, OMERIDIAN, AR_PLANET_LEN, OMOON, AR_PLANET_LEN, OSATURN}, false },
		{ _( "Lost Objects"  ), { AR_PLANET_LEN, OMOON, AR_PLANETS_DISPOSITOR_LEN, OASCENDANT, AR_HOUSECUSP_LEN, HOUSE4}, false }
		// end of generated code
	};
	return parts[i];
}

/*****************************************************
**
**   ArabicPartsExpert   ---   updatePart
**
******************************************************/
void ArabicPartsExpert::updatePart( const int &p )
{
	assert( p >= 0 && p < MAX_ARABIC_OBJECTS );
	int type;
	int obj;
	double vlen = 0, wlen = 0;
	ObjectId lord;
	Rasi rasi;
	double len;

	// daybirth in Horoscope.cpp is relative to sunrise, this is house position
	bool daybirth = ( h->getHouse( OSUN, false ) >= HOUSE7 );

	for ( int j = 0; j < 3; j++ )
	{
		type = getPart( p ).part[ 2 * j ];
		obj = getPart( p ).part[ 2 * j + 1 ];

		// longitude of element j
		switch ( type )
		{
			// longitude of a planet (0..MAX_EPHEM_OBJECTS)
		case AR_PLANET_LEN:
			ASSERT_VALID_EPHEM_OBJECT( (ObjectId)obj )
			vlen = h->getVedicLongitude( (ObjectId)obj );
			wlen = h->getWesternLongitude( (ObjectId)obj );
			break;

			// longitude of other arabic part (only Fortune and Spirit used)
		case AR_ARABIC_LEN:
			// TODO: was ist mit Ayanamsa ???
			ASSERT_VALID_ARABIC_OBJECT( obj );
			vlen = h->getVedicLongitude( (ObjectId)obj );
			wlen = h->getWesternLongitude( (ObjectId)obj );
			//vlen = varabic_len[obj - OFORTUNE];
			//wlen = warabic_len[obj - OFORTUNE];
			break;

			// fixed zodiacal longitude (0..360) : ! NO AYANAMSA APPLIED!
		case AR_FIXED_LEN:
			ASSERT_VALID_DEGREE( obj )
			vlen = wlen = (double)obj;
			break;

			// longitude of house cusp (0..11) = h
		case AR_HOUSECUSP_LEN:
			ASSERT_VALID_HOUSE( obj )
			vlen = h->getHouse( obj, true );
			wlen = h->getHouse( obj, false );
			break;

			// like above plus 10 deg (0..11) = j
		case AR_HOUSECUSP_LEN_10:
			ASSERT_VALID_HOUSE( obj )
			vlen = h->getHouse( obj, true ) + 10;
			wlen = h->getHouse( obj, false ) + 10;
			break;

			// longitude of lord of house cusp (0..11) = r
		case AR_HOUSECUSP_LORD_LEN:
			ASSERT_VALID_HOUSE( obj )
			vlen = h->getObjectPosition( getLord( getRasi( h->getHouse( obj, true ))), true ).longitude;
			wlen = h->getObjectPosition( getLord( getRasi( h->getHouse( obj, false ))), false ).longitude;
			break;

			// longitude of Dispositor (0..MAX_EPHEM_OBJECTS) = D
		case AR_PLANETS_DISPOSITOR_LEN:
			ASSERT_VALID_EPHEM_OBJECT( obj )

			len = h->getVedicLongitude( (ObjectId)obj );
			rasi = getRasi( len );
			lord = getLord( rasi, false );
			vlen = h->getVedicLongitude( (ObjectId)lord );

			len = h->getWesternLongitude( (ObjectId)obj );
			rasi = getRasi( len );
			lord = getLord( rasi, false );
			wlen = h->getWesternLongitude( (ObjectId)lord );
			//printf( "Dispositor: Planet %d len %f, rasi %d lord %d wlen %f\n", obj, len, rasi, lord, wlen );
			break;

			// planet's house cusp (like AR_HOUSECUSP_LEN, but reference to planet instead of house number), (0..MAX_EPHEM_OBJECTS) = H
		case AR_PLANETS_HOUSECUSP_LEN:
			ASSERT_VALID_EPHEM_OBJECT( obj )
			vlen = h->getHouse( (int)h->getHouse4Longitude( h->getObjectPosition( (ObjectId)obj, true ).longitude, true ), true );
			wlen = h->getHouse( (int)h->getHouse4Longitude( h->getObjectPosition( (ObjectId)obj, false ).longitude, false ), false );
			break;

			// longitude of lord of a planet's house cusp (0..MAX_EPHEM_OBJECTS) = R
		case AR_PLANETS_HOUSECUSP_LORD_LEN:
			ASSERT_VALID_EPHEM_OBJECT( obj )
			printf( "HALLO obj %d\n", (int)obj );
			vlen = h->getObjectPosition( getLord( getRasi( h->getHouse( (int)h->getHouse4Longitude(
				h->getObjectPosition( (ObjectId)obj, true ).longitude, true ), true )), true ), true ).longitude;
			wlen = h->getObjectPosition( getLord( getRasi( h->getHouse( (int)h->getHouse4Longitude(
				h->getObjectPosition( (ObjectId)obj, false ).longitude, false ), false )), false ), false ).longitude;
			break;
		default:
			assert( false );
			break;
		}
		//printf( "Type %d obj %d vlen %f wlen %f\n", type, obj, vlen, wlen );

		// longitude of part
		switch ( j )
		{
		case 0:
			vpos[p].longitude = vlen;
			wpos[p].longitude = wlen;
			break;
		case 1:
			if ( getPart( p ).flip && ! daybirth )
			{
				vpos[p].longitude += vlen;
				wpos[p].longitude += wlen;
			}
			else
			{
				vpos[p].longitude -= vlen;
				wpos[p].longitude -= wlen;
			}
			break;
		case 2:
			if ( getPart( p ).flip && ! daybirth )
			{
				vpos[p].longitude -= vlen;
				wpos[p].longitude -= wlen;
			}
			else
			{
				vpos[p].longitude += vlen;
				wpos[p].longitude += wlen;
			}
			break;
		default:
			assert( false );
			break;
		}
	}
	vpos[p].longitude = red_deg( vpos[p].longitude );
	wpos[p].longitude = red_deg( wpos[p].longitude );
}

/*****************************************************
**
**   ArabicPartsExpert   ---   getObjectPosition
**
******************************************************/
ObjectPosition ArabicPartsExpert::getObjectPosition( const int &p, const bool vedic )
{
	ASSERT_VALID_ARABIC_OBJECT( p )
	if ( dirty ) updateAll();
	return ( vedic ? vpos[p - FIRST_ARABIC_OBJECT ] : wpos[p - FIRST_ARABIC_OBJECT ] );
}

/*****************************************************
**
**   ArabicPartsExpert   ---   getVargaData
**
******************************************************/
VargaData *ArabicPartsExpert::getVargaData( const int &planet, const Varga &varga )
{
	ASSERT_VALID_ARABIC_OBJECT( planet )
	ASSERT_VALID_VARGA( varga )
	if ( dirty ) updateAll();
	return planets[planet - FIRST_ARABIC_OBJECT]->getVargaData( varga );
}

/**************************************************************
***
**   ArabicPartsExpert   ---   getPlanet
***
***************************************************************/
VedicPlanet *ArabicPartsExpert::getPlanet( const int &planet )
{
	ASSERT_VALID_ARABIC_OBJECT( planet )
	if ( dirty ) updateAll();
	return planets[planet - FIRST_ARABIC_OBJECT];
}

/*****************************************************
**
**   ArabicPartsExpert   ---   getObjectName
**
******************************************************/
wxString ArabicPartsExpert::getObjectName( const int &p, const int &format )
{
	// static method
	ASSERT_VALID_ARABIC_OBJECT( p )
	switch ( format )
	{
	case TF_SHORT:
		return getPart( p - FIRST_ARABIC_OBJECT ).name.Left( 2 );
		break;
	case TF_MEDIUM:
		return getPart( p - FIRST_ARABIC_OBJECT ).name.Left( 4 );
		break;
	}
	return getPart( p - FIRST_ARABIC_OBJECT ).name;
}

/*****************************************************
**
**   ArabicPartsExpert   ---   update
**
******************************************************/
void ArabicPartsExpert::update()
{
	dirty = true;
}

/*****************************************************
**
**   ArabicPartsExpert   ---   updateAll
**
******************************************************/
void ArabicPartsExpert::updateAll()
{
	dirty = false;
	vayanamsa = h->getAyanamsa( true );
	wayanamsa = h->getAyanamsa( false );
	for ( int i = 0; i < MAX_ARABIC_OBJECTS; i++ )
	{
		updatePart( i );
		planets[i]->update( vpos[i].longitude );
	}
}

/*****************************************************
**
**   ArabicPartsExpert   ---   getItemDescription
**
******************************************************/
wxString ArabicPartsExpert::getItemDescription( Sheet* /*sheet*/, const int &type, const ObjectId &obj )
{
	wxString s;
	Lang lang;
	SheetFormatter fmt;

	switch ( type )
	{
	case AR_PLANET_LEN:
	case AR_ARABIC_LEN:
		s = fmt.fragment2PlainText( fmt.getObjectName( obj, TF_MEDIUM ));
		break;
	case AR_FIXED_LEN:
		s << (int)getRasiLen( obj ) << wxT( " " ) << lang.getSignName( getRasi( obj ), TF_MEDIUM );
		break;
	case AR_HOUSECUSP_LEN:
		s.Printf( _( "Cusp %d" ), obj+1 );
		break;
	case AR_HOUSECUSP_LEN_10:
		s.Printf( _( "Cusp %d + 10" ), obj+1 );
		break;
	case AR_HOUSECUSP_LORD_LEN:
		s.Printf( _( "Lord of cusp %d" ), obj+1 );
		break;
	case AR_PLANETS_DISPOSITOR_LEN:
		s << _( "Dispositor" ) << wxT( " " ) << lang.getObjectName( obj, TF_MEDIUM );
		break;
	case AR_PLANETS_HOUSECUSP_LEN:
		s << _( "House cusp" ) << wxT( " " ) << lang.getObjectName( obj, TF_MEDIUM );
		break;
	case AR_PLANETS_HOUSECUSP_LORD_LEN:
		s << _( "Lord of cusp" ) << wxT( " " ) << lang.getObjectName( obj, TF_MEDIUM );
		break;
	default:
		assert( false );
		break;
	}
	return s;
}

/*****************************************************
**
**   ArabicPartsExpert   ---   write
**
******************************************************/
void ArabicPartsExpert::write( Sheet *sheet, const bool &vedic, const bool show_header )
{
	wxString s;
	double len;
	SheetFormatter fmt;

	if ( dirty ) updateAll();
	//s << _( "Mode" ) << wxT( ": " ) << ( vedic ? _( "Vedic" ) : _( "Western" ));
	sheet->addLine( s );

	Table *table = new Table( 6, MAX_ARABIC_OBJECTS + 1 );
	if ( show_header ) table->setHeader(  wxT( "Arabic Parts" ));
	table->setHeader( 0, _( "No." ));
	table->setHeader( 1, _( "Name" ));
	table->setHeader( 2, _( "Longitude" ));
	table->setHeader( 3, _( "House" ));
	table->setHeader( 4, _( "Rule" ));
	table->setHeader( 5, _( "Flip" ));

	for ( int i = 0; i < MAX_ARABIC_OBJECTS; i++ )
	{
		s.Printf( wxT( "%d" ), i + 1 );
		table->setEntry( 0, i+1, s );
		table->setEntry( 1, i+1, getPart( i ).name );

		len = vedic ? vpos[i].longitude : wpos[i].longitude;
		//printf( "ARABIC %d : v %f w %f\n", i, varabic_len[i], warabic_len[i] );

		table->setEntry( 2, i+1, fmt.getPosFormatted( len ));
		s.Printf( wxT( "%d" ), (int)h->getHouse4Longitude( len, vedic ) + 1 );
		table->setEntry( 3, i+1, s );

		ArabicPartDefinition def = getPart( i );
		s.Clear();
		s << getItemDescription( sheet, def.part[0], (ObjectId)def.part[1] )
		<< wxT( " - " )
		<< getItemDescription( sheet, def.part[2], (ObjectId)def.part[3] )
		<< wxT( " + " )
		<< getItemDescription( sheet, def.part[4], (ObjectId)def.part[5] );
		table->setEntry( 4, i + 1, s );

		if ( def.flip ) table->setEntry( 5, i + 1, wxT( "x" ));
	}
	sheet->addItem( table );
}

