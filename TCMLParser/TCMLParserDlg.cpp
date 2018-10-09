// TCMLParserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TCMLParser.h"
#include "TCMLParserDlg.h"

#include <iostream>
#include <regex>
#include <stdarg.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CUSTOM_TRACE(const char *fmt, ...)
{
	char textString[1024 * 5] = { '\0' };

	// -- Empty the buffer properly to ensure no leaks.
	memset(textString, '\0', sizeof(textString));

	va_list args;
	va_start(args, fmt);
	vsnprintf(textString, 1024 * 5, fmt, args);
	va_end(args);
	std::string retStr = textString;

	OutputDebugStringA(retStr.c_str());
}

extern COMPDESC* TCMLFrame;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTCMLParserDlg dialog



CTCMLParserDlg::CTCMLParserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCMLParserDlg::IDD, pParent)
	, m_strSRC(_T(""))
	, m_strDEST(_T(""))
	, m_strHEADER(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCMLParserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DEST, m_cDEST);
	DDX_Control(pDX, IDC_BUTTON_SRC, m_cSRC);
	DDX_Control(pDX, IDC_BUTTON_HEADER, m_cHEADER);
	DDX_Text(pDX, IDC_EDIT_DEST, m_strDEST);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSRC);
	DDX_Text(pDX, IDC_EDIT_HEADER, m_strHEADER);
}

BEGIN_MESSAGE_MAP(CTCMLParserDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SRC, OnBnClickedButtonSrc)
	ON_BN_CLICKED(IDC_BUTTON_DEST, OnBnClickedButtonDest)
	ON_BN_CLICKED(IDC_BUTTON_HEADER, OnBnClickedButtonHeader)
	ON_BN_CLICKED(ID_COMPILE, OnBnClickedCompile)
	ON_BN_CLICKED(ID_DECOMPILE, OnBnClickedDecompile)
END_MESSAGE_MAP()


// CTCMLParserDlg message handlers

