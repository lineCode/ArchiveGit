/*
 * Name:
 *	IADMEntry.hpp
 *
 * Copyright 1986-1998 Adobe Systems Incorporated.
 * All Rights Reserved.
 *
 * Purpose:
 *	Adobe Dialog Manager Entry Interface Class.
 *
 * Distribution:
 *	PUBLIC
 *
 * Version history:
 *	1.0.0 5/7/1996	DL	First version.
 *		Created by Dave Lazarony.
 */

#ifndef __IADMEntry_hpp__
#define __IADMEntry_hpp__

/*
 * Includes
 */
 
#ifndef __ADMEntry__
#include "ADMEntry.h"
#endif

#ifndef __IASTypes_hpp__
#include "IASTypes.hpp"
#endif


/*
 * Global Suite Pointer
 */

extern "C" ADMEntrySuite *sADMEntry;


/*
 * Interface Wrapper Class
 */

class IADMEntry
{
protected:
	ADMEntry *fEntry;

public:	
	IADMEntry();
	IADMEntry(ADMEntryRef entry);
	IADMEntry(int nilEntry);

	operator ADMEntryRef(void);
		
	void SendNotify(const char *notifierType = nil);
	
	int GetIndex();
	
	ADMListRef GetList();
	
	void SetID(int entryID);
	int GetID();
	
	void SetUserData(ADMUserData data);
	ADMUserData GetUserData();

	void Select(ASBoolean show = true);
	void Deselect();
	ASBoolean IsSelected();

	void MakeInBounds();
	ASBoolean IsInBounds();
	
	void Enable(ASBoolean enable = true);
	void Disable();
	ASBoolean IsEnabled();
	
	void Activate(ASBoolean activate = true);
	void Deactivate();
	ASBoolean IsActive();
	
	void Check(ASBoolean check = true);
	void Uncheck();
	ASBoolean IsChecked();
	
	void MakeSeparator(ASBoolean separator);
	ASBoolean IsSeparator();

	void GetLocalRect(IASRect &localRect);
	
	void GetBoundsRect(IASRect &boundsRect);
	
	void LocalToScreenPoint(IASPoint &point);
	void ScreenToLocalPoint(IASPoint &point);
	
	void LocalToScreenRect(IASRect &rect);
	void ScreenToLocalRect(IASRect &rect);
	
	void Invalidate();
	void Update();
	
	void SetPictureID(int pictureResID);
	int GetPictureID();
	
	void SetSelectedPictureID(int pictureResID);
	int GetSelectedPictureID();
	
	void SetDisabledPictureID(int pictureResID);
	int GetDisabledPictureID();
	
	void SetText(const char *text);
	void GetText(char *text, int maxLength);
	int GetTextLength();

	ADMTimerRef CreateTimer(unsigned long milliseconds,
							ADMActionMask abortMask,
							ADMEntryTimerProc timerProc,
							ADMEntryTimerAbortProc abortProc);

	void AbortTimer(ADMTimerRef timer);
};

inline IADMEntry::IADMEntry()								
{
}

inline IADMEntry::IADMEntry(ADMEntryRef entry)	
{
	fEntry = entry;
}

inline IADMEntry::IADMEntry(int nilEntry)
{
	if (nilEntry == nil)
		fEntry = (ADMEntry *)nilEntry;
}

inline IADMEntry::operator ADMEntryRef(void)
{
	return fEntry;
}

inline void IADMEntry::SendNotify(const char *notifierType)
{
	sADMEntry->SendNotify(fEntry, (char *)notifierType);
}
	
inline int IADMEntry::GetIndex()
{
	return sADMEntry->GetIndex(fEntry);
}

inline ADMListRef IADMEntry::GetList()
{
	return sADMEntry->GetList(fEntry);
}

inline void IADMEntry::SetID(int entryID)
{
	sADMEntry->SetID(fEntry, entryID);
}

inline int IADMEntry::GetID()
{
	return sADMEntry->GetID(fEntry);
}

inline void IADMEntry::SetUserData(ADMUserData data)
{
	sADMEntry->SetUserData(fEntry, data);
}

inline ADMUserData IADMEntry::GetUserData()
{
	return sADMEntry->GetUserData(fEntry);
}

inline void IADMEntry::Select(ASBoolean show)
{
	sADMEntry->Select(fEntry, show);
}

inline void IADMEntry::Deselect()
{
	sADMEntry->Select(fEntry, false);
}

inline ASBoolean IADMEntry::IsSelected()
{
	return sADMEntry->IsSelected(fEntry);
}

