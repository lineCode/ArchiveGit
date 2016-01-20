static void CALLBACK DrawGridDCProc( HDC hDC, HWND hWindow );
void DrawGrid( HWND hWindow );
void GridTimerSet( HWND hWindow, BOOL bOn );
static void StuffCrack( LPCRACK lpCrack, POINT pt1, BOOL bxpos, BOOL bypos, BOOL bxlty, int iMaxLen );
static void AddCrack( LPCRACK lpCrack );
static void FreeCracks( LPCRACK lpCrack );
static void InitCracks( HWND hWindow, BOOL bInit );
void CrackTimerSet( HWND hWindow, BOOL bOn );
static void DrawCracks( HDC hDC, LPCRACK lpCrack );
void CALLBACK DrawCracksDCProc( HDC hDC, HWND hWindow );
void DrawCracks( HWND hWindow );
void DrawBumps( HWND hWindow, BOOL bOn );
void BumpTimerSet( HWND hWindow, BOOL bOn );
void DrawHotspots (HWND);
void HotspotTimerSet (HWND, BOOL);
void CrackSet( HWND hWindow, BOOL bOn );          
static void CrackDraw( HDC hDC, LPCRACK lpCrack );
void CrackDraw( HWND hWindow );