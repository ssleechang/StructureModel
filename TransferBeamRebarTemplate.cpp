#include "StdAfx.h"
#include "TransferBeamRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "ShearFrictionBarTemplate.h"

TransferBeamRebarTemplate::TransferBeamRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pTransferBeamShearFrictionBarTemplate = new ShearFrictionBarTemplate();
}

TransferBeamRebarTemplate::~TransferBeamRebarTemplate(void)
{
}
ShearFrictionBarTemplate *TransferBeamRebarTemplate::GetTransferBeamShearFrictionBarTemplate()
{
	return m_pTransferBeamShearFrictionBarTemplate;
}
void TransferBeamRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	TransferBeamRebarTemplate* pBeamRebarTemplate = dynamic_cast<TransferBeamRebarTemplate*>(pBaseRebarTemplate);
	if (pBeamRebarTemplate == nullptr) return;

	m_pTransferBeamShearFrictionBarTemplate->CopyFromMe(pBeamRebarTemplate->GetTransferBeamShearFrictionBarTemplate());

	m_bTransferBeamGeneralInfoSet = pBeamRebarTemplate->m_bTransferBeamGeneralInfoSet;
	m_nTransferBeamTopMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nTransferBeamTopMaxLen2HookAnchorBar;
	m_nTransferBeamBotMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nTransferBeamBotMaxLen2HookAnchorBar;
	m_nTransferBeamBarLayerGap = pBeamRebarTemplate->m_nTransferBeamBarLayerGap;
	m_nTransferBeamMainBarConnectionTolerance = pBeamRebarTemplate->GetTransferBeamMainBarConnectionTolerance();
	m_nTransferBeamLineContinuityHorEcc = pBeamRebarTemplate->GetTransferBeamLineContinuityHorEcc();
	m_nTransferBeamLineContinuityVerEcc = pBeamRebarTemplate->GetTransferBeamLineContinuityVerEcc();

	m_bTransferBeamSpliceDevConditionSet = pBeamRebarTemplate->m_bTransferBeamSpliceDevConditionSet;
	m_nTransferBeamSpliceDevConditionLength = pBeamRebarTemplate->m_nTransferBeamSpliceDevConditionLength;

	m_bTransferBeamStirrupInfoSet = pBeamRebarTemplate->m_bTransferBeamStirrupInfoSet;
	m_nTransferBeamShearBarType = pBeamRebarTemplate->m_nTransferBeamShearBarType;
	m_nTransferBeamHorShearBarType = pBeamRebarTemplate->m_nTransferBeamHorShearBarType;
	m_nTransferBeamCapbarRatio = pBeamRebarTemplate->m_nTransferBeamCapbarRatio;
	m_nTransferBeamStirrupType = pBeamRebarTemplate->m_nTransferBeamStirrupType;
	m_nTransferBeamCoverBarType = pBeamRebarTemplate->m_nTransferBeamCoverBarType;

	m_bTransferBeamSubBeamInfoSet = pBeamRebarTemplate->m_bTransferBeamSubBeamInfoSet;
	m_nTransferBeamSubBeamStirrupType = pBeamRebarTemplate->m_nTransferBeamSubBeamStirrupType;
	m_nTransferBeamSubBeamCapbarRatio = pBeamRebarTemplate->m_nTransferBeamSubBeamCapbarRatio;
	m_nTransferBeamSubBeamStirrupShape = pBeamRebarTemplate->m_nTransferBeamSubBeamStirrupShape;
	m_bTransferBeamSubBeamOnlyCenterStirrupPlace = pBeamRebarTemplate->m_bTransferBeamSubBeamOnlyCenterStirrupPlace;

	m_bTransferBeamSubBarInfoSet = pBeamRebarTemplate->m_bTransferBeamSubBarInfoSet;
	m_nTransferBeamTopSubStirrupBarShape = pBeamRebarTemplate->m_nTransferBeamTopSubStirrupBarShape;
	m_nTransferBeamBotSubStirrupBarShape = pBeamRebarTemplate->m_nTransferBeamBotSubStirrupBarShape;
	m_nTransferBeamSubStirrupStartSpacing = pBeamRebarTemplate->m_nTransferBeamSubStirrupStartSpacing;
	m_nTransferBeamSubTopStirrupType = pBeamRebarTemplate->m_nTransferBeamSubTopStirrupType;
	m_nTransferBeamSubTopStirrupNum = pBeamRebarTemplate->m_nTransferBeamSubTopStirrupNum;
	m_nTransferBeamSubTopStirrupSpacing = pBeamRebarTemplate->m_nTransferBeamSubTopStirrupSpacing;
	m_nTransferBeamSubBotStirrupType = pBeamRebarTemplate->m_nTransferBeamSubBotStirrupType;
	m_nTransferBeamSubBotStirrupNum = pBeamRebarTemplate->m_nTransferBeamSubBotStirrupNum;
	m_nTransferBeamSubBotStirrupSpacing = pBeamRebarTemplate->m_nTransferBeamSubBotStirrupSpacing;
	m_nTransferBeamSubStirrupTopLen = pBeamRebarTemplate->m_nTransferBeamSubStirrupTopLen;
	m_nTransferBeamSubStirrupBotLen = pBeamRebarTemplate->m_nTransferBeamSubStirrupBotLen;
	m_bTransferBeamSubStirrupUserHookLen = pBeamRebarTemplate->m_bTransferBeamSubStirrupUserHookLen;

	SetTransferBeamTopSubStirrupHookLenMap(pBeamRebarTemplate->mm_TransferBeamTopSubStirrupHookLen);
	SetTransferBeamBotSubStirrupHookLenMap(pBeamRebarTemplate->mm_TransferBeamBotSubStirrupHookLen);
	SetTransferBeamShearBarHookLenAMap(pBeamRebarTemplate->mm_TransferBeamShearBarHookLenA);
	SetTransferBeamShearBarHookLenBMap(pBeamRebarTemplate->mm_TransferBeamShearBarHookLenB);
	SetTransferBeamTopSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_TransferBeamTopSubStirrupDiaIndex);
	SetTransferBeamBotSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_TransferBeamBotSubStirrupDiaIndex);

	m_bTransferBeamDeepDualUBARPlaceSet = pBeamRebarTemplate->m_bTransferBeamDeepDualUBARPlaceSet;
	m_nTransferBeamDeepDepth = pBeamRebarTemplate->m_nTransferBeamDeepDepth;
	m_nTransferBeamDeepSpliceType = pBeamRebarTemplate->m_nTransferBeamDeepSpliceType;

	//m_bBuiltUpBeamInfoSet = pBeamRebarTemplate->m_bBuiltUpBeamInfoSet;
	//m_nBuiltUpBeamPlacerType = pBeamRebarTemplate->m_nBuiltUpBeamPlacerType;

	m_nTransferBeamSubStirrupBarType = pBeamRebarTemplate->m_nTransferBeamSubStirrupBarType;

	m_nTransferBeamStartSpacingType = pBeamRebarTemplate->m_nTransferBeamStartSpacingType;
	m_nTransferBeamSubBeamStartSpacingType = pBeamRebarTemplate->m_nTransferBeamSubBeamStartSpacingType;

	m_bTransferBeamSupportRebarsSet = pBeamRebarTemplate->m_bTransferBeamSupportRebarsSet;
	m_nTransferBeamSupportRebarsDiaIndex = pBeamRebarTemplate->m_nTransferBeamSupportRebarsDiaIndex;
	m_dTransferBeamSupportRebarsTotalLengthRate = pBeamRebarTemplate->m_dTransferBeamSupportRebarsTotalLengthRate;
	m_nTransferBeamSupportRebarsOrderLength = pBeamRebarTemplate->m_nTransferBeamSupportRebarsOrderLength;
	m_nTransferBeamSupportRebarsSpliceLength = pBeamRebarTemplate->m_nTransferBeamSupportRebarsSpliceLength;
	m_nTransferBeamSupportRebarsCorrectiveLength = pBeamRebarTemplate->m_nTransferBeamSupportRebarsCorrectiveLength;

	m_nTransferBeamCapBarPlaceType = pBeamRebarTemplate->m_nTransferBeamCapBarPlaceType;
	m_nTransferBeamCapBarNum = pBeamRebarTemplate->m_nTransferBeamCapBarNum;

	m_nTransferBeamSubBeamCapBarPlaceType = pBeamRebarTemplate->m_nTransferBeamSubBeamCapBarPlaceType;
	m_nTransferBeamSubBeamCapBarNum = pBeamRebarTemplate->m_nTransferBeamSubBeamCapBarNum;

	m_bTransferBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bTransferBeamBottomBarEnd150Dev;
	m_bTransferBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bTransferBeamBottomBarLink150Dev;

	m_bTransferBeamSubBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bTransferBeamSubBeamBottomBarEnd150Dev;
	m_bTransferBeamSubBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bTransferBeamSubBeamBottomBarLink150Dev;
	m_bTransferBeamSupportRebarUserSteelGradeS = pBeamRebarTemplate->m_bTransferBeamSupportRebarUserSteelGradeS;

	m_bTransferBeamSupportRebarsPlaceConditionSet = pBeamRebarTemplate->GetTransferBeamSupportRebarsPlaceConditionSet();
	m_nTransferBeamSupportRebarsPlaceCondition = pBeamRebarTemplate->GetTransferBeamSupportRebarsPlaceCondition();

	m_nTransferBeamPlaceTypeDiaIndex = pBeamRebarTemplate->m_nTransferBeamPlaceTypeDiaIndex;

	m_nTransferBeamSpliceTypeBelowDia = pBeamRebarTemplate->m_nTransferBeamSpliceTypeBelowDia;
	m_nTransferBeamSpliceWayBelowDia = pBeamRebarTemplate->m_nTransferBeamSpliceWayBelowDia;
	m_nTransferBeamCZoneSpliceWayBelowDia = pBeamRebarTemplate->m_nTransferBeamCZoneSpliceWayBelowDia;
	m_nTransferBeamCZoneSpliceLengthBelowDia = pBeamRebarTemplate->m_nTransferBeamCZoneSpliceLengthBelowDia;

	m_nTransferBeamSpliceTypeExcessDia = pBeamRebarTemplate->m_nTransferBeamSpliceTypeExcessDia;
	m_nTransferBeamSpliceWayExcessDia = pBeamRebarTemplate->m_nTransferBeamSpliceWayExcessDia;
	m_nTransferBeamCZoneSpliceWayExcessDia = pBeamRebarTemplate->m_nTransferBeamCZoneSpliceWayExcessDia;
	m_nTransferBeamCZoneSpliceLengthExcessDia = pBeamRebarTemplate->m_nTransferBeamCZoneSpliceLengthExcessDia;

	 m_bTransferBeamSpliceAddLengthSet = pBeamRebarTemplate->GetTransferBeamSpliceAddLengthSet();
	 m_nTransferBeamSpliceAddLengthNumSplice = pBeamRebarTemplate->GetTransferBeamSpliceAddLengthNumSplice();


	 m_bTransferBeamPlaceHorShearBar = pBeamRebarTemplate->GetTransferBeamPlaceHorShearBar();
	 m_nTransferBeamHorShearBarMainDirDiaIndex = pBeamRebarTemplate->GetTransferBeamHorShearBarMainDirDiaIndex();
	 m_nTransferBeamHorShearBarMainDirPlaceType = pBeamRebarTemplate->GetTransferBeamHorShearBarMainDirPlaceType();
	 m_nTransferBeamHorShearBarMainDirSpacing = pBeamRebarTemplate->GetTransferBeamHorShearBarMainDirSpacing();
	 m_nTransferBeamHorShearBarMainDirNumBar = pBeamRebarTemplate->GetTransferBeamHorShearBarMainDirNumBar();
	 m_nTransferBeamHorShearBarStirrupDirDiaIndex = pBeamRebarTemplate->GetTransferBeamHorShearBarStirrupDirDiaIndex();
	 m_nTransferBeamHorShearBarStirrupDirSpacing = pBeamRebarTemplate->GetTransferBeamHorShearBarStirrupDirSpacing();
	 m_bTransferBeamHorShearBarMainDirUserSteelGradeS = pBeamRebarTemplate->GetTransferBeamHorShearBarMainDirUserSteelGradeS();
	 m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS = pBeamRebarTemplate->GetTransferBeamHorShearBarStirrupDirUserSteelGradeS();
}

