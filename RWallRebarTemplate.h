#pragma once
#include "BaseRebarTemplate.h"

class RWallVerBarRebarTemplate;
class RWallDowelRebarTemplate;
class VerMemberLinkDowelTemplate;
class StructureModel_CLASS RWallRebarTemplate : public BaseRebarTemplate
{
public:
	RWallRebarTemplate();
	~RWallRebarTemplate(void);

	void Serialize(CArchive& ar);

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void DataInit();

	//Get Property
	virtual long GetRWallVerBarPlaceTypeDiaIndex() {return m_nRWallVerBarPlaceTypeDiaIndex; }
	virtual long GetRWallDowelPlaceTypeDiaIndex() { return m_nRWallDowelPlaceTypeDiaIndex; }

	virtual BOOL GetRWallPlaceCrossBar() { return m_bRWallPlaceCrossBar; }
	virtual long GetRWallCrossBarEAType() { return m_nRWallCrossBarEAType; }
	virtual BOOL GetRWallCrossBarEATypeExcessThickSet() { return m_bRWallCrossBarEATypeExcessThickSet; }
	virtual long GetRWallCrossBarEATypeExcessThick() { return m_nRWallCrossBarEATypeExcessThick; }
	virtual long GetRWallCrossBarStartSpacingType() { return m_nRWallCrossBarStartSpacingType; }
	

