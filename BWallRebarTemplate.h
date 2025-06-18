#pragma once
#include "BaseRebarTemplate.h"

class BWallVerBarRebarTemplate;
class BWallDowelRebarTemplate;
class VerMemberLinkDowelTemplate;
class StructureModel_CLASS BWallRebarTemplate : public BaseRebarTemplate
{
public:
	BWallRebarTemplate();
	~BWallRebarTemplate(void);

	void DataInit();

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void Serialize(CArchive& ar);

	//Get Property

	virtual long GetBWallVerBarPlaceTypeDiaIndex() {return m_nBWallVerBarPlaceTypeDiaIndex; }
	virtual long GetBWallDowelPlaceTypeDiaIndex() { return m_nBWallDowelPlaceTypeDiaIndex; }

	virtual long GetBWallVerNumBarCrossBarIncludeSet() { return m_nBWallVerNumBarCrossBarIncludeSet; }
	virtual long GetBWallCrossBarEAType() { return m_nBWallCrossBarEAType; }
	virtual BOOL GetBWallCrossBarEATypeExcessThickSet() { return m_bBWallCrossBarEATypeExcessThickSet; }
	virtual long GetBWallCrossBarEATypeExcessThick() { return m_nBWallCrossBarEATypeExcessThick; }
	virtual long GetBWallCrossBarStartSpacingType() { return m_nBWallCrossBarStartSpacingType; }
	virtual BOOL GetBWallPlaceEndBarSet() { return m_bBWallPlaceEndBarSet; }
	virtual long GetBWallEndVerAddBarCount() { return m_nBWallEndVerAddBarCount; }
	virtual long GetBWallEndVerAddBarDist() { return m_nBWallEndVerAddBarDist; }
	virtual long GetBWallEndVerStartSpacingType() { return m_nBWallEndVerStartSpacingType; }

