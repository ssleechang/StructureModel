#include "StdAfx.h"
#include "MSObject.h"
#include "TransferSlabRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "SlabHorSupportRebarsTemplate.h"
#include "ShearFrictionBarTemplate.h"

TransferSlabRebarTemplate::TransferSlabRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pTransferSlabShearFrictionBarTemplate = new ShearFrictionBarTemplate();
}

TransferSlabRebarTemplate::~TransferSlabRebarTemplate(void)
{
	delete m_pTransferSlabShearFrictionBarTemplate;
}

void TransferSlabRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	TransferSlabRebarTemplate* pTransferSlabRebarTemplate = dynamic_cast<TransferSlabRebarTemplate*>(pBaseRebarTemplate);
	if (pTransferSlabRebarTemplate == nullptr) return;

	m_pTransferSlabShearFrictionBarTemplate->CopyFromMe(pBaseRebarTemplate->GetTransferSlabShearFrictionBarTemplate());
	
	// TransferSlab Slab
	m_nTransferSlabMaxDualHookLength = pTransferSlabRebarTemplate->m_nTransferSlabMaxDualHookLength;
	m_nTransferSlabSpliceWay = pTransferSlabRebarTemplate->m_nTransferSlabSpliceWay;
	m_nTransferSlabSpliceType = pTransferSlabRebarTemplate->m_nTransferSlabSpliceType;
		
	m_nTransferSlabTopEndHookType = pTransferSlabRebarTemplate->m_nTransferSlabTopEndHookType;
	m_nTransferSlabBotEndHookType = pTransferSlabRebarTemplate->m_nTransferSlabBotEndHookType;
	m_nTransferSlabIsoFootDevType = pTransferSlabRebarTemplate->m_nTransferSlabIsoFootDevType;
	m_nTransferSlabSpliceDevSlabType = pTransferSlabRebarTemplate->m_nTransferSlabSpliceDevSlabType;

	m_nTransferSlabTopEndDevTypeUserLength = pTransferSlabRebarTemplate->m_nTransferSlabTopEndDevTypeUserLength;
	m_nTransferSlabBotEndDevTypeUserLength = pTransferSlabRebarTemplate->m_nTransferSlabBotEndDevTypeUserLength;
	m_nTransferSlabOpeningTopDevType = pTransferSlabRebarTemplate->m_nTransferSlabOpeningTopDevType;
	m_nTransferSlabOpeningBotDevType = pTransferSlabRebarTemplate->m_nTransferSlabOpeningBotDevType;

	m_nTransferSlabLevelDifPlaceShape = pTransferSlabRebarTemplate->m_nTransferSlabLevelDifPlaceShape;
	m_nTransferSlabLevelDifHaunchPlaceType = pTransferSlabRebarTemplate->m_nTransferSlabLevelDifHaunchPlaceType;
		
	m_nTransferSlabRebarTolDist = pTransferSlabRebarTemplate->m_nTransferSlabRebarTolDist;
	m_nTransferSlabRebarTolLenDif = pTransferSlabRebarTemplate->m_nTransferSlabRebarTolLenDif;

	m_bTransferSlabSpliceDevConditionSet = pTransferSlabRebarTemplate->m_bTransferSlabSpliceDevConditionSet;
	m_nTransferSlabSpliceDevConditionLength = pTransferSlabRebarTemplate->m_nTransferSlabSpliceDevConditionLength;

	m_bTransferSlabSpliceAddLengthSet = pTransferSlabRebarTemplate->m_bTransferSlabSpliceAddLengthSet;
	m_nTransferSlabSpliceAddLengthNumSplice = pTransferSlabRebarTemplate->m_nTransferSlabSpliceAddLengthNumSplice;

	m_bTransferSlabTopSpacerSet = pTransferSlabRebarTemplate->m_bTransferSlabTopSpacerSet;
	m_nTransferSlabTopSpacerDiaType = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerDiaType;
	m_nTransferSlabTopSpacerSlabThick = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerSlabThick;
	m_nTransferSlabTopSpacerSlabThickOverDiaIndex = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerSlabThickOverDiaIndex;
	m_nTransferSlabTopSpacerSlabThickUnderDiaIndex = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerSlabThickUnderDiaIndex;
	m_nTransferSlabTopSpacerSpacingType = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerSpacingType;
	m_nTransferSlabTopSpacerSlabSpacing = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerSlabSpacing;
	m_nTransferSlabTopSpacerUserSpacing1 = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerUserSpacing1;
	m_nTransferSlabTopSpacerUserSpacing2 = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerUserSpacing2;
	m_nTransferSlabTopSpacerHeightType = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerHeightType;
	m_nTransferSlabTopSpacerUserHeight = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerUserHeight;
	m_nTransferSlabTopSpacerHookLenAType = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerHookLenAType;
	m_nTransferSlabTopSpacerMainBarHookLenA = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerMainBarHookLenA;
	m_nTransferSlabTopSpacerUserHookLenA = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerUserHookLenA;
	m_nTransferSlabTopSpacerHookLenBType = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerHookLenBType;
	m_nTransferSlabTopSpacerMainBarHookLenB = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerMainBarHookLenB;
	m_nTransferSlabTopSpacerUserHookLenB = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerUserHookLenB;
	m_nTransferSlabTopSpacerWidth = pTransferSlabRebarTemplate->m_nTransferSlabTopSpacerWidth;

	m_bTransferSlabBotSpacerSet = pTransferSlabRebarTemplate->m_bTransferSlabBotSpacerSet;
	m_nTransferSlabBotSpacerDiaType = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerDiaType;
	m_nTransferSlabBotSpacerDiaIndex = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerDiaIndex;
	m_nTransferSlabBotSpacerSpacingType = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerSpacingType;
	m_nTransferSlabBotSpacerSlabSpacing = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerSlabSpacing;
	m_nTransferSlabBotSpacerUserSpacing1 = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerUserSpacing1;
	m_nTransferSlabBotSpacerUserSpacing2 = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerUserSpacing2;
	m_nTransferSlabBotSpacerHeightType = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerHeightType;
	m_nTransferSlabBotSpacerUserHeight = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerUserHeight;
	m_nTransferSlabBotSpacerHookLenAType = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerHookLenAType;
	m_nTransferSlabBotSpacerMainBarHookLenA = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerMainBarHookLenA;
	m_nTransferSlabBotSpacerUserHookLenA = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerUserHookLenA;
	m_nTransferSlabBotSpacerHookLenBType = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerHookLenBType;
	m_nTransferSlabBotSpacerMainBarHookLenB = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerMainBarHookLenB;
	m_nTransferSlabBotSpacerUserHookLenB = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerUserHookLenB;
	m_nTransferSlabBotSpacerWidth = pTransferSlabRebarTemplate->m_nTransferSlabBotSpacerWidth;

	m_bTransferSlabSpacerSupportBarsSet = pTransferSlabRebarTemplate->m_bTransferSlabSpacerSupportBarsSet;
	m_bTransferSlabSpacerSupportBarsConditionSet = pTransferSlabRebarTemplate->m_bTransferSlabSpacerSupportBarsConditionSet;
	m_nTransferSlabSpacerSupportBarsCondition = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsCondition;
	m_nTransferSlabSpacerSupportBarsDiaType = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsDiaType;
	m_nTransferSlabSpacerSupportBarsUserDiaIndex = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsUserDiaIndex;
	m_nTransferSlabSpacerSupportBarsNumType = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsNumType;
	m_dTransferSlabSpacerSupportBarsTopSpacerNums = pTransferSlabRebarTemplate->m_dTransferSlabSpacerSupportBarsTopSpacerNums;
	m_nTransferSlabSpacerSupportBarsUserNums = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsUserNums;
	m_nTransferSlabSpacerSupportBarsLengthType = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsLengthType;
	m_dTransferSlabSpacerSupportBarsUserRatio = pTransferSlabRebarTemplate->m_dTransferSlabSpacerSupportBarsUserRatio;
	m_nTransferSlabSpacerSupportBarsUserLength = pTransferSlabRebarTemplate->m_nTransferSlabSpacerSupportBarsUserLength;

	m_nTransferSlabCZoneSpliceWay = pTransferSlabRebarTemplate->m_nTransferSlabCZoneSpliceWay;
	m_nTransferSlabCZoneSpliceLength = pTransferSlabRebarTemplate->m_nTransferSlabCZoneSpliceLength;
	
	m_bTransferSlabBotSpacerUserSteelGradeS = pTransferSlabRebarTemplate->m_bTransferSlabBotSpacerUserSteelGradeS;
	m_bTransferSlabTopSpacerUserSteelGradeS = pTransferSlabRebarTemplate->m_bTransferSlabTopSpacerUserSteelGradeS;
	m_bTransferSlabSpacerSupportBarUserSteelGradeS = pTransferSlabRebarTemplate->m_bTransferSlabSpacerSupportBarUserSteelGradeS;

	m_bTransferSlabTopSpacerPlaceConditionSet = pTransferSlabRebarTemplate->GetTransferSlabTopSpacerPlaceConditionSet();
	m_nTransferSlabTopSpacerPlaceCondition = pTransferSlabRebarTemplate->GetTransferSlabTopSpacerPlaceCondition();	

	mm_TransferSlabThickToDiaIndex = pTransferSlabRebarTemplate->mm_TransferSlabThickToDiaIndex;

	m_bTransferSlabEndAddBarSet = pTransferSlabRebarTemplate->GetTransferSlabEndAddBarSet();
	m_nTransferSlabEndVerAddBarDiaIndex = pTransferSlabRebarTemplate->GetTransferSlabEndVerAddBarDiaIndex();
	m_nTransferSlabEndVerAddBarSpacing = pTransferSlabRebarTemplate->GetTransferSlabEndVerAddBarSpacing();
	m_nTransferSlabEndHorAddBarPlaceCondition = pTransferSlabRebarTemplate->GetTransferSlabEndHorAddBarPlaceCondition();
	m_nTransferSlabEndHorAddBarDiaIndex = pTransferSlabRebarTemplate->GetTransferSlabEndHorAddBarDiaIndex();
	m_nTransferSlabEndHorAddBarSpacing = pTransferSlabRebarTemplate->GetTransferSlabEndHorAddBarSpacing();
}

void TransferSlabRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		m_pTransferSlabShearFrictionBarTemplate->Serialize(ar);
	
		ar << m_bTransferSlabMainBarInfoSet;
		ar << m_nTransferSlabMaxDualHookLength;
		ar << m_nTransferSlabSpliceWay;
		ar << m_nTransferSlabSpliceType;
		ar << m_bTransferSlabEndInfoSet;
		ar << m_nTransferSlabTopEndHookType;
		ar << m_nTransferSlabBotEndHookType;
		ar << m_nTransferSlabIsoFootDevType;
		ar << m_nTransferSlabSpliceDevSlabType;
		ar << m_nTransferSlabTopEndDevTypeUserLength;
		ar << m_nTransferSlabBotEndDevTypeUserLength;
		ar << m_nTransferSlabOpeningTopDevType;
		ar << m_nTransferSlabOpeningBotDevType;
		ar << m_bTransferSlabLevelDifInfoSet;
		ar << m_nTransferSlabLevelDifPlaceShape;
		ar << m_nTransferSlabLevelDifHaunchPlaceType;
		ar << m_bTransferSlabOpeningInfoSet;
		ar << m_bTransferSlabRebarAreaInfoSet;
		ar << m_nTransferSlabRebarTolDist;
		ar << m_nTransferSlabRebarTolLenDif;
		ar << m_bTransferSlabSpliceDevConditionSet;
		ar << m_nTransferSlabSpliceDevConditionLength;
		ar << m_bTransferSlabSpliceAddLengthSet;
		ar << m_nTransferSlabSpliceAddLengthNumSplice;
		ar << m_bTransferSlabTopSpacerInfoSet;
		ar << m_bTransferSlabTopSpacerSet;
		ar << m_nTransferSlabTopSpacerDiaType;
		ar << m_nTransferSlabTopSpacerSlabThick;
		ar << m_nTransferSlabTopSpacerSlabThickOverDiaIndex;
		ar << m_nTransferSlabTopSpacerSlabThickUnderDiaIndex;
		ar << m_nTransferSlabTopSpacerSpacingType;
		ar << m_nTransferSlabTopSpacerSlabSpacing;
		ar << m_nTransferSlabTopSpacerUserSpacing1;
		ar << m_nTransferSlabTopSpacerUserSpacing2;
		ar << m_nTransferSlabTopSpacerHeightType;
		ar << m_nTransferSlabTopSpacerUserHeight;
		ar << m_nTransferSlabTopSpacerHookLenAType;
		ar << m_nTransferSlabTopSpacerMainBarHookLenA;
		ar << m_nTransferSlabTopSpacerUserHookLenA;
		ar << m_nTransferSlabTopSpacerHookLenBType;
		ar << m_nTransferSlabTopSpacerMainBarHookLenB;
		ar << m_nTransferSlabTopSpacerUserHookLenB;
		ar << m_nTransferSlabTopSpacerWidth;
		ar << m_bTransferSlabBotSpacerInfoSet;
		ar << m_bTransferSlabBotSpacerSet;
		ar << m_nTransferSlabBotSpacerDiaType;
		ar << m_nTransferSlabBotSpacerDiaIndex;
		ar << m_nTransferSlabBotSpacerSpacingType;
		ar << m_nTransferSlabBotSpacerSlabSpacing;
		ar << m_nTransferSlabBotSpacerUserSpacing1;
		ar << m_nTransferSlabBotSpacerUserSpacing2;
		ar << m_nTransferSlabBotSpacerHeightType;
		ar << m_nTransferSlabBotSpacerUserHeight;
		ar << m_nTransferSlabBotSpacerHookLenAType;
		ar << m_nTransferSlabBotSpacerMainBarHookLenA;
		ar << m_nTransferSlabBotSpacerUserHookLenA;
		ar << m_nTransferSlabBotSpacerHookLenBType;
		ar << m_nTransferSlabBotSpacerMainBarHookLenB;
		ar << m_nTransferSlabBotSpacerUserHookLenB;
		ar << m_nTransferSlabBotSpacerWidth;
		ar << m_bTransferSlabSpacerSupportBarsSet;
		ar << m_bTransferSlabSpacerSupportBarsConditionSet;
		ar << m_nTransferSlabSpacerSupportBarsCondition;
		ar << m_nTransferSlabSpacerSupportBarsDiaType;
		ar << m_nTransferSlabSpacerSupportBarsUserDiaIndex;
		ar << m_nTransferSlabSpacerSupportBarsNumType;
		ar << m_dTransferSlabSpacerSupportBarsTopSpacerNums;
		ar << m_nTransferSlabSpacerSupportBarsUserNums;
		ar << m_nTransferSlabSpacerSupportBarsLengthType;
		ar << m_dTransferSlabSpacerSupportBarsUserRatio;
		ar << m_nTransferSlabSpacerSupportBarsUserLength;
		ar << m_nTransferSlabCZoneSpliceWay;
		ar << m_nTransferSlabCZoneSpliceLength;
		ar << m_bTransferSlabBotSpacerUserSteelGradeS;
		ar << m_bTransferSlabTopSpacerUserSteelGradeS;
		ar << m_bTransferSlabSpacerSupportBarUserSteelGradeS;
		ar << m_bTransferSlabTopSpacerPlaceConditionSet;
		ar << m_nTransferSlabTopSpacerPlaceCondition;

		ar << (int)mm_TransferSlabThickToDiaIndex.size();
		for (auto it = mm_TransferSlabThickToDiaIndex.begin(); it != mm_TransferSlabThickToDiaIndex.end(); it++)
		{
			CString sThick = it->first;
			long nDiaIndex = it->second;
			ar << sThick;
			ar << nDiaIndex;
		}

		ar << m_bTransferSlabEndAddBarSet;
		ar << m_nTransferSlabEndVerAddBarDiaIndex;
		ar << m_nTransferSlabEndVerAddBarSpacing;
		ar << m_nTransferSlabEndHorAddBarPlaceCondition;
		ar << m_nTransferSlabEndHorAddBarDiaIndex;
		ar << m_nTransferSlabEndHorAddBarSpacing;
	}
	else
	{
		m_pTransferSlabShearFrictionBarTemplate->Serialize(ar);
	
		ar >> m_bTransferSlabMainBarInfoSet;
		ar >> m_nTransferSlabMaxDualHookLength;
		ar >> m_nTransferSlabSpliceWay;
		ar >> m_nTransferSlabSpliceType;
		ar >> m_bTransferSlabEndInfoSet;
		ar >> m_nTransferSlabTopEndHookType;
		ar >> m_nTransferSlabBotEndHookType;
		ar >> m_nTransferSlabIsoFootDevType;
		ar >> m_nTransferSlabSpliceDevSlabType;
		ar >> m_nTransferSlabTopEndDevTypeUserLength;
		ar >> m_nTransferSlabBotEndDevTypeUserLength;
		ar >> m_nTransferSlabOpeningTopDevType;
		ar >> m_nTransferSlabOpeningBotDevType;
		ar >> m_bTransferSlabLevelDifInfoSet;
		ar >> m_nTransferSlabLevelDifPlaceShape;
		ar >> m_nTransferSlabLevelDifHaunchPlaceType;
		ar >> m_bTransferSlabOpeningInfoSet;
		ar >> m_bTransferSlabRebarAreaInfoSet;
		ar >> m_nTransferSlabRebarTolDist;
		ar >> m_nTransferSlabRebarTolLenDif;
		ar >> m_bTransferSlabSpliceDevConditionSet;
		ar >> m_nTransferSlabSpliceDevConditionLength;
		ar >> m_bTransferSlabSpliceAddLengthSet;
		ar >> m_nTransferSlabSpliceAddLengthNumSplice;
		ar >> m_bTransferSlabTopSpacerInfoSet;
		ar >> m_bTransferSlabTopSpacerSet;
		ar >> m_nTransferSlabTopSpacerDiaType;
		ar >> m_nTransferSlabTopSpacerSlabThick;
		ar >> m_nTransferSlabTopSpacerSlabThickOverDiaIndex;
		ar >> m_nTransferSlabTopSpacerSlabThickUnderDiaIndex;
		ar >> m_nTransferSlabTopSpacerSpacingType;
		ar >> m_nTransferSlabTopSpacerSlabSpacing;
		ar >> m_nTransferSlabTopSpacerUserSpacing1;
		ar >> m_nTransferSlabTopSpacerUserSpacing2;
		ar >> m_nTransferSlabTopSpacerHeightType;
		ar >> m_nTransferSlabTopSpacerUserHeight;
		ar >> m_nTransferSlabTopSpacerHookLenAType;
		ar >> m_nTransferSlabTopSpacerMainBarHookLenA;
		ar >> m_nTransferSlabTopSpacerUserHookLenA;
		ar >> m_nTransferSlabTopSpacerHookLenBType;
		ar >> m_nTransferSlabTopSpacerMainBarHookLenB;
		ar >> m_nTransferSlabTopSpacerUserHookLenB;
		ar >> m_nTransferSlabTopSpacerWidth;
		ar >> m_bTransferSlabBotSpacerInfoSet;
		ar >> m_bTransferSlabBotSpacerSet;
		ar >> m_nTransferSlabBotSpacerDiaType;
		ar >> m_nTransferSlabBotSpacerDiaIndex;
		ar >> m_nTransferSlabBotSpacerSpacingType;
		ar >> m_nTransferSlabBotSpacerSlabSpacing;
		ar >> m_nTransferSlabBotSpacerUserSpacing1;
		ar >> m_nTransferSlabBotSpacerUserSpacing2;
		ar >> m_nTransferSlabBotSpacerHeightType;
		ar >> m_nTransferSlabBotSpacerUserHeight;
		ar >> m_nTransferSlabBotSpacerHookLenAType;
		ar >> m_nTransferSlabBotSpacerMainBarHookLenA;
		ar >> m_nTransferSlabBotSpacerUserHookLenA;
		ar >> m_nTransferSlabBotSpacerHookLenBType;
		ar >> m_nTransferSlabBotSpacerMainBarHookLenB;
		ar >> m_nTransferSlabBotSpacerUserHookLenB;
		ar >> m_nTransferSlabBotSpacerWidth;
		ar >> m_bTransferSlabSpacerSupportBarsSet;
		ar >> m_bTransferSlabSpacerSupportBarsConditionSet;
		ar >> m_nTransferSlabSpacerSupportBarsCondition;
		ar >> m_nTransferSlabSpacerSupportBarsDiaType;
		ar >> m_nTransferSlabSpacerSupportBarsUserDiaIndex;
		ar >> m_nTransferSlabSpacerSupportBarsNumType;
		ar >> m_dTransferSlabSpacerSupportBarsTopSpacerNums;
		ar >> m_nTransferSlabSpacerSupportBarsUserNums;
		ar >> m_nTransferSlabSpacerSupportBarsLengthType;
		ar >> m_dTransferSlabSpacerSupportBarsUserRatio;
		ar >> m_nTransferSlabSpacerSupportBarsUserLength;
		ar >> m_nTransferSlabCZoneSpliceWay;
		ar >> m_nTransferSlabCZoneSpliceLength;
		ar >> m_bTransferSlabBotSpacerUserSteelGradeS;
		ar >> m_bTransferSlabTopSpacerUserSteelGradeS;
		ar >> m_bTransferSlabSpacerSupportBarUserSteelGradeS;
		ar >> m_bTransferSlabTopSpacerPlaceConditionSet;
		ar >> m_nTransferSlabTopSpacerPlaceCondition;

		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			int NumObject = 0;
			mm_TransferSlabThickToDiaIndex.clear();
			ar >> NumObject;
			if (NumObject > 0)
				mm_TransferSlabThickToDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				CString sThick = _T("");
				long nDiaIndex = 0;

				ar >> sThick;
				ar >> nDiaIndex;
				mm_TransferSlabThickToDiaIndex.insert(make_pair(sThick, nDiaIndex));
			}
		}
		else
		{
			mm_TransferSlabThickToDiaIndex.clear();
			CString sThick;

			sThick.Format(_T("%ld"), m_nTransferSlabTopSpacerSlabThick - 1);
			mm_TransferSlabThickToDiaIndex.insert(make_pair(sThick, m_nTransferSlabTopSpacerSlabThickUnderDiaIndex));
			mm_TransferSlabThickToDiaIndex.insert(make_pair(_T("Default"), m_nTransferSlabTopSpacerSlabThickOverDiaIndex));
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20250225)
		{
			ar >> m_bTransferSlabEndAddBarSet;
			ar >> m_nTransferSlabEndVerAddBarDiaIndex;
			ar >> m_nTransferSlabEndVerAddBarSpacing;
			ar >> m_nTransferSlabEndHorAddBarPlaceCondition;
			ar >> m_nTransferSlabEndHorAddBarDiaIndex;
			ar >> m_nTransferSlabEndHorAddBarSpacing;
		}
	}
}

