#include "stdafx.h"
#include "resource.h"
#include "ProgDlg.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif _DEBUG

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	//}}AFX_MSG_MAP
	ON_STN_CLICKED(CG_IDC_PROGDLG_PERCENT, OnStnClickedIdcProgdlgPercent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog
CProgressDlg::CProgressDlg(UINT nCaptionID)
{
	m_nCaptionID = CG_IDS_PROGRESS_CAPTION;
	if (nCaptionID != 0)
		m_nCaptionID = nCaptionID;

	m_nLower = 0;
	m_nUpper = 100;
	m_nStep = 1;
	m_bCancel = false;
	m_bParentDisabled = false;
	//{{AFX_DATA_INIT(CProgressDlg)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
CProgressDlg::~CProgressDlg()
{
	if (m_hWnd)
		DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
BOOL CProgressDlg::DestroyWindow()
{
	ReEnableParent();
	return CDialog::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::ReEnableParent()
{
	if (m_bParentDisabled && (m_pParentWnd!=NULL))
		m_pParentWnd->EnableWindow(true);
	m_bParentDisabled = false;
}

/////////////////////////////////////////////////////////////////////////////
BOOL CProgressDlg::Create(CWnd* pParent)
{
	// Get the true parent of the dialog
	m_pParentWnd = CWnd::GetSafeOwner(pParent);

	// m_bParentDisabled is used to re-enable the parent window
	// when the dialog is destroyed. So we don't want to set
	// it to true unless the parent was already enabled.

	if (m_pParentWnd && m_pParentWnd->IsWindowEnabled())
	{
		m_pParentWnd->EnableWindow(false);
		m_bParentDisabled = true;
	}

	if (!CDialog::Create(CProgressDlg::IDD,pParent))
	{
		ReEnableParent();
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, CG_IDC_PROGDLG_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::SetStatus(LPCTSTR lpszMessage)
{
	ASSERT(m_hWnd); // Don't call this _before_ the dialog has
					// been created. Can be called from OnInitDialog
	CWnd* pWndStatus = GetDlgItem(CG_IDC_PROGDLG_STATUS);

	// Verify that the static text control exists
	ASSERT(pWndStatus!=NULL);
	pWndStatus->SetWindowText(lpszMessage);
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::OnCancel()
{
	m_bCancel = true;
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::SetRange(int nLower,int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	m_Progress.SetRange(nLower,nUpper);
}

/////////////////////////////////////////////////////////////////////////////
int CProgressDlg::SetPos(int nPos)
{
	PumpMessages();
	int iResult = m_Progress.SetPos(nPos);
	UpdatePercent(nPos);
	return iResult;
}

/////////////////////////////////////////////////////////////////////////////
int CProgressDlg::SetStep(int nStep)
{
	m_nStep = nStep;					// Store for later use in calculating percentage
	return m_Progress.SetStep(nStep);
}

/////////////////////////////////////////////////////////////////////////////
int CProgressDlg::OffsetPos(int nPos)
{
	PumpMessages();
	int iResult = m_Progress.OffsetPos(nPos);
	UpdatePercent(iResult+nPos);
	return iResult;
}

/////////////////////////////////////////////////////////////////////////////
int CProgressDlg::StepIt()
{
	PumpMessages();
	int iResult = m_Progress.StepIt();
	UpdatePercent(iResult+m_nStep);
	return iResult;
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::PumpMessages()
{
	// Must call Create() before using the dialog
	ASSERT(m_hWnd != NULL);

	MSG msg;
	// Handle dialog messages
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (!IsDialogMessage(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
BOOL CProgressDlg::CheckCancelButton()
{
	// Process all pending messages
	PumpMessages();

	if (m_bCancel && (::MessageBox(::GetActiveWindow(), "Are you sure you want to cancel?", NULL, MB_YESNO) == IDNO))
		m_bCancel = false;
	
	return m_bCancel;
}

/////////////////////////////////////////////////////////////////////////////
void CProgressDlg::UpdatePercent(int nNewPos)
{
	CWnd* pWndPercent = GetDlgItem(CG_IDC_PROGDLG_PERCENT);
	
	int nRange = m_nUpper - m_nLower;
	ASSERT(nRange > 0);				// m_nLower should be smaller than m_nUpper

	int nComplete = (nNewPos - m_nLower);
	ASSERT(nComplete >= 0); 			// Current position should be greater than m_nLower

	int nPercent = (nComplete * 100) / nRange;

	// Since the Progress Control wraps, we will wrap the percentage
	// along with it. However, don't reset 100% back to 0%
	if (nPercent != 100)
		nPercent %= 100;

	// Display the percentage
	CString strBuf;
	strBuf.Format(_T("%d%% of %d files"), nPercent, m_nUpper - m_nLower);

	CString strCur; 					// get current percentage
	pWndPercent->GetWindowText(strCur);

	if (strCur != strBuf)
		pWndPercent->SetWindowText(strBuf);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_Progress.SetRange(m_nLower,m_nUpper);
	m_Progress.SetStep(m_nStep);
	m_Progress.SetPos(m_nLower);

	CString strCaption;
	VERIFY(strCaption.LoadString(m_nCaptionID));
	SetWindowText(strCaption);

	return true;
}

void CProgressDlg::OnStnClickedIdcProgdlgPercent()
{
	// TODO: Add your control notification handler code here
}