	virtual BOOL GetBWallPlaceBraceSupportBar() { return m_bBWallPlaceBraceSupportBar; }
	virtual long GetBWallBraceSupportBarOverDiaIndex() { return m_nBWallBraceSupportBarOverDiaIndex; }
	virtual BOOL GetBWallBraceSupportBarUserSteelGradeS() { return m_bBWallBraceSupportBarUserSteelGradeS; }
	virtual long GetBWallBraceSupportBarWallLengthCondition() { return m_nBWallBraceSupportBarWallLengthCondition; }
	virtual long GetBWallBraceSupportBarWallLengthConditionOverLength() { return m_nBWallBraceSupportBarWallLengthConditionOverLength; }
	virtual long GetBWallBraceSupportBarWallLengthConditionUnderLength() { return m_nBWallBraceSupportBarWallLengthConditionUnderLength; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionDiaSet() { return m_bBWallBraceSupportNotPlaceConditionDiaSet; }
	virtual long GetBWallBraceSupportNotPlaceConditionDiaIndex() { return m_nBWallBraceSupportNotPlaceConditionDiaIndex; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionLengthSet() { return m_bBWallBraceSupportNotPlaceConditionLengthSet; }
	virtual long GetBWallBraceSupportNotPlaceConditionLength() { return m_nBWallBraceSupportNotPlaceConditionLength; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionBottomDowel() { return m_bBWallBraceSupportNotPlaceConditionBottomDowel; }

	virtual BOOL GetBWallBraceSupportNotPlaceConditionHeightSet() { return m_bBWallBraceSupportNotPlaceConditionHeightSet; }
	virtual long GetBWallBraceSupportNotPlaceConditionHeight() { return m_nBWallBraceSupportNotPlaceConditionHeight; }

	virtual BOOL GetBWallPlaceUBar4ShortWall() { return m_bBWallPlaceUBar4ShortWall; }
	virtual long GetBWallMaxUbarLegLength() { return m_nBWallMaxUbarLegLength; }
	virtual BOOL GetBWallPlaceStirrup4ShortWall() { return m_bBWallPlaceStirrup4ShortWall; }
	virtual long GetBWallHBarMaxStirrupLen() { return m_nBWallHBarMaxStirrupLen; }
	virtual BOOL GetBWallHorBarStartSet() { return m_bBWallHorBarStartSet; }
	virtual long GetBWallHorBarStartLength() { return m_nBWallHorBarStartLength; }
	virtual BOOL GetBWallDefaultUbarLen() { return m_bBWallDefaultUbarLen; }
	virtual BOOL GetBWallCrossBarUBarHalfPlace() { return m_bBWallCrossBarUBarHalfPlace; }
	virtual BOOL GetBWallStraightEndUBarPlaceSet() { return m_bBWallStraightEndUBarPlaceSet; }

	virtual long GetBWallSpacerDiaType() { return m_nBWallSpacerDiaType; }
	virtual long GetBWallSpacerDiaIndex() { return m_nBWallSpacerDiaIndex; }
	virtual BOOL GetBWallSpacerUserSteelGradeS() { return m_bBWallSpacerUserSteelGradeS; }
	virtual long GetBWallSpacerHorSpacing() { return m_nBWallSpacerHorSpacing; }
	virtual long GetBWallSpacerVerSpacingType() { return m_nBWallSpacerVerSpacingType; }
	virtual long GetBWallSpacerVerSpacing() { return m_nBWallSpacerVerSpacing; }
	virtual long GetBWallSpacerVerSpacingRate() { return m_nBWallSpacerVerSpacingRate; }
	virtual long GetBWallSpacerVerSpacingCount() { return m_nBWallSpacerVerSpacingCount; }
	virtual BOOL GetBWallSpacerVerEndBarPlaceSet() { return m_bBWallSpacerVerEndBarPlaceSet; }
	virtual long GetBWallSpacerVerEndBarSpacingType() { return m_nBWallSpacerVerEndBarSpacingType; }
	virtual long GetBWallSpacerVerEndBarSpacing() { return m_nBWallSpacerVerEndBarSpacing; }
	virtual long GetBWallSpacerVerEndBarSpacingRate() { return m_nBWallSpacerVerEndBarSpacingRate; }
	virtual long GetBWallSpacerVerEndBarSpacingCount() { return m_nBWallSpacerVerEndBarSpacingCount; }
	virtual long GetBWallDowelSpacerVerSpacingCount() { return m_nBWallDowelSpacerVerSpacingCount; }
	virtual long GetBWallSpacerLenType() { return m_nBWallSpacerLenType; }
	virtual long GetBWallSpacerLenCalWayA() { return m_nBWallSpacerLenCalWayA; }
	virtual long GetBWallSpacerLenCalWayB() { return m_nBWallSpacerLenCalWayB; }
	virtual long GetBWallSpacerLenA() { return m_nBWallSpacerLenA; }
	virtual long GetBWallSpacerLenB() { return m_nBWallSpacerLenB; }
	virtual long GetBWallSpacerShape() { return m_nBWallSpacerShape; }
	virtual BOOL GetBWallSpacerRebarPlaceSet() { return m_bBWallSpacerRebarPlaceSet; }
	virtual long GetBWallSpacerRebarPlaceCondition() { return m_nBWallSpacerRebarPlaceCondition; }

	virtual BOOL GetBWallSupportRebarsSet() { return m_bBWallSupportRebarsSet; }
	virtual long GetBWallSupportRebarsDiaIndex() { return m_nBWallSupportRebarsDiaIndex; }
	virtual BOOL GetBWallSupportRebarsUserSteelGradeS() { return m_bBWallSupportRebarsUserSteelGradeS; }
	virtual double GetBWallSupportRebarsTotalLengthRate() { return m_dBWallSupportRebarsTotalLengthRate; }
	virtual long GetBWallSupportRebarsOrderLength() { return m_nBWallSupportRebarsOrderLength; }
	virtual long GetBWallSupportRebarsSpliceLength() { return m_nBWallSupportRebarsSpliceLength; }
	virtual long GetBWallSupportRebarsCorrectiveLength() { return m_nBWallSupportRebarsCorrectiveLength; }

	virtual BOOL GetBWallAddBarsSet() { return m_bBWallAddBarsSet; }
	virtual long GetBWallAddBarsDiaIndex() { return m_nBWallAddBarsDiaIndex; }
	virtual BOOL GetBWallAddBarsUserSteelGradeS() { return m_bBWallAddBarsUserSteelGradeS; }
	virtual long GetBWallAddBarsNumber() { return m_nBWallAddBarsNumber; }
	virtual long GetBWallAddBarsLengthType() { return m_nBWallAddBarsLengthType; }
	virtual long GetBWallAddBarsTensionLengthType() { return m_nBWallAddBarsTensionLengthType; }
	virtual long GetBWallAddBarsUserLength() { return m_nBWallAddBarsUserLength; }

	virtual BOOL GetBWallDowelSupportPlaceType() { return m_bBWallDowelSupportPlaceType; }

	virtual long GetBWallVerBarSpliceType(double dDia);
	virtual long GetBWallVerBarTopLenType(double dDia);
	virtual long GetBWallVerBarTopFloorDevType(double dDia);
	virtual long GetBWallVerBarConstructionHeight(double dDia);
	virtual BOOL GetBWallVerBarAlterSpliceSet(double dDia);
	virtual long GetBWallVerBarAlterSpliceDist(double dDia);

	virtual long GetBWallVerBarAlterSpliceType(double dDia);
	virtual long GetBWallDowelAlterSpliceType(double dDia);

	virtual long GetBWallDowelBarSpliceType(double dDia);
	virtual long GetBWallDowelBarTopLenType(double dDia);
	virtual long GetBWallDowelDowelShape(double dDia);
	virtual long GetBWallDowelConstructionHeight(double dDia);
	virtual BOOL GetBWallDowelAlterSpliceSet(double dDia);
	virtual long GetBWallDowelAlterSpliceDist(double dDia);
	virtual long GetBWallDowelSupportBarDiaIndex(double dDia);
	virtual BOOL GetBWallDowelSupportBarUserSteelGradeS(double dDia);
	virtual long GetBWallDowelSupportBarNum(double dDia);
	virtual long GetBWallDowelBarAnchorType(double dDia);
	virtual BOOL GetBWallDowelHookAnchorPlaceSpacingSet(double dDia);
	virtual long GetBWallDowelHookAnchorPlaceSpacing(double dDia);
	virtual BOOL GetBWallUserDowelHookLenSet(double dDia);
	

	virtual BOOL GetBWallShearBarSet() { return m_bBWallShearBarSet; }
	virtual long GetBWallShearBarDiaType() { return m_nBWallShearBarDiaType; }
	virtual long GetBWallShearBarDiaIndex() { return m_nBWallShearBarDiaIndex; }
	virtual BOOL GetBWallShearBarUserSteelGradeS() { return m_bBWallShearBarUserSteelGradeS; }
	virtual long GetBWallShearBarUserDiaType() { return m_nBWallShearBarUserDiaType; }

	virtual long GetBWallShearBarShapeType() { return m_nBWallShearBarShapeType; }

	virtual long GetBWallShearBarVerSpacingCondition() { return m_nBWallShearBarVerSpacingCondition; }
	virtual long GetBWallShearBarVerBelowType() { return m_nBWallShearBarVerBelowType; }
	virtual long GetBWallShearBarVerBelowSpacing() { return m_nBWallShearBarVerBelowSpacing; }
	virtual long GetBWallShearBarVerBelowRatio() { return m_nBWallShearBarVerBelowRatio; }
	virtual long GetBWallShearBarVerBelowNum() { return m_nBWallShearBarVerBelowNum; }
	virtual long GetBWallShearBarVerOverType() { return m_nBWallShearBarVerOverType; }
	virtual long GetBWallShearBarVerOverSpacing() { return m_nBWallShearBarVerOverSpacing; }
	virtual long GetBWallShearBarVerOverRatio() { return m_nBWallShearBarVerOverRatio; }
	virtual long GetBWallShearBarVerOverNum() { return m_nBWallShearBarVerOverNum; }

	virtual long GetBWallShearBarHorSpacingCondition() { return m_nBWallShearBarHorSpacingCondition; }
	virtual long GetBWallShearBarHorBelowType() { return m_nBWallShearBarHorBelowType; }
	virtual long GetBWallShearBarHorBelowSpacing() { return m_nBWallShearBarHorBelowSpacing; }
	virtual long GetBWallShearBarHorBelowRatio() { return m_nBWallShearBarHorBelowRatio; }
	virtual long GetBWallShearBarHorBelowNum() { return m_nBWallShearBarHorBelowNum; }
	virtual long GetBWallShearBarHorOverType() { return m_nBWallShearBarHorOverType; }
	virtual long GetBWallShearBarHorOverSpacing() { return m_nBWallShearBarHorOverSpacing; }
	virtual long GetBWallShearBarHorOverRatio() { return m_nBWallShearBarHorOverRatio; }
	virtual long GetBWallShearBarHorOverNum() { return m_nBWallShearBarHorOverNum; }

	virtual BOOL GetBWallUBarAddLengthSet() { return m_bBWallUBarAddLengthSet; }
	virtual long GetBWallUBarAddLength() { return m_nBWallUBarAddLength; }
	virtual BOOL GetBWallSpacerAddLengthSet() { return m_bBWallSpacerAddLengthSet; }
	virtual long GetBWallSpacerAddLength() { return m_nBWallSpacerAddLength; }

	//Set Property
	virtual void SetBWallVerBarPlaceTypeDiaIndex(long value) { m_nBWallVerBarPlaceTypeDiaIndex = value; }
	virtual void SetBWallDowelPlaceTypeDiaIndex(long value) { m_nBWallDowelPlaceTypeDiaIndex = value; }
		
	virtual void SetBWallVerNumBarCrossBarIncludeSet(long value) { m_nBWallVerNumBarCrossBarIncludeSet = value; }
	virtual void SetBWallCrossBarEAType(long value) { m_nBWallCrossBarEAType = value; }
	virtual void SetBWallCrossBarEATypeExcessThickSet(BOOL value) { m_bBWallCrossBarEATypeExcessThickSet = value; }
	virtual void SetBWallCrossBarEATypeExcessThick(long value) { m_nBWallCrossBarEATypeExcessThick = value; }
	virtual void SetBWallCrossBarStartSpacingType(long value) { m_nBWallCrossBarStartSpacingType = value; }
	virtual void SetBWallPlaceEndBarSet(BOOL value) { m_bBWallPlaceEndBarSet = value; }
	virtual void SetBWallEndVerAddBarCount(long value) { m_nBWallEndVerAddBarCount = value; }
	virtual void SetBWallEndVerAddBarDist(long value) { m_nBWallEndVerAddBarDist = value; }
	virtual void SetBWallEndVerStartSpacingType(long value) { m_nBWallEndVerStartSpacingType = value; }

	virtual void SetBWallPlaceBraceSupportBar(BOOL value) { m_bBWallPlaceBraceSupportBar = value; }
	virtual void SetBWallBraceSupportBarOverDiaIndex(long value) { m_nBWallBraceSupportBarOverDiaIndex = value; }
	virtual void SetBWallBraceSupportBarUserSteelGradeS(BOOL value) { m_bBWallBraceSupportBarUserSteelGradeS = value; }
	virtual void SetBWallBraceSupportBarWallLengthCondition(long value) { m_nBWallBraceSupportBarWallLengthCondition = value; }
	virtual void SetBWallBraceSupportBarWallLengthConditionOverLength(long value) { m_nBWallBraceSupportBarWallLengthConditionOverLength = value; }
	virtual void SetBWallBraceSupportBarWallLengthConditionUnderLength(long value) { m_nBWallBraceSupportBarWallLengthConditionUnderLength = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionDiaSet(BOOL value) { m_bBWallBraceSupportNotPlaceConditionDiaSet = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionDiaIndex(long value) { m_nBWallBraceSupportNotPlaceConditionDiaIndex = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionLengthSet(BOOL value) { m_bBWallBraceSupportNotPlaceConditionLengthSet = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionLength(long value) { m_nBWallBraceSupportNotPlaceConditionLength = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionBottomDowel(BOOL value) { m_bBWallBraceSupportNotPlaceConditionBottomDowel = value; }

	virtual void SetBWallBraceSupportNotPlaceConditionHeightSet(BOOL value) { m_bBWallBraceSupportNotPlaceConditionHeightSet = value; }
	virtual void SetBWallBraceSupportNotPlaceConditionHeight(long value) { m_nBWallBraceSupportNotPlaceConditionHeight = value; }

	virtual void SetBWallPlaceUBar4ShortWall(BOOL value) { m_bBWallPlaceUBar4ShortWall = value; }
	virtual void SetBWallMaxUbarLegLength(long value) { m_nBWallMaxUbarLegLength = value; }
	virtual void SetBWallPlaceStirrup4ShortWall(BOOL value) { m_bBWallPlaceStirrup4ShortWall = value; }
	virtual void SetBWallHBarMaxStirrupLen(long value) { m_nBWallHBarMaxStirrupLen = value; }
	virtual void SetBWallHorBarStartSet(BOOL value) { m_bBWallHorBarStartSet = value; }
	virtual void SetBWallHorBarStartLength(long value) { m_nBWallHorBarStartLength = value; }
	virtual void SetBWallDefaultUbarLen(BOOL value) { m_bBWallDefaultUbarLen = value; }
	virtual void SetBWallCrossBarUBarHalfPlace(BOOL value) { m_bBWallCrossBarUBarHalfPlace = value; }
	virtual void SetBWallStraightEndUBarPlaceSet(BOOL value) { m_bBWallStraightEndUBarPlaceSet = value; }

	virtual void SetBWallSpacerDiaType(long value) { m_nBWallSpacerDiaType = value; }
	virtual void SetBWallSpacerDiaIndex(long value) { m_nBWallSpacerDiaIndex = value; }
	virtual void SetBWallSpacerUserSteelGradeS(BOOL value) { m_bBWallSpacerUserSteelGradeS = value; }
	virtual void SetBWallSpacerHorSpacing(long value) { m_nBWallSpacerHorSpacing = value; }
	virtual void SetBWallSpacerVerSpacingType(long value) { m_nBWallSpacerVerSpacingType = value; }
	virtual void SetBWallSpacerVerSpacing(long value) { m_nBWallSpacerVerSpacing = value; }
	virtual void SetBWallSpacerVerSpacingRate(long value) { m_nBWallSpacerVerSpacingRate = value; }
	virtual void SetBWallSpacerVerSpacingCount(long value) { m_nBWallSpacerVerSpacingCount = value; }
	virtual void SetBWallSpacerVerEndBarPlaceSet(BOOL value) { m_bBWallSpacerVerEndBarPlaceSet = value; }
	virtual void SetBWallSpacerVerEndBarSpacingType(long value) { m_nBWallSpacerVerEndBarSpacingType = value; }
	virtual void SetBWallSpacerVerEndBarSpacing(long value) { m_nBWallSpacerVerEndBarSpacing = value; }
	virtual void SetBWallSpacerVerEndBarSpacingRate(long value) { m_nBWallSpacerVerEndBarSpacingRate = value; }
	virtual void SetBWallSpacerVerEndBarSpacingCount(long value) { m_nBWallSpacerVerEndBarSpacingCount = value; }
	virtual void SetBWallDowelSpacerVerSpacingCount(long value) { m_nBWallDowelSpacerVerSpacingCount = value; }
	virtual void SetBWallSpacerLenType(long value) { m_nBWallSpacerLenType = value; }
	virtual void SetBWallSpacerLenCalWayA(long value) { m_nBWallSpacerLenCalWayA = value; }
	virtual void SetBWallSpacerLenCalWayB(long value) { m_nBWallSpacerLenCalWayB = value; }
	virtual void SetBWallSpacerLenA(long value) { m_nBWallSpacerLenA = value; }
	virtual void SetBWallSpacerLenB(long value) { m_nBWallSpacerLenB = value; }
	virtual void SetBWallSpacerShape(long value) { m_nBWallSpacerShape = value; }
	virtual void SetBWallSpacerRebarPlaceSet(BOOL value) { m_bBWallSpacerRebarPlaceSet = value; }
	virtual void SetBWallSpacerRebarPlaceCondition(long value) { m_nBWallSpacerRebarPlaceCondition = value; }

	virtual void SetBWallSupportRebarsSet(BOOL value) { m_bBWallSupportRebarsSet = value; }
	virtual void SetBWallSupportRebarsDiaIndex(long value) { m_nBWallSupportRebarsDiaIndex = value; }
	virtual void SetBWallSupportRebarsUserSteelGradeS(BOOL value) { m_bBWallSupportRebarsUserSteelGradeS = value; }
	virtual void SetBWallSupportRebarsTotalLengthRate(double value) { m_dBWallSupportRebarsTotalLengthRate = value; }
	virtual void SetBWallSupportRebarsOrderLength(long value) { m_nBWallSupportRebarsOrderLength = value; }
	virtual void SetBWallSupportRebarsSpliceLength(long value) { m_nBWallSupportRebarsSpliceLength = value; }
	virtual void SetBWallSupportRebarsCorrectiveLength(long value) { m_nBWallSupportRebarsCorrectiveLength = value; }

	virtual void SetBWallAddBarsSet(BOOL value) { m_bBWallAddBarsSet = value; }
	virtual void SetBWallAddBarsDiaIndex(long value) { m_nBWallAddBarsDiaIndex = value; }
	virtual void SetBWallAddBarsUserSteelGradeS(BOOL value) { m_bBWallAddBarsUserSteelGradeS = value; }
	virtual void SetBWallAddBarsNumber(long value) { m_nBWallAddBarsNumber = value; }
	virtual void SetBWallAddBarsLengthType(long value) { m_nBWallAddBarsLengthType = value; }
	virtual void SetBWallAddBarsTensionLengthType(long value) { m_nBWallAddBarsTensionLengthType = value; }
	virtual void SetBWallAddBarsUserLength(long value) { m_nBWallAddBarsUserLength = value; }

	virtual void SetBWallDowelSupportPlaceType(BOOL value) { m_bBWallDowelSupportPlaceType = value; }
		
	virtual void SetBWallShearBarSet(long value) { m_bBWallShearBarSet = value; }
	virtual void SetBWallShearBarDiaType(long value) { m_nBWallShearBarDiaType = value; }
	virtual void SetBWallShearBarDiaIndex(long value) { m_nBWallShearBarDiaIndex = value; }
	virtual void SetBWallShearBarUserSteelGradeS(BOOL value) { m_bBWallShearBarUserSteelGradeS = value; }
	virtual void SetBWallShearBarUserDiaType(long value) { m_nBWallShearBarUserDiaType = value; }

	virtual void SetBWallShearBarShapeType(long value) { m_nBWallShearBarShapeType = value; }

	virtual void SetBWallShearBarVerSpacingCondition(long value) { m_nBWallShearBarVerSpacingCondition = value; }
	virtual void SetBWallShearBarVerBelowType(long value) { m_nBWallShearBarVerBelowType = value; }
	virtual void SetBWallShearBarVerBelowSpacing(long value) { m_nBWallShearBarVerBelowSpacing = value; }
	virtual void SetBWallShearBarVerBelowRatio(long value) { m_nBWallShearBarVerBelowRatio = value; }
	virtual void SetBWallShearBarVerBelowNum(long value) { m_nBWallShearBarVerBelowNum = value; }
	virtual void SetBWallShearBarVerOverType(long value) { m_nBWallShearBarVerOverType = value; }
	virtual void SetBWallShearBarVerOverSpacing(long value) { m_nBWallShearBarVerOverSpacing = value; }
	virtual void SetBWallShearBarVerOverRatio(long value) { m_nBWallShearBarVerOverRatio = value; }
	virtual void SetBWallShearBarVerOverNum(long value) { m_nBWallShearBarVerOverNum = value; }

	virtual void SetBWallShearBarHorSpacingCondition(long value) { m_nBWallShearBarHorSpacingCondition = value; }
	virtual void SetBWallShearBarHorBelowType(long value) { m_nBWallShearBarHorBelowType = value; }
	virtual void SetBWallShearBarHorBelowSpacing(long value) { m_nBWallShearBarHorBelowSpacing = value; }
	virtual void SetBWallShearBarHorBelowRatio(long value) { m_nBWallShearBarHorBelowRatio = value; }
	virtual void SetBWallShearBarHorBelowNum(long value) { m_nBWallShearBarHorBelowNum = value; }
	virtual void SetBWallShearBarHorOverType(long value) { m_nBWallShearBarHorOverType = value; }
	virtual void SetBWallShearBarHorOverSpacing(long value) { m_nBWallShearBarHorOverSpacing = value; }
	virtual void SetBWallShearBarHorOverRatio(long value) { m_nBWallShearBarHorOverRatio = value; }
	virtual void SetBWallShearBarHorOverNum(long value) { m_nBWallShearBarHorOverNum = value; }
	
	virtual void SetBWallUBarAddLengthSet(BOOL value) { m_bBWallUBarAddLengthSet = value; }
	virtual void SetBWallUBarAddLength(long value) { m_nBWallUBarAddLength = value; }
	virtual void SetBWallSpacerAddLengthSet(BOOL value) { m_bBWallSpacerAddLengthSet = value; }
	virtual void SetBWallSpacerAddLength(long value) { m_nBWallSpacerAddLength = value; }

	//Function
	virtual map<long, long> GetBWallAddBarDiaIndexMap() { return mm_BWallAddBarDiaIndex; }
	virtual map<long, long> GetBWallEndBarDiaIndexMap() { return mm_BWallEndBarDiaIndex; }
	virtual map<long, long> GetBWallDefaultUbarLenMap() { return mm_BWallDefaultUbarLen; }
	virtual map<long, long> GetBWallUserDowelHookLenMap() { return mm_BWallUserDowelHookLen; }
	virtual map<long, long> GetBWallUserDowelSpliceLenMap() { return mm_BWallUserDowelSpliceLen; }
	virtual map<long, long> GetBWallShearBarVerUserDiaIndexMap() { return mm_BWallShearBarVerUserDiaIndex; }
	virtual map<long, long> GetBWallShearBarHorUserDiaIndexMap() { return mm_BWallShearBarHorUserDiaIndex; }
	virtual map<long, long> GetBWallDowelVerBarUserDiaIndexMap() { return mm_BWallDowelVerBarUserDiaIndex; }

	virtual void SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex);
	virtual void SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex);
	virtual void SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen);
	virtual void SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetBWallShearBarVerUserDiaIndexMap(map<long, long> mapShearBarVerUserDiaIndex);
	virtual void SetBWallShearBarHorUserDiaIndexMap(map<long, long> mapShearBarHorUserDiaIndex);
	virtual void SetBWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex);

	virtual double GetBWallAddBarDia(double dDia);
	virtual double GetBWallEndBarDia(double dDia);
	virtual double GetBWallDefaultUbarLen(double dDia);
	virtual double GetBWallUserDowelHookLen(double dDia);
	virtual double GetBWallUserDowelSpliceLen(double dDia);
	virtual double GetBWallShearBarVerUserDia(double dDia);
	virtual double GetBWallShearBarHorUserDia(double dDia);
	virtual double GetBWallDowelVerBarUserDia(double dDia);

	virtual void SetBWallAddBarDiaIndex(long key, long value) { mm_BWallAddBarDiaIndex[key] = value; }
	virtual void SetBWallEndBarDiaIndex(long key, long value) { mm_BWallEndBarDiaIndex[key] = value; }
	virtual void SetBWallDefaultUbarLen(long key, long value) { mm_BWallDefaultUbarLen[key] = value; }
	virtual void SetBWallUserDowelHookLen(long key, long value) { mm_BWallUserDowelHookLen[key] = value; }
	virtual void SetBWallUserDowelSpliceLen(long key, long value) { mm_BWallUserDowelSpliceLen[key] = value; }
	virtual void SetBWallShearBarVerUserDiaIndex(long key, long value) { mm_BWallShearBarVerUserDiaIndex[key] = value; }
	virtual void SetBWallShearBarHorUserDiaIndex(long key, long value) { mm_BWallShearBarHorUserDiaIndex[key] = value; }
	virtual void SetBWallDowelVerBarUserDiaIndex(long key, long value) { mm_BWallDowelVerBarUserDiaIndex[key] = value; }

	virtual BWallVerBarRebarTemplate *GetBWallVerBarTemplateBelowDia();
	virtual BWallVerBarRebarTemplate *GetBWallVerBarTemplateExcessDia();
	virtual BWallDowelRebarTemplate *GetBWallDowelTemplateBelowDia();
	virtual BWallDowelRebarTemplate *GetBWallDowelTemplateExcessDia();

	virtual BOOL GetBWallDowelVerBarUserDiaSet();
	virtual void SetBWallDowelVerBarUserDiaSet(BOOL value);

	virtual BOOL GetBWallDowelVerBarUserSteelGradeS() { return m_bBWallDowelVerBarUserSteelGradeS; }
	virtual void SetBWallDowelVerBarUserSteelGradeS(BOOL value){ m_bBWallDowelVerBarUserSteelGradeS = value; }

	virtual VerMemberLinkDowelTemplate *GetBWallVerMemberLinkDowelTemplate();

	virtual BOOL GetBWallStraightCenterUmaShape() { return m_bBWallStraightCenterUmaShape; }
	virtual void SetBWallStraightCenterUmaShape(BOOL value) { m_bBWallStraightCenterUmaShape = value; }

	virtual BOOL GetBWallUmaSpacerConditionSet() { return m_bBWallUmaSpacerConditionSet; }
	virtual long GetBWallUmaSpacerCondition() { return m_nBWallUmaSpacerCondition; }
	virtual void SetBWallUmaSpacerConditionSet(BOOL value) { m_bBWallUmaSpacerConditionSet = value; }
	virtual void SetBWallUmaSpacerCondition(long value) { m_nBWallUmaSpacerCondition = value; }

	virtual long GetBWallUmaSpacerDiaType() { return m_nBWallUmaSpacerDiaType; }
	virtual long GetBWallUmaSpacerWallThick() { return m_nBWallUmaSpacerWallThick; }
	virtual long GetBWallUmaSpacerWallThickOverDiaIndex() { return m_nBWallUmaSpacerWallThickOverDiaIndex; }
	virtual long GetBWallUmaSpacerWallThickUnderDiaIndex() { return m_nBWallUmaSpacerWallThickUnderDiaIndex; }
	virtual BOOL GetBWallUmaSpacerUserSteelGradeS() { return m_bBWallUmaSpacerUserSteelGradeS; }
	virtual long GetBWallUmaSpacerSpacingType() { return m_nBWallUmaSpacerSpacingType; }
	virtual long GetBWallUmaSpacerWallSpacing() { return m_nBWallUmaSpacerWallSpacing; }
	virtual long GetBWallUmaSpacerUserSpacing1() { return m_nBWallUmaSpacerUserSpacing1; }
	virtual long GetBWallUmaSpacerUserSpacing2() { return m_nBWallUmaSpacerUserSpacing2; }
	virtual long GetBWallUmaSpacerHeightType() { return m_nBWallUmaSpacerHeightType; }
	virtual long GetBWallUmaSpacerUserHeight() { return m_nBWallUmaSpacerUserHeight; }
	virtual long GetBWallUmaSpacerHookLenAType() { return m_nBWallUmaSpacerHookLenAType; }
	virtual long GetBWallUmaSpacerMainBarHookLenA() { return m_nBWallUmaSpacerMainBarHookLenA; }
	virtual long GetBWallUmaSpacerUserHookLenA() { return m_nBWallUmaSpacerUserHookLenA; }
	virtual long GetBWallUmaSpacerHookLenBType() { return m_nBWallUmaSpacerHookLenBType; }
	virtual long GetBWallUmaSpacerMainBarHookLenB() { return m_nBWallUmaSpacerMainBarHookLenB; }
	virtual long GetBWallUmaSpacerUserHookLenB() { return m_nBWallUmaSpacerUserHookLenB; }
	virtual long GetBWallUmaSpacerWidth() { return m_nBWallUmaSpacerWidth; }

	virtual BOOL GetBWallUShapeTopSpacer() { return m_bBWallUShapeTopSpacer; }
	virtual BOOL GetBWallUShapeBotSpacer() { return m_bBWallUShapeBotSpacer; }
	virtual long GetBWallUShapeSpacerDiaType() { return m_nBWallUShapeSpacerDiaType; }
	virtual long GetBWallUShapeSpacerDiaIndex() { return m_nBWallUShapeSpacerDiaIndex; }
	virtual BOOL GetBWallUShapeSpacerUserSteelGradeS() { return m_bBWallUShapeSpacerUserSteelGradeS; }
	virtual long GetBWallUShapeSpacerHorSpacingType() { return m_nBWallUShapeSpacerHorSpacingType; }
	virtual long GetBWallUShapeSpacerHorSpacing() { return m_nBWallUShapeSpacerHorSpacing; }
	virtual long GetBWallUShapeSpacerHorSpacingRate() { return m_nBWallUShapeSpacerHorSpacingRate; }
	virtual long GetBWallUShapeSpacerHorSpacingCount() { return m_nBWallUShapeSpacerHorSpacingCount; }
	virtual long GetBWallUShapeSpacerLenType() { return m_nBWallUShapeSpacerLenType; }
	virtual long GetBWallUShapeSpacerUserLen() { return m_nBWallUShapeSpacerUserLen; }

	virtual void SetBWallUmaSpacerDiaType(long value) { m_nBWallUmaSpacerDiaType = value; }
	virtual void SetBWallUmaSpacerWallThick(long value) { m_nBWallUmaSpacerWallThick = value; }
	virtual void SetBWallUmaSpacerWallThickOverDiaIndex(long value) { m_nBWallUmaSpacerWallThickOverDiaIndex = value; }
	virtual void SetBWallUmaSpacerWallThickUnderDiaIndex(long value) { m_nBWallUmaSpacerWallThickUnderDiaIndex = value; }
	virtual void SetBWallUmaSpacerUserSteelGradeS(BOOL value) { m_bBWallUmaSpacerUserSteelGradeS = value; }
	virtual void SetBWallUmaSpacerSpacingType(long value) { m_nBWallUmaSpacerSpacingType = value; }
	virtual void SetBWallUmaSpacerWallSpacing(long value) { m_nBWallUmaSpacerWallSpacing = value; }
	virtual void SetBWallUmaSpacerUserSpacing1(long value) { m_nBWallUmaSpacerUserSpacing1 = value; }
	virtual void SetBWallUmaSpacerUserSpacing2(long value) { m_nBWallUmaSpacerUserSpacing2 = value; }
	virtual void SetBWallUmaSpacerHeightType(long value) { m_nBWallUmaSpacerHeightType = value; }
	virtual void SetBWallUmaSpacerUserHeight(long value) { m_nBWallUmaSpacerUserHeight = value; }
	virtual void SetBWallUmaSpacerHookLenAType(long value) { m_nBWallUmaSpacerHookLenAType = value; }
	virtual void SetBWallUmaSpacerMainBarHookLenA(long value) { m_nBWallUmaSpacerMainBarHookLenA = value; }
	virtual void SetBWallUmaSpacerUserHookLenA(long value) { m_nBWallUmaSpacerUserHookLenA = value; }
	virtual void SetBWallUmaSpacerHookLenBType(long value) { m_nBWallUmaSpacerHookLenBType = value; }
	virtual void SetBWallUmaSpacerMainBarHookLenB(long value) { m_nBWallUmaSpacerMainBarHookLenB = value; }
	virtual void SetBWallUmaSpacerUserHookLenB(long value) { m_nBWallUmaSpacerUserHookLenB = value; }
	virtual void SetBWallUmaSpacerWidth(long value) { m_nBWallUmaSpacerWidth = value; }

	virtual void SetBWallUShapeTopSpacer(BOOL value) { m_bBWallUShapeTopSpacer = value; }
	virtual void SetBWallUShapeBotSpacer(BOOL value) { m_bBWallUShapeBotSpacer = value; }
	virtual void SetBWallUShapeSpacerDiaType(long value) { m_nBWallUShapeSpacerDiaType = value; }
	virtual void SetBWallUShapeSpacerDiaIndex(long value) { m_nBWallUShapeSpacerDiaIndex = value; }
	virtual void SetBWallUShapeSpacerUserSteelGradeS(BOOL value) { m_bBWallUShapeSpacerUserSteelGradeS = value; }
	virtual void SetBWallUShapeSpacerHorSpacingType(long value) { m_nBWallUShapeSpacerHorSpacingType = value; }
	virtual void SetBWallUShapeSpacerHorSpacing(long value) { m_nBWallUShapeSpacerHorSpacing = value; }
	virtual void SetBWallUShapeSpacerHorSpacingRate(long value) { m_nBWallUShapeSpacerHorSpacingRate = value; }
	virtual void SetBWallUShapeSpacerHorSpacingCount(long value) { m_nBWallUShapeSpacerHorSpacingCount = value; }
	virtual void SetBWallUShapeSpacerLenType(long value) { m_nBWallUShapeSpacerLenType = value; }
	virtual void SetBWallUShapeSpacerUserLen(long value) { m_nBWallUShapeSpacerUserLen = value; }

	virtual BOOL GetBWallDowelPlaceSpacer(double dDia);
	virtual long GetBWallDowelSpacerDiaIndex(double dDia);
	virtual long GetBWallDowelSpacerSpacing(double dDia);
	virtual BOOL GetBWallDowelSpacerUserSteelGradeS(double dDia);
	virtual long GetBWallDowelSpacerHeightType(double dDia);
	virtual long GetBWallDowelSpacerHeightSpliceAddLength(double dDia);
	virtual long GetBWallDowelSpacerUserHeight(double dDia);
	virtual long GetBWallDowelSpacerLengthA(double dDia);
	virtual long GetBWallDowelSpacerLengthB(double dDia);
	virtual long GetBWallDowelSpacerWidthType(double dDia);
	virtual long GetBWallDowelSpacerUserWidth(double dDia);

	virtual BOOL GetBWallOpeningAddBarDeductionCondition() { return m_bBWallOpeningAddBarDeductionCondition; }
	virtual void SetBWallOpeningAddBarDeductionCondition(BOOL value) { m_bBWallOpeningAddBarDeductionCondition = value; }

	virtual BOOL GetBWallBraceSupportBarDiaConditionHeight() { return m_bBWallBraceSupportBarDiaConditionHeight; }
	virtual void SetBWallBraceSupportBarDiaConditionHeight(BOOL value) { m_bBWallBraceSupportBarDiaConditionHeight = value; }
	virtual long GetBWallBraceSupportBarUnderDiaIndex() { return m_nBWallBraceSupportBarUnderDiaIndex; }
	virtual void SetBWallBraceSupportBarUnderDiaIndex(long value) { m_nBWallBraceSupportBarUnderDiaIndex = value; }

	virtual BOOL GetBWallSpacerVerStartSpacingSet() { return m_bBWallSpacerVerStartSpacingSet; }
	virtual void SetBWallSpacerVerStartSpacingSet(BOOL value) { m_bBWallSpacerVerStartSpacingSet = value; }
	virtual long GetBWallSpacerVerStartSpacing() { return m_nBWallSpacerVerStartSpacing; }
	virtual void SetBWallSpacerVerStartSpacing(long value) { m_nBWallSpacerVerStartSpacing = value; }

	virtual BOOL GetBWallSpliceAddLengthSet() { return m_bBWallSpliceAddLengthSet; }
	virtual void SetBWallSpliceAddLengthSet(BOOL value) { m_bBWallSpliceAddLengthSet = value; }
	virtual long GetBWallSpliceAddLengthNumSplice() { return m_nBWallSpliceAddLengthNumSplice; }
	virtual void SetBWallSpliceAddLengthNumSplice(long value) { m_nBWallSpliceAddLengthNumSplice = value; }

	virtual long GetBWallPlusCrossShapeType() { return m_nBWallPlusCrossShapeType; }
	virtual long GetBWallTCrossShapeType() { return m_nBWallTCrossShapeType; }
	virtual long GetBWallLCrossShapeType() { return m_nBWallLCrossShapeType; }

	virtual void SetBWallPlusCrossShapeType(long value) { m_nBWallPlusCrossShapeType = value; }
	virtual void SetBWallTCrossShapeType(long value) { m_nBWallTCrossShapeType = value; }
	virtual void SetBWallLCrossShapeType(long value) { m_nBWallLCrossShapeType = value; }

	virtual long GetBWallStraightCenterUmaPlaceType() { return m_nBWallStraightCenterUmaPlaceType; }
	virtual void SetBWallStraightCenterUmaPlaceType(long value) { m_nBWallStraightCenterUmaPlaceType = value; }

private:
	long m_nBWallVerBarPlaceTypeDiaIndex;
	long m_nBWallDowelPlaceTypeDiaIndex;
	BWallVerBarRebarTemplate *m_pVerBarTemplateBelowDia, *m_pVerBarTemplateExcessDia;
	BWallDowelRebarTemplate *m_pDowelTemplateBelowDia, *m_pDowelTemplateExcessDia;
	

	long m_nBWallVerNumBarCrossBarIncludeSet;	//수직근 개수 산정 시 교차보강근 0:포함 1:미포함
	long m_nBWallCrossBarEAType;	//교차보강근 개수
	BOOL m_bBWallCrossBarEATypeExcessThickSet;
	long m_nBWallCrossBarEATypeExcessThick;
	long m_nBWallCrossBarStartSpacingType;	//교차보강근 주근 시작간격 0:S/2 1:S
	BOOL m_bBWallPlaceEndBarSet;	//단부보강근 설정
	long m_nBWallEndVerAddBarCount;	//단부보강근 개수
	long m_nBWallEndVerAddBarDist;	//단부보강근 간격
	long m_nBWallEndVerStartSpacingType;	//단부보강근 주근 시작간격 0:S/2 1:S

	//SK건설 리바템플릿 개선
	BOOL m_bBWallPlaceBraceSupportBar;	
	long m_nBWallBraceSupportBarOverDiaIndex;
	BOOL m_bBWallBraceSupportBarDiaConditionHeight;
	long m_nBWallBraceSupportBarUnderDiaIndex;
	BOOL m_bBWallBraceSupportBarUserSteelGradeS;
	long m_nBWallBraceSupportBarWallLengthCondition;
	long m_nBWallBraceSupportBarWallLengthConditionOverLength;
	long m_nBWallBraceSupportBarWallLengthConditionUnderLength;
	BOOL m_bBWallBraceSupportNotPlaceConditionDiaSet;
	long m_nBWallBraceSupportNotPlaceConditionDiaIndex;
	BOOL m_bBWallBraceSupportNotPlaceConditionLengthSet;
	long m_nBWallBraceSupportNotPlaceConditionLength;
	BOOL m_bBWallBraceSupportNotPlaceConditionBottomDowel;

	BOOL m_bBWallBraceSupportNotPlaceConditionHeightSet;
	long m_nBWallBraceSupportNotPlaceConditionHeight;
	//	

	map<long, long> mm_BWallAddBarDiaIndex;		//DiaIndex별 교차보강근 직경설정 값
	map<long, long> mm_BWallEndBarDiaIndex;		//DiaIndex별 단부보강근 직경설정 값

	BOOL m_bBWallPlaceUBar4ShortWall;		//짧은 벽체에 U자형 철근 적용
	long m_nBWallMaxUbarLegLength;			//최대 벽체 길이
	BOOL m_bBWallPlaceStirrup4ShortWall;	//짧은 벽체에 스터럽 철근 적용
	long m_nBWallHBarMaxStirrupLen;			//최대 벽체 길이
	BOOL m_bBWallHorBarStartSet;			//시작 간격 설정
	long m_nBWallHorBarStartLength;			//시작 간격
	BOOL m_bBWallDefaultUbarLen;			//단부 U자형 철근 발길이 별도 지정
	BOOL m_bBWallCrossBarUBarHalfPlace;		//벽 교차부에 U자형 철근 50% 적용
	BOOL m_bBWallStraightEndUBarPlaceSet;	//일자형 벽체 단부에 U자형 철근 적용
	
	map<long, long> mm_BWallDefaultUbarLen;		//DiaIndex별 단부 U-BAR 발길이 설정 설정 값

	long m_nBWallSpacerDiaType;				//직경 설정	0:수평근 직경에 따름 1:직경
	long m_nBWallSpacerDiaIndex;			//직경Index
	BOOL m_bBWallSpacerUserSteelGradeS;
	long m_nBWallSpacerHorSpacing;			//수평 간격
	long m_nBWallSpacerVerSpacingType;		//수직 간격 타입	0:간격 1:비율 2:개수
	long m_nBWallSpacerVerSpacing;			//수직 간격
	long m_nBWallSpacerVerSpacingRate;		//수직 비율
	long m_nBWallSpacerVerSpacingCount;		//수직 개수
	BOOL m_bBWallSpacerVerEndBarPlaceSet;		//일자형 벽체 단부 폭고정근 추가 설정
	long m_nBWallSpacerVerEndBarSpacingType;	//수직 간격 타입	0:간격 1:비율 2:개수
	long m_nBWallSpacerVerEndBarSpacing;		//수직 간격
	long m_nBWallSpacerVerEndBarSpacingRate;	//수직 비율
	long m_nBWallSpacerVerEndBarSpacingCount;	//수직 개수
	long m_nBWallDowelSpacerVerSpacingCount;		//다월 폭 고정근 수직 개수
	long m_nBWallSpacerLenType;				//발길이 타입	0:인장이음 1:후크
	long m_nBWallSpacerLenCalWayA;			//발길이 발길이	0:표준 1:사용자 정의
	long m_nBWallSpacerLenCalWayB;			//발길이 발길이	0:표준 1:사용자 정의
	long m_nBWallSpacerLenA;				//사용자 정의 길이1
	long m_nBWallSpacerLenB;				//사용자 정의 길이2
	long m_nBWallSpacerShape;				//형상
	BOOL m_bBWallSpacerRebarPlaceSet;		//배근 조건 설정
	long m_nBWallSpacerRebarPlaceCondition;	//배근 조건
	
	BOOL m_bBWallSupportRebarsSet;					//수평 도리근 설정
	long m_nBWallSupportRebarsDiaIndex;				//직경
	BOOL m_bBWallSupportRebarsUserSteelGradeS;
	double m_dBWallSupportRebarsTotalLengthRate;	//벽 길이 비율
	long m_nBWallSupportRebarsOrderLength;			//주문길이
	long m_nBWallSupportRebarsSpliceLength;			//이음길이
	long m_nBWallSupportRebarsCorrectiveLength;		//보정길이

	BOOL m_bBWallAddBarsSet;						//보강근 설정
	long m_nBWallAddBarsDiaIndex;					//보강근 직경
	BOOL m_bBWallAddBarsUserSteelGradeS;
	long m_nBWallAddBarsNumber;						//보강근 개수
	long m_nBWallAddBarsLengthType;					//보강근 길이 타입
	long m_nBWallAddBarsTensionLengthType;			//보강근 인장길이 타입
	long m_nBWallAddBarsUserLength;			//보강근 사용자 길이

	BOOL m_bBWallUBarAddLengthSet;
	long m_nBWallUBarAddLength;
	BOOL m_bBWallSpacerAddLengthSet;
	long m_nBWallSpacerAddLength;

	BOOL m_bBWallDowelSupportPlaceType;	//지지근 설정
		
	map<long, long> mm_BWallUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_BWallUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값

	BOOL m_bBWallShearBarSet;		//전단보강근 정보 설정
	long m_nBWallShearBarDiaType;	//직경 정보 0: 설정한 직경 1: 사용자 직경
	long m_nBWallShearBarDiaIndex;	//설정한 직경 Index
	BOOL m_bBWallShearBarUserSteelGradeS;
	long m_nBWallShearBarUserDiaType;	//직경 정보 사용자 직경 0:수직근 1:수평근
	map<long, long> mm_BWallShearBarVerUserDiaIndex;		//DiaIndex별 전단보강근 수직근 직경설정 값
	map<long, long> mm_BWallShearBarHorUserDiaIndex;		//DiaIndex별 전단보강근 수평근 직경설정 값
	
	long m_nBWallShearBarShapeType;		//형상 0:90*90	1:135*135	2:90*135

	long m_nBWallShearBarVerSpacingCondition;	//수직 간격 조건
	long m_nBWallShearBarVerBelowType;			//수평근 간격 <= 수직 간격 조건
	long m_nBWallShearBarVerBelowSpacing;		//간격
	long m_nBWallShearBarVerBelowRatio;			//비율
	long m_nBWallShearBarVerBelowNum;			//개수
	long m_nBWallShearBarVerOverType;			//수평근 간격 > 수직 간격 조건
	long m_nBWallShearBarVerOverSpacing;		//간격
	long m_nBWallShearBarVerOverRatio;			//비율
	long m_nBWallShearBarVerOverNum;			//개수

	long m_nBWallShearBarHorSpacingCondition;	//수평 간격 조건
	long m_nBWallShearBarHorBelowType;			//수직근 간격 <= 수평 간격 조건
	long m_nBWallShearBarHorBelowSpacing;		//간격
	long m_nBWallShearBarHorBelowRatio;			//비율
	long m_nBWallShearBarHorBelowNum;			//개수
	long m_nBWallShearBarHorOverType;			//수직근 간격 > 수평 간격 조건
	long m_nBWallShearBarHorOverSpacing;		//간격
	long m_nBWallShearBarHorOverRatio;			//비율
	long m_nBWallShearBarHorOverNum;			//개수

	BOOL m_bBWallDowelVerBarUserDiaSet;
	BOOL m_bBWallDowelVerBarUserSteelGradeS;

	map<long, long> mm_BWallDowelVerBarUserDiaIndex;

	VerMemberLinkDowelTemplate *m_pBWallVerMemberLinkDowelTemplate;

	BOOL m_bBWallStraightCenterUmaShape;
	long m_nBWallStraightCenterUmaPlaceType;
	BOOL m_bBWallUmaSpacerConditionSet;
	long m_nBWallUmaSpacerCondition;
	long m_nBWallUmaSpacerDiaType;		//직경 0:해당벽체직경 1:벽체 두께
	long m_nBWallUmaSpacerWallThick;		//벽체 두께
	long m_nBWallUmaSpacerWallThickOverDiaIndex;	//벽체 두께 이상 직경
	long m_nBWallUmaSpacerWallThickUnderDiaIndex;	//벽체 두께 미만 직경
	BOOL m_bBWallUmaSpacerUserSteelGradeS;
	long m_nBWallUmaSpacerSpacingType;	//간격 0:해당 벽체 간격 1:사용자
	long m_nBWallUmaSpacerWallSpacing;	//간격 : 해당 벽체 간격 입력
	long m_nBWallUmaSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nBWallUmaSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nBWallUmaSpacerHeightType;		//높이
	long m_nBWallUmaSpacerUserHeight;		//높이 : 사용자 입력
	long m_nBWallUmaSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nBWallUmaSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nBWallUmaSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nBWallUmaSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nBWallUmaSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nBWallUmaSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nBWallUmaSpacerWidth;			//폭

	BOOL m_bBWallUShapeTopSpacer;
	BOOL m_bBWallUShapeBotSpacer;
	long m_nBWallUShapeSpacerDiaType;				//직경 설정	0:수직근 직경에 따름 1:직경
	long m_nBWallUShapeSpacerDiaIndex;			//직경Index
	BOOL m_bBWallUShapeSpacerUserSteelGradeS;	
	long m_nBWallUShapeSpacerHorSpacingType;		//수평 간격 타입	0:간격 1:비율 2:개수
	long m_nBWallUShapeSpacerHorSpacing;			//수평 간격
	long m_nBWallUShapeSpacerHorSpacingRate;		//수평 비율
	long m_nBWallUShapeSpacerHorSpacingCount;	//수평 개수
	long m_nBWallUShapeSpacerLenType;				//0:인장이음 1:사용자지정	
	long m_nBWallUShapeSpacerUserLen;			//사용자지정

	BOOL m_bBWallOpeningAddBarDeductionCondition;

	BOOL m_bBWallSpacerVerStartSpacingSet;
	long m_nBWallSpacerVerStartSpacing;

	BOOL m_bBWallSpliceAddLengthSet;
	long m_nBWallSpliceAddLengthNumSplice;


	long m_nBWallPlusCrossShapeType;
	long m_nBWallTCrossShapeType;
	long m_nBWallLCrossShapeType;
};

