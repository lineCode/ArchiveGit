#pragma once

#include "resource.h"
#include "PagePanel.h"

#include "ToolsPanelContainer.h"
#include "ToolsPanelCal1.h"
#include "ToolsPanelCal2.h"
#include "ToolsPanelText1.h"
#include "ToolsPanelText2.h"
#include "ToolsPanelImage1.h"
#include "ToolsPanelImage2.h"
#include "ToolsPanelGraphics1.h"
#include "ToolsPanelGraphics2.h"
#include "ToolsPanelPrint.h"

#include "DocWindow.h"
#include "AGSym.h"
#include "MessageBox.h"
#include "Download.h"
#include "Extensions.h"
#include "AutoUpdate.h"
#include "ColorScheme.h"
#include "HTMLDialogEx.h"
#include "AddressbookData.h"
#include "ArtGalleryDialog.h"

#ifdef NETSCAPE
	#include "..\NpCtp\npsdk\NpApi.h"
#else
	#include "..\AxCtp2\AxCtp2.h"
	#include "..\AxCtp2\CtpEvents.h"
#endif NETSCAPE

#include <shlobj.h> // for IExtractImage, IExtractImage2, IContextMenu, IContextMenu2, IContextMenu3
#include <comdef.h>

class CDocWindow;
class CBlankPanel;
class CInfoPanel;

#ifdef NETSCAPE

class CCtp : public CWindowImpl<CCtp>
{
public:

#else

#define dwSAFETY_OPTIONS	INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA

class ATL_NO_VTABLE CCtp : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCtp, &CLSID_Ctp>,
	public CComControl<CCtp>,
	public IDispatchImpl<ICtp, &IID_ICtp, &LIBID_AXCTP2Lib>,
	public IPersistStreamInitImpl<CCtp>,
	public IPersistStorageImpl<CCtp>,
	public IPersistPropertyBagImpl<CCtp>,
	public IOleControlImpl<CCtp>,
	public IOleObjectImpl<CCtp>,
	public IOleInPlaceActiveObjectImpl<CCtp>,
	public IViewObjectExImpl<CCtp>,
	public IOleInPlaceObjectWindowlessImpl<CCtp>,
	public IConnectionPointContainerImpl<CCtp>, //j
	public IProvideClassInfo2Impl<&CLSID_Ctp, &DIID__ICtpEvents, &LIBID_AXCTP2Lib>, //j
	public IPropertyNotifySinkCP<CCtp>, //j
	public CProxy_ICtpEvents<CCtp>, //j
	public IQuickActivateImpl<CCtp>, //j
	public IDataObjectImpl<CCtp>, //j
	public IPersistFile, // for CImageExtractor
	public IExtractImage2, // for CImageExtractor
	public IObjectSafetyImpl<CCtp, dwSAFETY_OPTIONS>,
	public IDropTarget,
	public IDropSource
{
public:
	DECLARE_REGISTRY_RESOURCEID(IDR_CTP)
	DECLARE_NOT_AGGREGATABLE(CCtp)
	DECLARE_OLEMISC_STATUS(
		/*0x00001*/ OLEMISC_RECOMPOSEONRESIZE | 
		/*0x00010*/ OLEMISC_CANTLINKINSIDE | 
		/*0x00080*/ OLEMISC_INSIDEOUT | 
		/*0x00100*/ OLEMISC_ACTIVATEWHENVISIBLE | 
		/*0x20000*/ OLEMISC_SETCLIENTSITEFIRST
	)

	BEGIN_COM_MAP(CCtp)
		COM_INTERFACE_ENTRY(ICtp)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IViewObjectEx)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
		COM_INTERFACE_ENTRY(IOleControl)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY(IPersistStreamInit)
		COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
		COM_INTERFACE_ENTRY(IPersistPropertyBag)
		COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IQuickActivate)
		COM_INTERFACE_ENTRY(IPersistStorage)
		COM_INTERFACE_ENTRY(IDataObject)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IPersistFile) // for CImageExtractor
		COM_INTERFACE_ENTRY(IExtractImage2) // for CImageExtractor
		COM_INTERFACE_ENTRY(IExtractImage) // for CImageExtractor
		COM_INTERFACE_ENTRY(IObjectSafety)
		COM_INTERFACE_ENTRY(IDropTarget) 
		COM_INTERFACE_ENTRY(IDropSource) 
	END_COM_MAP()

	BEGIN_PROPERTY_MAP(CCtp)
		PROP_ENTRY("Src", 1, CLSID_NULL)
		PROP_ENTRY("Context", 2, CLSID_NULL)
	END_PROPERTY_MAP()

	BEGIN_CONNECTION_POINT_MAP(CCtp)
		CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
		CONNECTION_POINT_ENTRY(DIID__ICtpEvents)
	END_CONNECTION_POINT_MAP()

