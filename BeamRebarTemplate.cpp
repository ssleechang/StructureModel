#include "StdAfx.h"
#include "BeamRebarTemplate.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"
#include "ShearFrictionBarTemplate.h"

BeamRebarTemplate::BeamRebarTemplate() :BaseRebarTemplate()
{
	DataInit();
	m_pBeamShearFrictionBarTemplate = new ShearFrictionBarTemplate();
}

BeamRebarTemplate::~BeamRebarTemplate(void)
{
}
ShearFrictionBarTemplate *BeamRebarTemplate::GetBeamShearFrictionBarTemplate()
{
	return m_pBeamShearFrictionBarTemplate;
}
void BeamRebarTemplate::CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate)
{
	BeamRebarTemplate* pBeamRebarTemplate = dynamic_cast<BeamRebarTemplate*>(pBaseRebarTemplate);
	if (pBeamRebarTemplate == nullptr) return;

	m_pBeamShearFrictionBarTemplate->CopyFromMe(pBaseRebarTemplate->GetBeamShearFrictionBarTemplate());

	m_bBeamGeneralInfoSet = pBeamRebarTemplate->m_bBeamGeneralInfoSet;
	m_nBeamTopMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nBeamTopMaxLen2HookAnchorBar;
	m_nBeamBotMaxLen2HookAnchorBar = pBeamRebarTemplate->m_nBeamBotMaxLen2HookAnchorBar;
	m_nBeamBarLayerGap = pBeamRebarTemplate->m_nBeamBarLayerGap;
	m_nBeamMainBarConnectionTolerance = pBeamRebarTemplate->GetBeamMainBarConnectionTolerance();
	m_nBeamLineContinuityHorEcc = pBeamRebarTemplate->GetBeamLineContinuityHorEcc();
	m_nBeamLineContinuityVerEcc = pBeamRebarTemplate->GetBeamLineContinuityVerEcc();
	

	m_bBeamSpliceDevConditionSet = pBeamRebarTemplate->m_bBeamSpliceDevConditionSet;
	m_nBeamSpliceDevConditionLength = pBeamRebarTemplate->m_nBeamSpliceDevConditionLength;

	m_bBeamStirrupInfoSet = pBeamRebarTemplate->m_bBeamStirrupInfoSet;
	m_nBeamShearBarType = pBeamRebarTemplate->m_nBeamShearBarType;
	m_nBeamHorShearBarType = pBeamRebarTemplate->m_nBeamHorShearBarType;
	m_nBeamCapbarRatio = pBeamRebarTemplate->m_nBeamCapbarRatio;
	m_nBeamStirrupType = pBeamRebarTemplate->m_nBeamStirrupType;
	m_nBeamCoverBarType = pBeamRebarTemplate->m_nBeamCoverBarType;

	m_bBeamSubBeamInfoSet = pBeamRebarTemplate->m_bBeamSubBeamInfoSet;
	m_nSubBeamStirrupType = pBeamRebarTemplate->m_nSubBeamStirrupType;
	m_nSubBeamCapbarRatio = pBeamRebarTemplate->m_nSubBeamCapbarRatio;
	m_nSubBeamStirrupShape = pBeamRebarTemplate->m_nSubBeamStirrupShape;
	m_bSubBeamOnlyCenterStirrupPlace = pBeamRebarTemplate->m_bSubBeamOnlyCenterStirrupPlace;

	m_bBeamSubBarInfoSet = pBeamRebarTemplate->m_bBeamSubBarInfoSet;
	m_nBeamTopSubStirrupBarShape = pBeamRebarTemplate->m_nBeamTopSubStirrupBarShape;
	m_nBeamBotSubStirrupBarShape = pBeamRebarTemplate->m_nBeamBotSubStirrupBarShape;
	m_nBeamSubStirrupStartSpacing = pBeamRebarTemplate->m_nBeamSubStirrupStartSpacing;
	m_nBeamSubTopStirrupType = pBeamRebarTemplate->m_nBeamSubTopStirrupType;
	m_nBeamSubTopStirrupNum = pBeamRebarTemplate->m_nBeamSubTopStirrupNum;
	m_nBeamSubTopStirrupSpacing = pBeamRebarTemplate->m_nBeamSubTopStirrupSpacing;
	m_nBeamSubBotStirrupType = pBeamRebarTemplate->m_nBeamSubBotStirrupType;
	m_nBeamSubBotStirrupNum = pBeamRebarTemplate->m_nBeamSubBotStirrupNum;
	m_nBeamSubBotStirrupSpacing = pBeamRebarTemplate->m_nBeamSubBotStirrupSpacing;
	m_nBeamSubStirrupTopLen = pBeamRebarTemplate->m_nBeamSubStirrupTopLen;
	m_nBeamSubStirrupBotLen = pBeamRebarTemplate->m_nBeamSubStirrupBotLen;
	m_bBeamSubStirrupUserHookLen = pBeamRebarTemplate->m_bBeamSubStirrupUserHookLen;

	SetBeamTopSubStirrupHookLenMap(pBeamRebarTemplate->mm_BeamTopSubStirrupHookLen);
	SetBeamBotSubStirrupHookLenMap(pBeamRebarTemplate->mm_BeamBotSubStirrupHookLen);
	SetBeamShearBarHookLenAMap(pBeamRebarTemplate->mm_BeamShearBarHookLenA);
	SetBeamShearBarHookLenBMap(pBeamRebarTemplate->mm_BeamShearBarHookLenB);
	SetBeamTopSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_BeamTopSubStirrupDiaIndex);
	SetBeamBotSubStirrupDiaIndexMap(pBeamRebarTemplate->mm_BeamBotSubStirrupDiaIndex);
	
	m_bBeamDeepDualUBARPlaceSet = pBeamRebarTemplate->m_bBeamDeepDualUBARPlaceSet;
	m_nBeamDeepDepth = pBeamRebarTemplate->m_nBeamDeepDepth;
	m_nBeamDeepSpliceType = pBeamRebarTemplate->m_nBeamDeepSpliceType;

	//m_bBuiltUpBeamInfoSet = pBeamRebarTemplate->m_bBuiltUpBeamInfoSet;
	//m_nBuiltUpBeamPlacerType = pBeamRebarTemplate->m_nBuiltUpBeamPlacerType;

	m_nBeamSubStirrupBarType = pBeamRebarTemplate->m_nBeamSubStirrupBarType;

	m_nBeamStartSpacingType = pBeamRebarTemplate->m_nBeamStartSpacingType;
	m_nSubBeamStartSpacingType = pBeamRebarTemplate->m_nSubBeamStartSpacingType;

	m_bBeamSupportRebarsSet = pBeamRebarTemplate->m_bBeamSupportRebarsSet;
	m_nBeamSupportRebarsDiaIndex = pBeamRebarTemplate->m_nBeamSupportRebarsDiaIndex;
	m_dBeamSupportRebarsTotalLengthRate = pBeamRebarTemplate->m_dBeamSupportRebarsTotalLengthRate;
	m_nBeamSupportRebarsOrderLength = pBeamRebarTemplate->m_nBeamSupportRebarsOrderLength;
	m_nBeamSupportRebarsSpliceLength = pBeamRebarTemplate->m_nBeamSupportRebarsSpliceLength;
	m_nBeamSupportRebarsCorrectiveLength = pBeamRebarTemplate->m_nBeamSupportRebarsCorrectiveLength;

	m_nBeamCapBarPlaceType = pBeamRebarTemplate->m_nBeamCapBarPlaceType;
	m_nBeamCapBarNum = pBeamRebarTemplate->m_nBeamCapBarNum;

	m_nSubBeamCapBarPlaceType = pBeamRebarTemplate->m_nSubBeamCapBarPlaceType;
	m_nSubBeamCapBarNum = pBeamRebarTemplate->m_nSubBeamCapBarNum;
	
	m_bBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bBeamBottomBarEnd150Dev;
	m_bBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bBeamBottomBarLink150Dev;

	m_bBeamSubBeamBottomBarEnd150Dev = pBeamRebarTemplate->m_bBeamSubBeamBottomBarEnd150Dev;
	m_bBeamSubBeamBottomBarLink150Dev = pBeamRebarTemplate->m_bBeamSubBeamBottomBarLink150Dev;
	m_bBeamSupportRebarUserSteelGradeS = pBeamRebarTemplate->m_bBeamSupportRebarUserSteelGradeS;

	m_bBeamSupportRebarsPlaceConditionSet = pBeamRebarTemplate->GetBeamSupportRebarsPlaceConditionSet();
	m_nBeamSupportRebarsPlaceCondition = pBeamRebarTemplate->GetBeamSupportRebarsPlaceCondition();

	m_nBeamPlaceTypeDiaIndex = pBeamRebarTemplate->m_nBeamPlaceTypeDiaIndex;

	m_nBeamSpliceTypeBelowDia = pBeamRebarTemplate->m_nBeamSpliceTypeBelowDia;
	m_nBeamSpliceWayBelowDia = pBeamRebarTemplate->m_nBeamSpliceWayBelowDia;
	m_nBeamCZoneSpliceWayBelowDia = pBeamRebarTemplate->m_nBeamCZoneSpliceWayBelowDia;
	m_nBeamCZoneSpliceLengthBelowDia = pBeamRebarTemplate->m_nBeamCZoneSpliceLengthBelowDia;

	m_nBeamSpliceTypeExcessDia = pBeamRebarTemplate->m_nBeamSpliceTypeExcessDia;
	m_nBeamSpliceWayExcessDia = pBeamRebarTemplate->m_nBeamSpliceWayExcessDia;
	m_nBeamCZoneSpliceWayExcessDia = pBeamRebarTemplate->m_nBeamCZoneSpliceWayExcessDia;
	m_nBeamCZoneSpliceLengthExcessDia = pBeamRebarTemplate->m_nBeamCZoneSpliceLengthExcessDia;

	m_bBeamSpliceAddLengthSet = pBeamRebarTemplate->GetBeamSpliceAddLengthSet();
	m_nBeamSpliceAddLengthNumSplice = pBeamRebarTemplate->GetBeamSpliceAddLengthNumSplice();

	m_bBeamPlaceHorShearBar = pBeamRebarTemplate->GetBeamPlaceHorShearBar();
	m_nBeamHorShearBarMainDirDiaIndex = pBeamRebarTemplate->GetBeamHorShearBarMainDirDiaIndex();
	m_nBeamHorShearBarMainDirPlaceType = pBeamRebarTemplate->GetBeamHorShearBarMainDirPlaceType();
	m_nBeamHorShearBarMainDirSpacing = pBeamRebarTemplate->GetBeamHorShearBarMainDirSpacing();
	m_nBeamHorShearBarMainDirNumBar = pBeamRebarTemplate->GetBeamHorShearBarMainDirNumBar();
	m_nBeamHorShearBarStirrupDirDiaIndex = pBeamRebarTemplate->GetBeamHorShearBarStirrupDirDiaIndex();
	m_nBeamHorShearBarStirrupDirSpacing = pBeamRebarTemplate->GetBeamHorShearBarStirrupDirSpacing();
	m_bBeamHorShearBarMainDirUserSteelGradeS = pBeamRebarTemplate->GetBeamHorShearBarMainDirUserSteelGradeS();
	m_bBeamHorShearBarStirrupDirUserSteelGradeS = pBeamRebarTemplate->GetBeamHorShearBarStirrupDirUserSteelGradeS();
}

