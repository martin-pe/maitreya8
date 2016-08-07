///////////////////////////////////////////////////////////////////////////////
// Name:        pdfdc.cpp
// Purpose:     Implementation of wxPdfDC
// Author:      Ulrich Telle
// Modified by:
// Created:     2010-11-28
// Copyright:   (c) Ulrich Telle
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/// \file pdfdc.cpp Implementation of the wxPdfDC graphics primitives

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#if wxCHECK_VERSION(2,9,0)
// Implementation of wxPdfDC for wxWidgets 2.9.x (and above)
#include "pdfdc29.inc"
#else
// Implementation of wxPdfDC for wxWidgets 2.8.x
#include "pdfdc28.inc"
#endif