void TransferBeamRebarTemplate::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar << m_bTransferBeamGeneralInfoSet;
		ar << m_nTransferBeamTopMaxLen2HookAnchorBar;
		ar << m_nTransferBeamBotMaxLen2HookAnchorBar;
		ar << m_nTransferBeamSpliceTypeBelowDia;

		ar << m_bTransferBeamStirrupInfoSet;
		ar << m_nTransferBeamShearBarType;
		ar << m_nTransferBeamCapbarRatio;
		ar << m_nTransferBeamStirrupType;
		ar << m_nTransferBeamCoverBarType;

		ar << m_bTransferBeamSubBeamInfoSet;
		ar << m_nTransferBeamSubBeamStirrupType;
		ar << m_nTransferBeamSubBeamCapbarRatio;
		ar << m_nTransferBeamSubBeamStirrupShape;
		ar << m_bTransferBeamSubBeamOnlyCenterStirrupPlace;

		ar << m_bTransferBeamSubBarInfoSet;
		ar << m_nTransferBeamTopSubStirrupBarShape;
		ar << m_nTransferBeamBotSubStirrupBarShape;
		ar << m_nTransferBeamSubTopStirrupNum;
		ar << m_nTransferBeamSubBotStirrupNum;
		ar << m_nTransferBeamSubStirrupTopLen;
		ar << m_nTransferBeamSubStirrupBotLen;
		ar << m_bTransferBeamSubStirrupUserHookLen;

		map<long, long>::iterator it;
		ar << (int)mm_TransferBeamTopSubStirrupHookLen.size();
		for (it = mm_TransferBeamTopSubStirrupHookLen.begin(); it != mm_TransferBeamTopSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_TransferBeamShearBarHookLenA.size();
		for (it = mm_TransferBeamShearBarHookLenA.begin(); it != mm_TransferBeamShearBarHookLenA.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_TransferBeamShearBarHookLenB.size();
		for (it = mm_TransferBeamShearBarHookLenB.begin(); it != mm_TransferBeamShearBarHookLenB.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << m_nTransferBeamSpliceWayBelowDia;
		ar << m_bTransferBeamDeepDualUBARPlaceSet;
		ar << m_nTransferBeamDeepDepth;
		ar << m_nTransferBeamDeepSpliceType;
		
		ar << (int)mm_TransferBeamTopSubStirrupDiaIndex.size();
		for (it = mm_TransferBeamTopSubStirrupDiaIndex.begin(); it != mm_TransferBeamTopSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nTransferBeamSubStirrupBarType;

		ar << m_nTransferBeamStartSpacingType;
		ar << m_nTransferBeamSubBeamStartSpacingType;

		ar << m_bTransferBeamSupportRebarsSet;
		ar << m_nTransferBeamSupportRebarsDiaIndex;
		ar << m_dTransferBeamSupportRebarsTotalLengthRate;
		ar << m_nTransferBeamSupportRebarsOrderLength;
		ar << m_nTransferBeamSupportRebarsSpliceLength;
		ar << m_nTransferBeamSupportRebarsCorrectiveLength;

		ar << m_nTransferBeamSubStirrupStartSpacing;
		ar << m_nTransferBeamSubTopStirrupType;
		ar << m_nTransferBeamSubTopStirrupSpacing;
		ar << m_nTransferBeamSubBotStirrupType;
		ar << m_nTransferBeamSubBotStirrupSpacing;

		ar << m_nTransferBeamBarLayerGap;

		ar << m_bTransferBeamSpliceDevConditionSet;
		ar << m_nTransferBeamSpliceDevConditionLength;

		ar << m_nTransferBeamHorShearBarType;
		ar << m_nTransferBeamCapBarPlaceType;
		ar << m_nTransferBeamCapBarNum;

		ar << m_nTransferBeamSubBeamCapBarPlaceType;
		ar << m_nTransferBeamSubBeamCapBarNum;
		
		ar << m_nTransferBeamMainBarConnectionTolerance;
		ar << m_nTransferBeamLineContinuityHorEcc;
		ar << m_nTransferBeamLineContinuityVerEcc;

		ar << m_nTransferBeamCZoneSpliceWayBelowDia;
		ar << m_nTransferBeamCZoneSpliceLengthBelowDia;
		ar << m_bTransferBeamBottomBarEnd150Dev;
		ar << m_bTransferBeamBottomBarLink150Dev;

		ar << m_bTransferBeamSubBeamBottomBarEnd150Dev;
		ar << m_bTransferBeamSubBeamBottomBarLink150Dev;

		m_pTransferBeamShearFrictionBarTemplate->Serialize(ar);

		ar << m_bTransferBeamSupportRebarUserSteelGradeS;

		ar << (int)mm_TransferBeamBotSubStirrupHookLen.size();
		for (it = mm_TransferBeamBotSubStirrupHookLen.begin(); it != mm_TransferBeamBotSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_TransferBeamBotSubStirrupDiaIndex.size();
		for (it = mm_TransferBeamBotSubStirrupDiaIndex.begin(); it != mm_TransferBeamBotSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_bTransferBeamSupportRebarsPlaceConditionSet;
		ar << m_nTransferBeamSupportRebarsPlaceCondition;

		ar << m_nTransferBeamPlaceTypeDiaIndex;
		ar << m_nTransferBeamSpliceTypeExcessDia;
		ar << m_nTransferBeamSpliceWayExcessDia;
		ar << m_nTransferBeamCZoneSpliceWayExcessDia;
		ar << m_nTransferBeamCZoneSpliceLengthExcessDia;

		ar << m_bTransferBeamSpliceAddLengthSet;
		ar << m_nTransferBeamSpliceAddLengthNumSplice;

		ar << m_bTransferBeamPlaceHorShearBar;
		ar << m_nTransferBeamHorShearBarMainDirDiaIndex;
		ar << m_nTransferBeamHorShearBarMainDirPlaceType;
		ar << m_nTransferBeamHorShearBarMainDirSpacing;
		ar << m_nTransferBeamHorShearBarMainDirNumBar;
		ar << m_nTransferBeamHorShearBarStirrupDirDiaIndex;
		ar << m_nTransferBeamHorShearBarStirrupDirSpacing;
		ar << m_bTransferBeamHorShearBarMainDirUserSteelGradeS;
		ar << m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS;
	}
	else
	{
		ar >> m_bTransferBeamGeneralInfoSet;
		ar >> m_nTransferBeamTopMaxLen2HookAnchorBar;
		ar >> m_nTransferBeamBotMaxLen2HookAnchorBar;
		ar >> m_nTransferBeamSpliceTypeBelowDia;

		ar >> m_bTransferBeamStirrupInfoSet;
		ar >> m_nTransferBeamShearBarType;
		ar >> m_nTransferBeamCapbarRatio;
		ar >> m_nTransferBeamStirrupType;
		ar >> m_nTransferBeamCoverBarType;

		ar >> m_bTransferBeamSubBeamInfoSet;
		ar >> m_nTransferBeamSubBeamStirrupType;
		ar >> m_nTransferBeamSubBeamCapbarRatio;
		ar >> m_nTransferBeamSubBeamStirrupShape;
		ar >> m_bTransferBeamSubBeamOnlyCenterStirrupPlace;

		ar >> m_bTransferBeamSubBarInfoSet;
		ar >> m_nTransferBeamTopSubStirrupBarShape;
		ar >> m_nTransferBeamBotSubStirrupBarShape;
		ar >> m_nTransferBeamSubTopStirrupNum;
		ar >> m_nTransferBeamSubBotStirrupNum;
		ar >> m_nTransferBeamSubStirrupTopLen;
		ar >> m_nTransferBeamSubStirrupBotLen;
		ar >> m_bTransferBeamSubStirrupUserHookLen;

		int NumObject;
		
		ar >> NumObject;
		if(NumObject > 0)
			mm_TransferBeamTopSubStirrupHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_TransferBeamTopSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferBeamShearBarHookLenA.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_TransferBeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
		}
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferBeamShearBarHookLenB.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_TransferBeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> m_nTransferBeamSpliceWayBelowDia;
		ar >> m_bTransferBeamDeepDualUBARPlaceSet;
		ar >> m_nTransferBeamDeepDepth;
		ar >> m_nTransferBeamDeepSpliceType;
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferBeamTopSubStirrupDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_TransferBeamTopSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}

		ar >> m_nTransferBeamSubStirrupBarType;

		ar >> m_nTransferBeamStartSpacingType;
		ar >> m_nTransferBeamSubBeamStartSpacingType;

		ar >> m_bTransferBeamSupportRebarsSet;
		ar >> m_nTransferBeamSupportRebarsDiaIndex;
		ar >> m_dTransferBeamSupportRebarsTotalLengthRate;
		ar >> m_nTransferBeamSupportRebarsOrderLength;
		ar >> m_nTransferBeamSupportRebarsSpliceLength;
		ar >> m_nTransferBeamSupportRebarsCorrectiveLength;
		
		ar >> m_nTransferBeamSubStirrupStartSpacing;
		ar >> m_nTransferBeamSubTopStirrupType;
		ar >> m_nTransferBeamSubTopStirrupSpacing;
		ar >> m_nTransferBeamSubBotStirrupType;
		ar >> m_nTransferBeamSubBotStirrupSpacing;
		
		ar >> m_nTransferBeamBarLayerGap;
		
		ar >> m_bTransferBeamSpliceDevConditionSet;
		ar >> m_nTransferBeamSpliceDevConditionLength;
		
		ar >> m_nTransferBeamHorShearBarType;
	
		ar >> m_nTransferBeamCapBarPlaceType;
		ar >> m_nTransferBeamCapBarNum;
	
		ar >> m_nTransferBeamSubBeamCapBarPlaceType;
		ar >> m_nTransferBeamSubBeamCapBarNum;
	
		ar >> m_nTransferBeamMainBarConnectionTolerance;
		ar >> m_nTransferBeamLineContinuityHorEcc;
		ar >> m_nTransferBeamLineContinuityVerEcc;
			
		ar >> m_nTransferBeamCZoneSpliceWayBelowDia;
		ar >> m_nTransferBeamCZoneSpliceLengthBelowDia;
	
		ar >> m_bTransferBeamBottomBarEnd150Dev;
		ar >> m_bTransferBeamBottomBarLink150Dev;
	
		ar >> m_bTransferBeamSubBeamBottomBarEnd150Dev;
		ar >> m_bTransferBeamSubBeamBottomBarLink150Dev;
	
		m_pTransferBeamShearFrictionBarTemplate->Serialize(ar);

		ar >> m_bTransferBeamSupportRebarUserSteelGradeS;
		
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferBeamBotSubStirrupHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_TransferBeamBotSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}
		ar >> NumObject;
		if (NumObject > 0)
			mm_TransferBeamBotSubStirrupDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_TransferBeamBotSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}
		
		ar >> m_bTransferBeamSupportRebarsPlaceConditionSet;
		ar >> m_nTransferBeamSupportRebarsPlaceCondition;
		
		if(MSVersionInfo::GetCurrentVersion() >= 20240711)
		{
			ar >> m_nTransferBeamPlaceTypeDiaIndex;
			ar >> m_nTransferBeamSpliceTypeExcessDia;
			ar >> m_nTransferBeamSpliceWayExcessDia;
			ar >> m_nTransferBeamCZoneSpliceWayExcessDia;
			ar >> m_nTransferBeamCZoneSpliceLengthExcessDia;
		}
		else
		{
			m_nTransferBeamSpliceTypeExcessDia = m_nTransferBeamSpliceTypeBelowDia;
			m_nTransferBeamSpliceWayExcessDia = m_nTransferBeamSpliceWayBelowDia;
			m_nTransferBeamCZoneSpliceWayExcessDia = m_nTransferBeamCZoneSpliceWayBelowDia;
			m_nTransferBeamCZoneSpliceLengthExcessDia = m_nTransferBeamCZoneSpliceLengthBelowDia;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bTransferBeamSpliceAddLengthSet;
			ar >> m_nTransferBeamSpliceAddLengthNumSplice;
		}
		else
		{
			m_bTransferBeamSpliceAddLengthSet = FALSE;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20250319)
		{
			ar >> m_bTransferBeamPlaceHorShearBar;
			ar >> m_nTransferBeamHorShearBarMainDirDiaIndex;
			ar >> m_nTransferBeamHorShearBarMainDirPlaceType;
			ar >> m_nTransferBeamHorShearBarMainDirSpacing;
			ar >> m_nTransferBeamHorShearBarMainDirNumBar;
			ar >> m_nTransferBeamHorShearBarStirrupDirDiaIndex;
			ar >> m_nTransferBeamHorShearBarStirrupDirSpacing;
			ar >> m_bTransferBeamHorShearBarMainDirUserSteelGradeS;
			ar >> m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS;
		}
	}
}

