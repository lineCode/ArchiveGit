#if	_MSC_VER > 1000
#pragma once
#endif

#ifndef __FILETYPEACCESS_H__
#define __FILETYPEACCESS_H__
#ifdef  __cplusplus

class CFileTypeAccess : public CObject
{
	DECLARE_DYNAMIC(CFileTypeAccess)

public:

	CFileTypeAccess();
	virtual ~CFileTypeAccess();

	virtual void ClearAllData(void);

	//- ----------------------------------------
	//- Effect Registry

	virtual BOOL	RegSetAllInfo		(void);

	virtual BOOL	RegSetExtension		(void);
	virtual BOOL	RegSetDocumentType	(void);
	virtual BOOL	RegSetCLSID			(void);
	virtual BOOL	RegSetShellInfo		(void);

	//- ----------------------------------------
	//- Modify Member Variables

	void	SetExtension		(LPCTSTR);
	void	GetExtension		(CString&) const;

	void	SetContentType		(LPCTSTR);
	void	GetContentType		(CString&) const;

	void	SetShellOpenCommand	(LPCTSTR);
	void	GetShellOpenCommand	(CString&) const;

	void	SetShellNewCommand	(LPCTSTR);
	void	GetShellNewCommand	(CString&) const;

	void	SetShellNewFileName	(LPCTSTR);
	void	GetShellNewFileName	(CString&) const;

	void	SetDocumentClassName		(LPCTSTR);
	void	GetDocumentClassName		(CString&) const;

	void	SetDocumentDescription		(LPCTSTR);
	void	GetDocumentDescription		(CString&) const;

	void	SetDocumentCLSID			(LPCTSTR);
	void	GetDocumentCLSID			(CString&) const;

	void	SetDocumentCurrentVersion	(LPCTSTR);
	void	GetDocumentCurrentVersion	(CString&) const;

	void	SetDocumentDefaultIcon		(LPCTSTR);
	void	GetDocumentDefaultIcon		(CString&) const;

	void	SetDocumentShellOpenCommand	(LPCTSTR);
	void	GetDocumentShellOpenCommand	(CString&) const;

protected:

	BOOL SetRegistryValue(HKEY, LPCTSTR, LPCTSTR, LPCTSTR);

	CString		m_csExtension;
	CString		m_csContentType;
	CString		m_csShellOpenCommand;
	CString		m_csShellNewCommand;
	CString		m_csShellNewFileName;

	CString		m_csDocumentClassName;
	CString		m_csDocumentDescription;
	CString		m_csDocumentCLSID;
	CString		m_csDocumentCurrentVersion;
	CString		m_csDocumentDefaultIcon;
	CString		m_csDocumentShellOpenCommand;

};

#endif // __cplusplus
#endif // __FILETYPEACCESS_H__

//-	-----------------------------------------------------------------
//- END CGFILTYP.H Header File
//-	-----------------------------------------------------------------
