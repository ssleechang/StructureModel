#include "StdAfx.h"
#include "MSObject.h"
#include "ButtressColumnRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "ColumnVerBarRebarTemplate.h"
#include "ColumnDowelRebarTemplate.h"

ButtressColumnRebarTemplate::ButtressColumnRebarTemplate() : BaseRebarTemplate()
{
	DataInit();
	m_pVerBarTemplateBelowDia = new ColumnVerBarRebarTemplate();
	m_pVerBarTemplateExcessDia = new ColumnVerBarRebarTemplate();
	m_pDowelTemplateBelowDia = new ColumnDowelRebarTemplate();
	m_pDowelTemplateExcessDia = new ColumnDowelRebarTemplate();
}

ButtressColumnRebarTemplate::~ButtressColumnRebarTemplate(void)
{
	delete m_pVerBarTemplateBelowDia;
	delete m_pVerBarTemplateExcessDia;
	delete m_pDowelTemplateBelowDia;
	delete m_pDowelTemplateExcessDia;
}

void ButtressColumnRebarTemplate::DataInit()
{
	CStringArray BarDiaArr;
	MSDeformedBar::GetRebarNameList(BarDiaArr);

	m_nButtressColumnVerBarPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	m_nButtressColumnDowelPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	
	InitUserDowelHookLenMap(mm_ButtressColumnUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_ButtressColumnUserDowelSpliceLen);

	InitUserDiaIndexMap(mm_ButtressColumnDowelMainBarUserDiaIndex);
	InitUserDiaIndexMap(mm_ButtressColumnDowelHoopBarUserDiaIndex);

	
	m_bButtressColumnDowelMainBarUserDiaSet = FALSE;
	m_bButtressColumnDowelHoopBarUserDiaSet = FALSE;

	m_bButtressColumnDowelMainBarUserSteelGradeS = FALSE;
	m_bButtressColumnDowelHoopBarUserSteelGradeS = FALSE;
}

void ButtressColumnRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	ButtressColumnRebarTemplate* pButtressColumnRebarTemplate = dynamic_cast<ButtressColumnRebarTemplate*>(pBaseRebarTemplate);
	if(pButtressColumnRebarTemplate == nullptr)
		return;

	m_nButtressColumnVerBarPlaceTypeDiaIndex = pButtressColumnRebarTemplate->GetButtressColumnVerBarPlaceTypeDiaIndex();
	m_nButtressColumnDowelPlaceTypeDiaIndex = pButtressColumnRebarTemplate->GetButtressColumnDowelPlaceTypeDiaIndex();

	m_pVerBarTemplateBelowDia->CopyFromMe(pButtressColumnRebarTemplate->GetButtressColumnVerBarTemplateBelowDia());
	m_pVerBarTemplateExcessDia->CopyFromMe(pButtressColumnRebarTemplate->GetButtressColumnVerBarTemplateExcessDia());

	m_pDowelTemplateBelowDia->CopyFromMe(pButtressColumnRebarTemplate->GetButtressColumnDowelTemplateBelowDia());
	m_pDowelTemplateExcessDia->CopyFromMe(pButtressColumnRebarTemplate->GetButtressColumnDowelTemplateExcessDia());
	
	UserDowelHookLenMapDataCopy(mm_ButtressColumnUserDowelHookLen, pButtressColumnRebarTemplate->mm_ButtressColumnUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_ButtressColumnUserDowelSpliceLen, pButtressColumnRebarTemplate->mm_ButtressColumnUserDowelSpliceLen);
	
	UserMapDataCopy(mm_ButtressColumnDowelMainBarUserDiaIndex, pButtressColumnRebarTemplate->mm_ButtressColumnDowelMainBarUserDiaIndex);
	UserMapDataCopy(mm_ButtressColumnDowelHoopBarUserDiaIndex, pButtressColumnRebarTemplate->mm_ButtressColumnDowelHoopBarUserDiaIndex);

	
	m_bButtressColumnDowelMainBarUserDiaSet = pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserDiaSet();
	m_bButtressColumnDowelHoopBarUserDiaSet = pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserDiaSet();

	m_bButtressColumnDowelMainBarUserSteelGradeS = pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserSteelGradeS();
	m_bButtressColumnDowelHoopBarUserSteelGradeS = pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserSteelGradeS();
}

void ButtressColumnRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{		
		map<long, long>::iterator it;
		ar << (int)mm_ButtressColumnUserDowelSpliceLen.size();
		for (it = mm_ButtressColumnUserDowelSpliceLen.begin(); it != mm_ButtressColumnUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << (int)mm_ButtressColumnUserDowelHookLen.size();
		for (it = mm_ButtressColumnUserDowelHookLen.begin(); it != mm_ButtressColumnUserDowelHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		
		ar << m_nButtressColumnVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar << m_nButtressColumnDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);

		ar << m_bButtressColumnDowelMainBarUserDiaSet;
		ar << m_bButtressColumnDowelHoopBarUserDiaSet;

		ar << (int)mm_ButtressColumnDowelMainBarUserDiaIndex.size();
		for(it = mm_ButtressColumnDowelMainBarUserDiaIndex.begin(); it != mm_ButtressColumnDowelMainBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}

		ar << (int)mm_ButtressColumnDowelHoopBarUserDiaIndex.size();
		for(it = mm_ButtressColumnDowelHoopBarUserDiaIndex.begin(); it != mm_ButtressColumnDowelHoopBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}

		ar << m_bButtressColumnDowelMainBarUserSteelGradeS;
		ar << m_bButtressColumnDowelHoopBarUserSteelGradeS;
	}
	else
	{
		int NumObject;
		mm_ButtressColumnUserDowelSpliceLen.clear();
		ar >> NumObject;
		if (NumObject > 0)
			mm_ButtressColumnUserDowelSpliceLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_ButtressColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		mm_ButtressColumnUserDowelHookLen.clear();
		ar >> NumObject;
		if (NumObject > 0)
			mm_ButtressColumnUserDowelHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_ButtressColumnUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		ar >> m_nButtressColumnVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar >> m_nButtressColumnDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);
		
		ar >> m_bButtressColumnDowelMainBarUserDiaSet;
		ar >> m_bButtressColumnDowelHoopBarUserDiaSet;

		ar >> NumObject;
		if (NumObject > 0)
			mm_ButtressColumnDowelMainBarUserDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long key = 0, value = 0;
			ar >> key;
			ar >> value;
			mm_ButtressColumnDowelMainBarUserDiaIndex.insert(make_pair(key, value));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_ButtressColumnDowelHoopBarUserDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long key = 0, value = 0;
			ar >> key;
			ar >> value;
			mm_ButtressColumnDowelHoopBarUserDiaIndex.insert(make_pair(key, value));
		}		

		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bButtressColumnDowelMainBarUserSteelGradeS;
			ar >> m_bButtressColumnDowelHoopBarUserSteelGradeS;
		}
	}
}
ColumnVerBarRebarTemplate *ButtressColumnRebarTemplate::GetButtressColumnVerBarTemplateBelowDia()
{
	return m_pVerBarTemplateBelowDia;
}
ColumnVerBarRebarTemplate *ButtressColumnRebarTemplate::GetButtressColumnVerBarTemplateExcessDia()
{
	return m_pVerBarTemplateExcessDia;
}
ColumnDowelRebarTemplate *ButtressColumnRebarTemplate::GetButtressColumnDowelTemplateBelowDia()
{
	return m_pDowelTemplateBelowDia;
}
ColumnDowelRebarTemplate *ButtressColumnRebarTemplate::GetButtressColumnDowelTemplateExcessDia()
{
	return m_pDowelTemplateExcessDia;
}
double ButtressColumnRebarTemplate::GetButtressColumnUserDowelHookLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it = mm_ButtressColumnUserDowelHookLen.find(DiaIndex);
	if (it == mm_ButtressColumnUserDowelHookLen.end())
		return 0;
	return it->second;
}

