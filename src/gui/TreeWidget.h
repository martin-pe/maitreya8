/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/TreeWidget.h
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
/*
	 This is a hacked version of wxWindows generic tree control.
	 This version adds the functionality of hiding items.
*/

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/treectlg.h
// Purpose:     wxTreeCtrl class
// Author:      Robert Roebling
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id: treectlg.h,v 1.20.2.3 2002/12/29 07:48:10 RL Exp $
// Copyright:   (c) 1997,1998 Robert Roebling
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


#ifndef _MY_TREECTRL_H_
#define _MY_TREECTRL_H_

#include <wx/treebase.h>
#include "wx/scrolwin.h"
#include "wx/pen.h"
#include "wx/imaglist.h"

#include "Commands.h"

// -----------------------------------------------------------------------------
// forward declaration
// -----------------------------------------------------------------------------

class MyGenericTreeItem;

class TreeItemClientData;

class MyTreeRenameTimer;
class wxTreeFindTimer;
class MyTreeTextCtrl;
class wxTextCtrl;
class MyGenericTreeCtrl;

// wx 3.2 has no wxTreeItemAttr
#if wxCHECK_VERSION( 3, 2, 0 )
class WXDLLIMPEXP_CORE wxTreeItemAttr
{
public:
    // ctors
    wxTreeItemAttr() { }
    wxTreeItemAttr(const wxColour& colText,
                   const wxColour& colBack,
                   const wxFont& font)
        : m_colText(colText), m_colBack(colBack), m_font(font) { }

    // setters
    void SetTextColour(const wxColour& colText) { m_colText = colText; }
    void SetBackgroundColour(const wxColour& colBack) { m_colBack = colBack; }
    void SetFont(const wxFont& font) { m_font = font; }

    // accessors
    bool HasTextColour() const { return m_colText.IsOk(); }
    bool HasBackgroundColour() const { return m_colBack.IsOk(); }
    bool HasFont() const { return m_font.IsOk(); }

    const wxColour& GetTextColour() const { return m_colText; }
    const wxColour& GetBackgroundColour() const { return m_colBack; }
    const wxFont& GetFont() const { return m_font; }

private:
    wxColour m_colText,
             m_colBack;
    wxFont   m_font;
};
#endif


class TreeItemClientData: public wxClientData
{
public:

	TreeItemClientData() {}

	const wxTreeItemId& GetId() const { return m_pItem; }
	void SetId(const wxTreeItemId& id) { m_pItem = id; }

protected:
	wxTreeItemId m_pItem;
};

class MyTreeEvent : public wxNotifyEvent
{
public:
	MyTreeEvent(wxEventType commandType = wxEVT_NULL, int id = 0);

	// accessors
	// get the item on which the operation was performed or the newly
	// selected item for wxEVT_COMMAND_TREE_SEL_CHANGED/ING events
	wxTreeItemId GetItem() const {
		return m_item;
	}
	void SetItem(const wxTreeItemId& item) {
		m_item = item;
	}

	// for wxEVT_COMMAND_TREE_SEL_CHANGED/ING events, get the previously
	// selected item
	wxTreeItemId GetOldItem() const {
		return m_itemOld;
	}
	void SetOldItem(const wxTreeItemId& item) {
		m_itemOld = item;
	}

	// the point where the mouse was when the drag operation started (for
	// wxEVT_COMMAND_TREE_BEGIN_(R)DRAG events only) or click position
	wxPoint GetPoint() const {
		return m_pointDrag;
	}
	void SetPoint(const wxPoint& pt) {
		m_pointDrag = pt;
	}

	// keyboard data (for wxEVT_COMMAND_TREE_KEY_DOWN only)
	const wxKeyEvent& GetKeyEvent() const {
		return m_evtKey;
	}
	int GetKeyCode() const {
		return m_evtKey.GetKeyCode();
	}
	void SetKeyEvent(const wxKeyEvent& evt) {
		m_evtKey = evt;
	}

	// label (for EVT_TREE_{BEGIN|END}_LABEL_EDIT only)
	const wxString& GetLabel() const {
		return m_label;
	}
	void SetLabel(const wxString& label) {
		m_label = label;
	}

	// edit cancel flag (for EVT_TREE_{BEGIN|END}_LABEL_EDIT only)
	bool IsEditCancelled() const {
		return m_editCancelled;
	}
	void SetEditCanceled(bool editCancelled) {
		m_editCancelled = editCancelled;
	}

#if WXWIN_COMPATIBILITY_2_2
	// for compatibility only, don't use
	int GetCode() const {
		return m_evtKey.GetKeyCode();
	}
#endif // WXWIN_COMPATIBILITY_2_2

private:
	// not all of the members are used (or initialized) for all events
	wxKeyEvent    m_evtKey;
	wxTreeItemId  m_item,
	m_itemOld;
	wxPoint       m_pointDrag;
	wxString      m_label;
	bool          m_editCancelled;