double TransferBeamRebarTemplate::GetTransferBeamTopSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_TransferBeamTopSubStirrupHookLen.find(DiaIndex);
	if (it == mm_TransferBeamTopSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double TransferBeamRebarTemplate::GetTransferBeamBotSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_TransferBeamBotSubStirrupHookLen.find(DiaIndex);
	if (it == mm_TransferBeamBotSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double TransferBeamRebarTemplate::GetTransferBeamShearBarHookLenA(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_TransferBeamShearBarHookLenA.find(DiaIndex);
	if (it == mm_TransferBeamShearBarHookLenA.end())
		return 0.;
	return it->second;
}

double TransferBeamRebarTemplate::GetTransferBeamShearBarHookLenB(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_TransferBeamShearBarHookLenB.find(DiaIndex);
	if (it == mm_TransferBeamShearBarHookLenB.end())
		return 0.;
	return it->second;
}

double TransferBeamRebarTemplate::GetTransferBeamTopSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_TransferBeamTopSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_TransferBeamTopSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double TransferBeamRebarTemplate::GetTransferBeamBotSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_TransferBeamBotSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_TransferBeamBotSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void TransferBeamRebarTemplate::SetTransferBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_TransferBeamTopSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_TransferBeamTopSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void TransferBeamRebarTemplate::SetTransferBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_TransferBeamBotSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_TransferBeamBotSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void TransferBeamRebarTemplate::SetTransferBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA)
{
	mm_TransferBeamShearBarHookLenA.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenA.begin(); it != mapShearBarHookLenA.end(); it++)
	{
		mm_TransferBeamShearBarHookLenA.insert(make_pair(it->first, it->second));
	}
}

