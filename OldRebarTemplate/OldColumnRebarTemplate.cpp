#include "StdAfx.h"
#include "OldColumnRebarTemplate.h"
#include "..\MSDeformedBar.h"
#include "..\..\GMLib\MSVersionInfo.h"
#include "..\ColumnVerBarRebarTemplate.h"
#include "..\ColumnDowelRebarTemplate.h"
//#include "..\MSRAP\RAPRebarOption.h"

IMPLEMENT_SERIAL(OldColumnRebarTemplate, OldBaseRebarTemplate, VERSIONABLE_SCHEMA | 1)
OldColumnRebarTemplate::OldColumnRebarTemplate() :OldBaseRebarTemplate()
{
	DataInit();
}

OldColumnRebarTemplate::~OldColumnRebarTemplate(void)
{
}

void OldColumnRebarTemplate::CopyFromMe(OldColumnRebarTemplate* pColumnRebarTemplate)
{

}

void OldColumnRebarTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	OldBaseRebarTemplate::Dump(buffer);
	buffer << m_bColumnGeneralInfoSet;
	buffer << m_bColumnConnectLengthSet;
	buffer << m_nColumnConnectLength;

	buffer << m_bColumnHoopBarInfoSet;
	buffer << m_bColumnHoopBarStartLengthSet;
	buffer << m_nColumnHoopBarStartLength;
	buffer << m_bColumnTieAddLengthSet;
	buffer << m_nColumnTieAddLength;
	buffer << m_nColumnTopMemberCover;
	buffer << m_nColumnHoopBarShapeIdx;
	buffer << m_nColumnTieBarShapeIdx;

	buffer << m_bColumnVerInfoSet;
	buffer << m_nColumnVerBarSpliceType;
	buffer << m_nColumnVerBarTopLenType;
	buffer << m_bColumnVerBarAlterSpliceSet;
	buffer << m_nColumnVerBarAlterSpliceValue1;
	buffer << m_nColumnVerBarAlterSpliceValue2;

	buffer << m_bColumnDowelInfoSet;
	buffer << m_bColumnDowelFootingCoverSet;
	buffer << m_nColumnDowelFootingCover;
	buffer << m_nColumnDowelBotPlaceType;
	buffer << m_nColumnDowelTopPlaceType;
	buffer << m_nColumnDowelHoopRebarType;
	buffer << m_nColumnDowelHoopBySpace;
	buffer << m_nColumnDowelHoopByNum;
	buffer << m_nColumnDowelSupportBarDiaIndex;
	buffer << m_nColumnDowelSupportBarLength;
	buffer << m_nColumnDowelSupportBarNum;

	map<long, long>::iterator it;
	int size = (int)mm_ColumnUserDowelSpliceLen.size();
	buffer << size;
	for (it = mm_ColumnUserDowelSpliceLen.begin(); it != mm_ColumnUserDowelSpliceLen.end(); it++)
	{
		long nDiaIndex = it->first;
		long spliceLen = it->second;
		buffer << nDiaIndex;
		buffer << spliceLen;
	}

	buffer << m_bColumnEndHoopBarShapeSet;

	buffer << m_nColumnDowelBarSpliceType;
	buffer << m_bColumnDowelBarAlterSpliceSet;
	buffer << m_nColumnDowelBarAlterSpliceValue1;
	buffer << m_nColumnDowelBarAlterSpliceValue2;
	buffer << m_bColumnDowelFootingPercentSet;
	buffer << m_nColumnDowelFootingPercent;
}

void OldColumnRebarTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	OldBaseRebarTemplate::Recovery(buffer);
	buffer >> m_bColumnGeneralInfoSet;
	buffer >> m_bColumnConnectLengthSet;
	buffer >> m_nColumnConnectLength;

	buffer >> m_bColumnHoopBarInfoSet;
	buffer >> m_bColumnHoopBarStartLengthSet;
	buffer >> m_nColumnHoopBarStartLength;
	buffer >> m_bColumnTieAddLengthSet;
	buffer >> m_nColumnTieAddLength;
	buffer >> m_nColumnTopMemberCover;
	buffer >> m_nColumnHoopBarShapeIdx;
	buffer >> m_nColumnTieBarShapeIdx;

	buffer >> m_bColumnVerInfoSet;
	buffer >> m_nColumnVerBarSpliceType;
	buffer >> m_nColumnVerBarTopLenType;
	buffer >> m_bColumnVerBarAlterSpliceSet;
	buffer >> m_nColumnVerBarAlterSpliceValue1;
	buffer >> m_nColumnVerBarAlterSpliceValue2;

	buffer >> m_bColumnDowelInfoSet;
	buffer >> m_bColumnDowelFootingCoverSet;
	buffer >> m_nColumnDowelFootingCover;
	buffer >> m_nColumnDowelBotPlaceType;
	buffer >> m_nColumnDowelTopPlaceType;
	buffer >> m_nColumnDowelHoopRebarType;
	buffer >> m_nColumnDowelHoopBySpace;
	buffer >> m_nColumnDowelHoopByNum;
	buffer >> m_nColumnDowelSupportBarDiaIndex;
	buffer >> m_nColumnDowelSupportBarLength;
	buffer >> m_nColumnDowelSupportBarNum;

	int NumObject;

	mm_ColumnUserDowelSpliceLen.clear();
	buffer >> NumObject;
	for (int index = 0; index < NumObject; index++)
	{
		long nDiaIndex = 0, spliceLen = 0;

		buffer >> nDiaIndex;
		buffer >> spliceLen;
		mm_ColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
	}

	buffer >> m_bColumnEndHoopBarShapeSet;

	buffer >> m_nColumnDowelBarSpliceType;
	buffer >> m_bColumnDowelBarAlterSpliceSet;
	buffer >> m_nColumnDowelBarAlterSpliceValue1;
	buffer >> m_nColumnDowelBarAlterSpliceValue2;
	buffer >> m_bColumnDowelFootingPercentSet;
	buffer >> m_nColumnDowelFootingPercent;
}

void OldColumnRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bColumnGeneralInfoSet;
		ar << m_bColumnConnectLengthSet;
		ar << m_nColumnConnectLength;

		ar << m_bColumnHoopBarInfoSet;
		ar << m_bColumnHoopBarStartLengthSet;
		ar << m_nColumnHoopBarStartLength;
		ar << m_bColumnTieAddLengthSet;
		ar << m_nColumnTieAddLength;
		ar << m_nColumnTopMemberCover;
		ar << m_nColumnHoopBarShapeIdx;
		ar << m_nColumnTieBarShapeIdx;

		ar << m_bColumnVerInfoSet;
		ar << m_nColumnVerBarSpliceType;
		ar << m_nColumnVerBarTopLenType;
		ar << m_bColumnVerBarAlterSpliceSet;
		ar << m_nColumnVerBarAlterSpliceValue1;
		ar << m_nColumnVerBarAlterSpliceValue2;

		ar << m_bColumnDowelInfoSet;
		ar << m_bColumnDowelFootingCoverSet;
		ar << m_nColumnDowelFootingCover;
		ar << m_nColumnDowelBotPlaceType;
		ar << m_nColumnDowelTopPlaceType;
		ar << m_nColumnDowelHoopRebarType;
		ar << m_nColumnDowelHoopBySpace;
		ar << m_nColumnDowelHoopByNum;
		ar << m_nColumnDowelSupportBarDiaIndex;
		ar << m_nColumnDowelSupportBarLength;
		ar << m_nColumnDowelSupportBarNum;

		map<long, long>::iterator it;
		ar << (int)mm_ColumnUserDowelSpliceLen.size();
		for (it = mm_ColumnUserDowelSpliceLen.begin(); it != mm_ColumnUserDowelSpliceLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long spliceLen = it->second;
			ar << nDiaIndex;
			ar << spliceLen;
		}

		ar << m_bColumnEndHoopBarShapeSet;

		ar << m_nColumnDowelBarSpliceType;
		ar << m_bColumnDowelBarAlterSpliceSet;
		ar << m_nColumnDowelBarAlterSpliceValue1;
		ar << m_nColumnDowelBarAlterSpliceValue2;
		ar << m_bColumnDowelFootingPercentSet;
		ar << m_nColumnDowelFootingPercent;
	}
	else
	{
		ar >> m_bColumnGeneralInfoSet;
		ar >> m_bColumnConnectLengthSet;
		ar >> m_nColumnConnectLength;

		ar >> m_bColumnHoopBarInfoSet;
		ar >> m_bColumnHoopBarStartLengthSet;
		ar >> m_nColumnHoopBarStartLength;
		ar >> m_bColumnTieAddLengthSet;
		ar >> m_nColumnTieAddLength;
		ar >> m_nColumnTopMemberCover;
		ar >> m_nColumnHoopBarShapeIdx;
		ar >> m_nColumnTieBarShapeIdx;

		ar >> m_bColumnVerInfoSet;
		ar >> m_nColumnVerBarSpliceType;
		ar >> m_nColumnVerBarTopLenType;
		ar >> m_bColumnVerBarAlterSpliceSet;
		ar >> m_nColumnVerBarAlterSpliceValue1;
		ar >> m_nColumnVerBarAlterSpliceValue2;

		ar >> m_bColumnDowelInfoSet;
		ar >> m_bColumnDowelFootingCoverSet;
		ar >> m_nColumnDowelFootingCover;
		ar >> m_nColumnDowelBotPlaceType;
		ar >> m_nColumnDowelTopPlaceType;
		ar >> m_nColumnDowelHoopRebarType;
		ar >> m_nColumnDowelHoopBySpace;
		ar >> m_nColumnDowelHoopByNum;
		ar >> m_nColumnDowelSupportBarDiaIndex;
		ar >> m_nColumnDowelSupportBarLength;
		ar >> m_nColumnDowelSupportBarNum;

		int NumObject;

		mm_ColumnUserDowelSpliceLen.clear();
		ar >> NumObject;
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, spliceLen = 0;

			ar >> nDiaIndex;
			ar >> spliceLen;
			mm_ColumnUserDowelSpliceLen.insert(make_pair(nDiaIndex, spliceLen));
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20160212)
		{
			ar >> m_bColumnEndHoopBarShapeSet;

			ar >> m_nColumnDowelBarSpliceType;
			ar >> m_bColumnDowelBarAlterSpliceSet;
			ar >> m_nColumnDowelBarAlterSpliceValue1;
			ar >> m_nColumnDowelBarAlterSpliceValue2;
			ar >> m_bColumnDowelFootingPercentSet;
			ar >> m_nColumnDowelFootingPercent;
		}
		else
		{
			if (m_nColumnHoopBarShapeIdx == 1) 
				m_nColumnHoopBarShapeIdx = 2;
			else if (m_nColumnHoopBarShapeIdx == 2) 
				m_nColumnHoopBarShapeIdx = 1;

			if (m_nColumnTieBarShapeIdx == 1)
				m_nColumnTieBarShapeIdx = 2;
			else if (m_nColumnTieBarShapeIdx == 2)
				m_nColumnTieBarShapeIdx = 1;
		}

	}
}