inline void IADMEntry::MakeInBounds()
{
	sADMEntry->MakeInBounds(fEntry);
}

inline ASBoolean IADMEntry::IsInBounds()
{
	return sADMEntry->IsInBounds(fEntry);
}

inline void IADMEntry::Enable(ASBoolean enable)
{
	sADMEntry->Enable(fEntry, enable);
}

inline void IADMEntry::Disable()
{
	sADMEntry->Enable(fEntry, false);
}

inline ASBoolean IADMEntry::IsEnabled()
{
	return sADMEntry->IsEnabled(fEntry);
}

inline void IADMEntry::Activate(ASBoolean activate)
{
	sADMEntry->Activate(fEntry, activate);
}

inline void IADMEntry::Deactivate()
{
	sADMEntry->Activate(fEntry, false);
}

inline ASBoolean IADMEntry::IsActive()
{
	return sADMEntry->IsActive(fEntry);
}

inline void IADMEntry::Check(ASBoolean check)
{
	sADMEntry->Check(fEntry, check);
}

inline void IADMEntry::Uncheck()
{
	sADMEntry->Check(fEntry, false);
}

inline ASBoolean IADMEntry::IsChecked()
{
	return sADMEntry->IsChecked(fEntry);
}

inline void IADMEntry::MakeSeparator(ASBoolean separator)
{
	sADMEntry->MakeSeparator(fEntry, separator);
}

inline ASBoolean IADMEntry::IsSeparator()
{
	return sADMEntry->IsSeparator(fEntry);
}

inline void IADMEntry::GetLocalRect(IASRect &localRect)
{
	sADMEntry->GetLocalRect(fEntry, (ASRect *)&localRect);
}

inline void IADMEntry::GetBoundsRect(IASRect &boundsRect)
{
	sADMEntry->GetBoundsRect(fEntry, (ASRect *)&boundsRect);
}

inline void IADMEntry::LocalToScreenPoint(IASPoint &point)
{
	sADMEntry->LocalToScreenPoint(fEntry, (ASPoint *)&point);
}

inline void IADMEntry::ScreenToLocalPoint(IASPoint &point)
{
	sADMEntry->ScreenToLocalPoint(fEntry, (ASPoint *)&point);
}

inline void IADMEntry::LocalToScreenRect(IASRect &rect)
{
	sADMEntry->LocalToScreenRect(fEntry, (ASRect *)&rect);
}

inline void IADMEntry::ScreenToLocalRect(IASRect &rect)
{
	sADMEntry->ScreenToLocalRect(fEntry, (ASRect *)&rect);
}

inline void IADMEntry::Invalidate()
{
	sADMEntry->Invalidate(fEntry);
}

inline void IADMEntry::Update()
{
	sADMEntry->Update(fEntry);
}

inline void IADMEntry::SetPictureID(int pictureResID)
{
	sADMEntry->SetPictureID(fEntry, pictureResID);
}

inline int IADMEntry::GetPictureID()
{
	return sADMEntry->GetPictureID(fEntry);
}

inline void IADMEntry::SetSelectedPictureID(int pictureResID)
{
	sADMEntry->SetSelectedPictureID(fEntry, pictureResID);
}

inline int IADMEntry::GetSelectedPictureID()
{
	return sADMEntry->GetSelectedPictureID(fEntry);
}

inline void IADMEntry::SetDisabledPictureID(int pictureResID)
{
	sADMEntry->SetDisabledPictureID(fEntry, pictureResID);
}

inline int IADMEntry::GetDisabledPictureID()
{
	return sADMEntry->GetDisabledPictureID(fEntry);
}

inline void IADMEntry::SetText(const char *text)
{
	sADMEntry->SetText(fEntry, (char *)text);
}

inline void IADMEntry::GetText(char *text, int maxLength = 0)
{
	sADMEntry->GetText(fEntry, text, maxLength);
}

inline int IADMEntry::GetTextLength()
{
	return sADMEntry->GetTextLength(fEntry);
}


inline ADMTimerRef IADMEntry::CreateTimer(unsigned long milliseconds,
		ADMActionMask abortMask, ADMEntryTimerProc timerProc,
		ADMEntryTimerAbortProc abortProc)
{
	return sADMEntry->CreateTimer(fEntry, milliseconds,
		abortMask, timerProc, abortProc);
}

inline void IADMEntry::AbortTimer(ADMTimerRef timer)
{
	sADMEntry->AbortTimer(fEntry, timer);
}

#endif