void TransferBeamRebarTemplate::SetTransferBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB)
{
	mm_TransferBeamShearBarHookLenB.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenB.begin(); it != mapShearBarHookLenB.end(); it++)
	{
		mm_TransferBeamShearBarHookLenB.insert(make_pair(it->first, it->second));
	}
}

void TransferBeamRebarTemplate::SetTransferBeamTopSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_TransferBeamTopSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_TransferBeamTopSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void TransferBeamRebarTemplate::SetTransferBeamBotSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_TransferBeamBotSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_TransferBeamBotSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void TransferBeamRebarTemplate::DataInit()
{
	m_bTransferBeamGeneralInfoSet = false;
	m_nTransferBeamTopMaxLen2HookAnchorBar = 2000;
	m_nTransferBeamBotMaxLen2HookAnchorBar = 2000;
	m_nTransferBeamBarLayerGap = 25;
	m_nTransferBeamMainBarConnectionTolerance = 50;
	m_nTransferBeamLineContinuityHorEcc = 50;
	m_nTransferBeamLineContinuityVerEcc = 50;

	m_bTransferBeamSpliceDevConditionSet = false;
	m_nTransferBeamSpliceDevConditionLength = 340;

	m_bTransferBeamStirrupInfoSet = false;
	m_nTransferBeamShearBarType = 1;
	m_nTransferBeamHorShearBarType = 1;
	m_nTransferBeamCapbarRatio = 1;
	m_nTransferBeamStirrupType = 2;
	m_nTransferBeamCoverBarType = 1;
	m_bTransferBeamDeepDualUBARPlaceSet = false;
	m_nTransferBeamDeepDepth = 1500;
	m_nTransferBeamDeepSpliceType = 0;

	m_bTransferBeamSubBeamInfoSet = false;
	m_nTransferBeamSubBeamStirrupType = 1;
	m_nTransferBeamSubBeamCapbarRatio = 1;
	m_nTransferBeamSubBeamStirrupShape = 0;
	m_bTransferBeamSubBeamOnlyCenterStirrupPlace = false;

	m_bTransferBeamSubBarInfoSet = false;
	m_nTransferBeamTopSubStirrupBarShape = 3;
	m_nTransferBeamBotSubStirrupBarShape = 3;
	m_nTransferBeamSubStirrupStartSpacing = 500;
	m_nTransferBeamSubTopStirrupType = 0;
	m_nTransferBeamSubTopStirrupNum = 4;
	m_nTransferBeamSubTopStirrupSpacing = 1000;
	m_nTransferBeamSubBotStirrupType = 0;
	m_nTransferBeamSubBotStirrupNum = 3;
	m_nTransferBeamSubBotStirrupSpacing = 1000;
	m_nTransferBeamSubStirrupTopLen = 120;
	m_nTransferBeamSubStirrupBotLen = 100;
	m_bTransferBeamSubStirrupUserHookLen = FALSE;
	InitSubStirrupHookLenMap(mm_TransferBeamTopSubStirrupHookLen);
	InitSubStirrupHookLenMap(mm_TransferBeamBotSubStirrupHookLen);
	InitShearBarHookLenMap(mm_TransferBeamShearBarHookLenA, mm_TransferBeamShearBarHookLenB);

	//m_bBuiltUpBeamInfoSet = false;
	//m_nBuiltUpBeamPlacerType = 0;

	InitSubStirrupDiaIndexMap(mm_TransferBeamTopSubStirrupDiaIndex);
	InitSubStirrupDiaIndexMap(mm_TransferBeamBotSubStirrupDiaIndex);
	for(int index = 0; index < 3; index++)
	{
		mm_TransferBeamTopSubStirrupDiaIndex[index] = index;
		mm_TransferBeamBotSubStirrupDiaIndex[index] = index;
	}

	m_nTransferBeamSubStirrupBarType = 1;

	m_nTransferBeamStartSpacingType = 0;
	m_nTransferBeamSubBeamStartSpacingType = 0;

	m_bTransferBeamSupportRebarsSet = false;
	m_nTransferBeamSupportRebarsDiaIndex = 0;
	m_dTransferBeamSupportRebarsTotalLengthRate = 2.0;
	m_nTransferBeamSupportRebarsOrderLength = 8000;
	m_nTransferBeamSupportRebarsSpliceLength = 300;
	m_nTransferBeamSupportRebarsCorrectiveLength = 10;

	m_nTransferBeamCapBarPlaceType = 0;
	m_nTransferBeamCapBarNum = 4;

	m_nTransferBeamSubBeamCapBarPlaceType = 0;
	m_nTransferBeamSubBeamCapBarNum = 4;

	m_bTransferBeamBottomBarEnd150Dev = FALSE;
	m_bTransferBeamBottomBarLink150Dev = FALSE;

	m_bTransferBeamSubBeamBottomBarEnd150Dev = FALSE;
	m_bTransferBeamSubBeamBottomBarLink150Dev = FALSE;

	m_bTransferBeamSupportRebarUserSteelGradeS = FALSE;

	m_bTransferBeamSupportRebarsPlaceConditionSet = FALSE;
	m_nTransferBeamSupportRebarsPlaceCondition = 300;

	m_nTransferBeamPlaceTypeDiaIndex = 11;
	m_nTransferBeamSpliceTypeBelowDia = 0;
	m_nTransferBeamSpliceWayBelowDia = 0;
	m_nTransferBeamCZoneSpliceWayBelowDia = 0;
	m_nTransferBeamCZoneSpliceLengthBelowDia = 300;

	m_nTransferBeamSpliceTypeExcessDia = 0;
	m_nTransferBeamSpliceWayExcessDia = 0;
	m_nTransferBeamCZoneSpliceWayExcessDia = 0;
	m_nTransferBeamCZoneSpliceLengthExcessDia = 300;

	m_bTransferBeamSpliceAddLengthSet = TRUE;
	m_nTransferBeamSpliceAddLengthNumSplice = 3;

	m_bTransferBeamPlaceHorShearBar = FALSE;
	m_nTransferBeamHorShearBarMainDirDiaIndex = 0;
	m_nTransferBeamHorShearBarMainDirPlaceType = 0;
	m_nTransferBeamHorShearBarMainDirSpacing = 300;
	m_nTransferBeamHorShearBarMainDirNumBar = 2;
	m_nTransferBeamHorShearBarStirrupDirDiaIndex = 0;
	m_nTransferBeamHorShearBarStirrupDirSpacing = 300;
	m_bTransferBeamHorShearBarMainDirUserSteelGradeS = false;
	m_bTransferBeamHorShearBarStirrupDirUserSteelGradeS = false;
}

long TransferBeamRebarTemplate::GetTransferBeamSpliceType(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferBeamPlaceTypeDiaIndex)
		return GetTransferBeamSpliceTypeBelowDia();
	else
		return GetTransferBeamSpliceTypeExcessDia();
}
long TransferBeamRebarTemplate::GetTransferBeamSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferBeamPlaceTypeDiaIndex)
		return GetTransferBeamSpliceWayBelowDia();
	else
		return GetTransferBeamSpliceWayExcessDia();
}
long TransferBeamRebarTemplate::GetTransferBeamCZoneSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferBeamPlaceTypeDiaIndex)
		return GetTransferBeamCZoneSpliceWayBelowDia();
	else
		return GetTransferBeamCZoneSpliceWayExcessDia();
}
long TransferBeamRebarTemplate::GetTransferBeamCZoneSpliceLength(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nTransferBeamPlaceTypeDiaIndex)
		return GetTransferBeamCZoneSpliceLengthBelowDia();
	else
		return GetTransferBeamCZoneSpliceLengthExcessDia();
}