	friend class MyGenericTreeCtrl;

	DECLARE_DYNAMIC_CLASS(MyTreeEvent);
};


// -----------------------------------------------------------------------------
// MyGenericTreeCtrl - the tree control
// -----------------------------------------------------------------------------

class MyGenericTreeCtrl : public wxScrolledWindow
{
public:
	// creation
	// --------
	MyGenericTreeCtrl() {
		Init();
	}

	MyGenericTreeCtrl(wxWindow *parent, wxWindowID id = -1,
	                  const wxPoint& pos = wxDefaultPosition,
	                  const wxSize& size = wxDefaultSize,
	                  long style = wxTR_DEFAULT_STYLE,
	                  const wxValidator &validator = wxDefaultValidator,
	                  const wxString& name = wxTreeCtrlNameStr)
	{
		Init();
		Create(parent, id, pos, size, style, validator, name);
	}

	virtual ~MyGenericTreeCtrl();

	bool Create(wxWindow *parent, wxWindowID id = -1,
	            const wxPoint& pos = wxDefaultPosition,
	            const wxSize& size = wxDefaultSize,
	            long style = wxTR_DEFAULT_STYLE,
	            const wxValidator &validator = wxDefaultValidator,
	            const wxString& name = wxTreeCtrlNameStr);

	// accessors
	// ---------

	// get the total number of items in the control
	size_t GetCount() const;

	// indent is the number of pixels the children are indented relative to
	// the parents position. SetIndent() also redraws the control
	// immediately.
	uint GetIndent() const {
		return m_indent;
	}
	void SetIndent(uint indent);

	// spacing is the number of pixels between the start and the Text
	uint GetSpacing() const {
		return m_spacing;
	}
	void SetSpacing(uint spacing);

	// image list: these functions allow to associate an image list with
	// the control and retrieve it. Note that when assigned with
	// SetImageList, the control does _not_ delete
	// the associated image list when it's deleted in order to allow image
	// lists to be shared between different controls. If you use
	// AssignImageList, the control _does_ delete the image list.
	//
	// The normal image list is for the icons which correspond to the
	// normal tree item state (whether it is selected or not).
	// Additionally, the application might choose to show a state icon
	// which corresponds to an app-defined item state (for example,
	// checked/unchecked) which are taken from the state image list.
	wxImageList *GetImageList() const;
	wxImageList *GetStateImageList() const;
	wxImageList *GetButtonsImageList() const;

	void SetImageList(wxImageList *imageList);
	void SetStateImageList(wxImageList *imageList);
	void SetButtonsImageList(wxImageList *imageList);
	void AssignImageList(wxImageList *imageList);
	void AssignStateImageList(wxImageList *imageList);
	void AssignButtonsImageList(wxImageList *imageList);

	// Functions to work with tree ctrl items.

	// accessors
	// ---------

	// retrieve item's label
	wxString GetItemText(const wxTreeItemId& item) const;
	// get one of the images associated with the item (normal by default)
	int GetItemImage(const wxTreeItemId& item,
	                 wxTreeItemIcon which = wxTreeItemIcon_Normal) const;
	// get the data associated with the item
	TreeItemClientData *GetItemData(const wxTreeItemId& item) const;

	// get the item's text colour
	wxColour GetItemTextColour(const wxTreeItemId& item) const;

	// get the item's background colour
	wxColour GetItemBackgroundColour(const wxTreeItemId& item) const;

	// get the item's font
	wxFont GetItemFont(const wxTreeItemId& item) const;

	// modifiers
	// ---------

	// set item's label
	void SetItemText(const wxTreeItemId& item, const wxString& text);
	// get one of the images associated with the item (normal by default)
	void SetItemImage(const wxTreeItemId& item, int image,
	                  wxTreeItemIcon which = wxTreeItemIcon_Normal);
	// associate some data with the item
	void SetItemData(const wxTreeItemId& item, TreeItemClientData *data);

	// force appearance of [+] button near the item. This is useful to
	// allow the user to expand the items which don't have any children now
	// - but instead add them only when needed, thus minimizing memory
	// usage and loading time.
	void SetItemHasChildren(const wxTreeItemId& item, bool has = TRUE);

	// the item will be shown in bold
	void SetItemBold(const wxTreeItemId& item, bool bold = TRUE);
	void SetItemHidden(const wxTreeItemId& item, bool hidden = TRUE);

