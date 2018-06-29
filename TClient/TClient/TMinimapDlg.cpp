#include "StdAfx.h"
#include "TClientGame.h"

LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMINIMAP = NULL;
LPDIRECT3DTEXTURE9 CTMinimapDlg::m_pTMASK = NULL;


CTMinimapDlg::CTMinimapDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
:CTClientUIBase( pParent, pDesc)
{
#ifdef NEW_IF
	static DWORD dwCompID[] = {
		ID_CTRLINST_FRAME, 
		ID_CTRLINST_MINIMAP_NEW,
		ID_CTRLINST_ZOOM}; 

	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
		m_vTCOMP[i] = FindKid(dwCompID[i]);

	m_bMinimize = FALSE;
	m_bSND = FALSE;

	m_pDevice = NULL;
	m_pTMON = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;

	TComponent *pTAREA = FindKid(ID_CTRLINST_AREA);
	pTAREA->GetComponentRect(&m_rcAREA);

	RemoveKid(pTAREA);
	delete pTAREA;

	m_pTZOOMSCROLL = (TScroll*) m_vTCOMP[TMINIMAPCOMP_ZOOM];
	m_pTZOOMSCROLL->SetScrollPos((INT)TMINIMAP_SCALE_COUNT, (INT)TMINIMAP_SCALE_COUNT);
#else
	static DWORD dwCompID[] = {
		ID_CTRLINST_MAX, //ID_CTRLINST_FRAME
		ID_CTRLINST_WORLD, //
		ID_CTRLINST_MINIMAP, //26272
		ID_CTRLINST_ZOOM}; //"

	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
		m_vTCOMP[i] = FindKid(dwCompID[i]);

	m_pTITLE = FindKid(ID_CTRLINST_MAX_TITLE); //TITLE
	m_pMINMAXBTN = FindKid(ID_CTRLINST_MAX_BTN); //MINIMIZE
	m_pTOPFRAME = FindKid(ID_CTRLINST_MIN); //Opframe

	m_bMinimize = FALSE;
	m_bSND = FALSE;

	m_pDevice = NULL;
	m_pTMON = NULL;
	m_pHost = NULL;
	m_pTMAP = NULL;
	m_pCAM = NULL;

	TComponent *pTAREA = FindKid(ID_CTRLINST_AREA);
	pTAREA->GetComponentRect(&m_rcAREA);

	RemoveKid(pTAREA);
	delete pTAREA;

	m_pTZOOMSCROLL = (TScroll*) m_vTCOMP[TMINIMAPCOMP_ZOOM];
	m_pTZOOMSCROLL->SetScrollPos((INT)TMINIMAP_SCALE_COUNT, (INT)TMINIMAP_SCALE_COUNT);
#endif
}

CTMinimapDlg::~CTMinimapDlg()
{
}

void CTMinimapDlg::ResetVisible()
{
	for( BYTE i=0; i<TMINIMAPCOMP_COUNT; i++)
	{
		m_vTCOMP[i]->EnableComponent(!m_bMinimize);
		m_vTCOMP[i]->ShowComponent(!m_bMinimize);
	}
}

BOOL CTMinimapDlg::CanWithItemUI()
{
	return TRUE;
}

BOOL CTMinimapDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( m_bMinimize )
		return FALSE;

	CRect rc = m_rc;
	ComponentToScreen(&rc);
	if(!rc.PtInRect(pt))
		return FALSE;

	int nRange, nPos;
	m_pTZOOMSCROLL->GetScrollPos(nRange, nPos);

	nPos += zDelta > 0 ? -1 : 1;
	nPos = min( max( nPos, 0), nRange);
	m_pTZOOMSCROLL->SetScrollPos(nRange, nPos);

	return TRUE;
}

BOOL CTMinimapDlg::HitTest( CPoint pt)
{
	CRect rect(m_rcAREA);

	rect.OffsetRect(m_rc.TopLeft());
	ComponentToScreen(&rect);

	if(rect.PtInRect(pt))
		return TRUE;

	return CTClientUIBase::HitTest(pt);
}

void CTMinimapDlg::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent(bVisible);

	if(m_bVisible)
		ResetVisible();
}