void BeamRebarTemplate::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bBeamGeneralInfoSet;
		ar << m_nBeamTopMaxLen2HookAnchorBar;
		ar << m_nBeamBotMaxLen2HookAnchorBar;
		ar << m_nBeamSpliceTypeBelowDia;

		ar << m_bBeamStirrupInfoSet;
		ar << m_nBeamShearBarType;
		ar << m_nBeamCapbarRatio;
		ar << m_nBeamStirrupType;
		ar << m_nBeamCoverBarType;

		ar << m_bBeamSubBeamInfoSet;
		ar << m_nSubBeamStirrupType;
		ar << m_nSubBeamCapbarRatio;
		ar << m_nSubBeamStirrupShape;
		ar << m_bSubBeamOnlyCenterStirrupPlace;

		ar << m_bBeamSubBarInfoSet;
		ar << m_nBeamTopSubStirrupBarShape;
		ar << m_nBeamBotSubStirrupBarShape;
		ar << m_nBeamSubTopStirrupNum;
		ar << m_nBeamSubBotStirrupNum;
		ar << m_nBeamSubStirrupTopLen;
		ar << m_nBeamSubStirrupBotLen;
		ar << m_bBeamSubStirrupUserHookLen;

		map<long, long>::iterator it;
		ar << (int)mm_BeamTopSubStirrupHookLen.size();
		for (it = mm_BeamTopSubStirrupHookLen.begin(); it != mm_BeamTopSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_BeamShearBarHookLenA.size();
		for (it = mm_BeamShearBarHookLenA.begin(); it != mm_BeamShearBarHookLenA.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_BeamShearBarHookLenB.size();
		for (it = mm_BeamShearBarHookLenB.begin(); it != mm_BeamShearBarHookLenB.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}

		ar << m_nBeamSpliceWayBelowDia;
		ar << m_bBeamDeepDualUBARPlaceSet;
		ar << m_nBeamDeepDepth;
		ar << m_nBeamDeepSpliceType;

		BOOL bBuiltUpBeamInfoSet = FALSE;
		long nBuiltUpBeamPlacerType = 0;
		ar << bBuiltUpBeamInfoSet;
		ar << nBuiltUpBeamPlacerType;

		ar << (int)mm_BeamTopSubStirrupDiaIndex.size();
		for (it = mm_BeamTopSubStirrupDiaIndex.begin(); it != mm_BeamTopSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_nBeamSubStirrupBarType;

		ar << m_nBeamStartSpacingType;
		ar << m_nSubBeamStartSpacingType;

		ar << m_bBeamSupportRebarsSet;
		ar << m_nBeamSupportRebarsDiaIndex;
		ar << m_dBeamSupportRebarsTotalLengthRate;
		ar << m_nBeamSupportRebarsOrderLength;
		ar << m_nBeamSupportRebarsSpliceLength;
		ar << m_nBeamSupportRebarsCorrectiveLength;

		ar << m_nBeamSubStirrupStartSpacing;
		ar << m_nBeamSubTopStirrupType;
		ar << m_nBeamSubTopStirrupSpacing;
		ar << m_nBeamSubBotStirrupType;
		ar << m_nBeamSubBotStirrupSpacing;

		ar << m_nBeamBarLayerGap;

		ar << m_bBeamSpliceDevConditionSet;
		ar << m_nBeamSpliceDevConditionLength;

		ar << m_nBeamHorShearBarType;
		ar << m_nBeamCapBarPlaceType;
		ar << m_nBeamCapBarNum;

		ar << m_nSubBeamCapBarPlaceType;
		ar << m_nSubBeamCapBarNum;

		ar << m_nBeamMainBarConnectionTolerance;
		ar << m_nBeamLineContinuityHorEcc;
		ar << m_nBeamLineContinuityVerEcc;

		ar << m_nBeamCZoneSpliceWayBelowDia;
		ar << m_nBeamCZoneSpliceLengthBelowDia;
		ar << m_bBeamBottomBarEnd150Dev;
		ar << m_bBeamBottomBarLink150Dev;

		ar << m_bBeamSubBeamBottomBarEnd150Dev;
		ar << m_bBeamSubBeamBottomBarLink150Dev;

		m_pBeamShearFrictionBarTemplate->Serialize(ar);

		ar << m_bBeamSupportRebarUserSteelGradeS;

		ar << (int)mm_BeamBotSubStirrupHookLen.size();
		for (it = mm_BeamBotSubStirrupHookLen.begin(); it != mm_BeamBotSubStirrupHookLen.end(); it++)
		{
			long nDiaIndex = it->first;
			long hookLen = it->second;
			ar << nDiaIndex;
			ar << hookLen;
		}
		ar << (int)mm_BeamBotSubStirrupDiaIndex.size();
		for (it = mm_BeamBotSubStirrupDiaIndex.begin(); it != mm_BeamBotSubStirrupDiaIndex.end(); it++)
		{
			long index1 = it->first;
			long index2 = it->second;
			ar << index1;
			ar << index2;
		}

		ar << m_bBeamSupportRebarsPlaceConditionSet;
		ar << m_nBeamSupportRebarsPlaceCondition;

		ar << m_nBeamPlaceTypeDiaIndex;
		ar << m_nBeamSpliceTypeExcessDia;
		ar << m_nBeamSpliceWayExcessDia;
		ar << m_nBeamCZoneSpliceWayExcessDia;
		ar << m_nBeamCZoneSpliceLengthExcessDia;

		ar << m_bBeamSpliceAddLengthSet;
		ar << m_nBeamSpliceAddLengthNumSplice;

		ar << m_bBeamPlaceHorShearBar;
		ar << m_nBeamHorShearBarMainDirDiaIndex;
		ar << m_nBeamHorShearBarMainDirPlaceType;
		ar << m_nBeamHorShearBarMainDirSpacing;
		ar << m_nBeamHorShearBarMainDirNumBar;
		ar << m_nBeamHorShearBarStirrupDirDiaIndex;
		ar << m_nBeamHorShearBarStirrupDirSpacing;
		ar << m_bBeamHorShearBarMainDirUserSteelGradeS;
		ar << m_bBeamHorShearBarStirrupDirUserSteelGradeS;
	}
	else
	{
		ar >> m_bBeamGeneralInfoSet;
		ar >> m_nBeamTopMaxLen2HookAnchorBar;
		ar >> m_nBeamBotMaxLen2HookAnchorBar;
		ar >> m_nBeamSpliceTypeBelowDia;

		ar >> m_bBeamStirrupInfoSet;
		ar >> m_nBeamShearBarType;
		ar >> m_nBeamCapbarRatio;
		ar >> m_nBeamStirrupType;
		ar >> m_nBeamCoverBarType;

		ar >> m_bBeamSubBeamInfoSet;
		ar >> m_nSubBeamStirrupType;
		ar >> m_nSubBeamCapbarRatio;
		ar >> m_nSubBeamStirrupShape;
		ar >> m_bSubBeamOnlyCenterStirrupPlace;

		ar >> m_bBeamSubBarInfoSet;
		ar >> m_nBeamTopSubStirrupBarShape;
		ar >> m_nBeamBotSubStirrupBarShape;
		ar >> m_nBeamSubTopStirrupNum;
		ar >> m_nBeamSubBotStirrupNum;
		ar >> m_nBeamSubStirrupTopLen;
		ar >> m_nBeamSubStirrupBotLen;
		ar >> m_bBeamSubStirrupUserHookLen;

		int NumObject;

		ar >> NumObject;
		if (NumObject > 0)
			mm_BeamTopSubStirrupHookLen.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamTopSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_BeamShearBarHookLenA.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamShearBarHookLenA.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> NumObject;
		if (NumObject > 0)
			mm_BeamShearBarHookLenB.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex = 0, hookLen = 0;

			ar >> nDiaIndex;
			ar >> hookLen;
			mm_BeamShearBarHookLenB.insert(make_pair(nDiaIndex, hookLen));
		}

		ar >> m_nBeamSpliceWayBelowDia;
		ar >> m_bBeamDeepDualUBARPlaceSet;
		ar >> m_nBeamDeepDepth;
		ar >> m_nBeamDeepSpliceType;

		BOOL bBuiltUpBeamInfoSet = FALSE;//삭제됨
		long nBuiltUpBeamPlacerType = 0;
		ar >> bBuiltUpBeamInfoSet;
		ar >> nBuiltUpBeamPlacerType;


		ar >> NumObject;
		if (NumObject > 0)
			mm_BeamTopSubStirrupDiaIndex.clear();
		for (int index = 0; index < NumObject; index++)
		{
			long nDiaIndex1 = 0, nDiaIndex2 = 0;

			ar >> nDiaIndex1;
			ar >> nDiaIndex2;
			mm_BeamTopSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
		}

		ar >> m_nBeamSubStirrupBarType;

		ar >> m_nBeamStartSpacingType;
		ar >> m_nSubBeamStartSpacingType;

		if (MSVersionInfo::GetCurrentVersion() >= 20170822)
		{
			ar >> m_bBeamSupportRebarsSet;
			ar >> m_nBeamSupportRebarsDiaIndex;
			ar >> m_dBeamSupportRebarsTotalLengthRate;
			ar >> m_nBeamSupportRebarsOrderLength;
			ar >> m_nBeamSupportRebarsSpliceLength;
			ar >> m_nBeamSupportRebarsCorrectiveLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180226)
		{
			ar >> m_nBeamSubStirrupStartSpacing;
			ar >> m_nBeamSubTopStirrupType;
			ar >> m_nBeamSubTopStirrupSpacing;
			ar >> m_nBeamSubBotStirrupType;
			ar >> m_nBeamSubBotStirrupSpacing;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20180612)
		{
			ar >> m_nBeamBarLayerGap;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20190423)
		{
			ar >> m_bBeamSpliceDevConditionSet;
			ar >> m_nBeamSpliceDevConditionLength;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20200111)
		{
			ar >> m_nBeamHorShearBarType;
		}

		if (MSVersionInfo::GetCurrentVersion() >= 20191001)
		{
			ar >> m_nBeamCapBarPlaceType;
			ar >> m_nBeamCapBarNum;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20191002)
		{
			ar >> m_nSubBeamCapBarPlaceType;
			ar >> m_nSubBeamCapBarNum;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200219)
		{
			ar >> m_nBeamMainBarConnectionTolerance;
			ar >> m_nBeamLineContinuityHorEcc;
			ar >> m_nBeamLineContinuityVerEcc;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200529)
		{
			ar >> m_nBeamCZoneSpliceWayBelowDia;
			ar >> m_nBeamCZoneSpliceLengthBelowDia;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20200601)
		{
			ar >> m_bBeamBottomBarEnd150Dev;
			ar >> m_bBeamBottomBarLink150Dev;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20230503)
		{
			ar >> m_bBeamSubBeamBottomBarEnd150Dev;
			ar >> m_bBeamSubBeamBottomBarLink150Dev;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20230608)
		{
			m_pBeamShearFrictionBarTemplate->Serialize(ar);
		}
		if (MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_bBeamSupportRebarUserSteelGradeS;
		}
		if ((MSObject::IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231223) || MSVersionInfo::IsOldTotalVersion())
		{
			ar >> NumObject;
			if (NumObject > 0)
				mm_BeamBotSubStirrupHookLen.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex = 0, hookLen = 0;

				ar >> nDiaIndex;
				ar >> hookLen;
				mm_BeamBotSubStirrupHookLen.insert(make_pair(nDiaIndex, hookLen));
			}
			ar >> NumObject;
			if (NumObject > 0)
				mm_BeamBotSubStirrupDiaIndex.clear();
			for (int index = 0; index < NumObject; index++)
			{
				long nDiaIndex1 = 0, nDiaIndex2 = 0;

				ar >> nDiaIndex1;
				ar >> nDiaIndex2;
				mm_BeamBotSubStirrupDiaIndex.insert(make_pair(nDiaIndex1, nDiaIndex2));
			}
		}
		else
		{
			mm_BeamBotSubStirrupHookLen = mm_BeamTopSubStirrupHookLen;
			mm_BeamBotSubStirrupHookLen = mm_BeamTopSubStirrupHookLen;
			mm_BeamBotSubStirrupDiaIndex = mm_BeamTopSubStirrupDiaIndex;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240424)
		{
			ar >> m_bBeamSupportRebarsPlaceConditionSet;
			ar >> m_nBeamSupportRebarsPlaceCondition;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20240624)
		{
			ar >> m_nBeamPlaceTypeDiaIndex;
			ar >> m_nBeamSpliceTypeExcessDia;
			ar >> m_nBeamSpliceWayExcessDia;
			ar >> m_nBeamCZoneSpliceWayExcessDia;
			ar >> m_nBeamCZoneSpliceLengthExcessDia;
		}
		else
		{
			m_nBeamSpliceTypeExcessDia = m_nBeamSpliceTypeBelowDia;
			m_nBeamSpliceWayExcessDia = m_nBeamSpliceWayBelowDia;
			m_nBeamCZoneSpliceWayExcessDia = m_nBeamCZoneSpliceWayBelowDia;
			m_nBeamCZoneSpliceLengthExcessDia = m_nBeamCZoneSpliceLengthBelowDia;
		}
		if(MSVersionInfo::GetCurrentVersion() >= 20241018)
		{
			ar >> m_bBeamSpliceAddLengthSet;
			ar >> m_nBeamSpliceAddLengthNumSplice;
		}
		else
		{
			m_bBeamSpliceAddLengthSet = FALSE;
		}
		if (MSVersionInfo::GetCurrentVersion() >= 20250319)
		{
			ar >> m_bBeamPlaceHorShearBar;
			ar >> m_nBeamHorShearBarMainDirDiaIndex;
			ar >> m_nBeamHorShearBarMainDirPlaceType;
			ar >> m_nBeamHorShearBarMainDirSpacing;
			ar >> m_nBeamHorShearBarMainDirNumBar;
			ar >> m_nBeamHorShearBarStirrupDirDiaIndex;
			ar >> m_nBeamHorShearBarStirrupDirSpacing;
			ar >> m_bBeamHorShearBarMainDirUserSteelGradeS;
			ar >> m_bBeamHorShearBarStirrupDirUserSteelGradeS;
		}
	}
}

