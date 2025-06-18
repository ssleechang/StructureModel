#include "StdAfx.h"
#include "ColumnRebarTemplate.h"
#include "MSDeformedBar.h"
#include "MSObject.h"
#include "..\GMLib\MSVersionInfo.h"
#include "ColumnVerBarRebarTemplate.h"
#include "ColumnDowelRebarTemplate.h"
#include "ButtressColumnRebarTemplate.h"
#include "VerMemberLinkDowelTemplate.h"

ColumnRebarTemplate::ColumnRebarTemplate() : BaseRebarTemplate()
{
	DataInit();
	m_pVerBarTemplateBelowDia = new ColumnVerBarRebarTemplate();
	m_pVerBarTemplateExcessDia = new ColumnVerBarRebarTemplate();
	m_pDowelTemplateBelowDia = new ColumnDowelRebarTemplate();
	m_pDowelTemplateExcessDia = new ColumnDowelRebarTemplate();
	m_pButtressColumnRebarTemplate = new ButtressColumnRebarTemplate();
	m_pColumnVerMemberLinkDowelTemplate = new VerMemberLinkDowelTemplate();
}

ColumnRebarTemplate::~ColumnRebarTemplate(void)
{
	delete m_pVerBarTemplateBelowDia;
	delete m_pVerBarTemplateExcessDia;
	delete m_pDowelTemplateBelowDia;
	delete m_pDowelTemplateExcessDia;
	delete m_pButtressColumnRebarTemplate;
	delete m_pColumnVerMemberLinkDowelTemplate;
}

void ColumnRebarTemplate::DataInit()
{
	CStringArray BarDiaArr;
	MSDeformedBar::GetRebarNameList(BarDiaArr);

	m_nColumnVerBarPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	m_nColumnDowelPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
		
	m_bColumnEndHoopBarShapeSet = false;
	m_nColumnHoopBarShapeIdx = 1;
	m_nColumnTieBarShapeIdx = 1;
	m_bColumnHoopBarStartLengthSet = false;
	m_nColumnHoopBarStartLength = 50;
	m_nColumnTopMemberCover = 0;
	m_bColumnTieAddLengthSet = false;
	m_nColumnTieAddLength = 10;
	m_bColumnOutsideColumnTieBarDelete = FALSE;
	
	m_nColumnDowelHoopBarShapeIdx = 1;
	InitUserDowelHookLenMap(mm_ColumnUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_ColumnUserDowelSpliceLen);

	InitUserDiaIndexMap(mm_ColumnDowelMainBarUserDiaIndex);
	InitUserDiaIndexMap(mm_ColumnDowelHoopBarUserDiaIndex);

	
	m_bColumnDowelMainBarUserDiaSet = FALSE;
	m_bColumnDowelHoopBarUserDiaSet= FALSE;
	m_bColumnDowelMainBarUserSteelGradeS = FALSE;
	m_bColumnDowelHoopBarUserSteelGradeS = FALSE;
}

void ColumnRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	ColumnRebarTemplate* pColumnRebarTemplate = dynamic_cast<ColumnRebarTemplate*>(pBaseRebarTemplate);
	if(pColumnRebarTemplate == nullptr)
		return;

	m_nColumnVerBarPlaceTypeDiaIndex = pColumnRebarTemplate->GetColumnVerBarPlaceTypeDiaIndex();
	m_nColumnDowelPlaceTypeDiaIndex = pColumnRebarTemplate->GetColumnDowelPlaceTypeDiaIndex();

	m_pVerBarTemplateBelowDia->CopyFromMe(pColumnRebarTemplate->GetColumnVerBarTemplateBelowDia());
	m_pVerBarTemplateExcessDia->CopyFromMe(pColumnRebarTemplate->GetColumnVerBarTemplateExcessDia());

	m_pDowelTemplateBelowDia->CopyFromMe(pColumnRebarTemplate->GetColumnDowelTemplateBelowDia());
	m_pDowelTemplateExcessDia->CopyFromMe(pColumnRebarTemplate->GetColumnDowelTemplateExcessDia());


	m_bColumnEndHoopBarShapeSet = pColumnRebarTemplate->m_bColumnEndHoopBarShapeSet;
	m_nColumnHoopBarShapeIdx = pColumnRebarTemplate->m_nColumnHoopBarShapeIdx;
	m_nColumnTieBarShapeIdx = pColumnRebarTemplate->m_nColumnTieBarShapeIdx;
	m_bColumnHoopBarStartLengthSet = pColumnRebarTemplate->m_bColumnHoopBarStartLengthSet;
	m_nColumnHoopBarStartLength = pColumnRebarTemplate->m_nColumnHoopBarStartLength;
	m_nColumnTopMemberCover = pColumnRebarTemplate->m_nColumnTopMemberCover;
	m_bColumnTieAddLengthSet = pColumnRebarTemplate->m_bColumnTieAddLengthSet;
	m_nColumnTieAddLength = pColumnRebarTemplate->m_nColumnTieAddLength;
	m_bColumnOutsideColumnTieBarDelete = pColumnRebarTemplate->m_bColumnOutsideColumnTieBarDelete;

	m_nColumnDowelHoopBarShapeIdx = pColumnRebarTemplate->m_nColumnDowelHoopBarShapeIdx;
	
	UserDowelHookLenMapDataCopy(mm_ColumnUserDowelHookLen, pColumnRebarTemplate->mm_ColumnUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_ColumnUserDowelSpliceLen, pColumnRebarTemplate->mm_ColumnUserDowelSpliceLen);
	
	UserMapDataCopy(mm_ColumnDowelMainBarUserDiaIndex, pColumnRebarTemplate->mm_ColumnDowelMainBarUserDiaIndex);
	UserMapDataCopy(mm_ColumnDowelHoopBarUserDiaIndex, pColumnRebarTemplate->mm_ColumnDowelHoopBarUserDiaIndex);

	
	m_bColumnDowelMainBarUserDiaSet = pColumnRebarTemplate->GetColumnDowelMainBarUserDiaSet();
	m_bColumnDowelHoopBarUserDiaSet = pColumnRebarTemplate->GetColumnDowelHoopBarUserDiaSet();
	m_bColumnDowelMainBarUserSteelGradeS = pColumnRebarTemplate->GetColumnDowelMainBarUserSteelGradeS();
	m_bColumnDowelHoopBarUserSteelGradeS = pColumnRebarTemplate->GetColumnDowelHoopBarUserSteelGradeS();

	this->m_pButtressColumnRebarTemplate->CopyFromMe(pColumnRebarTemplate->m_pButtressColumnRebarTemplate);

	m_pColumnVerMemberLinkDowelTemplate->CopyFromMe(pColumnRebarTemplate->GetColumnVerMemberLinkDowelTemplate());
}

