#pragma once

	LRESULT OnDocNew(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocEnvelopeWiz(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT LaunchEnvelopeWizard(AGDOCTYPE DocType, bool bAtPrintTime);
	LRESULT OnDocOpen(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	CString GetDocFileName();
	LRESULT OpenDoc(LPCTSTR szFileName);
	LRESULT OnDocClose(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT Close(bool bExit);
	LRESULT OnDocSave(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocSaveAs(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT SaveAs(bool bSilent, bool bExit);
	LRESULT OnDocSaveAsPDF(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocSaveAsXML(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT SaveAsPDF();
	LRESULT OnDocPrint(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocConvert(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocProperties(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocAddImage(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT AddImage(int iOffset);
	LRESULT AddImage(LPCSTR strFileName, int iOffset, int x=0, int y=0, bool bImageDropped = false);
	LRESULT AddImage(HANDLE hMemory, bool bIsDIB, int iOffset, int x=0, int y=0, bool bImageDropped=false);
	LRESULT OnDocAddAPhoto(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocAddTextbox(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocAddCalendar(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT AddTextbox(SYMTYPE SymType);
	LRESULT AddShape(LPCSTR strShapeName, bool bFilled);
	LRESULT OnDocAddShape(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocTransform(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocCrop(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT UndoAdd(CAGSym* pSym);
	LRESULT UndoDelete(CAGSym* pSym);
	LRESULT UndoPermDelete(CAGSym* pSym);
	LRESULT UndoMirrorFlip(CAGSym* pSym, bool bFlipX, bool bFlipy);
	LRESULT UndoRotate(CAGSym* pSym, bool bCW);
	LRESULT UndoLayer(CAGSym* pSym, int nOffset);
	LRESULT UndoTransform(CAGSym* pSym, CAGMatrix Matrix);
	LRESULT UndoDestRect(CAGSym* pSym, RECT& DestRect);
	LRESULT UndoCropRect(CAGSym* pSym, RECT& CropRect);
	LRESULT UndoColor(CAGSym* pSym, COLORREF Color, bool bLineColor, bool bFillColor2 = false);
	LRESULT UndoLineWidth(CAGSym* pSym, int nLineWidth, int nLineType);
	LRESULT UndoFillType(CAGSym* pSym, int nFillType);
	LRESULT UndoTextEdit(CUndoItem* pUndoItem);
	LRESULT UndoLocked(CAGSym* pSym);
	LRESULT UndoHidden(CAGSym* pSym);
	LRESULT UndoTransparent(CAGSym* pSym);
	LRESULT UndoPageTurn(int nPage);
	LRESULT OnDocUndo(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocRedo(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT AddBitmap(HBITMAP hData, HPALETTE hPal);
	LRESULT PasteText(HANDLE hMemory);
	LRESULT OnDocCut(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocDelete(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT DeleteSymbol(bool bCut, UINT nCommand);
	LRESULT OnDocCopy(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocPaste(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT MirrorFlip(CAGSym* pSym, UINT nCommand, bool bFlipX, bool bFlipY);
	LRESULT OnDocFlip(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMirror(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT Rotate(CAGSym* pSym, UINT nCommand, bool bCW);
	LRESULT OnDocRotateCW(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocRotateCCW(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocLocked(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocTransparent(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT Zoom(UINT nCommand, int iOffset);
	LRESULT ZoomEx(UINT nZoomLevel, double fZoomScale);
	LRESULT OnZoomIn(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomOut(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnZoomLevelPageWidth(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnZoomLevelSelection(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnZoomLevel0(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnZoomLevel1Out(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel2Out(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel3Out(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel4Out(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel5Out(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel1(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel2(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel3(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel4(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomLevel5(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnZoomFull(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT Layer(CAGSym* pSym, UINT nCommand, int iOffset);
	LRESULT OnDocLayerBackOne(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocLayerForwardOne(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocLayerToFront(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocLayerToBack(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT Position(int xPosition, int yPosition, UINT nCommand);
	LRESULT OnDocPosition(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMaxWidth(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMaxHeight(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMaximize(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT MaximizeSymbolSize(bool bWidth, bool bHeight);
	void OnDocChangeFillColor(COLORREF Color);
	void OnDocChangeFillColor2(COLORREF Color);
	void OnDocChangeFillColor3(COLORREF Color);
	void OnDocChangeLineColor(COLORREF Color);
	LRESULT OnDocChangeCalendarMonth(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeCalendarYear(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeCalendarStyle(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeLineWidth(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeTextSize(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeFillType(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocChangeShapeName(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMasterLayer(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocMetric(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocGrid(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocDownloadSpellCheck(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocDownloadFonts(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnPageAdd(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnPageDelete(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnPagePrev(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnPageNext(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnSelectAll(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnSelectPrev(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnSelectNext(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT SelectNextPrev(bool bNext);
	LRESULT OnGiveGift(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnArtGallery(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocHidden(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocShowHidden(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnFileDownload(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnCheckForUpdates(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnAbout(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnCheckSpelling(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocHelp(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnDocExtras(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
	LRESULT OnConvertToJPG(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnDocFreeRotate(UINT code, UINT id, HWND hWnd, BOOL& bHandled);

    LRESULT OnTextAlign(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBFile(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBEdit(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBView(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBFormat(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBTools(UINT code, UINT id, HWND hWnd, BOOL& bHandled);
    LRESULT OnTBHelp(UINT code, UINT id, HWND hWnd, BOOL& bHandled);


	void SetCascadePoint(const POINT& pt);
	void CascadeRect(RECT& Rect);
