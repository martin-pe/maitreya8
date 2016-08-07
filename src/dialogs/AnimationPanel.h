/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AnimationPanel.h
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