void ColumnRebarTemplate::Serialize(CArchive& ar)
{
	long AlterSpliceValue1 = 0, AlterSpliceValue2 = 0, DowelAlterSpliceValue1 = 0, DowelAlterSpliceValue2 = 0;
	long nColumnVerBarSpliceType = 0, nColumnVerBarTopLenType = 0, nColumnVerBarTopFloorDevType = 0, 
		nColumnVerBarConstructionHeight = 0, nColumnVerBarAlterSpliceDist = 0;
	BOOL bColumnVerBarAlterSpliceSet = FALSE;
	
	long nColumnDowelBarSpliceType = 0, nColumnDowelTopPlaceType = 0, nColumnDowelConstructionHeight = 0, nColumnDowelAlterSpliceDist = 0,
		nColumnDowelHoopRebarType = 0, nColumnDowelHoopBySpace = 0, nColumnDowelHoopByNum = 0, nColumnDowelSupportBarDiaIndex = 0,
		nColumnDowelSupportBarLengthType = 0, nColumnDowelSupportBarLength = 0, nColumnDowelSupportBarCorrectiveLength = 0,
		nColumnDowelSupportBarNum = 0, nColumnDowelBotPlaceType = 0, nColumnDowelFootingPercent = 0, nColumnDowelFootingCover = 0;
	BOOL bColumnDowelAlterSpliceSet = FALSE, bColumnDowelFootingPercentSet = FALSE, bColumnDowelFootingCoverSet = FALSE, bColumnUserDowelHookLen = FALSE;

	if(ar.IsStoring())
	{
		ar << nColumnVerBarSpliceType;
		ar << nColumnVerBarTopLenType;
		ar << bColumnVerBarAlterSpliceSet;
		ar << AlterSpliceValue1;
		ar << AlterSpliceValue2;

		ar << m_bColumnEndHoopBarShapeSet;
		ar << m_nColumnHoopBarShapeIdx;
		ar << m_nColumnTieBarShapeIdx;
		ar << m_bColumnHoopBarStartLengthSet;
		ar << m_nColumnHoopBarStartLength;
		ar << m_nColumnTopMemberCover;
		ar << m_bColumnTieAddLengthSet;
		ar << m_nColumnTieAddLength;
		
		ar << nColumnDowelBarSpliceType;
		ar << nColumnDowelTopPlaceType;
		ar << bColumnDowelAlterSpliceSet;
		ar << DowelAlterSpliceValue1;
		ar << DowelAlterSpliceValue2;
		ar << nColumnDowelHoopRebarType;
		ar << nColumnDowelHoopBySpace;
		ar << nColumnDowelHoopByNum;
		ar << nColumnDowelSupportBarDiaIndex;
		ar << nColumnDowelSupportBarLength;
		ar << nColumnDowelSupportBarNum;
		ar << nColumnDowelBotPlaceType;
		ar << bColumnDowelFootingPercentSet;
		ar << nColumnDowelFootingPercent;
		ar << bColumnDowelFootingCoverSet;
		ar << nColumnDowelFootingCover;
		
		map<long, long>::iterator it;
		ar << (int)mm_ColumnUserDowelSpliceLen.size();
		for (it = mm_ColumnUserDowelSpliceLen.begin(); it != mm_ColumnUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << bColumnUserDowelHookLen;

		ar << (int)mm_ColumnUserDowelHookLen.size();
		for (it = mm_ColumnUserDowelHookLen.begin(); it != mm_ColumnUserDowelHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << nColumnVerBarTopFloorDevType;
		ar << nColumnVerBarConstructionHeight;
		ar << nColumnVerBarAlterSpliceDist;
		ar << nColumnDowelConstructionHeight;
		ar << nColumnDowelAlterSpliceDist;

		ar << m_nColumnDowelHoopBarShapeIdx;
		
		ar << nColumnDowelSupportBarLengthType;
		ar << nColumnDowelSupportBarCorrectiveLength;

		ar << m_nColumnVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar << m_nColumnDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);

		ar << m_bColumnOutsideColumnTieBarDelete;

		
		ar << m_bColumnDowelMainBarUserDiaSet;
		ar << m_bColumnDowelHoopBarUserDiaSet;

		ar << (int)mm_ColumnDowelMainBarUserDiaIndex.size();
		for(it = mm_ColumnDowelMainBarUserDiaIndex.begin(); it != mm_ColumnDowelMainBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}

		ar << (int)mm_ColumnDowelHoopBarUserDiaIndex.size();
		for(it = mm_ColumnDowelHoopBarUserDiaIndex.begin(); it != mm_ColumnDowelHoopBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}
		m_pButtressColumnRebarTemplate->Serialize(ar);

		m_pColumnVerMemberLinkDowelTemplate->Serialize(ar);

		ar << m_bColumnDowelMainBarUserSteelGradeS;
		ar << m_bColumnDowelHoopBarUserSteelGradeS;
	}
	else
	{
		ar >> nColumnVerBarSpliceType;
		ar >> nColumnVerBarTopLenType;
		ar >> bColumnVerBarAlterSpliceSet;
		ar >> AlterSpliceValue1;
		ar >> AlterSpliceValue2;

		ar >> m_bColumnEndHoopBarShapeSet;
		ar >> m_nColumnHoopBarShapeIdx;
		ar >> m_nColumnTieBarShapeIdx;
		ar >> m_bColumnHoopBarStartLengthSet;
		ar >> m_nColumnHoopBarStartLength;
		ar >> m_nColumnTopMemberCover;
		ar >> m_bColumnTieAddLengthSet;
		ar >> m_nColumnTieAddLength;

		ar >> nColumnDowelBarSpliceType;
		ar >> nColumnDowelTopPlaceType;
		ar >> bColumnDowelAlterSpliceSet;
		ar >> DowelAlterSpliceValue1;
		ar >> DowelAlterSpliceValue2;
		ar >> nColumnDowelHoopRebarType;
		ar >> nColumnDowelHoopBySpace;
		ar >> nColumnDowelHoopByNum;
		ar >> nColumnDowelSupportBarDiaIndex;
		ar >> nColumnDowelSupportBarLength;
		ar >> nColumnDowelSupportBarNum;
		ar >> nColumnDowelBotPlaceType;
		ar >> bColumnDowelFootingPercentSet;
		ar >> nColumnDowelFootingPercent;
		ar >> bColumnDowelFootingCoverSet;
		ar >> nColumnDowelFootingCover;

		int NumObject;

		mm_ColumnUserDowelSpliceLen.clear();
		ar >> NumObject;
		if (NumObject > 0)
			mm_ColumnUserDowelSpliceLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_ColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20171027)
		{
			ar >> bColumnUserDowelHookLen;

			mm_ColumnUserDowelHookLen.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_ColumnUserDowelHookLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, hookLen = 0;

				ar >> nDiaIndex;
				ar >> hookLen;
				mm_ColumnUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
			}
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180426)
		{
			ar >> nColumnVerBarTopFloorDevType;
			ar >> nColumnVerBarConstructionHeight;
			ar >> nColumnVerBarAlterSpliceDist;
			ar >> nColumnDowelConstructionHeight;
			ar >> nColumnDowelAlterSpliceDist;
		}
		else
		{
			nColumnVerBarTopFloorDevType = 0;
			nColumnVerBarConstructionHeight = min(AlterSpliceValue1, AlterSpliceValue2);
			nColumnVerBarAlterSpliceDist = abs(AlterSpliceValue1 - AlterSpliceValue2);
			nColumnDowelConstructionHeight = min(DowelAlterSpliceValue1, DowelAlterSpliceValue2);
			nColumnDowelAlterSpliceDist = abs(DowelAlterSpliceValue1 - DowelAlterSpliceValue2);
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200114)
		{
			ar >> m_nColumnDowelHoopBarShapeIdx;
		}
		else
		{
			m_nColumnDowelHoopBarShapeIdx = m_nColumnHoopBarShapeIdx;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20191002)
		{
			ar >> nColumnDowelSupportBarLengthType;
			ar >> nColumnDowelSupportBarCorrectiveLength;						
		}
		else
		{
			nColumnDowelSupportBarLengthType = 0;
			nColumnDowelSupportBarCorrectiveLength = 10;						
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200110)
		{
			ar >> m_nColumnVerBarPlaceTypeDiaIndex;
			m_pVerBarTemplateBelowDia->Serialize(ar);
			m_pVerBarTemplateExcessDia->Serialize(ar);
			ar >> m_nColumnDowelPlaceTypeDiaIndex;
			m_pDowelTemplateBelowDia->Serialize(ar);
			m_pDowelTemplateExcessDia->Serialize(ar);
		}
		else
		{
			m_pVerBarTemplateBelowDia->SetBarSpliceType(nColumnVerBarSpliceType);
			m_pVerBarTemplateBelowDia->SetTopLenType(nColumnVerBarTopLenType);
			m_pVerBarTemplateBelowDia->SetTopFloorDevType(nColumnVerBarTopFloorDevType);
			m_pVerBarTemplateBelowDia->SetConstructionHeight(nColumnVerBarConstructionHeight);
			m_pVerBarTemplateBelowDia->SetAlterSpliceDist(nColumnVerBarAlterSpliceDist);
			m_pVerBarTemplateBelowDia->SetAlterSpliceSet(bColumnVerBarAlterSpliceSet);
			
			m_pVerBarTemplateExcessDia->CopyFromMe(m_pVerBarTemplateBelowDia);

			m_pDowelTemplateBelowDia->SetBarSpliceType(nColumnDowelBarSpliceType);
			m_pDowelTemplateBelowDia->SetTopLenType(nColumnDowelTopPlaceType);
			m_pDowelTemplateBelowDia->SetConstructionHeight(nColumnDowelConstructionHeight);
			m_pDowelTemplateBelowDia->SetAlterSpliceSet(bColumnDowelAlterSpliceSet);
			m_pDowelTemplateBelowDia->SetAlterSpliceDist(nColumnDowelAlterSpliceDist);
			m_pDowelTemplateBelowDia->SetHoopRebarType(nColumnDowelHoopRebarType);
			m_pDowelTemplateBelowDia->SetHoopBySpace(nColumnDowelHoopBySpace);
			m_pDowelTemplateBelowDia->SetHoopByNum(nColumnDowelHoopByNum);
			m_pDowelTemplateBelowDia->SetSupportBarDiaIndex(nColumnDowelSupportBarDiaIndex);
			m_pDowelTemplateBelowDia->SetSupportBarLengthType(nColumnDowelSupportBarLengthType);
			m_pDowelTemplateBelowDia->SetSupportBarLength(nColumnDowelSupportBarLength);
			m_pDowelTemplateBelowDia->SetSupportBarCorrectiveLength(nColumnDowelSupportBarCorrectiveLength);
			m_pDowelTemplateBelowDia->SetSupportBarNum(nColumnDowelSupportBarNum);
			m_pDowelTemplateBelowDia->SetBotPlaceType(nColumnDowelBotPlaceType);
			m_pDowelTemplateBelowDia->SetFootingPercentSet(bColumnDowelFootingPercentSet);
			m_pDowelTemplateBelowDia->SetFootingPercent(nColumnDowelFootingPercent);
			m_pDowelTemplateBelowDia->SetFootingCoverSet(bColumnDowelFootingCoverSet);
			m_pDowelTemplateBelowDia->SetFootingCover(nColumnDowelFootingCover);
			m_pDowelTemplateBelowDia->SetUserHookLenSet(bColumnUserDowelHookLen);

			m_pDowelTemplateExcessDia->CopyFromMe(m_pDowelTemplateBelowDia);
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200709)
		{
			ar >> m_bColumnOutsideColumnTieBarDelete;
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20200907)
		{
			ar >> m_bColumnDowelMainBarUserDiaSet;
			ar >> m_bColumnDowelHoopBarUserDiaSet;

			ar >> NumObject;
			if (NumObject > 0)
				mm_ColumnDowelMainBarUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long key = 0, value = 0;
				ar >> key;
				ar >> value;
				mm_ColumnDowelMainBarUserDiaIndex.insert(make_pair(key, value));
			}

			ar >> NumObject;
			if (NumObject > 0)
				mm_ColumnDowelHoopBarUserDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long key = 0, value = 0;
				ar >> key;
				ar >> value;
				mm_ColumnDowelHoopBarUserDiaIndex.insert(make_pair(key, value));
			}
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20220223)
			m_pButtressColumnRebarTemplate->Serialize(ar);

		if(MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			m_pColumnVerMemberLinkDowelTemplate->Serialize(ar);
		
			ar >> m_bColumnDowelMainBarUserSteelGradeS;
			ar >> m_bColumnDowelHoopBarUserSteelGradeS;
		}
	}
}
ColumnVerBarRebarTemplate *ColumnRebarTemplate::GetColumnVerBarTemplateBelowDia()
{
	return m_pVerBarTemplateBelowDia;
}
ColumnVerBarRebarTemplate *ColumnRebarTemplate::GetColumnVerBarTemplateExcessDia()
{
	return m_pVerBarTemplateExcessDia;
}
ColumnDowelRebarTemplate *ColumnRebarTemplate::GetColumnDowelTemplateBelowDia()
{
	return m_pDowelTemplateBelowDia;
}
ColumnDowelRebarTemplate *ColumnRebarTemplate::GetColumnDowelTemplateExcessDia()
{
	return m_pDowelTemplateExcessDia;
}
double ColumnRebarTemplate::GetColumnUserDowelHookLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it = mm_ColumnUserDowelHookLen.find(DiaIndex);
	if (it == mm_ColumnUserDowelHookLen.end())
		return 0;
	return it->second;
}

