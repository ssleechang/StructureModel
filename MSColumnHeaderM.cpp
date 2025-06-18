#include "StdAfx.h"
#include "MSColumnHeaderM.h"
#include "..\GMLib\MSVersionInfo.h"


IMPLEMENT_SERIAL(MSColumnHeaderM, MSCompoM, VERSIONABLE_SCHEMA | 1)

MSColumnHeaderM::MSColumnHeaderM(void)
{
	CString sBar = _T("");
	m_nType = msColumnHeaderM;

	m_HeaderType = 0;
	m_Direction = 0;
	m_L1 = 250;
	m_L2 = 250;
	m_L3 = 0;
	m_L4 = 0;
	m_L5 = 0;

	m_T1 = 500;
	m_T2 = 200;
	m_T3 = 0;
	m_T4 = 0;
	m_T5 = 0;

	sBar = MSDeformedBar::GetMainBarStr(4);
	m_LxMainBar.InitData(sBar);
	m_LxAddBar.InitData(sBar);
	m_LyMainBar.InitData(sBar);
	m_LyAddBar.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(150);
	m_StirrupBar.InitData(sBar);
	m_IsNumPlaceBar = false;
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_StirrupMainBar.InitData(sBar);

	m_PlaceRebar = true;
}

MSColumnHeaderM::~MSColumnHeaderM(void)
{
}

void MSColumnHeaderM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSCompoM::CopyFromMe(pSource);

	MSColumnHeaderM* pSourceM = dynamic_cast<MSColumnHeaderM*>(pSource);
	if (pSourceM == NULL) return;

	m_HeaderType = pSourceM->m_HeaderType;
	m_Direction = pSourceM->m_Direction;
	m_L1 = pSourceM->m_L1;
	m_L2 = pSourceM->m_L2;
	m_L3 = pSourceM->m_L3;
	m_L4 = pSourceM->m_L4;
	m_L5 = pSourceM->m_L5;

	m_T1 = pSourceM->m_T1;
	m_T2 = pSourceM->m_T2;
	m_T3 = pSourceM->m_T3;
	m_T4 = pSourceM->m_T4;
	m_T5 = pSourceM->m_T5;

	m_LxMainBar.CopyFromMe(&pSourceM->m_LxMainBar);
	m_StirrupBar.CopyFromMe(&pSourceM->m_StirrupBar);
	m_IsNumPlaceBar = pSourceM->m_IsNumPlaceBar;
	m_StirrupMainBar.CopyFromMe(&pSourceM->m_StirrupMainBar);

	m_LxAddBar.CopyFromMe(&pSourceM->m_LxAddBar);
	m_LyMainBar.CopyFromMe(&pSourceM->m_LyMainBar);
	m_LyAddBar.CopyFromMe(&pSourceM->m_LyAddBar);

	m_PlaceRebar = pSourceM->m_PlaceRebar;
}

bool MSColumnHeaderM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSCompoM::IsGeometryDifferent(compoM))
		return true;

	MSColumnHeaderM* other = dynamic_cast<MSColumnHeaderM*>(compoM);
	if (other == NULL) return true;

	if (m_Direction != other->m_Direction)	return true;
	if (m_L1 != other->m_L1)	return true;
	if (m_L2 != other->m_L2)	return true;
	if (m_L3 != other->m_L3)	return true;
	if (m_L4 != other->m_L4)	return true;
	if (m_L5 != other->m_L5)	return true;
	if (m_T1 != other->m_T1)	return true;
	if (m_T2 != other->m_T2)	return true;
	if (m_T3 != other->m_T3)	return true;
	if (m_T4 != other->m_T4)	return true;
	if (m_T5 != other->m_T5)	return true;

	return false;
}

void MSColumnHeaderM::Dump( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Dump(buffer);
	buffer << m_HeaderType;
	buffer << m_Direction;
	buffer << m_L1 << m_L2 << m_L3 << m_L4;
	buffer << m_T1 << m_T2 << m_T3 << m_T4;

	m_LxMainBar.Dump(buffer);
	m_StirrupBar.Dump(buffer);
	buffer << m_IsNumPlaceBar;
	m_StirrupMainBar.Dump(buffer);

	m_LxAddBar.Dump(buffer);
	m_LyMainBar.Dump(buffer);
	m_LyAddBar.Dump(buffer);

	buffer << m_PlaceRebar;

	buffer << m_L5;
	buffer << m_T5;
}

void MSColumnHeaderM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Recovery(buffer);
	buffer >> m_HeaderType;
	buffer >> m_Direction;
	buffer >> m_L1 >> m_L2 >> m_L3 >> m_L4;
	buffer >> m_T1 >> m_T2 >> m_T3 >> m_T4;

	m_LxMainBar.Recovery(buffer);
	m_StirrupBar.Recovery(buffer);
	buffer >> m_IsNumPlaceBar;
	m_StirrupMainBar.Recovery(buffer);

	m_LxAddBar.Recovery(buffer);
	m_LyMainBar.Recovery(buffer);
	m_LyAddBar.Recovery(buffer);

	buffer >> m_PlaceRebar;
	buffer >> m_L5;
	buffer >> m_T5;
}

void MSColumnHeaderM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_HeaderType;
		ar << m_Direction;
		ar << m_L1;
		ar << m_L2;
		ar << m_L3;
		ar << m_L4;
		ar << m_T1;
		ar << m_T2;
		ar << m_T3;
		ar << m_T4;

		m_LxMainBar.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		ar << m_IsNumPlaceBar;
		m_StirrupMainBar.Serialize(ar);

		m_LxAddBar.Serialize(ar);
		m_LyMainBar.Serialize(ar);
		m_LyAddBar.Serialize(ar);

		ar << m_PlaceRebar;
		ar << m_L5;
		ar << m_T5;
	}
	else
	{
		ar >> m_HeaderType;
		ar >> m_Direction;
		ar >> m_L1;
		ar >> m_L2;
		ar >> m_L3;
		ar >> m_L4;
		ar >> m_T1;
		ar >> m_T2;
		ar >> m_T3;
		ar >> m_T4;

		m_LxMainBar.Serialize(ar);
		m_StirrupBar.Serialize(ar);

		if (m_StirrupBar.GetNumLayers() > 1)
			m_StirrupBar.OnlyFirstDiaSet();

		ar >> m_IsNumPlaceBar;
		m_StirrupMainBar.Serialize(ar);

		if (MSVersionInfo::GetCurrentVersion() >= 20160718)
		{
			m_LxAddBar.Serialize(ar);
			m_LyMainBar.Serialize(ar);
			m_LyAddBar.Serialize(ar);
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20221011)
		{
			ar >> m_PlaceRebar;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20240626)
		{
			ar >> m_L5;
			ar >> m_T5;
		}
	}
}