	// set the item's text colour
	void SetItemTextColour(const wxTreeItemId& item, const wxColour& col);

	// set the item's background colour
	void SetItemBackgroundColour(const wxTreeItemId& item, const wxColour& col);

	// set the item's font (should be of the same height for all items)
	void SetItemFont(const wxTreeItemId& item, const wxFont& font);

	// set the window font
	virtual bool SetFont( const wxFont &font );

	// set the styles.  No need to specify a GetWindowStyle here since
	// the base wxWindow member function will do it for us
	void SetWindowStyle(const long styles);

	// item status inquiries
	// ---------------------

	// is the item visible (it might be outside the view or not expanded)?
	bool IsVisible(const wxTreeItemId& item) const;
	// does the item has any children?
	bool HasChildren(const wxTreeItemId& item) const
	{
		return ItemHasChildren(item);
	}
	bool ItemHasChildren(const wxTreeItemId& item) const;
	// is the item expanded (only makes sense if HasChildren())?
	bool IsExpanded(const wxTreeItemId& item) const;
	// is this item currently selected (the same as has focus)?
	bool IsSelected(const wxTreeItemId& item) const;
	// is item text in bold font?
	bool IsBold(const wxTreeItemId& item) const;
	// does the layout include space for a button?

	// number of children
	// ------------------

	// if 'recursively' is FALSE, only immediate children count, otherwise
	// the returned number is the number of all items in this branch
	size_t GetChildrenCount(const wxTreeItemId& item, bool recursively = TRUE);

	// navigation
	// ----------

	// wxTreeItemId.IsOk() will return FALSE if there is no such item

	// get the root tree item
	wxTreeItemId GetRootItem() const {
		return m_anchor;
	}

	// get the item currently selected (may return NULL if no selection)
	wxTreeItemId GetSelection() const {
		return m_current;
	}

	// get the items currently selected, return the number of such item
	size_t GetSelections(wxArrayTreeItemIds&) const;

	// get the parent of this item (may return NULL if root)
	wxTreeItemId GetItemParent(const wxTreeItemId& item) const;

#if WXWIN_COMPATIBILITY_2_2
	// deprecated:  Use GetItemParent instead.
	wxTreeItemId GetParent(const wxTreeItemId& item) const
	{
		return GetItemParent( item );
	}

	// Expose the base class method hidden by the one above.
	wxWindow *GetParent() const {
		return wxScrolledWindow::GetParent();
	}
#endif  // WXWIN_COMPATIBILITY_2_2

	// for this enumeration function you must pass in a "cookie" parameter
	// which is opaque for the application but is necessary for the library
	// to make these functions reentrant (i.e. allow more than one
	// enumeration on one and the same object simultaneously). Of course,
	// the "cookie" passed to GetFirstChild() and GetNextChild() should be
	// the same!

	// get the first child of this item
	wxTreeItemId GetFirstChild(const wxTreeItemId& item, long& cookie) const;
	// get the next child
	wxTreeItemId GetNextChild(const wxTreeItemId& item, long& cookie) const;
	// get the last child of this item - this method doesn't use cookies
	wxTreeItemId GetLastChild(const wxTreeItemId& item) const;

	// get the next sibling of this item
	wxTreeItemId GetNextSibling(const wxTreeItemId& item) const;
	// get the previous sibling
	wxTreeItemId GetPrevSibling(const wxTreeItemId& item) const;

	// get first visible item
	wxTreeItemId GetFirstVisibleItem() const;
	// get the next visible item: item must be visible itself!
	// see IsVisible() and wxTreeCtrl::GetFirstVisibleItem()
	wxTreeItemId GetNextVisible(const wxTreeItemId& item) const;
	// get the previous visible item: item must be visible itself!
	wxTreeItemId GetPrevVisible(const wxTreeItemId& item) const;

	// Only for internal use right now, but should probably be public
	wxTreeItemId GetNext(const wxTreeItemId& item) const;

	// operations
	// ----------

	// add the root node to the tree
	wxTreeItemId AddRoot(const wxString& text,
	                     int image = -1, int selectedImage = -1,
	                     TreeItemClientData *data = NULL);

	// insert a new item in as the first child of the parent
	wxTreeItemId PrependItem(const wxTreeItemId& parent,
	                         const wxString& text,
	                         int image = -1, int selectedImage = -1,
	                         TreeItemClientData *data = NULL);

	// insert a new item after a given one
	wxTreeItemId InsertItem(const wxTreeItemId& parent,
	                        const wxTreeItemId& idPrevious,
	                        const wxString& text,
	                        int image = -1, int selectedImage = -1,
	                        TreeItemClientData *data = NULL);