double BeamRebarTemplate::GetBeamTopSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamTopSubStirrupHookLen.find(DiaIndex);
	if (it == mm_BeamTopSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double BeamRebarTemplate::GetBeamBotSubStirrupHookLen(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamBotSubStirrupHookLen.find(DiaIndex);
	if (it == mm_BeamBotSubStirrupHookLen.end())
		return 0.;
	return it->second;
}
double BeamRebarTemplate::GetBeamShearBarHookLenA(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamShearBarHookLenA.find(DiaIndex);
	if (it == mm_BeamShearBarHookLenA.end())
		return 0.;
	return it->second;
}

double BeamRebarTemplate::GetBeamShearBarHookLenB(double dDia)
{
	CString sDiaName = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDiaName);
	map<long, long>::iterator it = mm_BeamShearBarHookLenB.find(DiaIndex);
	if (it == mm_BeamShearBarHookLenB.end())
		return 0.;
	return it->second;
}

double BeamRebarTemplate::GetBeamTopSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BeamTopSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_BeamTopSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}
double BeamRebarTemplate::GetBeamBotSubStirrupDia(double dDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDia);
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDia);

	map<long, long>::iterator it = mm_BeamBotSubStirrupDiaIndex.find(DiaIndex);
	if (it == mm_BeamBotSubStirrupDiaIndex.end())
		return dDia;

	return MSDeformedBar::FindDeformedBarDiameterByIndex(it->second);
}

