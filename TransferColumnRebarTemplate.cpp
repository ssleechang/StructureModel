#include "StdAfx.h"
#include "TransferColumnRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "ColumnVerBarRebarTemplate.h"
#include "ColumnDowelRebarTemplate.h"
#include "VerMemberLinkDowelTemplate.h"

TransferColumnRebarTemplate::TransferColumnRebarTemplate() : BaseRebarTemplate()
{
	DataInit();
	m_pVerBarTemplateBelowDia = new ColumnVerBarRebarTemplate();
	m_pVerBarTemplateExcessDia = new ColumnVerBarRebarTemplate();
	m_pDowelTemplateBelowDia = new ColumnDowelRebarTemplate();
	m_pDowelTemplateExcessDia = new ColumnDowelRebarTemplate();	
	m_pTransferColumnVerMemberLinkDowelTemplate = new VerMemberLinkDowelTemplate();
}

TransferColumnRebarTemplate::~TransferColumnRebarTemplate(void)
{
	delete m_pVerBarTemplateBelowDia;
	delete m_pVerBarTemplateExcessDia;
	delete m_pDowelTemplateBelowDia;
	delete m_pDowelTemplateExcessDia;
	delete m_pTransferColumnVerMemberLinkDowelTemplate;
}

void TransferColumnRebarTemplate::DataInit()
{
	CStringArray BarDiaArr;
	MSDeformedBar::GetRebarNameList(BarDiaArr);

	m_nTransferColumnVerBarPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
	m_nTransferColumnDowelPlaceTypeDiaIndex = BarDiaArr.GetSize() - 1;
		
	m_bTransferColumnEndHoopBarShapeSet = false;
	m_nTransferColumnHoopBarShapeIdx = 1;
	m_nTransferColumnTieBarShapeIdx = 1;
	m_bTransferColumnHoopBarStartLengthSet = false;
	m_nTransferColumnHoopBarStartLength = 50;
	m_nTransferColumnTopMemberCover = 0;
	m_bTransferColumnTieAddLengthSet = false;
	m_nTransferColumnTieAddLength = 10;
	m_bTransferColumnOutsideColumnTieBarDelete = FALSE;
	
	m_nTransferColumnDowelHoopBarShapeIdx = 1;
	InitUserDowelHookLenMap(mm_TransferColumnUserDowelHookLen);
	InitUserDowelSpliceLenMap(mm_TransferColumnUserDowelSpliceLen);

	InitUserDiaIndexMap(mm_TransferColumnDowelMainBarUserDiaIndex);
	InitUserDiaIndexMap(mm_TransferColumnDowelHoopBarUserDiaIndex);

	m_bTransferColumnDowelMainBarUserDiaSet = FALSE;
	m_bTransferColumnDowelHoopBarUserDiaSet= FALSE;
	m_bTransferColumnDowelMainBarUserSteelGradeS = FALSE;
	m_bTransferColumnDowelHoopBarUserSteelGradeS = FALSE;
}

void TransferColumnRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	TransferColumnRebarTemplate* pColumnRebarTemplate = dynamic_cast<TransferColumnRebarTemplate*>(pBaseRebarTemplate);
	if(pColumnRebarTemplate == nullptr)
		return;

	m_nTransferColumnVerBarPlaceTypeDiaIndex = pColumnRebarTemplate->GetTransferColumnVerBarPlaceTypeDiaIndex();
	m_nTransferColumnDowelPlaceTypeDiaIndex = pColumnRebarTemplate->GetTransferColumnDowelPlaceTypeDiaIndex();

	m_pVerBarTemplateBelowDia->CopyFromMe(pColumnRebarTemplate->GetTransferColumnVerBarTemplateBelowDia());
	m_pVerBarTemplateExcessDia->CopyFromMe(pColumnRebarTemplate->GetTransferColumnVerBarTemplateExcessDia());

	m_pDowelTemplateBelowDia->CopyFromMe(pColumnRebarTemplate->GetTransferColumnDowelTemplateBelowDia());
	m_pDowelTemplateExcessDia->CopyFromMe(pColumnRebarTemplate->GetTransferColumnDowelTemplateExcessDia());

	m_bTransferColumnEndHoopBarShapeSet = pColumnRebarTemplate->m_bTransferColumnEndHoopBarShapeSet;
	m_nTransferColumnHoopBarShapeIdx = pColumnRebarTemplate->m_nTransferColumnHoopBarShapeIdx;
	m_nTransferColumnTieBarShapeIdx = pColumnRebarTemplate->m_nTransferColumnTieBarShapeIdx;
	m_bTransferColumnHoopBarStartLengthSet = pColumnRebarTemplate->m_bTransferColumnHoopBarStartLengthSet;
	m_nTransferColumnHoopBarStartLength = pColumnRebarTemplate->m_nTransferColumnHoopBarStartLength;
	m_nTransferColumnTopMemberCover = pColumnRebarTemplate->m_nTransferColumnTopMemberCover;
	m_bTransferColumnTieAddLengthSet = pColumnRebarTemplate->m_bTransferColumnTieAddLengthSet;
	m_nTransferColumnTieAddLength = pColumnRebarTemplate->m_nTransferColumnTieAddLength;
	m_bTransferColumnOutsideColumnTieBarDelete = pColumnRebarTemplate->m_bTransferColumnOutsideColumnTieBarDelete;

	m_nTransferColumnDowelHoopBarShapeIdx = pColumnRebarTemplate->m_nTransferColumnDowelHoopBarShapeIdx;
	
	UserDowelHookLenMapDataCopy(mm_TransferColumnUserDowelHookLen, pColumnRebarTemplate->mm_TransferColumnUserDowelHookLen);
	UserDowelSpliceLenMapDataCopy(mm_TransferColumnUserDowelSpliceLen, pColumnRebarTemplate->mm_TransferColumnUserDowelSpliceLen);
	
	UserMapDataCopy(mm_TransferColumnDowelMainBarUserDiaIndex, pColumnRebarTemplate->mm_TransferColumnDowelMainBarUserDiaIndex);
	UserMapDataCopy(mm_TransferColumnDowelHoopBarUserDiaIndex, pColumnRebarTemplate->mm_TransferColumnDowelHoopBarUserDiaIndex);

	m_bTransferColumnDowelMainBarUserDiaSet = pColumnRebarTemplate->GetTransferColumnDowelMainBarUserDiaSet();
	m_bTransferColumnDowelHoopBarUserDiaSet = pColumnRebarTemplate->GetTransferColumnDowelHoopBarUserDiaSet();
	m_bTransferColumnDowelMainBarUserSteelGradeS = pColumnRebarTemplate->GetTransferColumnDowelMainBarUserSteelGradeS();
	m_bTransferColumnDowelHoopBarUserSteelGradeS = pColumnRebarTemplate->GetTransferColumnDowelHoopBarUserSteelGradeS();

	m_pTransferColumnVerMemberLinkDowelTemplate->CopyFromMe(pColumnRebarTemplate->GetTransferColumnVerMemberLinkDowelTemplate());
}

void TransferColumnRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		map<long, long>::iterator it;
		ar << (int)mm_TransferColumnUserDowelSpliceLen.size();
		for (it = mm_TransferColumnUserDowelSpliceLen.begin(); it != mm_TransferColumnUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}
		ar << (int)mm_TransferColumnUserDowelHookLen.size();
		for (it = mm_TransferColumnUserDowelHookLen.begin(); it != mm_TransferColumnUserDowelHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		
		ar << m_nTransferColumnDowelHoopBarShapeIdx;
		ar << m_nTransferColumnVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar << m_nTransferColumnDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);
		ar << m_bTransferColumnOutsideColumnTieBarDelete;
		ar << m_bTransferColumnDowelMainBarUserDiaSet;
		ar << m_bTransferColumnDowelHoopBarUserDiaSet;

		ar << (int)mm_TransferColumnDowelMainBarUserDiaIndex.size();
		for(it = mm_TransferColumnDowelMainBarUserDiaIndex.begin(); it != mm_TransferColumnDowelMainBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}

		ar << (int)mm_TransferColumnDowelHoopBarUserDiaIndex.size();
		for(it = mm_TransferColumnDowelHoopBarUserDiaIndex.begin(); it != mm_TransferColumnDowelHoopBarUserDiaIndex.end(); it++)
		{
			long key = it->first;
			long value = it->second;
			ar << key;
			ar << value;
		}
		
		m_pTransferColumnVerMemberLinkDowelTemplate->Serialize(ar);

		ar << m_bTransferColumnDowelMainBarUserSteelGradeS;
		ar << m_bTransferColumnDowelHoopBarUserSteelGradeS;


		ar << m_bTransferColumnEndHoopBarShapeSet;
		ar << m_nTransferColumnHoopBarShapeIdx;
		ar << m_nTransferColumnTieBarShapeIdx;
		ar << m_bTransferColumnHoopBarStartLengthSet;
		ar << m_nTransferColumnHoopBarStartLength;
		ar << m_nTransferColumnTopMemberCover;
		ar << m_bTransferColumnTieAddLengthSet;
		ar << m_nTransferColumnTieAddLength;
		ar << m_bTransferColumnOutsideColumnTieBarDelete;
	}
	else
	{
		int NumObject;

		mm_TransferColumnUserDowelSpliceLen.clear();
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferColumnUserDowelSpliceLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_TransferColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		mm_TransferColumnUserDowelHookLen.clear();
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferColumnUserDowelHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_TransferColumnUserDowelHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> m_nTransferColumnDowelHoopBarShapeIdx;
		ar >> m_nTransferColumnVerBarPlaceTypeDiaIndex;
		m_pVerBarTemplateBelowDia->Serialize(ar);
		m_pVerBarTemplateExcessDia->Serialize(ar);
		ar >> m_nTransferColumnDowelPlaceTypeDiaIndex;
		m_pDowelTemplateBelowDia->Serialize(ar);
		m_pDowelTemplateExcessDia->Serialize(ar);	
		ar >> m_bTransferColumnOutsideColumnTieBarDelete;
		ar >> m_bTransferColumnDowelMainBarUserDiaSet;
		ar >> m_bTransferColumnDowelHoopBarUserDiaSet;

		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferColumnDowelMainBarUserDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long key = 0, value = 0;
			ar >> key;
			ar >> value;
			mm_TransferColumnDowelMainBarUserDiaIndex.insert(make_pair(key, value));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferColumnDowelHoopBarUserDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long key = 0, value = 0;
			ar >> key;
			ar >> value;
			mm_TransferColumnDowelHoopBarUserDiaIndex.insert(make_pair(key, value));
		}

		m_pTransferColumnVerMemberLinkDowelTemplate->Serialize(ar);
		
		ar >> m_bTransferColumnDowelMainBarUserSteelGradeS;
		ar >> m_bTransferColumnDowelHoopBarUserSteelGradeS;
		
		if(MSVersionInfo::GetCurrentVersion() >= 20240814)
		{
			ar >> m_bTransferColumnEndHoopBarShapeSet;
			ar >> m_nTransferColumnHoopBarShapeIdx;
			ar >> m_nTransferColumnTieBarShapeIdx;
			ar >> m_bTransferColumnHoopBarStartLengthSet;
			ar >> m_nTransferColumnHoopBarStartLength;
			ar >> m_nTransferColumnTopMemberCover;
			ar >> m_bTransferColumnTieAddLengthSet;
			ar >> m_nTransferColumnTieAddLength;
			ar >> m_bTransferColumnOutsideColumnTieBarDelete;
		}
	}
}
ColumnVerBarRebarTemplate *TransferColumnRebarTemplate::GetTransferColumnVerBarTemplateBelowDia()
{
	return m_pVerBarTemplateBelowDia;
}
ColumnVerBarRebarTemplate *TransferColumnRebarTemplate::GetTransferColumnVerBarTemplateExcessDia()
{
	return m_pVerBarTemplateExcessDia;
}
ColumnDowelRebarTemplate *TransferColumnRebarTemplate::GetTransferColumnDowelTemplateBelowDia()
{
	return m_pDowelTemplateBelowDia;
}
ColumnDowelRebarTemplate *TransferColumnRebarTemplate::GetTransferColumnDowelTemplateExcessDia()
{
	return m_pDowelTemplateExcessDia;
}
double TransferColumnRebarTemplate::GetTransferColumnUserDowelHookLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it = mm_TransferColumnUserDowelHookLen.find(DiaIndex);
	if (it == mm_TransferColumnUserDowelHookLen.end())
		return 0;
	return it->second;
}