BOOL CTCMLParserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cDEST.SetIcon(hIcon);
	m_cSRC.SetIcon(hIcon);
	m_cHEADER.SetIcon(hIcon);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTCMLParserDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTCMLParserDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTCMLParserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTCMLParserDlg::OnBnClickedButtonSrc()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tsc"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Tachyon Script Files (*.tsc)|*.tsc|All Files (*.*)|*.*||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strSRCFOLDER);
	if (dlg.DoModal() == IDOK)
	{
		UpdateData();
		m_strSRCFOLDER = dlg.GetFileName();
		m_strSRC = dlg.GetPathName();
		m_strSRCFOLDER = m_strSRC.Left(m_strSRC.GetLength() - m_strSRCFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTCMLParserDlg::OnBnClickedButtonDest()
{
	CFileDialog dlg(
		FALSE,
		_T("*.tif"),
		"TClientCmd",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("T-Project Interface Data Files (*.tif)|*.tif||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strDESTFOLDER);
	if (dlg.DoModal() == IDOK)
	{
		UpdateData();
		m_strDESTFOLDER = dlg.GetFileName();
		m_strDEST = dlg.GetPathName();
		m_strDESTFOLDER = m_strDEST.Left(m_strDEST.GetLength() - m_strDESTFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTCMLParserDlg::OnBnClickedButtonHeader()
{
	CFileDialog dlg(
		TRUE,
		_T("*.h"),
		"TClientID",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("TClient header file (*.h)|*.h||"));

	dlg.m_ofn.lpstrInitialDir = LPCSTR(m_strDESTFOLDER);
	if (dlg.DoModal() == IDOK)
	{
		UpdateData();
		m_strHEADERFOLDER = dlg.GetFileName();
		m_strHEADER = dlg.GetPathName();
		m_strHEADERFOLDER = m_strHEADER.Left(m_strHEADER.GetLength() - m_strHEADERFOLDER.GetLength());
		UpdateData(FALSE);
	}
}

void CTCMLParserDlg::OnBnClickedCompile()
{
	CFile vDEST(LPCSTR(m_strDEST), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	TCMLParser vTParser;

	COMP_MAP::iterator itCOMP;
	FONT_MAP::iterator itFONT;

	int nret = vTParser.Parse((char *)LPCTSTR(m_strSRC));
	if (nret != 0)
	{
		CString strFMT;
		strFMT.Format("Error :%d",
			nret);
		AfxMessageBox(strFMT);
	}

	int nCount = INT(vTParser.m_Comps.size());
	vDEST.Write(&nCount, sizeof(int));

	for (itCOMP = vTParser.m_Comps.begin(); itCOMP != vTParser.m_Comps.end(); itCOMP++)
		WriteFRAME(&vDEST, (*itCOMP).second);

	nCount = INT(vTParser.m_Fonts.size());
	vDEST.Write(&nCount, sizeof(int));

	for (itFONT = vTParser.m_Fonts.begin(); itFONT != vTParser.m_Fonts.end(); itFONT++)
		vDEST.Write((*itFONT).second, sizeof(TCML_LOGFONT));

	CDialog::OnOK();
}

int CTCMLParserDlg::GetNodeCount(LP_COMPDESC pCOMP)
{
	int nCount = 0;

	while (pCOMP)
	{
		pCOMP = pCOMP->next;
		nCount++;
	}

	return nCount;
}

void CTCMLParserDlg::WriteFRAME(CFile *pFILE, LP_COMPDESC pCOMP)
{
	pFILE->Write(&pCOMP->id, sizeof(DWORD));
	pFILE->Write(&pCOMP->type, sizeof(BYTE));

	pFILE->Write(pCOMP->menu, TCML_MENU_COUNT * sizeof(DWORD));
	pFILE->Write(pCOMP->images, 2 * sizeof(DWORD));
	pFILE->Write(&pCOMP->tipface, sizeof(DWORD));
	pFILE->Write(&pCOMP->face, sizeof(DWORD));
	pFILE->Write(&pCOMP->style, sizeof(DWORD));
	pFILE->Write(&pCOMP->color, sizeof(DWORD));
	pFILE->Write(&pCOMP->sound, sizeof(DWORD));

	pFILE->Write(&pCOMP->hmargine, sizeof(int));
	pFILE->Write(&pCOMP->vmargine, sizeof(int));
	pFILE->Write(&pCOMP->x, sizeof(int));
	pFILE->Write(&pCOMP->y, sizeof(int));
	pFILE->Write(&pCOMP->width, sizeof(int));
	pFILE->Write(&pCOMP->height, sizeof(int));

	pFILE->Write(&pCOMP->display, sizeof(BYTE));
	pFILE->Write(&pCOMP->align, sizeof(BYTE));
	pFILE->Write(&pCOMP->ex, sizeof(TSATR));

	int nCount = INT(strlen(pCOMP->tooltip));
	pFILE->Write(&nCount, sizeof(int));
	if (nCount > 0)
		pFILE->Write(pCOMP->tooltip, nCount * sizeof(char));

	nCount = INT(strlen(pCOMP->text));
	pFILE->Write(&nCount, sizeof(int));
	if (nCount > 0)
		pFILE->Write(pCOMP->text, nCount * sizeof(char));

	if (pCOMP->ex.combo.downlist != LIKE_NULL) {
		int i = 0;
	}
	if (pCOMP->ex.combo.drop > 0) {
		int i = 0;
	}

	if (pCOMP->child)
	{
		LP_COMPDESC pCHILD = pCOMP->child;
		nCount = GetNodeCount(pCHILD);

		pFILE->Write(&nCount, sizeof(int));
		while (pCHILD)
		{
			WriteFRAME(pFILE, pCHILD);
			pCHILD = pCHILD->next;
		}
	}
	else
	{
		nCount = 0;
		pFILE->Write(&nCount, sizeof(int));
	}
}


void CTCMLParserDlg::OnBnClickedDecompile()
{
	LoadHeaders();
	LoadFrames();
}

void CTCMLParserDlg::LoadHeaders()
{
	FILE *pFILE = fopen(m_strHEADER, "rb");
	char buffer[100];


	if (pFILE == NULL) {
		AfxMessageBox("Can't open TClient.h...");
		return;
	}
	else
	{
		while (!feof(pFILE))
		{
			if (fgets(buffer, 100, pFILE) == NULL) { break; }

			CString line(buffer);
			line.Replace('\t', ' ');
			int startID = line.Find(' ', 0);
			if (startID == -1) { continue; }
			int endID = line.Find(' ', startID + 1);
			if (endID == -1) { continue; }

			int startValue = line.Find('(', 0);
			if (startValue == -1) { continue; }
			int endValue = line.Find(')', startValue + 1);
			if (endValue == -1) { continue; }

			string ID_Line = line.Mid(startID + 1, endID - startID - 1);
			string VALUE_Line = line.Mid(startValue + 1, endValue - startValue - 1);

			defineValues.Add(ID_Line, VALUE_Line);
		}
		fclose(pFILE);
	}
}

void CTCMLParserDlg::LoadFrames()
{
	FILE *pFILE = fopen(m_strDEST, "rb");
	int nCount = 0;

	fread(&nCount, sizeof(int), 1, pFILE);


	// Create text file for append
	CFile finalTSC("DECOMPILED_FRAMES.tsc", CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	finalTSC.Close();


	for (int i = 0; i < nCount; i++) {
		Node node = ConvertFrame(pFILE, true, 0);


		CString formattedFrame = node.GetAllString();
		CFile finalTSC("DECOMPILED_FRAMES.tsc", CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		finalTSC.SeekToEnd();

		finalTSC.Write(formattedFrame, formattedFrame.GetLength());
		finalTSC.Close();
	}


	CFile finalFonts("DECOMPILED_FONTS.tsc", CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	finalFonts.Close();

	fread(&nCount, sizeof(int), 1, pFILE);
	for (int i = 0; i < nCount; i++)
	{
		LP_TCML_LOGFONT pFONT = new TCML_LOGFONT();

		fread(pFONT, sizeof(TCML_LOGFONT), 1, pFILE);

		CString ID_Formatted = defineValues.FindWithCriteria(pFONT->tlfId, "ID_FONT", "UNKNOWN_ID_FONT");

		CString formattedFont;
		formattedFont.Format("\t<FONT ID=\"%s\" FACENAME=\"%s\" WIDTH=%d HEIGHT=%d ESCAPEMENT=%d ORIENTATION=%d WEIGHT=%d ITALIC=%d UNDERLINE=%d STRIKEOUT=%d CHARSET=%d OUTPRECISION=%d CLIPPRECISION=%d QUALITY=%d PITCHANDFAMILY=%d FACECLR=%u>\n",
			ID_Formatted, pFONT->tlfFaceName, pFONT->tlfWidth, pFONT->tlfHeight - 3, pFONT->tlfEscapement, pFONT->tlfOrientation, pFONT->tlfWeight, pFONT->tlfItalic, pFONT->tlfUnderline,
			pFONT->tlfStrikeOut, pFONT->tlfCharSet, pFONT->tlfOutPrecision, pFONT->tlfClipPrecision, pFONT->tlfQuality, pFONT->tlfPitchAndFamily, pFONT->tlfColor
		);

		CFile finalFonts("DECOMPILED_FONTS.tsc", CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		finalFonts.SeekToEnd();
		finalFonts.Write(formattedFont, formattedFont.GetLength());
		finalFonts.Close();
	}

	fclose(pFILE);

	CString allDefines = defineValues.GetAllDefines();
	CFile finalDefines("DECOMPILED_DEFINES.tsc", CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	finalDefines.Write(allDefines, allDefines.GetLength());
	finalDefines.Close();

	AfxMessageBox("ENDED !");
}

Node CTCMLParserDlg::ConvertFrame(FILE *pFILE, bool isFrame, int level)
{
	LP_FRAMEDESC pFRAME = new FRAMEDESC();
	LP_FRAMEDESC *pNEXT = NULL;
	char pBUF[MAX_TCML_SYMBOL];
	DWORD dwID = 0;
	int nCount = 0;

	fread(&pFRAME->m_vCOMP.m_dwID, sizeof(DWORD), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_bType, sizeof(BYTE), 1, pFILE);

	fread(&pFRAME->m_vCOMP.m_vMENU, sizeof(DWORD), TCML_MENU_COUNT, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwImageID, sizeof(DWORD), 2, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwTooltipID, sizeof(DWORD), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwFontID, sizeof(DWORD), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwStyle, sizeof(DWORD), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwCOLOR, sizeof(DWORD), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_dwSND, sizeof(DWORD), 1, pFILE);

	fread(&pFRAME->m_vCOMP.m_nMargineH, sizeof(int), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_nMargineV, sizeof(int), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_nPosX, sizeof(int), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_nPosY, sizeof(int), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_nWidth, sizeof(int), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_nHeight, sizeof(int), 1, pFILE);

	fread(&pFRAME->m_vCOMP.m_bDisplay, sizeof(BYTE), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_bAlign, sizeof(BYTE), 1, pFILE);
	fread(&pFRAME->m_vCOMP.m_vEX, sizeof(TSATR), 1, pFILE);

	fread(&nCount, sizeof(int), 1, pFILE);
	if (nCount > 0)
	{
		fread(pBUF, sizeof(char), nCount, pFILE);
		pBUF[nCount] = '\0';
		pFRAME->m_vCOMP.m_strTooltip.Format("%s", pBUF);
	}

	fread(&nCount, sizeof(int), 1, pFILE);
	if (nCount > 0)
	{
		fread(pBUF, sizeof(char), nCount, pFILE);
		pBUF[nCount] = '\0';
		pFRAME->m_vCOMP.m_strText.Format("%s", pBUF);
	}

	fread(&nCount, sizeof(int), 1, pFILE);
	pNEXT = &pFRAME->m_pCHILD;

	Node node = Node::FromFrameDesc(pFRAME->m_vCOMP, &defineValues, level);

	level++;
	for (int i = 0; i < nCount; i++)
	{
		Node nodeChild = ConvertFrame(pFILE, false, level);
		node.AddChild(nodeChild);
		//pNEXT = &(*pNEXT)->m_pNEXT;
	}

	return node;
}