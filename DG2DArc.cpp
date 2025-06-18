#include "StdAfx.h"
#include "DG2DArc.h"

#include "DG2DLine.h"

#include "GMLib\GMQuadTree.h"
#include "GMLib\GM2DArc.h"

#include <math.h>

DG2DArc::DG2DArc(GMQuadTree* pQT)
{
	m_pQT = pQT;
	Init();
}

DG2DArc::~DG2DArc(void)
{
}

void DG2DArc::Init()
{
	m_bOutArc = FALSE;
	m_dT = 200;
	m_pArc = NULL;
}

void DG2DArc::SetArc(GM2DArc* pArc)
{
	m_pArc = pArc;
	AddGMArc(pArc);
}

// 중심점이 같고, 각도의 차가 DegreeTol이내일때
BOOL DG2DArc::MergeWhenSameArc(GM2DArc* pNewArc, double DegreeTol, double DistTol)
{
	if(m_pArc->m_Center.SameVector4DTOL(pNewArc->m_Center, DistTol)==FALSE)
		return FALSE;

	if(fabs(m_pArc->m_A-pNewArc->m_A) > DistTol)
		return FALSE;

	long Result = m_pArc->Overlap4DTol(*pNewArc, DegreeTol, DistTol);
	if(Result==0)
		return FALSE;
	switch(Result)
	{
	case 1:
		return FALSE;
	case 2:
	case 3:
	case 6:
		m_pArc->m_Theta1 = pNewArc->m_Theta1;
		pNewArc->m_Tag = -1;
		break;
	case 4:
	case 5:
	case 7:
	case 8:
		break;
	case 9:
		m_pArc->m_Theta1 = pNewArc->m_Theta1;
		m_pArc->m_Theta2 = pNewArc->m_Theta2;
		pNewArc->m_Tag = -1;
		break;
	case 10:
	case 11:
	case 12:
		m_pArc->m_Theta2 = pNewArc->m_Theta2;
		pNewArc->m_Tag = -1;
		break;
	default:
		return FALSE;
	}

	AddGMArc(pNewArc);
	return TRUE;
}

void DG2DArc::AddGMArc(GM2DArc* pNewArc)
{
	ma_GMs.Add(pNewArc);
	AddQTs(pNewArc);
}

void DG2DArc::AddQTs(GM2DArc* pNewArc)
{
	vector<GMQuadTree*> NewQTs = m_pQT->FindCrossQts(pNewArc);
	vector<GMQuadTree*> MyQTs = m_pQT->FindCrossQts(this);
	long NumNewQT = NewQTs.size();
	long NumMyQT = MyQTs.size();

	for(long QTNum=0; QTNum<NumNewQT; QTNum++)
	{
		BOOL IsExist = FALSE;
		for(long MyQTNum=0; MyQTNum<NumMyQT; MyQTNum++)
		{
			if(MyQTs[MyQTNum] == NewQTs[QTNum])
			{
				IsExist = TRUE;
				break;
			}
		}
		if(IsExist==FALSE)
			m_pQT->AddCrossQuadTree(this, NewQTs[QTNum]);
	}
}
