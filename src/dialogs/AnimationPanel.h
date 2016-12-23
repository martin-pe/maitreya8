/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AnimationPanel.h
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

#ifndef ANIMATIONPANEL_H
#define ANIMATIONPANEL_H

#include "ConfigPanel.h"

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class AnimationConfig;
class ViewConfig;

/*************************************************//**
*
* \brief ConfigPanel for animation parameters
*
******************************************************/
class AnimationPanel : public ConfigPanel
{
	DECLARE_CLASS( AnimationPanel )

public:
    // begin wxGlade: AnimationPanel::ids
    // end wxGlade

	AnimationPanel( wxWindow* parent );
	~AnimationPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: AnimationPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	virtual void updateUi();
	void OnAnimationModeChoice( wxCommandEvent& );

	AnimationConfig *animation;

protected:
    // begin wxGlade: AnimationPanel::attributes
    wxStaticBox* sizer_animate_staticbox;
    wxStaticText* label_animation_freq;
    wxSpinCtrl* spin_animation_freq;
    wxStaticText* label_animation_freq2;
    wxStaticText* label_animation_mode;
    wxChoice* choice_animation_mode;
    wxStaticText* label_animation_step;
    wxSpinCtrl* spin_animation_steplength;
    wxChoice* choice_animate_dimension;
    // end wxGlade
}; // wxGlade: end class


#endif // ANIMATIONPANEL_H