void BeamRebarTemplate::SetBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_BeamTopSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_BeamTopSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void BeamRebarTemplate::SetBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen)
{
	mm_BeamBotSubStirrupHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapSubStirrupHookLen.begin(); it != mapSubStirrupHookLen.end(); it++)
	{
		mm_BeamBotSubStirrupHookLen.insert(make_pair(it->first, it->second));
	}
}
void BeamRebarTemplate::SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA)
{
	mm_BeamShearBarHookLenA.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenA.begin(); it != mapShearBarHookLenA.end(); it++)
	{
		mm_BeamShearBarHookLenA.insert(make_pair(it->first, it->second));
	}
}

void BeamRebarTemplate::SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB)
{
	mm_BeamShearBarHookLenB.clear();

	map<long, long>::iterator it;
	for (it = mapShearBarHookLenB.begin(); it != mapShearBarHookLenB.end(); it++)
	{
		mm_BeamShearBarHookLenB.insert(make_pair(it->first, it->second));
	}
}

void BeamRebarTemplate::SetBeamTopSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_BeamTopSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_BeamTopSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void BeamRebarTemplate::SetBeamBotSubStirrupDiaIndexMap(map<long, long> mapBeamSubStirrupDiaIndex)
{
	mm_BeamBotSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapBeamSubStirrupDiaIndex.begin(); it != mapBeamSubStirrupDiaIndex.end(); it++)
	{
		mm_BeamBotSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}
