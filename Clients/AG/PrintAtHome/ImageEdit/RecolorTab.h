#pragma once

#include "resource.h"
#include "BaseTab.h"
#include "ColorButton.h"
#include "afxwin.h"

// CRecolorTab dialog

class CRecolorTab : public CBaseTab
{
public:
	CRecolorTab(CWnd* pParent = NULL);
	virtual ~CRecolorTab();

	enum { IDD = IDD_RECOLOR_TAB };

protected:
	DECLARE_DYNAMIC(CRecolorTab)
	DECLARE_MESSAGE_MAP()

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnRecolorSolid();
	afx_msg void OnRecolorSweepDown();
	afx_msg void OnRecolorSweepRight();
	afx_msg void OnRecolorSweepFromCenter();
	afx_msg void OnRecolorPrimary();
	afx_msg void OnRecolorSecondary();
	afx_msg void OnRecolorPrimaryPatch();
	afx_msg void OnRecolorSecondaryPatch();
	afx_msg void OnRecolorPrimaryPatchDoubleClick();
	afx_msg void OnRecolorSecondaryPatchDoubleClick();

	void SetDefaultFillState();

private:
	COLORREF m_PrimaryColor;
	COLORREF m_SecondaryColor;
	CColorButton m_btnPrimaryPatch;
	CColorButton m_btnSecondaryPatch;
};