long ButtressColumnRebarTemplate::GetButtressColumnUserDowelSpliceLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it;
	it = mm_ButtressColumnUserDowelSpliceLen.find(DiaIndex);
	if (it != mm_ButtressColumnUserDowelSpliceLen.end())
		return it->second;

	return 0;
}
double ButtressColumnRebarTemplate::GetButtressColumnDowelMainBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_ButtressColumnDowelMainBarUserDiaIndex.find(DiaIndex);
	if (it == mm_ButtressColumnDowelMainBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double ButtressColumnRebarTemplate::GetButtressColumnDowelHoopBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_ButtressColumnDowelHoopBarUserDiaIndex.find(DiaIndex);
	if (it == mm_ButtressColumnDowelHoopBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void ButtressColumnRebarTemplate::SetButtressColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_ButtressColumnUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_ButtressColumnUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void ButtressColumnRebarTemplate::SetButtressColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_ButtressColumnUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_ButtressColumnUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap)
{
	mm_ButtressColumnDowelMainBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelMainBarUserDiaIndexMap.begin(); it != dowelMainBarUserDiaIndexMap.end(); it++)
	{
		mm_ButtressColumnDowelMainBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap)
{
	mm_ButtressColumnDowelHoopBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelHoopBarUserDiaIndexMap.begin(); it != dowelHoopBarUserDiaIndexMap.end(); it++)
	{
		mm_ButtressColumnDowelHoopBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

long ButtressColumnRebarTemplate::GetButtressColumnVerBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetBarSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetBarSpliceType();
}
long ButtressColumnRebarTemplate::GetButtressColumnVerBarTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopLenType();
	else
		return m_pVerBarTemplateExcessDia->GetTopLenType();
}
long ButtressColumnRebarTemplate::GetButtressColumnVerBarTopFloorDevType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopFloorDevType();
	else
		return m_pVerBarTemplateExcessDia->GetTopFloorDevType();
}
long ButtressColumnRebarTemplate::GetButtressColumnVerBarConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetConstructionHeight();
	else
		return m_pVerBarTemplateExcessDia->GetConstructionHeight();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnVerBarAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceSet();
}
long ButtressColumnRebarTemplate::GetButtressColumnVerBarAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceDist();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetBarSpliceType();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetTopLenType();
	else
		return m_pDowelTemplateExcessDia->GetTopLenType();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetConstructionHeight();
	else
		return m_pDowelTemplateExcessDia->GetConstructionHeight();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceSet();
};
long ButtressColumnRebarTemplate::GetButtressColumnDowelAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceDist();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelHoopRebarType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopRebarType();
	else
		return m_pDowelTemplateExcessDia->GetHoopRebarType();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelHoopBySpace(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopBySpace();
	else
		return m_pDowelTemplateExcessDia->GetHoopBySpace();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelHoopByNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopByNum();
	else
		return m_pDowelTemplateExcessDia->GetHoopByNum();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarDiaIndex(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarDiaIndex();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarUserSteelGradeS(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarUserSteelGradeS();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLength();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarLengthType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLengthType();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLengthType();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarCorrectiveLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarCorrectiveLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarCorrectiveLength();
}
double ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarFootingThickMultiplicationValue();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarFootingThickMultiplicationValue();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelSupportBarNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarNum();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarNum();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelBotPlaceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotPlaceType();
	else
		return m_pDowelTemplateExcessDia->GetBotPlaceType();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelFootingPercentSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercentSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercentSet();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelFootingPercent(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercent();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercent();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelFootingCoverSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCoverSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingCoverSet();
}
long ButtressColumnRebarTemplate::GetButtressColumnDowelFootingCover(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCover();
	else
		return m_pDowelTemplateExcessDia->GetFootingCover();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnUserDowelHookLenSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nButtressColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetUserHookLenSet();
	else
		return m_pDowelTemplateExcessDia->GetUserHookLenSet();
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelMainBarUserDiaSet()
{
	return m_bButtressColumnDowelMainBarUserDiaSet;
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelMainBarUserDiaSet(BOOL value)
{
	m_bButtressColumnDowelMainBarUserDiaSet = value;
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelHoopBarUserDiaSet()
{
	return m_bButtressColumnDowelHoopBarUserDiaSet;
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelHoopBarUserDiaSet(BOOL value)
{
	m_bButtressColumnDowelHoopBarUserDiaSet = value;
}


BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelMainBarUserSteelGradeS()
{
	return m_bButtressColumnDowelMainBarUserSteelGradeS;
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelMainBarUserSteelGradeS(BOOL value)
{
	m_bButtressColumnDowelMainBarUserSteelGradeS = value;
}
BOOL ButtressColumnRebarTemplate::GetButtressColumnDowelHoopBarUserSteelGradeS()
{
	return m_bButtressColumnDowelHoopBarUserSteelGradeS;
}
void ButtressColumnRebarTemplate::SetButtressColumnDowelHoopBarUserSteelGradeS(BOOL value)
{
	m_bButtressColumnDowelHoopBarUserSteelGradeS = value;
}