void BeamRebarTemplate::DataInit()
{
	m_bBeamGeneralInfoSet = false;
	m_nBeamTopMaxLen2HookAnchorBar = 2000;
	m_nBeamBotMaxLen2HookAnchorBar = 2000;
	m_nBeamBarLayerGap = 25;
	m_nBeamMainBarConnectionTolerance = 50;
	m_nBeamLineContinuityHorEcc = 50;
	m_nBeamLineContinuityVerEcc = 50;	

	m_bBeamSpliceDevConditionSet = false;
	m_nBeamSpliceDevConditionLength = 340;

	m_bBeamStirrupInfoSet = false;
	m_nBeamShearBarType = 1;
	m_nBeamHorShearBarType = 1;
	m_nBeamCapbarRatio = 1;
	m_nBeamStirrupType = 2;
	m_nBeamCoverBarType = 1;
	m_bBeamDeepDualUBARPlaceSet = false;
	m_nBeamDeepDepth = 1500;
	m_nBeamDeepSpliceType = 0;

	m_bBeamSubBeamInfoSet = false;
	m_nSubBeamStirrupType = 1;
	m_nSubBeamCapbarRatio = 1;
	m_nSubBeamStirrupShape = 0;
	m_bSubBeamOnlyCenterStirrupPlace = false;

	m_bBeamSubBarInfoSet = false;
	m_nBeamTopSubStirrupBarShape = 3;
	m_nBeamBotSubStirrupBarShape = 3;
	m_nBeamSubStirrupStartSpacing = 500;
	m_nBeamSubTopStirrupType = 0;
	m_nBeamSubTopStirrupNum = 4;
	m_nBeamSubTopStirrupSpacing = 1000;
	m_nBeamSubBotStirrupType = 0;
	m_nBeamSubBotStirrupNum = 3;
	m_nBeamSubBotStirrupSpacing = 1000;
	m_nBeamSubStirrupTopLen = 120;
	m_nBeamSubStirrupBotLen = 100;
	m_bBeamSubStirrupUserHookLen = FALSE;
	InitSubStirrupHookLenMap(mm_BeamTopSubStirrupHookLen);
	InitSubStirrupHookLenMap(mm_BeamBotSubStirrupHookLen);
	InitShearBarHookLenMap(mm_BeamShearBarHookLenA, mm_BeamShearBarHookLenB);

	//m_bBuiltUpBeamInfoSet = false;
	//m_nBuiltUpBeamPlacerType = 0;

	InitSubStirrupDiaIndexMap(mm_BeamTopSubStirrupDiaIndex);
	InitSubStirrupDiaIndexMap(mm_BeamBotSubStirrupDiaIndex);
	for (int index = 0; index < 3; index++)
	{
		mm_BeamTopSubStirrupDiaIndex[index] = index;
		mm_BeamBotSubStirrupDiaIndex[index] = index;
	}

	m_nBeamSubStirrupBarType = 1;

	m_nBeamStartSpacingType = 0;
	m_nSubBeamStartSpacingType = 0;

	m_bBeamSupportRebarsSet = false;
	m_nBeamSupportRebarsDiaIndex = 0;
	m_dBeamSupportRebarsTotalLengthRate = 2.0;
	m_nBeamSupportRebarsOrderLength = 8000;
	m_nBeamSupportRebarsSpliceLength = 300;
	m_nBeamSupportRebarsCorrectiveLength = 10;

	m_nBeamCapBarPlaceType = 0;
	m_nBeamCapBarNum = 4;

	m_nSubBeamCapBarPlaceType = 0;
	m_nSubBeamCapBarNum = 4;
	
	m_bBeamBottomBarEnd150Dev = FALSE;
	m_bBeamBottomBarLink150Dev = FALSE;

	m_bBeamSubBeamBottomBarEnd150Dev = FALSE;
	m_bBeamSubBeamBottomBarLink150Dev = FALSE;

	m_bBeamSupportRebarUserSteelGradeS = FALSE;

	m_bBeamSupportRebarsPlaceConditionSet = FALSE;
	m_nBeamSupportRebarsPlaceCondition = 300;
		
	m_nBeamPlaceTypeDiaIndex = 11;
	m_nBeamSpliceTypeBelowDia = 0;
	m_nBeamSpliceWayBelowDia = 0;
	m_nBeamCZoneSpliceWayBelowDia = 0;
	m_nBeamCZoneSpliceLengthBelowDia = 300;

	m_nBeamSpliceTypeExcessDia = 0;
	m_nBeamSpliceWayExcessDia = 0;
	m_nBeamCZoneSpliceWayExcessDia = 0;
	m_nBeamCZoneSpliceLengthExcessDia = 300;

	m_bBeamSpliceAddLengthSet = TRUE;
	m_nBeamSpliceAddLengthNumSplice = 3;

	m_bBeamPlaceHorShearBar = FALSE;
	m_nBeamHorShearBarMainDirDiaIndex = 0;
	m_nBeamHorShearBarMainDirPlaceType = 0;
	m_nBeamHorShearBarMainDirSpacing = 300;
	m_nBeamHorShearBarMainDirNumBar = 2;
	m_nBeamHorShearBarStirrupDirDiaIndex = 0;
	m_nBeamHorShearBarStirrupDirSpacing = 300;
	m_bBeamHorShearBarMainDirUserSteelGradeS = false;
	m_bBeamHorShearBarStirrupDirUserSteelGradeS = false;
}
long BeamRebarTemplate::GetBeamSpliceType(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nBeamPlaceTypeDiaIndex)
		return GetBeamSpliceTypeBelowDia();
	else
		return GetBeamSpliceTypeExcessDia();
}
long BeamRebarTemplate::GetBeamSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nBeamPlaceTypeDiaIndex)
		return GetBeamSpliceWayBelowDia();
	else
		return GetBeamSpliceWayExcessDia();
}
long BeamRebarTemplate::GetBeamCZoneSpliceWay(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nBeamPlaceTypeDiaIndex)
		return GetBeamCZoneSpliceWayBelowDia();
	else
		return GetBeamCZoneSpliceWayExcessDia();
}
long BeamRebarTemplate::GetBeamCZoneSpliceLength(double dDia)
{ 
	int DiaIndex = MSDeformedBar::FindBarDiameterIndexByDia(dDia);
	if(DiaIndex <= m_nBeamPlaceTypeDiaIndex)
		return GetBeamCZoneSpliceLengthBelowDia();
	else
		return GetBeamCZoneSpliceLengthExcessDia();
}