long TransferColumnRebarTemplate::GetTransferColumnUserDowelSpliceLen(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	map<long, long>::iterator it;
	it = mm_TransferColumnUserDowelSpliceLen.find(DiaIndex);
	if (it != mm_TransferColumnUserDowelSpliceLen.end())
		return it->second;

	return 0;
}
double TransferColumnRebarTemplate::GetTransferColumnDowelMainBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_TransferColumnDowelMainBarUserDiaIndex.find(DiaIndex);
	if (it == mm_TransferColumnDowelMainBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double TransferColumnRebarTemplate::GetTransferColumnDowelHoopBarUserDia(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	
	map<long, long>::iterator it = mm_TransferColumnDowelHoopBarUserDiaIndex.find(DiaIndex);
	if (it == mm_TransferColumnDowelHoopBarUserDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void TransferColumnRebarTemplate::SetTransferColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen)
{
	mm_TransferColumnUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelHookLen.begin(); it != mapUserDowelHookLen.end(); it++)
	{
		mm_TransferColumnUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void TransferColumnRebarTemplate::SetTransferColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_TransferColumnUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_TransferColumnUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}
void TransferColumnRebarTemplate::SetTransferColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap)
{
	mm_TransferColumnDowelMainBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelMainBarUserDiaIndexMap.begin(); it != dowelMainBarUserDiaIndexMap.end(); it++)
	{
		mm_TransferColumnDowelMainBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void TransferColumnRebarTemplate::SetTransferColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap)
{
	mm_TransferColumnDowelHoopBarUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = dowelHoopBarUserDiaIndexMap.begin(); it != dowelHoopBarUserDiaIndexMap.end(); it++)
	{
		mm_TransferColumnDowelHoopBarUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

long TransferColumnRebarTemplate::GetTransferColumnVerBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetBarSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetBarSpliceType();
}
long TransferColumnRebarTemplate::GetTransferColumnVerBarTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopLenType();
	else
		return m_pVerBarTemplateExcessDia->GetTopLenType();
}
long TransferColumnRebarTemplate::GetTransferColumnVerBarTopFloorDevType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetTopFloorDevType();
	else
		return m_pVerBarTemplateExcessDia->GetTopFloorDevType();
}
long TransferColumnRebarTemplate::GetTransferColumnVerBarConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetConstructionHeight();
	else
		return m_pVerBarTemplateExcessDia->GetConstructionHeight();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnVerBarAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceSet();
}
long TransferColumnRebarTemplate::GetTransferColumnVerBarAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceDist();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelBarSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBarSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetBarSpliceType();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelTopLenType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetTopLenType();
	else
		return m_pDowelTemplateExcessDia->GetTopLenType();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelConstructionHeight(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetConstructionHeight();
	else
		return m_pDowelTemplateExcessDia->GetConstructionHeight();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelAlterSpliceSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceSet();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceSet();
};
long TransferColumnRebarTemplate::GetTransferColumnDowelAlterSpliceDist(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceDist();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceDist();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelHoopRebarType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopRebarType();
	else
		return m_pDowelTemplateExcessDia->GetHoopRebarType();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelHoopBySpace(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopBySpace();
	else
		return m_pDowelTemplateExcessDia->GetHoopBySpace();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelHoopByNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetHoopByNum();
	else
		return m_pDowelTemplateExcessDia->GetHoopByNum();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarDiaIndex(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarDiaIndex();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarDiaIndex();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarUserSteelGradeS(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarUserSteelGradeS();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarUserSteelGradeS();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLength();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarLengthType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarLengthType();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarLengthType();
}
double TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarFootingThickMultiplicationValue(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarFootingThickMultiplicationValue();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarFootingThickMultiplicationValue();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarCorrectiveLength(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarCorrectiveLength();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarCorrectiveLength();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelSupportBarNum(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetSupportBarNum();
	else
		return m_pDowelTemplateExcessDia->GetSupportBarNum();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelBotPlaceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetBotPlaceType();
	else
		return m_pDowelTemplateExcessDia->GetBotPlaceType();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelFootingPercentSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercentSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercentSet();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelFootingPercent(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingPercent();
	else
		return m_pDowelTemplateExcessDia->GetFootingPercent();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelFootingCoverSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCoverSet();
	else
		return m_pDowelTemplateExcessDia->GetFootingCoverSet();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelFootingCover(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetFootingCover();
	else
		return m_pDowelTemplateExcessDia->GetFootingCover();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnUserDowelHookLenSet(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetUserHookLenSet();
	else
		return m_pDowelTemplateExcessDia->GetUserHookLenSet();
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelMainBarUserDiaSet()
{
	return m_bTransferColumnDowelMainBarUserDiaSet;
}
void TransferColumnRebarTemplate::SetTransferColumnDowelMainBarUserDiaSet(BOOL value)
{
	m_bTransferColumnDowelMainBarUserDiaSet = value;
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelHoopBarUserDiaSet()
{
	return m_bTransferColumnDowelHoopBarUserDiaSet;
}
void TransferColumnRebarTemplate::SetTransferColumnDowelHoopBarUserDiaSet(BOOL value)
{
	m_bTransferColumnDowelHoopBarUserDiaSet = value;
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelMainBarUserSteelGradeS()
{
	return m_bTransferColumnDowelMainBarUserSteelGradeS;
}
void TransferColumnRebarTemplate::SetTransferColumnDowelMainBarUserSteelGradeS(BOOL value)
{
	m_bTransferColumnDowelMainBarUserSteelGradeS = value;
}
BOOL TransferColumnRebarTemplate::GetTransferColumnDowelHoopBarUserSteelGradeS()
{
	return m_bTransferColumnDowelHoopBarUserSteelGradeS;
}
void TransferColumnRebarTemplate::SetTransferColumnDowelHoopBarUserSteelGradeS(BOOL value)
{
	m_bTransferColumnDowelHoopBarUserSteelGradeS = value;
}

VerMemberLinkDowelTemplate *TransferColumnRebarTemplate::GetTransferColumnVerMemberLinkDowelTemplate()
{
	return m_pTransferColumnVerMemberLinkDowelTemplate;
}

long TransferColumnRebarTemplate::GetTransferColumnVerBarAlterSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (DiaIndex <= m_nTransferColumnVerBarPlaceTypeDiaIndex)
		return m_pVerBarTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pVerBarTemplateExcessDia->GetAlterSpliceType();
}
long TransferColumnRebarTemplate::GetTransferColumnDowelAlterSpliceType(double dDia)
{
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if (DiaIndex <= m_nTransferColumnDowelPlaceTypeDiaIndex)
		return m_pDowelTemplateBelowDia->GetAlterSpliceType();
	else
		return m_pDowelTemplateExcessDia->GetAlterSpliceType();
}