long ColumnRebarTemplate::GetColumnUserDowelSpliceLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it;
	it = mm_ColumnUserDowelSpliceLen.find(DiaIndex);
	if (it != mm_ColumnUserDowelSpliceLen.end())
		return it->second;

	return 0;
}
double ColumnRebarTemplate::GetColumnDowelMainBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_ColumnDowelMainBarUserDiaIndex.find(DiaIndex);
	if (it == mm_ColumnDowelMainBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double ColumnRebarTemplate::GetColumnDowelHoopBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_ColumnDowelHoopBarUserDiaIndex.find(DiaIndex);
	if (it == mm_ColumnDowelHoopBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void ColumnRebarTemplate::SetColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_ColumnUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_ColumnUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void ColumnRebarTemplate::SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_ColumnUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_ColumnUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}
void ColumnRebarTemplate::SetColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap)
{
	mm_ColumnDowelMainBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelMainBarUserDiaIndexMap.begin(); it != dowelMainBarUserDiaIndexMap.end(); it++)
	{
		mm_ColumnDowelMainBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void ColumnRebarTemplate::SetColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap)
{
	mm_ColumnDowelHoopBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelHoopBarUserDiaIndexMap.begin(); it != dowelHoopBarUserDiaIndexMap.end(); it++)
	{
		mm_ColumnDowelHoopBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

long ColumnRebarTemplate::GetColumnVerBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetBarSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetBarSpliceType();
}
long ColumnRebarTemplate::GetColumnVerBarTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopLenType();
	else
		return m_pVerBarTemplateExcessDia->GetTopLenType();
}
long ColumnRebarTemplate::GetColumnVerBarTopFloorDevType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopFloorDevType();
	else
		return m_pVerBarTemplateExcessDia->GetTopFloorDevType();
}
long ColumnRebarTemplate::GetColumnVerBarConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetConstructionHeight();
	else
		return m_pVerBarTemplateExcessDia->GetConstructionHeight();
}
BOOL ColumnRebarTemplate::GetColumnVerBarAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceSet();
}
long ColumnRebarTemplate::GetColumnVerBarAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceDist();
}
long ColumnRebarTemplate::GetColumnVerBarAlterSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (DiaIndex <= m_nColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceType();
}
long ColumnRebarTemplate::GetColumnDowelAlterSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceType();
}
long ColumnRebarTemplate::GetColumnDowelBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetBarSpliceType();
}
long ColumnRebarTemplate::GetColumnDowelTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetTopLenType();
	else
		return m_pDowelTemplateExcessDia->GetTopLenType();
}
long ColumnRebarTemplate::GetColumnDowelConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetConstructionHeight();
	else
		return m_pDowelTemplateExcessDia->GetConstructionHeight();
}
BOOL ColumnRebarTemplate::GetColumnDowelAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceSet();
};
long ColumnRebarTemplate::GetColumnDowelAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceDist();
}
long ColumnRebarTemplate::GetColumnDowelHoopRebarType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopRebarType();
	else
		return m_pDowelTemplateExcessDia->GetHoopRebarType();
}
long ColumnRebarTemplate::GetColumnDowelHoopBySpace(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopBySpace();
	else
		return m_pDowelTemplateExcessDia->GetHoopBySpace();
}
long ColumnRebarTemplate::GetColumnDowelHoopByNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopByNum();
	else
		return m_pDowelTemplateExcessDia->GetHoopByNum();
}
long ColumnRebarTemplate::GetColumnDowelSupportBarDiaIndex(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarDiaIndex();
}
BOOL ColumnRebarTemplate::GetColumnDowelSupportBarUserSteelGradeS(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarUserSteelGradeS();
}
long ColumnRebarTemplate::GetColumnDowelSupportBarLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLength();
}
long ColumnRebarTemplate::GetColumnDowelSupportBarLengthType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLengthType();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLengthType();
}
double ColumnRebarTemplate::GetColumnDowelSupportBarFootingThickMultiplicationValue(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarFootingThickMultiplicationValue();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarFootingThickMultiplicationValue();
}
long ColumnRebarTemplate::GetColumnDowelSupportBarCorrectiveLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarCorrectiveLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarCorrectiveLength();
}
long ColumnRebarTemplate::GetColumnDowelSupportBarNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarNum();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarNum();
}
long ColumnRebarTemplate::GetColumnDowelBotPlaceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotPlaceType();
	else
		return m_pDowelTemplateExcessDia->GetBotPlaceType();
}
BOOL ColumnRebarTemplate::GetColumnDowelFootingPercentSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercentSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercentSet();
}
long ColumnRebarTemplate::GetColumnDowelFootingPercent(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercent();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercent();
}
BOOL ColumnRebarTemplate::GetColumnDowelFootingCoverSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCoverSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingCoverSet();
}
long ColumnRebarTemplate::GetColumnDowelFootingCover(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCover();
	else
		return m_pDowelTemplateExcessDia->GetFootingCover();
}
BOOL ColumnRebarTemplate::GetColumnUserDowelHookLenSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetUserHookLenSet();
	else
		return m_pDowelTemplateExcessDia->GetUserHookLenSet();
}
BOOL ColumnRebarTemplate::GetColumnDowelMainBarUserDiaSet()
{
	return m_bColumnDowelMainBarUserDiaSet;
}
void ColumnRebarTemplate::SetColumnDowelMainBarUserDiaSet(BOOL value)
{
	m_bColumnDowelMainBarUserDiaSet = value;
}
BOOL ColumnRebarTemplate::GetColumnDowelHoopBarUserDiaSet()
{
	return m_bColumnDowelHoopBarUserDiaSet;
}
void ColumnRebarTemplate::SetColumnDowelHoopBarUserDiaSet(BOOL value)
{
	m_bColumnDowelHoopBarUserDiaSet = value;
}
BOOL ColumnRebarTemplate::GetColumnDowelMainBarUserSteelGradeS()
{
	return m_bColumnDowelMainBarUserSteelGradeS;
}
void ColumnRebarTemplate::SetColumnDowelMainBarUserSteelGradeS(BOOL value)
{
	m_bColumnDowelMainBarUserSteelGradeS = value;
}
BOOL ColumnRebarTemplate::GetColumnDowelHoopBarUserSteelGradeS()
{
	return m_bColumnDowelHoopBarUserSteelGradeS;
}
void ColumnRebarTemplate::SetColumnDowelHoopBarUserSteelGradeS(BOOL value)
{
	m_bColumnDowelHoopBarUserSteelGradeS = value;
}
long ColumnRebarTemplate::GetButtressColumnVerBarPlaceTypeDiaIndex()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarPlaceTypeDiaIndex();
}
long ColumnRebarTemplate::GetButtressColumnDowelPlaceTypeDiaIndex()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelPlaceTypeDiaIndex();
}
long ColumnRebarTemplate::GetButtressColumnVerBarSpliceType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarSpliceType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnVerBarTopLenType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarTopLenType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnVerBarTopFloorDevType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarTopFloorDevType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnVerBarConstructionHeight(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarConstructionHeight(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnVerBarAlterSpliceSet(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarAlterSpliceSet(dDia);
}
long ColumnRebarTemplate::GetButtressColumnVerBarAlterSpliceDist(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarAlterSpliceDist(dDia);
}	
long ColumnRebarTemplate::GetButtressColumnDowelBarSpliceType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelBarSpliceType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelTopLenType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelTopLenType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelConstructionHeight(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelConstructionHeight(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelAlterSpliceSet(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelAlterSpliceSet(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelAlterSpliceDist(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelAlterSpliceDist(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelHoopRebarType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopRebarType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelHoopBySpace(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBySpace(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelHoopByNum(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopByNum(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelSupportBarDiaIndex(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarDiaIndex(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelSupportBarUserSteelGradeS(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarUserSteelGradeS(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelSupportBarLength(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarLength(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelSupportBarLengthType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarLengthType(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelSupportBarCorrectiveLength(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarCorrectiveLength(dDia);
}
double ColumnRebarTemplate::GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelSupportBarNum(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelSupportBarNum(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelBotPlaceType(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelBotPlaceType(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelFootingPercentSet(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelFootingPercentSet(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelFootingPercent(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelFootingPercent(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelFootingCoverSet(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelFootingCoverSet(dDia);
}
long ColumnRebarTemplate::GetButtressColumnDowelFootingCover(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelFootingCover(dDia);
}
BOOL ColumnRebarTemplate::GetButtressColumnUserDowelHookLenSet(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnUserDowelHookLenSet(dDia);
}
//Set Property
void ColumnRebarTemplate::SetButtressColumnVerBarPlaceTypeDiaIndex(long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnVerBarPlaceTypeDiaIndex(value);
}
void ColumnRebarTemplate::SetButtressColumnDowelPlaceTypeDiaIndex(long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelPlaceTypeDiaIndex(value);
}	
//Function
map<long, long> ColumnRebarTemplate::GetButtressColumnUserDowelHookLenMap()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnUserDowelHookLenMap();
}
map<long, long> ColumnRebarTemplate::GetButtressColumnUserDowelSpliceLenMap()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnUserDowelSpliceLenMap();
}
map<long, long> ColumnRebarTemplate::GetButtressColumnDowelMainBarUserDiaIndexMap()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserDiaIndexMap();
}
map<long, long> ColumnRebarTemplate::GetButtressColumnDowelHoopBarUserDiaIndexMap()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserDiaIndexMap();
}

void ColumnRebarTemplate::SetButtressColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnUserDowelHookLenMap(mapUserDowelHookLen);
}
void ColumnRebarTemplate::SetButtressColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnUserDowelSpliceLenMap(mapUserDowelSpliceLen);
}
void ColumnRebarTemplate::SetButtressColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelMainBarUserDiaIndexMap(dowelMainBarUserDiaIndexMap);
}
void ColumnRebarTemplate::SetButtressColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelHoopBarUserDiaIndexMap(dowelHoopBarUserDiaIndexMap);
}

double ColumnRebarTemplate::GetButtressColumnUserDowelHookLen(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnUserDowelHookLen(dDia);
}
long ColumnRebarTemplate::GetButtressColumnUserDowelSpliceLen(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnUserDowelSpliceLen(dDia);
}
double ColumnRebarTemplate::GetButtressColumnDowelMainBarUserDia(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserDia(dDia);
}
double ColumnRebarTemplate::GetButtressColumnDowelHoopBarUserDia(double dDia)
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserDia(dDia);
}

void ColumnRebarTemplate::SetButtressColumnUserDowelHookLen(long key, long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnUserDowelHookLen(key, value);
}
void ColumnRebarTemplate::SetButtressColumnUserDowelSpliceLen(long key, long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnUserDowelSpliceLen(key, value);
}
void ColumnRebarTemplate::SetButtressColumnDowelMainBarUserDiaIndex(long key, long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelMainBarUserDiaIndex(key, value);
}
void ColumnRebarTemplate::SetButtressColumnDowelHoopBarUserDiaIndex(long key, long value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelHoopBarUserDiaIndex(key, value);
}

ColumnVerBarRebarTemplate *ColumnRebarTemplate::GetButtressColumnVerBarTemplateBelowDia()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarTemplateBelowDia();
}
ColumnVerBarRebarTemplate *ColumnRebarTemplate::GetButtressColumnVerBarTemplateExcessDia()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnVerBarTemplateExcessDia();
}
ColumnDowelRebarTemplate *ColumnRebarTemplate::GetButtressColumnDowelTemplateBelowDia()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelTemplateBelowDia();
}
ColumnDowelRebarTemplate *ColumnRebarTemplate::GetButtressColumnDowelTemplateExcessDia()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelTemplateExcessDia();
}
		
BOOL ColumnRebarTemplate::GetButtressColumnDowelMainBarUserDiaSet()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserDiaSet();
}
void ColumnRebarTemplate::SetButtressColumnDowelMainBarUserDiaSet(BOOL value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelMainBarUserDiaSet(value);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelHoopBarUserDiaSet()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserDiaSet();
}
void ColumnRebarTemplate::SetButtressColumnDowelHoopBarUserDiaSet(BOOL value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelHoopBarUserDiaSet(value);
}
VerMemberLinkDowelTemplate *ColumnRebarTemplate::GetColumnVerMemberLinkDowelTemplate()
{
	return m_pColumnVerMemberLinkDowelTemplate;
}

BOOL ColumnRebarTemplate::GetButtressColumnDowelMainBarUserSteelGradeS()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelMainBarUserSteelGradeS();
}
void ColumnRebarTemplate::SetButtressColumnDowelMainBarUserSteelGradeS(BOOL value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelMainBarUserSteelGradeS(value);
}
BOOL ColumnRebarTemplate::GetButtressColumnDowelHoopBarUserSteelGradeS()
{
	return m_pButtressColumnRebarTemplate->GetButtressColumnDowelHoopBarUserSteelGradeS();
}
void ColumnRebarTemplate::SetButtressColumnDowelHoopBarUserSteelGradeS(BOOL value)
{
	m_pButtressColumnRebarTemplate->SetButtressColumnDowelHoopBarUserSteelGradeS(value);
}