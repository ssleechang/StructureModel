#include "StdAfx.h"
#include "MSOpeningReinforceM.h"

#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSOpeningReinforceM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSOpeningReinforceM::MSOpeningReinforceM(void)
{
	m_Usage = Opening;
	m_CalcBy = User;
	m_Layer = Double;
	m_IsReinforceBar1 = true;
	m_IsReinforceBar2 = true;

	CString sBar = MSDeformedBar::GetMainBarStr(2);
	m_ReinforceBar1_Top.InitData(sBar);
	m_ReinforceBar2_Top.InitData(sBar);

	m_IsUsedMainBarDia1 = true;
	m_IsUsedMainBarDia2 = true;
	m_IsCalcFormArea = false;
	m_IsCalcConcVolumn = false;

	m_IsSameTopBotBar = true;
	m_ReinforceBar1_Bot.InitData(sBar);
	m_ReinforceBar2_Bot.InitData(sBar);
}


MSOpeningReinforceM::~MSOpeningReinforceM(void)
{
}

void MSOpeningReinforceM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSCompoM::CopyFromMe(pSource);

	MSOpeningReinforceM* pSourceM = (MSOpeningReinforceM*)pSource;

	m_Usage = pSourceM->m_Usage;
	m_CalcBy = pSourceM->m_CalcBy;
	m_Layer = pSourceM->m_Layer;
	m_IsReinforceBar1 = pSourceM->m_IsReinforceBar1;
	m_IsReinforceBar2 = pSourceM->m_IsReinforceBar2;

	m_ReinforceBar1_Top.CopyFromMe(&pSourceM->m_ReinforceBar1_Top);
	m_ReinforceBar2_Top.CopyFromMe(&pSourceM->m_ReinforceBar2_Top);

	m_IsUsedMainBarDia1 = pSourceM->m_IsUsedMainBarDia1;
	m_IsUsedMainBarDia2 = pSourceM->m_IsUsedMainBarDia2;
	m_IsCalcFormArea = pSourceM->m_IsCalcFormArea;
	m_IsCalcConcVolumn = pSourceM->m_IsCalcConcVolumn;

	m_IsSameTopBotBar = pSourceM->GetIsSameTopBotBar();
	m_ReinforceBar1_Bot.CopyFromMe(&pSourceM->m_ReinforceBar1_Bot);
	m_ReinforceBar2_Bot.CopyFromMe(&pSourceM->m_ReinforceBar2_Bot);
}

void MSOpeningReinforceM::Dump( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Dump(buffer);
	
	int usage = (int)m_Usage;
	int calcBy = (int)m_CalcBy;
	int layer = (int)m_Layer;
	buffer << usage << calcBy << layer;
	buffer << m_IsReinforceBar1	<< m_IsReinforceBar2;

	m_ReinforceBar1_Top.Dump(buffer);
	m_ReinforceBar2_Top.Dump(buffer);

	buffer << m_IsUsedMainBarDia1 << m_IsUsedMainBarDia2;
	buffer << m_IsCalcFormArea << m_IsCalcConcVolumn;

	buffer << m_IsSameTopBotBar;
	m_ReinforceBar1_Bot.Dump(buffer);
	m_ReinforceBar2_Bot.Dump(buffer);
}