#endif NETSCAPE

	friend class CToolsCtlPanel;
	friend class CToolsCtlPanelCommon;
	friend class CToolsPanelText1;
	friend class CToolsPanelImage1;
	friend class CToolsPanelGraphics1;

	friend class CPagePanel;
	friend class CDocWindow;
	friend class CDownload;

	BEGIN_MSG_MAP(CCtp)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_MOUSEACTIVATE, OnMouseActivate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_RANGE_HANDLER(WM_CTLCOLORMSGBOX, WM_CTLCOLORSTATIC, OnCtlColor)
#ifndef NETSCAPE
		CHAIN_MSG_MAP(CComControl<CCtp>) //j
#endif NETSCAPE
		DEFAULT_REFLECTION_HANDLER() //j
	END_MSG_MAP()

public:
	CCtp();
	~CCtp();
	void CreateBackPage();
	void ShowHideCardBack(bool bShow);
	static void CALLBACK MyArtSetupCallback(HWND hwnd, PFNARTGALLERYCALLBACK pfnArtGalleryCallback, LPARAM lArtGalParam, LPARAM lParam, LPCTSTR szValue);
	static void CALLBACK MyArtGalleryCallback(HWND hwnd, UINT uMsgId, LPARAM lParam, LPARAM lArtGalleryPtr, LPCTSTR szValue);
	static bool CALLBACK MyProjectDownloadCallback(LPARAM lParam, void* pDownloadVoid);
	static bool CALLBACK MyFontDownloadCallback(LPARAM lParam, void* pDownloadVoid);
	static bool CALLBACK MyGraphicsDownloadCallback(LPARAM lParam, void* pDownloadVoid);
	static bool CALLBACK MySpellingDownloadCallback(LPARAM lParam, void* pDownloadVoid);
	void FontListDownload(FONTDOWNLOADLIST& FontList);
	bool BeginSpellingDownload(CString& strDstPath);
	bool LaunchAddressbook(HWND hwndParent);
	bool LaunchClipArtPopup(HWND hwndParent, PFNARTGALLERYCALLBACK pfnArtGalleryCallback, LPARAM lParam, LPCTSTR szClient=CLIENT_CTP);

	CAGSymImage* GetImageFromResource(SYMID nID, const RECT* pMaxBounds = NULL);

	CDocWindow* GetDocWindow()
		{ return m_pDocWindow; }
	CFontList& GetFontList()
		{ return m_FontList; }
	CDownload& GetDownload()
		{ return m_Download; }
	CExtensions& GetExtensions()
		{ return m_Extend; }
	CAutoUpdate& GetAutoUpdate()
		{ return *m_pAutoUpdate; }
	CAddressbookData * GetAddressbookData()
		{ return m_pAddressbookData; }
	CArtGalleryDialog * GetArtGalleryDialog()
		{ return m_pArtGalleryDialog; }
	
	CPagePanel* GetPagePanel();
	CInfoPanel* GetInfoPanel();
	CToolsPanelContainer* GetCtlPanel(UINT nPanel);

	void SelectPanel(UINT nPanel);
	HWND GetBrowser();

	void ActivateBrowser();
	void ActivateNewDoc();
	void ConfigureWindows(int iWidth = NULL, int iHeight = NULL);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//j	HRESULT OnDraw(ATL_DRAWINFO& di);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCtlColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	STDMETHOD(get_Version)(BSTR* pVal);
	STDMETHOD(get_Context)(BSTR* bstrContext);
	STDMETHOD(GetContext)(BSTR* bstrContext);
	STDMETHOD(put_Context)(BSTR bstrContext);
	STDMETHOD(PutContext)(BSTR bstrContext);
	STDMETHOD(get_Src)(BSTR* bstrSrcURL);
	STDMETHOD(GetSrc)(BSTR* bstrSrcURL);
	STDMETHOD(put_Src)(BSTR bstrSrcURL);
	STDMETHOD(PutSrc)(BSTR bstrSrcURL);
	STDMETHOD(PutSrcURL)(BSTR bstrSrcURL);
	HRESULT SrcDownload(bool bStart);
	STDMETHOD(FileDownload)(BSTR bstrSrcURL, BSTR bstrInfo, BSTR bstrOptionString);
	STDMETHOD(GetVerse)(BSTR bstrFilePath, BSTR* bstrVerse);
	STDMETHOD(GetAttributes)(BSTR bstrFilePath, BSTR* bstrAttributes);
	STDMETHOD(ProcessImage)(BSTR bstrFilePath, int nMaxKB, int nQuality, BSTR* bstrFilePathNew);
	STDMETHOD(DoCommand)(BSTR bstrCommand, long* plResult);
	STDMETHOD(GetCommand)(BSTR bstrCommand, long* plResult);
	STDMETHOD(GetUsageData)(BSTR* pstrUsageData);
	STDMETHOD(FYSTransfer)(BSTR OrderId, BSTR CorrelationId, BSTR FYSInfo, BSTR* pstrError);
	STDMETHOD(FYSClose)();
	STDMETHOD(SetAddressbookUrl)(VARIANT varUrl);
	STDMETHOD(ShowAddressbookPopup)(VARIANT varShow);
	STDMETHOD(ResizeAddressbookPopup)(VARIANT varWidth, VARIANT varHeight);
	STDMETHOD(CloseAddressbookPopup)();
	STDMETHOD(ProcessAddressbook)(VARIANT varData);
	STDMETHOD(SetClipArtUrl)(VARIANT varUrl);
	STDMETHOD(ShowClipArtPopup)(VARIANT varShow);
	STDMETHOD(ResizeClipArtPopup)(VARIANT varWidth, VARIANT varHeight);
	STDMETHOD(CloseClipArtPopup)();
	
	CString GetHomeFolder();
	bool IsHpUI();
	bool IsApplication();
	bool IsAIM();
	bool IsBlueMountain();
	bool IsAmericanGreetings();
	bool IsDevelopmentServer();
	CString GetServerType();
	CString GetContextProductNumber();
	CString GetOptionStringValue(CString& strOptionString, CString& strNameTarget);
	CString GetContextApp();
	CString GetContextUser();
	CString GetContextMemberNumber();
	CString GetContextMemberStatus();
	CString GetContextItemInfo();
	CString GetContextCallProc();
	CString GetContextContentStatus();
	CString GetContextCpHost();
	CString GetContextImgHost();
	int GetContextRemotePrintStatus();
	int GetContextLicense();
	int GetContextMetric();
	CString GetDBItem() { return m_strDBItem; };
	void SetDBItem(CString& strDBItem) { m_strDBItem = strDBItem; };
	void DoDragDrop();
	void AutoCloseBrowser();
	bool AutoPrintEnabled() { return m_bAutoPrint; };
	bool SetWindowIcon(HWND hwnd);