	virtual BOOL GetRWallPlaceBraceSupportBar() { return m_bRWallPlaceBraceSupportBar; }
	virtual long GetRWallBraceSupportBarOverDiaIndex() { return m_nRWallBraceSupportBarOverDiaIndex; }
	virtual BOOL GetRWallBraceSupportBarUserSteelGradeS() { return m_bRWallBraceSupportBarUserSteelGradeS; }
	virtual long GetRWallBraceSupportBarWallLengthCondition() { return m_nRWallBraceSupportBarWallLengthCondition; }
	virtual long GetRWallBraceSupportBarWallLengthConditionOverLength() { return m_nRWallBraceSupportBarWallLengthConditionOverLength; }
	virtual long GetRWallBraceSupportBarWallLengthConditionUnderLength() { return m_nRWallBraceSupportBarWallLengthConditionUnderLength; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionDiaSet() { return m_bRWallBraceSupportNotPlaceConditionDiaSet; }
	virtual long GetRWallBraceSupportNotPlaceConditionDiaIndex() { return m_nRWallBraceSupportNotPlaceConditionDiaIndex; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionLengthSet() { return m_bRWallBraceSupportNotPlaceConditionLengthSet; }
	virtual long GetRWallBraceSupportNotPlaceConditionLength() { return m_nRWallBraceSupportNotPlaceConditionLength; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionBottomDowel() { return m_bRWallBraceSupportNotPlaceConditionBottomDowel; }

	virtual BOOL GetRWallBraceSupportNotPlaceConditionHeightSet() { return m_bRWallBraceSupportNotPlaceConditionHeightSet; }
	virtual long GetRWallBraceSupportNotPlaceConditionHeight() { return m_nRWallBraceSupportNotPlaceConditionHeight; }

	virtual BOOL GetRWallHorInfoSet() { return m_bRWallHorInfoSet; }
	virtual BOOL GetRWallHorBarStartSet() { return m_bRWallHorBarStartSet; }
	virtual long GetRWallHorBarStartLength() { return m_nRWallHorBarStartLength; }
	virtual long GetRWallEndUbar() { return m_nRWallEndUbar; }
	virtual BOOL GetRWallCrossEndUBarPlace() { return m_bRWallCrossEndUBarPlace; }
	virtual BOOL GetRWallDefaultUbarLen() { return m_bRWallDefaultUbarLen; }
	virtual BOOL GetRWallCrossBarUBarHalfPlace() { return m_bRWallCrossBarUBarHalfPlace; }

	virtual BOOL GetRWallSpacerInfoSet() { return m_bRWallSpacerInfoSet; }
	virtual long GetRWallSpacerDiaType() { return m_nRWallSpacerDiaType; }
	virtual long GetRWallSpacerDiaIndex() { return m_nRWallSpacerDiaIndex; }
	virtual long GetRWallSpacerHorSpacing() { return m_nRWallSpacerHorSpacing; }
	virtual long GetRWallSpacerVerSpacingType() { return m_nRWallSpacerVerSpacingType; }
	virtual long GetRWallSpacerVerSpacing() { return m_nRWallSpacerVerSpacing; }
	virtual long GetRWallSpacerVerSpacingRate() { return m_nRWallSpacerVerSpacingRate; }
	virtual long GetRWallSpacerVerSpacingCount() { return m_nRWallSpacerVerSpacingCount; }
	virtual long GetRWallDowelSpacerVerSpacingCount() { return m_nRWallDowelSpacerVerSpacingCount; }
	virtual long GetRWallSpacerLenType() { return m_nRWallSpacerLenType; }
	virtual long GetRWallSpacerLenCalWayA() { return m_nRWallSpacerLenCalWayA; }
	virtual long GetRWallSpacerLenCalWayB() { return m_nRWallSpacerLenCalWayB; }
	virtual long GetRWallSpacerLenA() { return m_nRWallSpacerLenA; }
	virtual long GetRWallSpacerLenB() { return m_nRWallSpacerLenB; }
	virtual long GetRWallSpacerShape() { return m_nRWallSpacerShape; }
	virtual BOOL GetRWallSpacerRebarPlaceSet() { return m_bRWallSpacerRebarPlaceSet; }
	virtual long GetRWallSpacerRebarPlaceCondition() { return m_nRWallSpacerRebarPlaceCondition; }

	virtual BOOL GetRWallSupportRebarsSet() { return m_bRWallSupportRebarsSet; }
	virtual long GetRWallSupportRebarsDiaIndex() { return m_nRWallSupportRebarsDiaIndex; }
	virtual double GetRWallSupportRebarsTotalLengthRate() { return m_dRWallSupportRebarsTotalLengthRate; }
	virtual long GetRWallSupportRebarsOrderLength() { return m_nRWallSupportRebarsOrderLength; }
	virtual long GetRWallSupportRebarsSpliceLength() { return m_nRWallSupportRebarsSpliceLength; }
	virtual long GetRWallSupportRebarsCorrectiveLength() { return m_nRWallSupportRebarsCorrectiveLength; }

	virtual BOOL GetRWallAddBarsSet() { return m_bRWallAddBarsSet; }
	virtual long GetRWallAddBarsDiaIndex() { return m_nRWallAddBarsDiaIndex; }
	virtual long GetRWallAddBarsNumber() { return m_nRWallAddBarsNumber; }
	virtual long GetRWallAddBarsLengthType() { return m_nRWallAddBarsLengthType; }
	virtual long GetRWallAddBarsTensionLengthType() { return m_nRWallAddBarsTensionLengthType; }
	virtual long GetRWallAddBarsUserLength() { return m_nRWallAddBarsUserLength; }

	virtual BOOL GetRWallVerInfoSet() { return m_bRWallVerInfoSet; }
	virtual BOOL GetRWallDowelInfoSet() { return m_bRWallDowelInfoSet; }
	virtual BOOL GetRWallDowelSupportPlaceType() { return m_bRWallDowelSupportPlaceType; }


	virtual long GetRWallVerBarTopLenType(double dDia);
	virtual long GetRWallVerBarSpliceType(double dDia);
	virtual long GetRWallVerBarTopFloorDevType(double dDia);
	virtual long GetRWallVerBarConstructionHeight(double dDia);
	virtual BOOL GetRWallVerBarAlterSpliceSet(double dDia);
	virtual long GetRWallVerBarAlterSpliceDist(double dDia);

	virtual long GetRWallVerBarAlterSpliceType(double dDia);
	virtual long GetRWallDowelAlterSpliceType(double dDia);

	virtual BOOL GetRWallUserDowelHookLenSet(double dDia);
	virtual long GetRWallDowelBarAnchorType(double dDia);
	virtual long GetRWallDowelBarTopLenType(double dDia);
	virtual long GetRWallDowelDowelShape(double dDia);
	virtual long GetRWallDowelSupportBarDiaIndex(double dDia);
	virtual BOOL GetRWallDowelSupportBarUserSteelGradeS(double dDia);
	virtual long GetRWallDowelSupportBarNum(double dDia);
	virtual long GetRWallDowelBarSpliceType(double dDia);
	virtual long GetRWallDowelConstructionHeight(double dDia);
	virtual BOOL GetRWallDowelAlterSpliceSet(double dDia);
	virtual long GetRWallDowelAlterSpliceDist(double dDia);
	virtual BOOL GetRWallDowelHookAnchorPlaceSpacingSet(double dDia);
	virtual long GetRWallDowelHookAnchorPlaceSpacing(double dDia);
	virtual BOOL GetRWallDowelBotAddBarIntegratedPlaceSet(double dDia);
	virtual BOOL GetRWallDowelAdjustRatioSet(double dDia);
	virtual int GetRWallDowelAdjustRatio(double dDia);
	virtual long GetRWallDowelBotAddBarIntegratedPlaceLengthConditionType(double dDia);	
	virtual long GetRWallDowelBotAddBarIntegratedPlaceLengthCondition(double dDia);

	virtual long GetRWallShearBarShapeType() { return m_nRWallShearBarShapeType; }

	virtual BOOL GetRWallUBarAddLengthSet() { return m_bRWallUBarAddLengthSet; }
	virtual long GetRWallUBarAddLength() { return m_nRWallUBarAddLength; }
	virtual BOOL GetRWallSpacerAddLengthSet() { return m_bRWallSpacerAddLengthSet; }
	virtual long GetRWallSpacerAddLength() { return m_nRWallSpacerAddLength; }

	//Set Property
	virtual void SetRWallVerBarPlaceTypeDiaIndex(long value) { m_nRWallVerBarPlaceTypeDiaIndex = value; }
	virtual void SetRWallDowelPlaceTypeDiaIndex(long value) { m_nRWallDowelPlaceTypeDiaIndex = value; }

	virtual void SetRWallPlaceCrossBar(BOOL value) { m_bRWallPlaceCrossBar = value; }
	virtual void SetRWallCrossBarEAType(long value) { m_nRWallCrossBarEAType = value; }
	virtual void SetRWallCrossBarEATypeExcessThickSet(BOOL value) { m_bRWallCrossBarEATypeExcessThickSet = value; }
	virtual void SetRWallCrossBarEATypeExcessThick(long value) { m_nRWallCrossBarEATypeExcessThick = value; }
	virtual void SetRWallCrossBarStartSpacingType(long value) { m_nRWallCrossBarStartSpacingType = value; }
	
	virtual void SetRWallPlaceBraceSupportBar(BOOL value) { m_bRWallPlaceBraceSupportBar = value; }
	virtual void SetRWallBraceSupportBarOverDiaIndex(long value) { m_nRWallBraceSupportBarOverDiaIndex = value; }
	virtual void SetRWallBraceSupportBarUserSteelGradeS(BOOL value) { m_bRWallBraceSupportBarUserSteelGradeS = value; }
	virtual void SetRWallBraceSupportBarWallLengthCondition(long value) { m_nRWallBraceSupportBarWallLengthCondition = value; }
	virtual void SetRWallBraceSupportBarWallLengthConditionOverLength(long value) { m_nRWallBraceSupportBarWallLengthConditionOverLength = value; }
	virtual void SetRWallBraceSupportBarWallLengthConditionUnderLength(long value) { m_nRWallBraceSupportBarWallLengthConditionUnderLength = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionDiaSet(BOOL value) { m_bRWallBraceSupportNotPlaceConditionDiaSet = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionDiaIndex(long value) { m_nRWallBraceSupportNotPlaceConditionDiaIndex = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionLengthSet(BOOL value) { m_bRWallBraceSupportNotPlaceConditionLengthSet = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionLength(long value) { m_nRWallBraceSupportNotPlaceConditionLength = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionBottomDowel(BOOL value) { m_bRWallBraceSupportNotPlaceConditionBottomDowel = value; }

	virtual void SetRWallBraceSupportNotPlaceConditionHeightSet(BOOL value) { m_bRWallBraceSupportNotPlaceConditionHeightSet = value; }
	virtual void SetRWallBraceSupportNotPlaceConditionHeight(long value) { m_nRWallBraceSupportNotPlaceConditionHeight = value; }

	virtual void SetRWallHorInfoSet(BOOL value) { m_bRWallHorInfoSet = value; }
	virtual void SetRWallHorBarStartSet(BOOL value) { m_bRWallHorBarStartSet = value; }
	virtual void SetRWallHorBarStartLength(long value) { m_nRWallHorBarStartLength = value; }
	virtual void SetRWallEndUbar(long value) { m_nRWallEndUbar = value; }
	virtual void SetRWallCrossEndUBarPlace(BOOL value) { m_bRWallCrossEndUBarPlace = value; }
	virtual void SetRWallDefaultUbarLen(BOOL value) { m_bRWallDefaultUbarLen = value; }
	virtual void SetRWallCrossBarUBarHalfPlace(BOOL value) {  m_bRWallCrossBarUBarHalfPlace = value; }

	virtual void SetRWallSpacerInfoSet(BOOL value) { m_bRWallSpacerInfoSet = value; }
	virtual void SetRWallSpacerDiaType(long value) { m_nRWallSpacerDiaType = value; }
	virtual void SetRWallSpacerDiaIndex(long value) { m_nRWallSpacerDiaIndex = value; }
	virtual void SetRWallSpacerHorSpacing(long value) { m_nRWallSpacerHorSpacing = value; }
	virtual void SetRWallSpacerVerSpacingType(long value) { m_nRWallSpacerVerSpacingType = value; }
	virtual void SetRWallSpacerVerSpacing(long value) { m_nRWallSpacerVerSpacing = value; }
	virtual void SetRWallSpacerVerSpacingRate(long value) { m_nRWallSpacerVerSpacingRate = value; }
	virtual void SetRWallSpacerVerSpacingCount(long value) { m_nRWallSpacerVerSpacingCount = value; }
	virtual void SetRWallDowelSpacerVerSpacingCount(long value) { m_nRWallDowelSpacerVerSpacingCount = value; }
	virtual void SetRWallSpacerLenType(long value) { m_nRWallSpacerLenType = value; }
	virtual void SetRWallSpacerLenCalWayA(long value) { m_nRWallSpacerLenCalWayA = value; }
	virtual void SetRWallSpacerLenCalWayB(long value) { m_nRWallSpacerLenCalWayB = value; }
	virtual void SetRWallSpacerLenA(long value) { m_nRWallSpacerLenA = value; }
	virtual void SetRWallSpacerLenB(long value) { m_nRWallSpacerLenB = value; }
	virtual void SetRWallSpacerShape(long value) { m_nRWallSpacerShape = value; }
	virtual void SetRWallSpacerRebarPlaceSet(BOOL value) { m_bRWallSpacerRebarPlaceSet = value; }
	virtual void SetRWallSpacerRebarPlaceCondition(long value) { m_nRWallSpacerRebarPlaceCondition = value; }
	
	virtual void SetRWallSupportRebarsSet(BOOL value) { m_bRWallSupportRebarsSet = value; }
	virtual void SetRWallSupportRebarsDiaIndex(long value) { m_nRWallSupportRebarsDiaIndex = value; }
	virtual void SetRWallSupportRebarsTotalLengthRate(double value) { m_dRWallSupportRebarsTotalLengthRate = value; }
	virtual void SetRWallSupportRebarsOrderLength(long value) { m_nRWallSupportRebarsOrderLength = value; }
	virtual void SetRWallSupportRebarsSpliceLength(long value) { m_nRWallSupportRebarsSpliceLength = value; }
	virtual void SetRWallSupportRebarsCorrectiveLength(long value) { m_nRWallSupportRebarsCorrectiveLength = value; }

	virtual void SetRWallAddBarsSet(BOOL value) { m_bRWallAddBarsSet = value; }
	virtual void SetRWallAddBarsDiaIndex(long value) { m_nRWallAddBarsDiaIndex = value; }
	virtual void SetRWallAddBarsNumber(long value) { m_nRWallAddBarsNumber = value; }
	virtual void SetRWallAddBarsLengthType(long value) { m_nRWallAddBarsLengthType = value; }
	virtual void SetRWallAddBarsTensionLengthType(long value) { m_nRWallAddBarsTensionLengthType = value; }
	virtual void SetRWallAddBarsUserLength(long value) { m_nRWallAddBarsUserLength = value; }

	virtual void SetRWallVerInfoSet(BOOL value) { m_bRWallVerInfoSet = value; }
	virtual void SetRWallDowelSupportPlaceType(BOOL value) { m_bRWallDowelSupportPlaceType = value; }
	virtual void SetRWallDowelInfoSet(BOOL value) { m_bRWallDowelInfoSet = value; }
				
	virtual void SetRWallShearBarShapeType(long value) { m_nRWallShearBarShapeType = value; }

	virtual void SetRWallUBarAddLengthSet(BOOL value) { m_bRWallUBarAddLengthSet = value; }
	virtual void SetRWallUBarAddLength(long value) { m_nRWallUBarAddLength = value; }
	virtual void SetRWallSpacerAddLengthSet(BOOL value) { m_bRWallSpacerAddLengthSet = value; }
	virtual void SetRWallSpacerAddLength(long value) { m_nRWallSpacerAddLength = value; }

	//Function
	virtual map<long, long> GetRWallDefaultUbarLenMap() { return mm_RWallDefaultUbarLen; }
	virtual map<long, long> GetRWallUserDowelHookLenMap() { return mm_RWallUserDowelHookLen; }
	virtual map<long, long> GetRWallUserDowelSpliceLenMap() { return mm_RWallUserDowelSpliceLen; }
	virtual map<long, long> GetRWallDowelVerBarUserDiaIndexMap() { return mm_RWallDowelVerBarUserDiaIndex; }

	virtual void SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen);
	virtual void SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetRWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex);

	virtual double GetRWallDefaultUbarLen(double dDia);
	virtual double GetRWallUserDowelHookLen(double dDia);
	virtual double GetRWallUserDowelSpliceLen(double dDia);
	virtual double GetRWallDowelVerBarUserDia(double dDia);

	virtual void SetRWallDefaultUbarLen(long key, long value) { mm_RWallDefaultUbarLen[key] = value; }
	virtual void SetRWallUserDowelHookLen(long key, long value) { mm_RWallUserDowelHookLen[key] = value; }
	virtual void SetRWallUserDowelSpliceLen(long key, long value) { mm_RWallUserDowelSpliceLen[key] = value; }
	virtual void SetRWallDowelVerBarUserDiaIndex(long key, long value) { mm_RWallDowelVerBarUserDiaIndex[key] = value; }
	
	virtual RWallVerBarRebarTemplate *GetRWallVerBarTemplateBelowDia();
	virtual RWallVerBarRebarTemplate *GetRWallVerBarTemplateExcessDia();
	virtual RWallDowelRebarTemplate *GetRWallDowelTemplateBelowDia();
	virtual RWallDowelRebarTemplate *GetRWallDowelTemplateExcessDia();

	virtual double GetRWallCrossBarDia(double dDia);
	virtual map<long, long> GetRWallCrossBarDiaIndexMap(){ return mm_RWallCrossBarDiaIndex; }
	virtual void SetRWallCrossBarDiaIndexMap(map<long, long> mapCrossBarDiaIndex);
	virtual void SetRWallCrossBarDiaIndex(long key, long value) { mm_RWallCrossBarDiaIndex[key] = value; }

	BOOL GetRWallDowelVerBarUserDiaSet();
	void SetRWallDowelVerBarUserDiaSet(BOOL value);

	virtual BOOL GetRWallDowelVerBarUserSteelGradeS() { return m_bRWallDowelVerBarUserSteelGradeS; }
	virtual void SetRWallDowelVerBarUserSteelGradeS(BOOL value){ m_bRWallDowelVerBarUserSteelGradeS = value; }

	virtual VerMemberLinkDowelTemplate *GetRWallVerMemberLinkDowelTemplate();

	virtual BOOL GetRWallAddBarsUserSteelGradeS() { return m_bRWallAddBarsUserSteelGradeS; }
	virtual void SetRWallAddBarsUserSteelGradeS(BOOL value) { m_bRWallAddBarsUserSteelGradeS = value; }
	virtual BOOL GetRWallSupportRebarsUserSteelGradeS() { return m_bRWallSupportRebarsUserSteelGradeS; }
	virtual void SetRWallSupportRebarsUserSteelGradeS(BOOL value) { m_bRWallSupportRebarsUserSteelGradeS = value; }
	virtual BOOL GetRWallSpacerUserSteelGradeS() { return m_bRWallSpacerUserSteelGradeS; }
	virtual void SetRWallSpacerUserSteelGradeS(BOOL value) { m_bRWallSpacerUserSteelGradeS = value; }

	virtual BOOL GetRWallStraightCenterUmaShape() { return m_bRWallStraightCenterUmaShape; }
	virtual void SetRWallStraightCenterUmaShape(BOOL value) { m_bRWallStraightCenterUmaShape = value; }

	virtual BOOL GetRWallUmaSpacerConditionSet() { return m_bRWallUmaSpacerConditionSet; }
	virtual long GetRWallUmaSpacerCondition() { return m_nRWallUmaSpacerCondition; }
	virtual void SetRWallUmaSpacerConditionSet(BOOL value) { m_bRWallUmaSpacerConditionSet = value; }
	virtual void SetRWallUmaSpacerCondition(long value) { m_nRWallUmaSpacerCondition = value; }

	virtual long GetRWallUmaSpacerDiaType() { return m_nRWallUmaSpacerDiaType; }
	virtual long GetRWallUmaSpacerWallThick() { return m_nRWallUmaSpacerWallThick; }
	virtual long GetRWallUmaSpacerWallThickOverDiaIndex() { return m_nRWallUmaSpacerWallThickOverDiaIndex; }
	virtual long GetRWallUmaSpacerWallThickUnderDiaIndex() { return m_nRWallUmaSpacerWallThickUnderDiaIndex; }
	virtual BOOL GetRWallUmaSpacerUserSteelGradeS() { return m_bRWallUmaSpacerUserSteelGradeS; }
	virtual long GetRWallUmaSpacerSpacingType() { return m_nRWallUmaSpacerSpacingType; }
	virtual long GetRWallUmaSpacerWallSpacing() { return m_nRWallUmaSpacerWallSpacing; }
	virtual long GetRWallUmaSpacerUserSpacing1() { return m_nRWallUmaSpacerUserSpacing1; }
	virtual long GetRWallUmaSpacerUserSpacing2() { return m_nRWallUmaSpacerUserSpacing2; }
	virtual long GetRWallUmaSpacerHeightType() { return m_nRWallUmaSpacerHeightType; }
	virtual long GetRWallUmaSpacerUserHeight() { return m_nRWallUmaSpacerUserHeight; }
	virtual long GetRWallUmaSpacerHookLenAType() { return m_nRWallUmaSpacerHookLenAType; }
	virtual long GetRWallUmaSpacerMainBarHookLenA() { return m_nRWallUmaSpacerMainBarHookLenA; }
	virtual long GetRWallUmaSpacerUserHookLenA() { return m_nRWallUmaSpacerUserHookLenA; }
	virtual long GetRWallUmaSpacerHookLenBType() { return m_nRWallUmaSpacerHookLenBType; }
	virtual long GetRWallUmaSpacerMainBarHookLenB() { return m_nRWallUmaSpacerMainBarHookLenB; }
	virtual long GetRWallUmaSpacerUserHookLenB() { return m_nRWallUmaSpacerUserHookLenB; }
	virtual long GetRWallUmaSpacerWidth() { return m_nRWallUmaSpacerWidth; }

	virtual BOOL GetRWallUShapeTopSpacer() { return m_bRWallUShapeTopSpacer; }
	virtual BOOL GetRWallUShapeBotSpacer() { return m_bRWallUShapeBotSpacer; }
	virtual long GetRWallUShapeSpacerDiaType() { return m_nRWallUShapeSpacerDiaType; }
	virtual long GetRWallUShapeSpacerDiaIndex() { return m_nRWallUShapeSpacerDiaIndex; }
	virtual BOOL GetRWallUShapeSpacerUserSteelGradeS() { return m_bRWallUShapeSpacerUserSteelGradeS; }
	virtual long GetRWallUShapeSpacerHorSpacingType() { return m_nRWallUShapeSpacerHorSpacingType; }
	virtual long GetRWallUShapeSpacerHorSpacing() { return m_nRWallUShapeSpacerHorSpacing; }
	virtual long GetRWallUShapeSpacerHorSpacingRate() { return m_nRWallUShapeSpacerHorSpacingRate; }
	virtual long GetRWallUShapeSpacerHorSpacingCount() { return m_nRWallUShapeSpacerHorSpacingCount; }
	virtual long GetRWallUShapeSpacerLenType() { return m_nRWallUShapeSpacerLenType; }
	virtual long GetRWallUShapeSpacerUserLen() { return m_nRWallUShapeSpacerUserLen; }

	virtual void SetRWallUmaSpacerDiaType(long value) { m_nRWallUmaSpacerDiaType = value; }
	virtual void SetRWallUmaSpacerWallThick(long value) { m_nRWallUmaSpacerWallThick = value; }
	virtual void SetRWallUmaSpacerWallThickOverDiaIndex(long value) { m_nRWallUmaSpacerWallThickOverDiaIndex = value; }
	virtual void SetRWallUmaSpacerWallThickUnderDiaIndex(long value) { m_nRWallUmaSpacerWallThickUnderDiaIndex = value; }
	virtual void SetRWallUmaSpacerUserSteelGradeS(BOOL value) { m_bRWallUmaSpacerUserSteelGradeS = value; }
	virtual void SetRWallUmaSpacerSpacingType(long value) { m_nRWallUmaSpacerSpacingType = value; }
	virtual void SetRWallUmaSpacerWallSpacing(long value) { m_nRWallUmaSpacerWallSpacing = value; }
	virtual void SetRWallUmaSpacerUserSpacing1(long value) { m_nRWallUmaSpacerUserSpacing1 = value; }
	virtual void SetRWallUmaSpacerUserSpacing2(long value) { m_nRWallUmaSpacerUserSpacing2 = value; }
	virtual void SetRWallUmaSpacerHeightType(long value) { m_nRWallUmaSpacerHeightType = value; }
	virtual void SetRWallUmaSpacerUserHeight(long value) { m_nRWallUmaSpacerUserHeight = value; }
	virtual void SetRWallUmaSpacerHookLenAType(long value) { m_nRWallUmaSpacerHookLenAType = value; }
	virtual void SetRWallUmaSpacerMainBarHookLenA(long value) { m_nRWallUmaSpacerMainBarHookLenA = value; }
	virtual void SetRWallUmaSpacerUserHookLenA(long value) { m_nRWallUmaSpacerUserHookLenA = value; }
	virtual void SetRWallUmaSpacerHookLenBType(long value) { m_nRWallUmaSpacerHookLenBType = value; }
	virtual void SetRWallUmaSpacerMainBarHookLenB(long value) { m_nRWallUmaSpacerMainBarHookLenB = value; }
	virtual void SetRWallUmaSpacerUserHookLenB(long value) { m_nRWallUmaSpacerUserHookLenB = value; }
	virtual void SetRWallUmaSpacerWidth(long value) { m_nRWallUmaSpacerWidth = value; }

	virtual void SetRWallUShapeTopSpacer(BOOL value) { m_bRWallUShapeTopSpacer = value; }
	virtual void SetRWallUShapeBotSpacer(BOOL value) { m_bRWallUShapeBotSpacer = value; }
	virtual void SetRWallUShapeSpacerDiaType(long value) { m_nRWallUShapeSpacerDiaType = value; }
	virtual void SetRWallUShapeSpacerDiaIndex(long value) { m_nRWallUShapeSpacerDiaIndex = value; }
	virtual void SetRWallUShapeSpacerUserSteelGradeS(BOOL value) { m_bRWallUShapeSpacerUserSteelGradeS = value; }
	virtual void SetRWallUShapeSpacerHorSpacingType(long value) { m_nRWallUShapeSpacerHorSpacingType = value; }
	virtual void SetRWallUShapeSpacerHorSpacing(long value) { m_nRWallUShapeSpacerHorSpacing = value; }
	virtual void SetRWallUShapeSpacerHorSpacingRate(long value) { m_nRWallUShapeSpacerHorSpacingRate = value; }
	virtual void SetRWallUShapeSpacerHorSpacingCount(long value) { m_nRWallUShapeSpacerHorSpacingCount = value; }
	virtual void SetRWallUShapeSpacerLenType(long value) { m_nRWallUShapeSpacerLenType = value; }
	virtual void SetRWallUShapeSpacerUserLen(long value) { m_nRWallUShapeSpacerUserLen = value; }

	virtual BOOL GetRWallDowelPlaceSpacer(double dDia);
	virtual long GetRWallDowelSpacerDiaIndex(double dDia);
	virtual long GetRWallDowelSpacerSpacing(double dDia);
	virtual BOOL GetRWallDowelSpacerUserSteelGradeS(double dDia);
	virtual long GetRWallDowelSpacerHeightType(double dDia);
	virtual long GetRWallDowelSpacerHeightSpliceAddLength(double dDia);
	virtual long GetRWallDowelSpacerUserHeight(double dDia);
	virtual long GetRWallDowelSpacerLengthA(double dDia);
	virtual long GetRWallDowelSpacerLengthB(double dDia);
	virtual long GetRWallDowelSpacerWidthType(double dDia);
	virtual long GetRWallDowelSpacerUserWidth(double dDia);

	virtual BOOL GetRWallOpeningAddBarDeductionCondition() { return m_bRWallOpeningAddBarDeductionCondition; }
	virtual void SetRWallOpeningAddBarDeductionCondition(BOOL value) { m_bRWallOpeningAddBarDeductionCondition = value; }

	virtual BOOL GetRWallBraceSupportBarDiaConditionHeight() { return m_bRWallBraceSupportBarDiaConditionHeight; }
	virtual void SetRWallBraceSupportBarDiaConditionHeight(BOOL value) { m_bRWallBraceSupportBarDiaConditionHeight = value; }
	virtual long GetRWallBraceSupportBarUnderDiaIndex() { return m_nRWallBraceSupportBarUnderDiaIndex; }
	virtual void SetRWallBraceSupportBarUnderDiaIndex(long value) { m_nRWallBraceSupportBarUnderDiaIndex = value; }

	virtual BOOL GetRWallSpacerVerStartSpacingSet() { return m_bRWallSpacerVerStartSpacingSet; }
	virtual void SetRWallSpacerVerStartSpacingSet(BOOL value) { m_bRWallSpacerVerStartSpacingSet = value; }
	virtual long GetRWallSpacerVerStartSpacing() { return m_nRWallSpacerVerStartSpacing; }
	virtual void SetRWallSpacerVerStartSpacing(long value) { m_nRWallSpacerVerStartSpacing = value; }

	virtual BOOL GetRWallSpliceAddLengthSet() { return m_bRWallSpliceAddLengthSet; }
	virtual void SetRWallSpliceAddLengthSet(BOOL value) { m_bRWallSpliceAddLengthSet = value; }
	virtual long GetRWallSpliceAddLengthNumSplice() { return m_nRWallSpliceAddLengthNumSplice; }
	virtual void SetRWallSpliceAddLengthNumSplice(long value) { m_nRWallSpliceAddLengthNumSplice = value; }

	virtual long GetRWallLCrossShapeType() { return m_nRWallLCrossShapeType; }

	virtual void SetRWallLCrossShapeType(long value) { m_nRWallLCrossShapeType = value; }

	virtual long GetRWallStraightCenterUmaPlaceType() { return m_nRWallStraightCenterUmaPlaceType; }
	virtual void SetRWallStraightCenterUmaPlaceType(long value) { m_nRWallStraightCenterUmaPlaceType = value; }

private:
	long m_nRWallVerBarPlaceTypeDiaIndex;
	long m_nRWallDowelPlaceTypeDiaIndex;
	RWallVerBarRebarTemplate *m_pVerBarTemplateBelowDia, *m_pVerBarTemplateExcessDia;
	RWallDowelRebarTemplate *m_pDowelTemplateBelowDia, *m_pDowelTemplateExcessDia;

	BOOL m_bRWallPlaceCrossBar;
	long m_nRWallCrossBarEAType;	//교차보강근 개수
	BOOL m_bRWallCrossBarEATypeExcessThickSet;
	long m_nRWallCrossBarEATypeExcessThick;
	long m_nRWallCrossBarStartSpacingType;	//교차보강근 주근 시작간격 0:S/2 1:S
	
	map<long, long> mm_RWallCrossBarDiaIndex;		//DiaIndex별 교차보강근 직경설정 값

	BOOL m_bRWallPlaceBraceSupportBar;
	long m_nRWallBraceSupportBarOverDiaIndex;
	BOOL m_bRWallBraceSupportBarDiaConditionHeight;
	long m_nRWallBraceSupportBarUnderDiaIndex;
	BOOL m_bRWallBraceSupportBarUserSteelGradeS;
	long m_nRWallBraceSupportBarWallLengthCondition;
	long m_nRWallBraceSupportBarWallLengthConditionOverLength;
	long m_nRWallBraceSupportBarWallLengthConditionUnderLength;
	BOOL m_bRWallBraceSupportNotPlaceConditionDiaSet;
	long m_nRWallBraceSupportNotPlaceConditionDiaIndex;
	BOOL m_bRWallBraceSupportNotPlaceConditionLengthSet;
	long m_nRWallBraceSupportNotPlaceConditionLength;
	BOOL m_bRWallBraceSupportNotPlaceConditionBottomDowel;

	BOOL m_bRWallBraceSupportNotPlaceConditionHeightSet;
	long m_nRWallBraceSupportNotPlaceConditionHeight;

	BOOL m_bRWallHorInfoSet;
	BOOL m_bRWallHorBarStartSet;    //시작간격 설정
	long m_nRWallHorBarStartLength; //시작간격
	long m_nRWallEndUbar;			//단부 배근형상 0:후크 1:U-BAR
	BOOL m_bRWallCrossEndUBarPlace;
	BOOL m_bRWallDefaultUbarLen;	//마구리U-BAR 발길이 설정
	BOOL m_bRWallCrossBarUBarHalfPlace;
	map<long, long> mm_RWallDefaultUbarLen;		//DiaIndex별 마구리U-BAR 발길이 설정 값

	BOOL m_bRWallSpacerInfoSet;
	long m_nRWallSpacerDiaType;				//직경설정	0:수평근 직경에 따름 1:직경
	long m_nRWallSpacerDiaIndex;			//직경Index
	BOOL m_bRWallSpacerUserSteelGradeS;
	long m_nRWallSpacerHorSpacing;			//수평간격
	long m_nRWallSpacerVerSpacingType;		//폭 고정근 수직간격 타입	0:간격 1:비율 2:개수
	long m_nRWallSpacerVerSpacing;			//수직간격
	long m_nRWallSpacerVerSpacingRate;		//수직비율
	long m_nRWallSpacerVerSpacingCount;		//수직갯수
	long m_nRWallDowelSpacerVerSpacingCount;		//다월 폭 고정근 수직갯수
	long m_nRWallSpacerLenType;				//발길이 타입	0:인장이음 1:후크
	long m_nRWallSpacerLenCalWayA;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nRWallSpacerLenCalWayB;			//발길이 계산 방법	0:표준 1:사용자 정의
	long m_nRWallSpacerLenA;				//사용자 정의 길이A
	long m_nRWallSpacerLenB;				//사용자 정의 길이B
	long m_nRWallSpacerShape;				//형상
	BOOL m_bRWallSpacerRebarPlaceSet;		//배근 조건 설정
	long m_nRWallSpacerRebarPlaceCondition;	//배근 조건

	BOOL m_bRWallSupportRebarsSet;					//수평 도리근 설정
	long m_nRWallSupportRebarsDiaIndex;				//직경
	BOOL m_bRWallSupportRebarsUserSteelGradeS;
	double m_dRWallSupportRebarsTotalLengthRate;	//벽 길이 비율
	long m_nRWallSupportRebarsOrderLength;			//주문길이
	long m_nRWallSupportRebarsSpliceLength;			//이음길이
	long m_nRWallSupportRebarsCorrectiveLength;		//보정길이

	BOOL m_bRWallAddBarsSet;						//보강근 설정
	long m_nRWallAddBarsDiaIndex;					//보강근 직경
	BOOL m_bRWallAddBarsUserSteelGradeS;
	long m_nRWallAddBarsNumber;						//보강근 개수
	long m_nRWallAddBarsLengthType;					//보강근 길이 타입
	long m_nRWallAddBarsTensionLengthType;			//보강근 인장길이 타입
	long m_nRWallAddBarsUserLength;			//보강근 사용자 길이

	BOOL m_bRWallUBarAddLengthSet;
	long m_nRWallUBarAddLength;
	BOOL m_bRWallSpacerAddLengthSet;
	long m_nRWallSpacerAddLength;

	BOOL m_bRWallVerInfoSet;
	
	BOOL m_bRWallDowelInfoSet;
	BOOL m_bRWallDowelSupportPlaceType;	//장대산출여부
		

	map<long, long> mm_RWallUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_RWallUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
	
	long m_nRWallShearBarShapeType;		//전단 보강근 형상 0:90*90	1:135*135	2:90*135

	map<long, long> mm_RWallDowelVerBarUserDiaIndex;

	BOOL m_bRWallDowelVerBarUserDiaSet;
	BOOL m_bRWallDowelVerBarUserSteelGradeS;

	VerMemberLinkDowelTemplate *m_pRWallVerMemberLinkDowelTemplate;

	BOOL m_bRWallStraightCenterUmaShape;
	long m_nRWallStraightCenterUmaPlaceType;
	BOOL m_bRWallUmaSpacerConditionSet;
	long m_nRWallUmaSpacerCondition;
	long m_nRWallUmaSpacerDiaType;		//직경 0:해당벽체직경 1:벽체 두께
	long m_nRWallUmaSpacerWallThick;		//벽체 두께
	long m_nRWallUmaSpacerWallThickOverDiaIndex;	//벽체 두께 이상 직경
	long m_nRWallUmaSpacerWallThickUnderDiaIndex;	//벽체 두께 미만 직경
	BOOL m_bRWallUmaSpacerUserSteelGradeS;
	long m_nRWallUmaSpacerSpacingType;	//간격 0:해당 벽체 간격 1:사용자
	long m_nRWallUmaSpacerWallSpacing;	//간격 : 해당 벽체 간격 입력
	long m_nRWallUmaSpacerUserSpacing1;	//간격 : 사용자1 입력
	long m_nRWallUmaSpacerUserSpacing2;	//간격 : 사용자2 입력
	long m_nRWallUmaSpacerHeightType;		//높이
	long m_nRWallUmaSpacerUserHeight;		//높이 : 사용자 입력
	long m_nRWallUmaSpacerHookLenAType;	//발길이(a) 0:주근간격 1:사용자지정
	long m_nRWallUmaSpacerMainBarHookLenA;//발길이(a) 주근간격
	long m_nRWallUmaSpacerUserHookLenA;	//발길이(a) 1:사용자지정
	long m_nRWallUmaSpacerHookLenBType;	//발길이(b) 0:주근간격 1:사용자지정
	long m_nRWallUmaSpacerMainBarHookLenB;//발길이(b) 주근간격
	long m_nRWallUmaSpacerUserHookLenB;	//발길이(b) 1:사용자지정
	long m_nRWallUmaSpacerWidth;			//폭

	BOOL m_bRWallUShapeTopSpacer;
	BOOL m_bRWallUShapeBotSpacer;
	long m_nRWallUShapeSpacerDiaType;				//직경 설정	0:수직근 직경에 따름 1:직경
	long m_nRWallUShapeSpacerDiaIndex;			//직경Index
	BOOL m_bRWallUShapeSpacerUserSteelGradeS;	
	long m_nRWallUShapeSpacerHorSpacingType;		//수평 간격 타입	0:간격 1:비율 2:개수
	long m_nRWallUShapeSpacerHorSpacing;			//수평 간격
	long m_nRWallUShapeSpacerHorSpacingRate;		//수평 비율
	long m_nRWallUShapeSpacerHorSpacingCount;	//수평 개수
	long m_nRWallUShapeSpacerLenType;				//0:인장이음 1:사용자지정	
	long m_nRWallUShapeSpacerUserLen;			//사용자지정

	BOOL m_bRWallOpeningAddBarDeductionCondition;

	BOOL m_bRWallSpacerVerStartSpacingSet;
	long m_nRWallSpacerVerStartSpacing;

	BOOL m_bRWallSpliceAddLengthSet;
	long m_nRWallSpliceAddLengthNumSplice;

	long m_nRWallLCrossShapeType;
};