void MSOpeningReinforceM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSCompoM::Recovery(buffer);

	int usage = 0;
	int calcBy = 0;
	int layer = 0;
	buffer >> usage >> calcBy >> layer;
	buffer >> m_IsReinforceBar1	>> m_IsReinforceBar2;

	m_Usage = (eUsage)usage;
	m_CalcBy = (eCalcBy)calcBy;
	m_Layer = (eLayer)layer;

	m_ReinforceBar1_Top.Recovery(buffer);
	m_ReinforceBar2_Top.Recovery(buffer);

	buffer >> m_IsUsedMainBarDia1 >> m_IsUsedMainBarDia2;
	buffer >> m_IsCalcFormArea >> m_IsCalcConcVolumn;

	buffer >> m_IsSameTopBotBar;
	m_ReinforceBar1_Bot.Recovery(buffer);
	m_ReinforceBar2_Bot.Recovery(buffer);
}
void MSOpeningReinforceM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	int usage = 0;
	int calcBy = 0;
	int layer = 0;

	if(ar.IsStoring())
	{
		usage = (int)m_Usage;
		calcBy = (int)m_CalcBy;
		layer = (int)m_Layer;
		ar << usage;
		ar << calcBy;
		ar << layer;
		ar << m_IsReinforceBar1;
		ar << m_IsReinforceBar2;
		m_ReinforceBar1_Top.Serialize(ar);
		m_ReinforceBar2_Top.Serialize(ar);

		ar << m_IsUsedMainBarDia1;
		ar << m_IsUsedMainBarDia2;

		ar << m_IsCalcFormArea;
		ar << m_IsCalcConcVolumn;

		ar << m_IsSameTopBotBar;
		m_ReinforceBar1_Bot.Serialize(ar);
		m_ReinforceBar2_Bot.Serialize(ar);
	}
	else
	{
		ar >> usage;
		ar >> calcBy;
		ar >> layer;
		ar >> m_IsReinforceBar1;
		ar >> m_IsReinforceBar2;

		m_Usage = (eUsage)usage;
		m_CalcBy = (eCalcBy)calcBy;
		m_Layer = (eLayer)layer;
		m_ReinforceBar1_Top.Serialize(ar);
		m_ReinforceBar2_Top.Serialize(ar);

		if(MSVersionInfo::GetCurrentVersion() >= 20150407)
		{
			ar >> m_IsUsedMainBarDia1;
			ar >> m_IsUsedMainBarDia2;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20150502)
		{
			ar >> m_IsCalcFormArea;
			ar >> m_IsCalcConcVolumn;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240416)
		{
			ar >> m_IsSameTopBotBar;
			m_ReinforceBar1_Bot.Serialize(ar);
			m_ReinforceBar2_Bot.Serialize(ar);
		}
	}
}

MSOpeningReinforceM::eUsage MSOpeningReinforceM::GetUsage()
{
	return m_Usage;
}

MSOpeningReinforceM::eCalcBy MSOpeningReinforceM::GetCalcBy()
{
	return m_CalcBy;
}

MSOpeningReinforceM::eLayer	MSOpeningReinforceM::GetLayer()
{
	return m_Layer;
}

bool MSOpeningReinforceM::GetIsReinforceBar1()
{
	return m_IsReinforceBar1;
}

bool MSOpeningReinforceM::GetIsReinforceBar2()
{
	return m_IsReinforceBar2;
}

bool MSOpeningReinforceM::GetIsUsedMainBarDia1()
{
	return m_IsUsedMainBarDia1;
}

bool MSOpeningReinforceM::GetIsUsedMainBarDia2()
{
	return m_IsUsedMainBarDia2;
}

bool MSOpeningReinforceM::GetIsCalcFormArea()
{
	return m_IsCalcFormArea;
}

bool MSOpeningReinforceM::GetIsCalcConcVolumn()
{
	return m_IsCalcConcVolumn;
}

void MSOpeningReinforceM::SetUsage(eUsage usage)
{
	m_Usage = usage;
}
void MSOpeningReinforceM::SetCalcBy(eCalcBy calcBy)
{
	m_CalcBy = calcBy;
}
void MSOpeningReinforceM::SetLayer(eLayer layer)
{
	m_Layer = layer;
}
void MSOpeningReinforceM::SetIsReinforceBar1(bool isReinforceBar)
{
	m_IsReinforceBar1 = isReinforceBar;
}
void MSOpeningReinforceM::SetIsReinforceBar2(bool isReinforceBar)
{
	m_IsReinforceBar2 = isReinforceBar;
}

void MSOpeningReinforceM::SetIsUsedMainBarDia1(bool isUsedMainBarDia)
{
	m_IsUsedMainBarDia1 = isUsedMainBarDia;
}

void MSOpeningReinforceM::SetIsUsedMainBarDia2(bool isUsedMainBarDia)
{
	m_IsUsedMainBarDia2 = isUsedMainBarDia;
}

void MSOpeningReinforceM::SetIsCalcFormArea(bool isCale)
{
	m_IsCalcFormArea = isCale;
}

void MSOpeningReinforceM::SetIsCalcConcVolumn(bool isCalc)
{
	m_IsCalcConcVolumn = isCalc;
}

bool MSOpeningReinforceM::GetIsSameTopBotBar()
{
	return m_IsSameTopBotBar;
}
void MSOpeningReinforceM::SetIsSameTopBotBar(bool value)
{
	m_IsSameTopBotBar = value;
}