#ifdef NETSCAPE
	void SetNPPInstance(NPP pInstance);
#else
	// IViewObjectEx
	STDMETHOD(GetViewStatus)(DWORD* pdwStatus);

	// IObjectSafety
	STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD* pdwSupportedOptions, DWORD* pdwEnabledOptions);
	STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);

	// IDropTarget
	virtual STDMETHODIMP Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect); 
	virtual STDMETHODIMP DragEnter(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect); 
	virtual STDMETHODIMP DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect); 
	virtual STDMETHODIMP DragLeave(void); 

	// IDropSource
	virtual STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed,  DWORD grfKeyState);
	virtual STDMETHODIMP GiveFeedback(DWORD dwEffect);

	// IShellExtInit interface
	STDMETHOD(Initialize)(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID);

#endif NETSCAPE

protected:
	void Free();
	CComPtr<IWebBrowser2> GetWebBrowserInterface();

protected:
	HWND m_hWndBrowser;
	CColorScheme m_ColorScheme;

	CToolsPanelContainer* m_pToolsPanel[NUM_PANELS];
	CToolsPanelPrint* m_pPrintPanel;

	CPagePanel* m_pPagePanel;
	CBlankPanel* m_pBlankPanel;
	CInfoPanel* m_pInfoPanel;
	CDocWindow* m_pDocWindow;
	CFontList m_FontList;
	CString m_strSrcURL;
	CString m_strContext;
	CString m_strDBItem;
	CDownload m_Download;
	CExtensions m_Extend;
	CAutoUpdate *m_pAutoUpdate;
	UINT m_nSelectedPanel;
	TCHAR m_szPrintFile[MAX_PATH];
	CAddressbookData * m_pAddressbookData;
	CArtGalleryDialog * m_pArtGalleryDialog;
	bool m_bAutoPrint;