long OldColumnRebarTemplate::GetColumnUserDowelSpliceLen(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it;
	it = mm_ColumnUserDowelSpliceLen.find(DiaIndex);
	if (it != mm_ColumnUserDowelSpliceLen.end())
		return it->second;

	return 0;
}

void OldColumnRebarTemplate::SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen)
{
	mm_ColumnUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapUserDowelSpliceLen.begin(); it != mapUserDowelSpliceLen.end(); it++)
	{
		mm_ColumnUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void OldColumnRebarTemplate::DataInit()
{
	m_bColumnGeneralInfoSet = false;
	m_bColumnConnectLengthSet = false;
	m_nColumnConnectLength = 75;

	m_bColumnHoopBarInfoSet = false;
	m_bColumnHoopBarStartLengthSet = false;
	m_nColumnHoopBarStartLength = 50;
	m_bColumnTieAddLengthSet = false;
	m_nColumnTieAddLength = 10;
	m_nColumnTopMemberCover = 0;
	m_nColumnHoopBarShapeIdx = 1;
	m_nColumnTieBarShapeIdx = 1;

	m_bColumnVerInfoSet = false;
	m_nColumnVerBarSpliceType = 0;
	m_nColumnVerBarTopLenType = 0;
	m_bColumnVerBarAlterSpliceSet = false;
	m_nColumnVerBarAlterSpliceValue1 = 300;
	m_nColumnVerBarAlterSpliceValue2 = 700;

	m_bColumnDowelInfoSet = false;
	m_bColumnDowelFootingCoverSet = false;
	m_nColumnDowelFootingCover = 100;
	m_nColumnDowelBotPlaceType = 1;
	m_nColumnDowelTopPlaceType = 0;
	m_nColumnDowelHoopRebarType = 1;
	m_nColumnDowelHoopBySpace = 1000;
	m_nColumnDowelHoopByNum = 3;
	m_nColumnDowelSupportBarDiaIndex = 2; // MSDeformedBar::FindBarDiameterIndexByName(_T("13"));
	m_nColumnDowelSupportBarLength = 1600;
	m_nColumnDowelSupportBarNum = 4;
	InitUserDowelSpliceLenMap(mm_ColumnUserDowelSpliceLen);

	m_bColumnEndHoopBarShapeSet = false;
	m_nColumnDowelBarSpliceType = 0;
	m_bColumnDowelBarAlterSpliceSet = false;
	m_nColumnDowelBarAlterSpliceValue1 = 300;
	m_nColumnDowelBarAlterSpliceValue2 = 700;
	m_bColumnDowelFootingPercentSet = false;
	m_nColumnDowelFootingPercent = 50;
}

bool OldColumnRebarTemplate::TopOptionDataCopy(OldColumnRebarTemplate* rebarTemplate)
{
	bool bDataCopy = false;

	if (!m_bColumnGeneralInfoSet)
	{
		m_bColumnConnectLengthSet = rebarTemplate->m_bColumnConnectLengthSet;
		m_nColumnConnectLength = rebarTemplate->m_nColumnConnectLength;
		bDataCopy = true;
	}

	if (!m_bColumnHoopBarInfoSet)
	{
		m_bColumnHoopBarStartLengthSet = rebarTemplate->m_bColumnHoopBarStartLengthSet;
		m_nColumnHoopBarStartLength = rebarTemplate->m_nColumnHoopBarStartLength;
		m_bColumnTieAddLengthSet = rebarTemplate->m_bColumnTieAddLengthSet;
		m_nColumnTieAddLength = rebarTemplate->m_nColumnTieAddLength;
		m_nColumnTopMemberCover = rebarTemplate->m_nColumnTopMemberCover;
		m_nColumnHoopBarShapeIdx = rebarTemplate->m_nColumnHoopBarShapeIdx;
		m_nColumnTieBarShapeIdx = rebarTemplate->m_nColumnTieBarShapeIdx;
		m_bColumnEndHoopBarShapeSet = rebarTemplate->m_bColumnEndHoopBarShapeSet;
		bDataCopy = true;
	}

	if (!m_bColumnVerInfoSet)
	{
		m_nColumnVerBarSpliceType = rebarTemplate->m_nColumnVerBarSpliceType;
		m_nColumnVerBarTopLenType = rebarTemplate->m_nColumnVerBarTopLenType;
		m_bColumnVerBarAlterSpliceSet = rebarTemplate->m_bColumnVerBarAlterSpliceSet;
		m_nColumnVerBarAlterSpliceValue1 = rebarTemplate->m_nColumnVerBarAlterSpliceValue1;
		m_nColumnVerBarAlterSpliceValue2 = rebarTemplate->m_nColumnVerBarAlterSpliceValue2;
		bDataCopy = true;
	}

	if (!m_bColumnDowelInfoSet)
	{
		m_bColumnDowelFootingCoverSet = rebarTemplate->m_bColumnDowelFootingCoverSet;
		m_nColumnDowelFootingCover = rebarTemplate->m_nColumnDowelFootingCover;
		m_nColumnDowelBotPlaceType = rebarTemplate->m_nColumnDowelBotPlaceType;
		m_nColumnDowelTopPlaceType = rebarTemplate->m_nColumnDowelTopPlaceType;
		m_nColumnDowelHoopRebarType = rebarTemplate->m_nColumnDowelHoopRebarType;
		m_nColumnDowelHoopBySpace = rebarTemplate->m_nColumnDowelHoopBySpace;
		m_nColumnDowelHoopByNum = rebarTemplate->m_nColumnDowelHoopByNum;
		m_nColumnDowelSupportBarDiaIndex = rebarTemplate->m_nColumnDowelSupportBarDiaIndex;
		m_nColumnDowelSupportBarLength = rebarTemplate->m_nColumnDowelSupportBarLength;
		m_nColumnDowelSupportBarNum = rebarTemplate->m_nColumnDowelSupportBarNum;
		m_nColumnDowelBarSpliceType = rebarTemplate->m_nColumnDowelBarSpliceType;
		m_bColumnDowelBarAlterSpliceSet = rebarTemplate->m_bColumnDowelBarAlterSpliceSet;
		m_nColumnDowelBarAlterSpliceValue1 = rebarTemplate->m_nColumnDowelBarAlterSpliceValue1;
		m_nColumnDowelBarAlterSpliceValue2 = rebarTemplate->m_nColumnDowelBarAlterSpliceValue2;
		m_bColumnDowelFootingPercentSet = rebarTemplate->m_bColumnDowelFootingPercentSet;
		m_nColumnDowelFootingPercent = rebarTemplate->m_nColumnDowelFootingPercent;
		UserDowelSpliceLenMapDataCopy(mm_ColumnUserDowelSpliceLen, rebarTemplate->mm_ColumnUserDowelSpliceLen);
		bDataCopy = true;
	}

	return bDataCopy;
}

void OldColumnRebarTemplate::DataCopy(OldColumnRebarTemplate* rebarTemplate)
{
	m_bColumnConnectLengthSet = rebarTemplate->m_bColumnConnectLengthSet;
	m_nColumnConnectLength = rebarTemplate->m_nColumnConnectLength;

	m_bColumnHoopBarStartLengthSet = rebarTemplate->m_bColumnHoopBarStartLengthSet;
	m_nColumnHoopBarStartLength = rebarTemplate->m_nColumnHoopBarStartLength;
	m_bColumnTieAddLengthSet = rebarTemplate->m_bColumnTieAddLengthSet;
	m_nColumnTieAddLength = rebarTemplate->m_nColumnTieAddLength;
	m_nColumnTopMemberCover = rebarTemplate->m_nColumnTopMemberCover;
	m_nColumnHoopBarShapeIdx = rebarTemplate->m_nColumnHoopBarShapeIdx;
	m_nColumnTieBarShapeIdx = rebarTemplate->m_nColumnTieBarShapeIdx;
	m_bColumnEndHoopBarShapeSet = rebarTemplate->m_bColumnEndHoopBarShapeSet;

	m_nColumnVerBarSpliceType = rebarTemplate->m_nColumnVerBarSpliceType;
	m_nColumnVerBarTopLenType = rebarTemplate->m_nColumnVerBarTopLenType;
	m_bColumnVerBarAlterSpliceSet = rebarTemplate->m_bColumnVerBarAlterSpliceSet;
	m_nColumnVerBarAlterSpliceValue1 = rebarTemplate->m_nColumnVerBarAlterSpliceValue1;
	m_nColumnVerBarAlterSpliceValue2 = rebarTemplate->m_nColumnVerBarAlterSpliceValue2;

	m_bColumnDowelFootingCoverSet = rebarTemplate->m_bColumnDowelFootingCoverSet;
	m_nColumnDowelFootingCover = rebarTemplate->m_nColumnDowelFootingCover;
	m_nColumnDowelBotPlaceType = rebarTemplate->m_nColumnDowelBotPlaceType;
	m_nColumnDowelTopPlaceType = rebarTemplate->m_nColumnDowelTopPlaceType;
	m_nColumnDowelHoopRebarType = rebarTemplate->m_nColumnDowelHoopRebarType;
	m_nColumnDowelHoopBySpace = rebarTemplate->m_nColumnDowelHoopBySpace;
	m_nColumnDowelHoopByNum = rebarTemplate->m_nColumnDowelHoopByNum;
	m_nColumnDowelSupportBarDiaIndex = rebarTemplate->m_nColumnDowelSupportBarDiaIndex;
	m_nColumnDowelSupportBarLength = rebarTemplate->m_nColumnDowelSupportBarLength;
	m_nColumnDowelSupportBarNum = rebarTemplate->m_nColumnDowelSupportBarNum;
	m_nColumnDowelBarSpliceType = rebarTemplate->m_nColumnDowelBarSpliceType;
	m_bColumnDowelBarAlterSpliceSet = rebarTemplate->m_bColumnDowelBarAlterSpliceSet;
	m_nColumnDowelBarAlterSpliceValue1 = rebarTemplate->m_nColumnDowelBarAlterSpliceValue1;
	m_nColumnDowelBarAlterSpliceValue2 = rebarTemplate->m_nColumnDowelBarAlterSpliceValue2;
	m_bColumnDowelFootingPercentSet = rebarTemplate->m_bColumnDowelFootingPercentSet;
	m_nColumnDowelFootingPercent = rebarTemplate->m_nColumnDowelFootingPercent;
	UserDowelSpliceLenMapDataCopy(mm_ColumnUserDowelSpliceLen, rebarTemplate->mm_ColumnUserDowelSpliceLen);
}

void OldColumnRebarTemplate::OldDataCopy(ColumnRebarTemplate* rebarTemplate)
{
	rebarTemplate->SetColumnHoopBarStartLengthSet(m_bColumnHoopBarStartLengthSet);
	rebarTemplate->SetColumnHoopBarStartLength(m_nColumnHoopBarStartLength);
	rebarTemplate->SetColumnTieAddLengthSet(m_bColumnTieAddLengthSet);
	rebarTemplate->SetColumnTieAddLength(m_nColumnTieAddLength);
	rebarTemplate->SetColumnTopMemberCover(m_nColumnTopMemberCover);
	rebarTemplate->SetColumnHoopBarShapeIdx(m_nColumnHoopBarShapeIdx);
	rebarTemplate->SetColumnTieBarShapeIdx(m_nColumnTieBarShapeIdx);
	rebarTemplate->SetColumnEndHoopBarShapeSet(m_bColumnEndHoopBarShapeSet);

	ColumnVerBarRebarTemplate *pVerBarTemplateBelowDia = rebarTemplate->GetColumnVerBarTemplateBelowDia();
	ColumnVerBarRebarTemplate *pVerBarTemplateExcessDia = rebarTemplate->GetColumnVerBarTemplateExcessDia();
	pVerBarTemplateBelowDia->SetBarSpliceType(m_nColumnVerBarSpliceType);
	pVerBarTemplateBelowDia->SetTopLenType(m_nColumnVerBarTopLenType);
	pVerBarTemplateBelowDia->SetAlterSpliceSet(m_bColumnVerBarAlterSpliceSet);
	pVerBarTemplateBelowDia->SetConstructionHeight(min(m_nColumnVerBarAlterSpliceValue1, m_nColumnVerBarAlterSpliceValue2));
	pVerBarTemplateBelowDia->SetAlterSpliceDist(abs(m_nColumnVerBarAlterSpliceValue1 - m_nColumnVerBarAlterSpliceValue2));

	pVerBarTemplateExcessDia->CopyFromMe(pVerBarTemplateBelowDia);

	ColumnDowelRebarTemplate *pDowelRebarTemplateBelowDia = rebarTemplate->GetColumnDowelTemplateBelowDia();
	ColumnDowelRebarTemplate *pDowelRebarTemplateExcessDia = rebarTemplate->GetColumnDowelTemplateExcessDia();
	pDowelRebarTemplateBelowDia->SetFootingCoverSet(m_bColumnDowelFootingCoverSet);
	pDowelRebarTemplateBelowDia->SetFootingCover(m_nColumnDowelFootingCover);
	pDowelRebarTemplateBelowDia->SetBotPlaceType(m_nColumnDowelBotPlaceType);
	pDowelRebarTemplateBelowDia->SetTopLenType(m_nColumnDowelTopPlaceType);
	pDowelRebarTemplateBelowDia->SetHoopRebarType(m_nColumnDowelHoopRebarType);
	pDowelRebarTemplateBelowDia->SetHoopBySpace(m_nColumnDowelHoopBySpace);
	pDowelRebarTemplateBelowDia->SetHoopByNum(m_nColumnDowelHoopByNum);
	pDowelRebarTemplateBelowDia->SetSupportBarDiaIndex(m_nColumnDowelSupportBarDiaIndex);
	pDowelRebarTemplateBelowDia->SetSupportBarLength(m_nColumnDowelSupportBarLength);
	pDowelRebarTemplateBelowDia->SetSupportBarNum(m_nColumnDowelSupportBarNum);
	pDowelRebarTemplateBelowDia->SetBarSpliceType(m_nColumnDowelBarSpliceType);
	pDowelRebarTemplateBelowDia->SetAlterSpliceSet(m_bColumnDowelBarAlterSpliceSet);
	pDowelRebarTemplateBelowDia->SetConstructionHeight(min(m_nColumnDowelBarAlterSpliceValue1, m_nColumnDowelBarAlterSpliceValue2));
	pDowelRebarTemplateBelowDia->SetAlterSpliceDist(abs(m_nColumnDowelBarAlterSpliceValue1 - m_nColumnDowelBarAlterSpliceValue2));
	pDowelRebarTemplateBelowDia->SetFootingPercentSet(m_bColumnDowelFootingPercentSet);
	pDowelRebarTemplateBelowDia->SetFootingPercent(m_nColumnDowelFootingPercent);
	pDowelRebarTemplateExcessDia->CopyFromMe(pDowelRebarTemplateBelowDia);



	rebarTemplate->SetColumnUserDowelSpliceLenMap(mm_ColumnUserDowelSpliceLen);
}

void OldColumnRebarTemplate::InfoSetCheckBoxChecked()
{
	m_bColumnGeneralInfoSet = true;
	m_bColumnHoopBarInfoSet = true;
	m_bColumnVerInfoSet = true;
	m_bColumnDowelInfoSet = true;
}
