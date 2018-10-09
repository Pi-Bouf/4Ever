// TCMLParserDlg.h : header file
//

#pragma once
#include <vector>
#include "afxwin.h"
#include "DefineValues.h"
#include "DefineValue.h"
#include "Nodes.h"
#include "Node.h"


// CTCMLParserDlg dialog
class CTCMLParserDlg : public CDialog
{
// Construction
public:
	CTCMLParserDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TCMLPARSER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;
	CString m_strHEADERFOLDER;

	CString m_strDEST;
	CString m_strSRC;
	CString m_strHEADER;

	CButton m_cDEST;
	CButton m_cSRC;
	CButton m_cHEADER;

private:
	DefineValues defineValues;
	Nodes nodes;

// Implementation
protected:
	HICON m_hIcon;

public:
	int GetNodeCount( LP_COMPDESC pCOMP);
	void WriteFRAME(
		CFile *pFILE,
		LP_COMPDESC pCOMP);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonSrc();
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonHeader();
	afx_msg void OnBnClickedCompile();
	afx_msg void OnBnClickedDecompile();
	void LoadHeaders();
	void LoadFrames();
	Node ConvertFrame(FILE *pFILE, bool isFrame, int level);
	CString GetFormattedType(byte id);
};