#ifdef NETSCAPE
public:
	NPP m_pNPPInstance;
#endif NETSCAPE	

#include "ImageExtractor.h"
};

/////////////////////////////////////////////////////////////////////////////
class CBlankPanel : public CWindowImpl<CDocWindow>
{
public:
	CBlankPanel() {}
	~CBlankPanel() {}

	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = (HDC)wParam;
		if (!hDC)
		{
			bHandled = false;
			return S_OK;
		}

		RECT rc;
		GetClientRect(&rc);
		::FillRect(hDC, &rc, CAGBrush(WHITE_BRUSH, eStock));
		return S_OK;
	}
public:
	BEGIN_MSG_MAP(CBlankPanel)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()
};

/////////////////////////////////////////////////////////////////////////////
class CInfoPanel : public CDialogImpl<CInfoPanel>
{
public:
	CInfoPanel(CCtp* pMainWnd)
	{
		m_pCtp = pMainWnd;
	}

	~CInfoPanel() {}

	enum { IDD = IDD_PANEL_INFO };

	LRESULT CInfoPanel::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hDC = (HDC)wParam;
		if (!hDC)
		{
			bHandled = false;
			return S_OK;
		}

		RECT rc;
		GetClientRect(&rc);
		::FillRect(hDC, &rc, CAGBrush(WHITE_BRUSH, eStock));
		return S_OK;
	}

	LRESULT CInfoPanel::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_LabelInfo.SubclassWindow(GetDlgItem(IDC_INFO));
		return IDOK;
	}

	LRESULT CInfoPanel::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (!m_pCtp || !::IsWindow(m_hWnd))
			return S_OK;

		// wParam values: SIZE_MAXIMIZED, SIZE_MINIMIZED, SIZE_RESTORED
		if (wParam != SIZE_RESTORED)
			return S_OK;

		// Get the new client size
		int iWidth = LOWORD(lParam);
		int iHeight = HIWORD(lParam);
		::SetWindowPos(GetDlgItem(IDC_INFO), NULL, 0, 0, iWidth, iHeight, SWP_NOZORDER /*| SWP_NOMOVE*/);
		return S_OK;
	}

	void CInfoPanel::UpdateInfo(LPCSTR pszInfo)
	{
		if (!m_pCtp || !::IsWindow(m_hWnd))
			return;

		if (pszInfo)
			m_LabelInfo.SetWindowText(pszInfo);
	}

public:
	BEGIN_MSG_MAP(CInfoPanel)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

protected:
	CCtp* m_pCtp;
	CEditBox m_LabelInfo;
};
