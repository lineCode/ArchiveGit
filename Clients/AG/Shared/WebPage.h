// WebPage.h: interface for the CWebPage class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <Mshtml.h>

class CWebPage  
{
public:
	CWebPage();
	CWebPage(IDispatch* pDisp);
	virtual ~CWebPage();

	bool SetDocument(IDispatch* pDisp);
	CComPtr<IHTMLDocument2> GetHtmlDocument() const;
	const CString GetLastError() const;
	bool GetJScript(CComPtr<IDispatch>& spDisp);
	bool GetJScripts(CComPtr<IHTMLElementCollection>& spColl);

	CString ScanJScript(CString& strAText, CSimpleArray<CString> &args);

	bool CallJScript(const CString strFunc, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CString strArg1, const CString strArg2, const CString strArg3, CComVariant* pVarResult = NULL);
	bool CallJScript(const CString strFunc, const CSimpleArray<CString> &paramArray, CComVariant* pVarResult = NULL);

protected:
	void ShowError(LPCSTR lpszText);

protected:
	CComPtr<IHTMLDocument2>	m_spDoc;
	CString	m_strError;
};

inline
void CWebPage::ShowError(LPCSTR lpszText)
{
	m_strError = _T("JSCall Error:\n") + CString(lpszText);
}
inline 
const CString CWebPage::GetLastError() const
{
	return m_strError;
}
inline
CComPtr<IHTMLDocument2> CWebPage::GetHtmlDocument() const
{
	return m_spDoc;
}