	// insert a new item before the one with the given index
	wxTreeItemId InsertItem(const wxTreeItemId& parent,
	                        size_t index,
	                        const wxString& text,
	                        int image = -1, int selectedImage = -1,
	                        TreeItemClientData *data = NULL);

	// insert a new item in as the last child of the parent
	wxTreeItemId AppendItem(const wxTreeItemId& parent,
	                        const wxString& text,
	                        int image = -1, int selectedImage = -1,
	                        TreeItemClientData *data = NULL);

	// delete this item and associated data if any
	void Delete(const wxTreeItemId& item);
	// delete all children (but don't delete the item itself)
	// NB: this won't send wxEVT_COMMAND_TREE_ITEM_DELETED events
	void DeleteChildren(const wxTreeItemId& item);
	// delete all items from the tree
	// NB: this won't send wxEVT_COMMAND_TREE_ITEM_DELETED events
	void DeleteAllItems();

	// expand this item
	void Expand(const wxTreeItemId& item);
	// expand this item and all subitems recursively
	void ExpandAll(const wxTreeItemId& item);
	// collapse the item without removing its children
	void Collapse(const wxTreeItemId& item);
	// collapse the item and remove all children
	void CollapseAndReset(const wxTreeItemId& item);
	// toggles the current state
	void Toggle(const wxTreeItemId& item);

	// remove the selection from currently selected item (if any)
	void Unselect();
	void UnselectAll();
	// select this item
	void SelectItem(const wxTreeItemId& item, bool unselect_others=TRUE, bool extended_select=FALSE);
	// make sure this item is visible (expanding the parent item and/or
	// scrolling to this item if necessary)
	void EnsureVisible(const wxTreeItemId& item);
	// scroll to this item (but don't expand its parent)
	void ScrollTo(const wxTreeItemId& item);
	void AdjustMyScrollbars();

	// The first function is more portable (because easier to implement
	// on other platforms), but the second one returns some extra info.
	wxTreeItemId HitTest(const wxPoint& point)
	{
		int dummy;
		return HitTest(point, dummy);
	}
	wxTreeItemId HitTest(const wxPoint& point, int& flags);

	// get the bounding rectangle of the item (or of its label only)
	bool GetBoundingRect(const wxTreeItemId& item,
	                     wxRect& rect,
	                     bool textOnly = FALSE) const;

	// Start editing the item label: this (temporarily) replaces the item
	// with a one line edit control. The item will be selected if it hadn't
	// been before.
	void EditLabel( const wxTreeItemId& item ) {
		Edit( item );
	}
	void Edit( const wxTreeItemId& item );
	// returns a pointer to the text edit control if the item is being
	// edited, NULL otherwise (it's assumed that no more than one item may
	// be edited simultaneously)
	wxTextCtrl* GetEditControl() const;

	// sorting
	// this function is called to compare 2 items and should return -1, 0
	// or +1 if the first item is less than, equal to or greater than the
	// second one. The base class version performs alphabetic comparaison
	// of item labels (GetText)
	virtual int OnCompareItems(const wxTreeItemId& item1,
	                           const wxTreeItemId& item2);
	// sort the children of this item using OnCompareItems
	//
	// NB: this function is not reentrant and not MT-safe (FIXME)!
	void SortChildren(const wxTreeItemId& item);

	// deprecated functions: use Set/GetItemImage directly
	// get the selected item image
	int GetItemSelectedImage(const wxTreeItemId& item) const
	{
		return GetItemImage(item, wxTreeItemIcon_Selected);
	}
	// set the selected item image
	void SetItemSelectedImage(const wxTreeItemId& item, int image)
	{
		SetItemImage(item, image, wxTreeItemIcon_Selected);
	}

	// implementation only from now on

	// overridden base class virtuals
	virtual bool SetBackgroundColour(const wxColour& colour);
	virtual bool SetForegroundColour(const wxColour& colour);

	// callbacks
	void OnPaint( wxPaintEvent &event );
	void OnSetFocus( wxFocusEvent &event );
	void OnKillFocus( wxFocusEvent &event );
	void OnChar( wxKeyEvent &event );
	void OnMouse( wxMouseEvent &event );
	void OnMouseWheel( wxMouseEvent &event );
	void OnIdle( wxIdleEvent &event );

	// implementation helpers
protected:
	friend class MyGenericTreeItem;
	friend class MyTreeRenameTimer;
	friend class wxTreeFindTimer;
	friend class MyTreeTextCtrl;

	wxFont               m_normalFont;
	wxFont               m_boldFont;