void CTMinimapDlg::RenderTMINIMAP()
{
#ifdef NEW_IF
	int nRng;
	int nPos;
	static const int CIRCLE_RESOLUTION = 64;
	static const int RADIUS = 98;

	m_pTZOOMSCROLL->GetScrollPos(nRng, nPos);
	m_pTMAP->m_fTSCALE = TMINIMAP_INIT_SCALE / powf(TMINIMAP_SCALE_FACTOR, (FLOAT)nPos);

	CRect rect(
		0, 0,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE);

	rect.OffsetRect(
		m_rc.left + m_rcAREA.left - (TMINIMAPTEX_SIZE - m_rcAREA.Width()) / 2,
		m_rc.top + m_rcAREA.top - (TMINIMAPTEX_SIZE - m_rcAREA.Height()) / 2);

	CPoint point((rect.Width() / 2) + rect.left, (rect.Height() / 2) + rect.top);

	TNLVERTEX vRECT[CIRCLE_RESOLUTION + 1];
	FLOAT fAngle = (FLOAT)( ( 2 * D3DX_PI ) / CIRCLE_RESOLUTION );

	for (int i = 0; i < CIRCLE_RESOLUTION + 1; i++)
	{
		FLOAT fTheta = (FLOAT)( i * fAngle ); 
	
		vRECT[i].m_fPosX = point.x + RADIUS * cos(D3DX_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
		vRECT[i].m_fPosY = point.y + RADIUS * sin(D3DX_PI * (i / (CIRCLE_RESOLUTION / 2.0f)));
		vRECT[i].m_fPosZ = 0;
		vRECT[i].m_fRHW = 1.0f;
		vRECT[i].m_dwColor = 0xFFFFFFFF;
		vRECT[i].m_fU = (FLOAT)( 0.5 + 0.5 * cos( -fTheta ));
		vRECT[i].m_fV = (FLOAT)( 0.5 - 0.5 * sin( -fTheta ));
	}
	FLOAT fMIP = 0.0f;
 
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->m_pDevice->SetTexture(0, m_pTMINIMAP);

    m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);
    m_pDevice->m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, CIRCLE_RESOLUTION, &vRECT, sizeof(TNLVERTEX));
#else
	int nRng;
	int nPos;

	m_pTZOOMSCROLL->GetScrollPos(nRng, nPos);
	m_pTMAP->m_fTSCALE = TMINIMAP_INIT_SCALE / powf(TMINIMAP_SCALE_FACTOR, (FLOAT)nPos);

	CRect rect(
		0, 0,
		TMINIMAPTEX_SIZE,
		TMINIMAPTEX_SIZE);

	rect.OffsetRect(
		m_rc.left + m_rcAREA.left - (TMINIMAPTEX_SIZE - m_rcAREA.Width()) / 2,
		m_rc.top + m_rcAREA.top - (TMINIMAPTEX_SIZE - m_rcAREA.Height()) / 2);

	TNLVERTEX vRECT[4] = {
		{ FLOAT(rect.left), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 0.0f},
		{ FLOAT(rect.right), FLOAT(rect.top), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f},
		{ FLOAT(rect.left), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 0.0f, 1.0f},
		{ FLOAT(rect.right), FLOAT(rect.bottom), 0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 1.0f}};
	FLOAT fMIP = 0.0f;

	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &fMIP));
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pDevice->m_pDevice->SetTexture( 0, m_pTMINIMAP);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_TNLVERTEX);

	m_pDevice->m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP,
		2, vRECT, sizeof(TNLVERTEX));
#endif
}

HRESULT CTMinimapDlg::Render( DWORD dwTickCount)
{
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	if( !pMainChar->m_bFlash )
	{
		if( m_bVisible && !m_bMinimize )
		{
			m_pTMAP->ResetTMINIMAP(
				m_pTMINIMAP,
				m_pTMON,
				m_pTRSCS,
				m_pHost,
				m_pDevice,
				m_pCAM);
			RenderTMINIMAP();
		}

		return CTClientUIBase::Render(dwTickCount);
	}

	return S_OK;
}