void TransferSlabRebarTemplate::DataInit()
{
	// TransferSlab Slab
	m_bTransferSlabMainBarInfoSet = false;
	m_nTransferSlabMaxDualHookLength = 2000;
	m_nTransferSlabSpliceWay = 0;
	m_nTransferSlabSpliceType = 0;
	m_bTransferSlabEndInfoSet = false;
	m_nTransferSlabTopEndHookType = 1;
	m_nTransferSlabBotEndHookType = 3;
	m_nTransferSlabIsoFootDevType = 1;
	m_nTransferSlabSpliceDevSlabType = 1;
	m_nTransferSlabTopEndDevTypeUserLength = 0;
	m_nTransferSlabBotEndDevTypeUserLength = 0;
	m_nTransferSlabOpeningTopDevType = 1;
	m_nTransferSlabOpeningBotDevType = 1;
	m_bTransferSlabLevelDifInfoSet = false;
	m_nTransferSlabLevelDifPlaceShape = 0;
	m_nTransferSlabLevelDifHaunchPlaceType = 0;	
	m_bTransferSlabOpeningInfoSet = false;	
	m_bTransferSlabRebarAreaInfoSet = false;
	m_nTransferSlabRebarTolDist = 10;
	m_nTransferSlabRebarTolLenDif = 10;
	m_bTransferSlabSpliceDevConditionSet = false;
	m_nTransferSlabSpliceDevConditionLength = 340;
	m_bTransferSlabTopSpacerInfoSet = false;
	m_bTransferSlabTopSpacerSet = false;
	m_nTransferSlabTopSpacerDiaType = 0;
	m_nTransferSlabTopSpacerSlabThick = 700;
	m_nTransferSlabTopSpacerSlabThickOverDiaIndex = 2;
	m_nTransferSlabTopSpacerSlabThickUnderDiaIndex = 1;
	m_nTransferSlabTopSpacerSpacingType = 1;
	m_nTransferSlabTopSpacerSlabSpacing = 3;
	m_nTransferSlabTopSpacerUserSpacing1 = 1500;
	m_nTransferSlabTopSpacerUserSpacing2 = 1500;
	m_nTransferSlabTopSpacerHeightType = 0;
	m_nTransferSlabTopSpacerUserHeight = 300;
	m_nTransferSlabTopSpacerHookLenAType = 1;
	m_nTransferSlabTopSpacerMainBarHookLenA = 100;
	m_nTransferSlabTopSpacerUserHookLenA = 100;
	m_nTransferSlabTopSpacerHookLenBType = 1;
	m_nTransferSlabTopSpacerMainBarHookLenB = 0;
	m_nTransferSlabTopSpacerUserHookLenB = 100;
	m_nTransferSlabTopSpacerWidth = 200;
	m_bTransferSlabBotSpacerInfoSet = false;
	m_bTransferSlabBotSpacerSet = false;
	m_nTransferSlabBotSpacerDiaType = 0;
	m_nTransferSlabBotSpacerDiaIndex = 0;
	m_nTransferSlabBotSpacerSpacingType = 1;
	m_nTransferSlabBotSpacerSlabSpacing = 3;
	m_nTransferSlabBotSpacerUserSpacing1 = 1500;
	m_nTransferSlabBotSpacerUserSpacing2 = 1500;
	m_nTransferSlabBotSpacerHeightType = 0;
	m_nTransferSlabBotSpacerUserHeight = 300;
	m_nTransferSlabBotSpacerHookLenAType = 1;
	m_nTransferSlabBotSpacerMainBarHookLenA = 100;
	m_nTransferSlabBotSpacerUserHookLenA = 100;
	m_nTransferSlabBotSpacerHookLenBType = 1;
	m_nTransferSlabBotSpacerMainBarHookLenB = 0;
	m_nTransferSlabBotSpacerUserHookLenB = 100;
	m_nTransferSlabBotSpacerWidth = 200;
	m_bTransferSlabSpacerSupportBarsSet = false;
	m_bTransferSlabSpacerSupportBarsConditionSet = true;
	m_nTransferSlabSpacerSupportBarsCondition = 1000;
	m_nTransferSlabSpacerSupportBarsDiaType = 1;
	m_nTransferSlabSpacerSupportBarsUserDiaIndex = 0;
	m_nTransferSlabSpacerSupportBarsNumType = 0;
	m_dTransferSlabSpacerSupportBarsTopSpacerNums = 0.5;
	m_nTransferSlabSpacerSupportBarsUserNums = 10;
	m_nTransferSlabSpacerSupportBarsLengthType = 1;
	m_nTransferSlabSpacerSupportBarsUserLength = 1600;
	m_bTransferSlabSpliceAddLengthSet = true;
	m_nTransferSlabSpliceAddLengthNumSplice = 3;	
	m_nTransferSlabCZoneSpliceWay = 0;
	m_nTransferSlabCZoneSpliceLength = 300;	
	m_dTransferSlabSpacerSupportBarsUserRatio = 1.4;	
	m_bTransferSlabBotSpacerUserSteelGradeS = FALSE;
	m_bTransferSlabTopSpacerUserSteelGradeS = FALSE;
	m_bTransferSlabSpacerSupportBarUserSteelGradeS = FALSE;	
	m_bTransferSlabTopSpacerPlaceConditionSet = FALSE;
	m_nTransferSlabTopSpacerPlaceCondition = 0;

	mm_TransferSlabThickToDiaIndex.insert(make_pair(_T("700"), 0));
	mm_TransferSlabThickToDiaIndex.insert(make_pair(_T("Default"), 1));

	m_bTransferSlabEndAddBarSet = FALSE;
	m_nTransferSlabEndVerAddBarDiaIndex = 1;
	m_nTransferSlabEndVerAddBarSpacing = 300;
	m_nTransferSlabEndHorAddBarPlaceCondition = 900;
	m_nTransferSlabEndHorAddBarDiaIndex = 0;
	m_nTransferSlabEndHorAddBarSpacing = 200;
}

ShearFrictionBarTemplate *TransferSlabRebarTemplate::GetTransferSlabShearFrictionBarTemplate()
{
	return m_pTransferSlabShearFrictionBarTemplate;
}

void TransferSlabRebarTemplate::SetTransferSlabThickToDiaIndex(map<CString, long> value)
{
	mm_TransferSlabThickToDiaIndex.clear();	
	mm_TransferSlabThickToDiaIndex.insert(value.begin(), value.end());
}