	MyGenericTreeItem   *m_anchor;
	MyGenericTreeItem   *m_current,
	*m_key_current;
	unsigned short       m_indent;
	unsigned short       m_spacing;
	int                  m_lineHeight;
	wxPen                m_dottedPen;
	wxBrush             *m_hilightBrush,
	*m_hilightUnfocusedBrush;
	bool                 m_hasFocus;
	bool                 m_dirty;
	bool                 m_ownsImageListNormal,
	m_ownsImageListState,
	m_ownsImageListButtons;
	bool                 m_isDragging; // true between BEGIN/END drag events
	bool                 m_lastOnSame;  // last click on the same item as prev
	wxImageList         *m_imageListNormal,
	*m_imageListState,
	*m_imageListButtons;

	int                  m_dragCount;
	wxPoint              m_dragStart;
	MyGenericTreeItem   *m_dropTarget;
	wxCursor             m_oldCursor;  // cursor is changed while dragging
	MyGenericTreeItem   *m_oldSelection;
	MyTreeTextCtrl      *m_textCtrl;

	wxTimer             *m_renameTimer;

	wxBitmap            *m_arrowRight,
	*m_arrowDown;

	// incremental search data
	wxString             m_findPrefix;
	wxTimer             *m_findTimer;

	// the common part of all ctors
	void Init();

	// misc helpers
	void SendDeleteEvent(MyGenericTreeItem *itemBeingDeleted);

	void DrawBorder(const wxTreeItemId& item);
	void DrawLine(const wxTreeItemId& item, bool below);
	void DrawDropEffect(MyGenericTreeItem *item);

	wxTreeItemId DoInsertItem(const wxTreeItemId& parent,
	                          size_t previous,
	                          const wxString& text,
	                          int image, int selectedImage,
	                          TreeItemClientData *data);

	// called by wxTextTreeCtrl when it marks itself for deletion
	void ResetTextControl();

	// find the first item starting with the given prefix after the given item
	wxTreeItemId FindItem(const wxTreeItemId& id, const wxString& prefix) const;

	bool HasButtons(void) const
	{
		return (m_imageListButtons != NULL)
		       || HasFlag(wxTR_TWIST_BUTTONS|wxTR_HAS_BUTTONS);
	}

	void CalculateLineHeight();
	int  GetLineHeight(MyGenericTreeItem *item) const;
	void PaintLevel( MyGenericTreeItem *item, wxDC& dc, int level, int &y );
	void PaintItem( MyGenericTreeItem *item, wxDC& dc);

	void CalculateLevel( MyGenericTreeItem *item, wxDC &dc, int level, int &y );
	void CalculatePositions();
	void CalculateSize( MyGenericTreeItem *item, wxDC &dc );

	void RefreshSubtree( MyGenericTreeItem *item );
	void RefreshLine( MyGenericTreeItem *item );

	// redraw all selected items
	void RefreshSelected();

	// RefreshSelected() recursive helper
	void RefreshSelectedUnder(MyGenericTreeItem *item);

	void OnRenameTimer();
	bool OnRenameAccept(MyGenericTreeItem *item, const wxString& value);
	void OnRenameCancelled(MyGenericTreeItem *item);

	void FillArray(MyGenericTreeItem*, wxArrayTreeItemIds&) const;
	void SelectItemRange( MyGenericTreeItem *item1, MyGenericTreeItem *item2 );
	bool TagAllChildrenUntilLast(MyGenericTreeItem *crt_item, MyGenericTreeItem *last_item, bool select);
	bool TagNextChildren(MyGenericTreeItem *crt_item, MyGenericTreeItem *last_item, bool select);
	void UnselectAllChildren( MyGenericTreeItem *item );

private:
	DECLARE_EVENT_TABLE()
	DECLARE_DYNAMIC_CLASS(MyGenericTreeCtrl)
};
class TreeWidget: public MyGenericTreeCtrl, public KeyEventObserver
{
	DECLARE_DYNAMIC_CLASS(TreeWidget)

public:
	TreeWidget() {}
	~TreeWidget() {}

	TreeWidget(wxWindow *parent, wxWindowID id = -1,
	           const wxPoint& pos = wxDefaultPosition,
	           const wxSize& size = wxDefaultSize,
	           long style = wxTR_DEFAULT_STYLE,
	           const wxValidator &validator = wxDefaultValidator,
	           const wxString& name = wxTreeCtrlNameStr);
};

typedef void (wxEvtHandler::*MyTreeEventFunction)(MyTreeEvent&);

#define MyTreeEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(MyTreeEventFunction, &func)

#endif // _GENERIC_TREECTRL_H_

