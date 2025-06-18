#pragma once

#include "..\MSObject.h"
class StructureModel_CLASS OldBaseRebarTemplate : public MSObject
{
public:
	OldBaseRebarTemplate();
	~OldBaseRebarTemplate(void);

	//Column
	//Get Property
	virtual BOOL GetColumnGeneralInfoSet() { return FALSE; }
	virtual BOOL GetColumnConnectLengthSet() { return FALSE; }
	virtual long GetColumnConnectLength() { return 0; }

	virtual BOOL GetColumnHoopBarInfoSet() { return FALSE; }
	virtual BOOL GetColumnHoopBarStartLengthSet() { return FALSE; }
	virtual long GetColumnHoopBarStartLength() { return 0; }
	virtual BOOL GetColumnTieAddLengthSet() { return FALSE; }
	virtual long GetColumnTieAddLength() { return 0; }
	virtual long GetColumnTopMemberCover() { return 0; }
	virtual long GetColumnHoopBarShapeIdx() { return 0; }
	virtual long GetColumnTieBarShapeIdx() { return 0; }
	virtual BOOL GetColumnEndHoopBarShapeSet() { return FALSE; }

	virtual BOOL GetColumnVerInfoSet() { return FALSE; }
	virtual long GetColumnVerBarSpliceType() { return 0; }
	virtual long GetColumnVerBarTopLenType() { return 0; }
	virtual BOOL GetColumnVerBarAlterSpliceSet() { return FALSE; }
	virtual long GetColumnVerBarAlterSpliceValue1() { return 0; }
	virtual long GetColumnVerBarAlterSpliceValue2() { return 0; }

	virtual BOOL GetColumnDowelInfoSet() { return FALSE; }
	virtual BOOL GetColumnDowelFootingCoverSet() { return FALSE; }
	virtual long GetColumnDowelFootingCover() { return 0; }
	virtual long GetColumnDowelBotPlaceType() { return 0; }
	virtual long GetColumnDowelTopPlaceType() { return 0; }
	virtual long GetColumnDowelHoopRebarType() { return 0; }
	virtual long GetColumnDowelHoopBySpace() { return 0; }
	virtual long GetColumnDowelHoopByNum() { return 0; }
	virtual long GetColumnDowelSupportBarDiaIndex() { return 0; }
	virtual long GetColumnDowelSupportBarLength() { return 0; }
	virtual long GetColumnDowelSupportBarNum() { return 0; }
	virtual long GetColumnDowelBarSpliceType() { return 0; }
	virtual BOOL GetColumnDowelBarAlterSpliceSet() { return FALSE; }
	virtual long GetColumnDowelBarAlterSpliceValue1() { return 0; }
	virtual long GetColumnDowelBarAlterSpliceValue2() { return 0; }
	virtual BOOL GetColumnDowelFootingPercentSet() { return FALSE; }
	virtual long GetColumnDowelFootingPercent() { return 0; }

	//Set Property
	virtual void SetColumnGeneralInfoSet(BOOL value) { }
	virtual void SetColumnConnectLengthSet(BOOL value) { }
	virtual void SetColumnConnectLength(long value) { }

	virtual void SetColumnHoopBarInfoSet(BOOL value) { }
	virtual void SetColumnHoopBarStartLengthSet(BOOL value) { }
	virtual void SetColumnHoopBarStartLength(long value) { }
	virtual void SetColumnTieAddLengthSet(BOOL value) { }
	virtual void SetColumnTieAddLength(long value) { }
	virtual void SetColumnTopMemberCover(long value) { }
	virtual void SetColumnHoopBarShapeIdx(long value) { }
	virtual void SetColumnTieBarShapeIdx(long value) { }
	virtual void SetColumnEndHoopBarShapeSet(BOOL value) { }

	virtual void SetColumnVerInfoSet(BOOL value) { }
	virtual void SetColumnVerBarSpliceType(long value) { }
	virtual void SetColumnVerBarTopLenType(long value) { }
	virtual void SetColumnVerBarAlterSpliceSet(BOOL value) { }
	virtual void SetColumnVerBarAlterSpliceValue1(long value) { }
	virtual void SetColumnVerBarAlterSpliceValue2(long value) { }

	virtual void SetColumnDowelInfoSet(BOOL value) { }
	virtual void SetColumnDowelFootingCoverSet(BOOL value) { }
	virtual void SetColumnDowelFootingCover(long value) { }
	virtual void SetColumnDowelBotPlaceType(long value) { }
	virtual void SetColumnDowelTopPlaceType(long value) { }
	virtual void SetColumnDowelHoopRebarType(long value) { }
	virtual void SetColumnDowelHoopBySpace(long value) { }
	virtual void SetColumnDowelHoopByNum(long value) { }
	virtual void SetColumnDowelSupportBarDiaIndex(long value) { }
	virtual void SetColumnDowelSupportBarLength(long value) { }
	virtual void SetColumnDowelSupportBarNum(long value) { }
	virtual void SetColumnDowelBarSpliceType(long value) { }
	virtual void SetColumnDowelBarAlterSpliceSet(BOOL value) { }
	virtual void SetColumnDowelBarAlterSpliceValue1(long value) { }
	virtual void SetColumnDowelBarAlterSpliceValue2(long value) { }
	virtual void SetColumnDowelFootingPercentSet(BOOL value) { }
	virtual void SetColumnDowelFootingPercent(long value) { }

	//Function
	virtual map<long, long> GetColumnUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual void SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }

	virtual long GetColumnUserDowelSpliceLen(double dDia) { return 0; }
	virtual void SetColumnUserDowelSpliceLen(long key, long value) { }

	//Beam
	//Get Property
	virtual BOOL GetBeamGeneralInfoSet() { return FALSE; }
	virtual long GetBeamTopMaxLen2HookAnchorBar() { return 0; }
	virtual long GetBeamBotMaxLen2HookAnchorBar() { return 0; }
	virtual long GetBeamTopSpliceType() { return 0; }
	virtual long GetBeamTopSpliceWay() { return 0; }

	virtual BOOL GetBeamStirrupInfoSet() { return FALSE; }
	virtual long GetBeamShearBarType() { return 0; }
	virtual long GetBeamCapbarRatio() { return 0; }
	virtual long GetBeamStirrupType() { return 0; }
	virtual long GetBeamCoverBarType() { return 0; }
	virtual BOOL GetBeamDeepDualUBARPlaceSet() { return FALSE; }
	virtual long GetBeamDeepDepth() { return 0; }
	virtual long GetBeamDeepSpliceType() { return 0; }
	virtual long GetBeamStartSpacingType() { return 0; }

	virtual BOOL GetBeamSubBeamInfoSet() { return FALSE; }
	virtual long GetSubBeamStirrupType() { return 0; }
	virtual long GetSubBeamCapbarRatio() { return 0; }
	virtual long GetSubBeamStirrupShape() { return 0; }
	virtual BOOL GetSubBeamOnlyCenterStirrupPlace() { return FALSE; }
	virtual long GetSubBeamStartSpacingType() { return 0; }

	virtual BOOL GetBeamSubBarInfoSet() { return FALSE; }
	virtual long GetBeamTopSubStirrupBarShape() { return 0; }
	virtual long GetBeamBotSubStirrupBarShape() { return 0; }
	virtual long GetBeamSubTopStirrupNum() { return 0; }
	virtual long GetBeamSubBotStirrupNum() { return 0; }
	virtual long GetBeamSubStirrupTopLen() { return 0; }
	virtual long GetBeamSubStirrupBotLen() { return 0; }
	virtual BOOL GetBeamSubStirrupUserHookLen() { return FALSE; }
	virtual long GetBeamSubStirrupBarType() { return 0; }

	virtual BOOL GetBuiltUpBeamInfoSet() { return FALSE; }
	virtual long GetBuiltUpBeamPlacerType() { return 0; }

	//Set Property
	virtual void SetBeamGeneralInfoSet(BOOL value) { }
	virtual void SetBeamTopMaxLen2HookAnchorBar(long value) { }
	virtual void SetBeamBotMaxLen2HookAnchorBar(long value) { }
	virtual void SetBeamTopSpliceType(long value) { }
	virtual void SetBeamTopSpliceWay(long value) { }

	virtual void SetBeamStirrupInfoSet(BOOL value) { }
	virtual void SetBeamShearBarType(long value) { }
	virtual void SetBeamCapbarRatio(long value) { }
	virtual void SetBeamStirrupType(long value) { }
	virtual void SetBeamCoverBarType(long value) { }
	virtual void SetBeamDeepDualUBARPlaceSet(BOOL value) { }
	virtual void SetBeamDeepDepth(long value) { }
	virtual void SetBeamDeepSpliceType(long value) { }
	virtual void SetBeamStartSpacingType(long value) { }

	virtual void SetBeamSubBeamInfoSet(BOOL value) { }
	virtual void SetSubBeamStirrupType(long value) { }
	virtual void SetSubBeamCapbarRatio(long value) { }
	virtual void SetSubBeamStirrupShape(long value) { }
	virtual void SetSubBeamOnlyCenterStirrupPlace(BOOL value) { }
	virtual void SetSubBeamStartSpacingType(long value) { }

	virtual void SetBeamSubBarInfoSet(BOOL value) { }
	virtual void SetBeamTopSubStirrupBarShape(long value) { }
	virtual void SetBeamBotSubStirrupBarShape(long value) { }
	virtual void SetBeamSubTopStirrupNum(long value) { }
	virtual void SetBeamSubBotStirrupNum(long value) { }
	virtual void SetBeamSubStirrupTopLen(long value) { }
	virtual void SetBeamSubStirrupBotLen(long value) { }
	virtual void SetBeamSubStirrupUserHookLen(BOOL value) { }
	virtual void SetBeamSubStirrupBarType(long value) { }
	
	virtual void SetBuiltUpBeamInfoSet(BOOL value) { }
	virtual void SetBuiltUpBeamPlacerType(long value) { }

	//Function
	virtual map<long, long> GetBeamSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamShearBarHookLenAMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamShearBarHookLenBMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamSubStirrupDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetBeamSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA) { }
	virtual void SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB) { }
	virtual void SetBeamSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }

	virtual double GetBeamSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetBeamShearBarHookLenA(double dDia) { return 0; }
	virtual double GetBeamShearBarHookLenB(double dDia) { return 0; }
	virtual double GetBeamSubStirrupDia(double dDia) { return 0; }

	virtual void SetBeamSubStirrupHookLen(long key, long value) { }
	virtual void SetBeamShearBarHookLenA(long key, long value) { }
	virtual void SetBeamShearBarHookLenB(long key, long value) { }
	virtual void SetBeamSubStirrupDiaIndex(long key, long value) { }

	//BWall
	//Get Property
	virtual BOOL GetBWallVerInfoSet() { return FALSE; }
	virtual BOOL GetBWallPlaceEndBarSet() { return FALSE; }
	virtual long GetBWallEndVerAddBarCount() { return 0; }
	virtual long GetBWallEndVerAddBarDist() { return 0; }
	virtual long GetBWallVerBarTopLenType() { return 0; }
	virtual long GetBWallCrossBarEAType() { return 0; }
	virtual long GetBWallVerBarSpliceWay() { return 0; }
	virtual BOOL GetBWallVerBarAlterSpliceSet() { return FALSE; }
	virtual long GetBWallVerBarAlterSpliceValue1() { return 0; }
	virtual long GetBWallVerBarAlterSpliceValue2() { return 0; }
	virtual long GetBWallVerNumBarCrossBarIncludeSet() { return 0; }

	virtual BOOL GetBWallHorInfoSet() { return FALSE; }
	virtual BOOL GetBWallPlaceUBar4ShortWall() { return FALSE; }
	virtual long GetBWallMaxUbarLegLength() { return 0; }
	virtual BOOL GetBWallPlaceStirrup4ShortWall() { return FALSE; }
	virtual long GetBWallHBarMaxStirrupLen() { return 0; }
	virtual BOOL GetBWallHorBarStartSet() { return FALSE; }
	virtual long GetBWallHorBarStartLength() { return 0; }
	virtual BOOL GetBWallStraightEndUBarPlaceSet() { return FALSE; }
	virtual BOOL GetBWallDefaultUbarLen() { return FALSE; }
	virtual BOOL GetBWallCrossBarUBarHalfPlace() { return FALSE; }

	virtual BOOL GetBWallSpacerInfoSet() { return FALSE; }
	virtual long GetBWallSpacerDiaType() { return 0; }
	virtual long GetBWallSpacerDiaIndex() { return 0; }
	virtual long GetBWallSpacerHorSpacing() { return 0; }
	virtual long GetBWallSpacerVerSpacingType() { return 0; }
	virtual long GetBWallSpacerVerSpacing() { return 0; }
	virtual long GetBWallSpacerVerSpacingRate() { return 0; }
	virtual long GetBWallSpacerVerSpacingCount() { return 0; }
	virtual long GetBWallDowelSpacerVerSpacingCount() { return 0; }
	virtual long GetBWallSpacerLenType() { return 0; }
	virtual long GetBWallSpacerLenCalWay() { return 0; }
	virtual long GetBWallSpacerLen1() { return 0; }
	virtual long GetBWallSpacerLen2() { return 0; }
	virtual long GetBWallSpacerShape() { return 0; }
	virtual BOOL GetBWallSpacerRebarPlaceSet() { return FALSE; }
	virtual long GetBWallSpacerRebarPlaceCondition() { return 0; }

	virtual BOOL GetBWallDowelInfoSet() { return FALSE; }
	virtual BOOL GetBWallUserDowelHookLen() { return FALSE; }
	virtual long GetBWallDowelBarAnchorType() { return 0; }
	virtual long GetBWallDowelBarTopLenType() { return 0; }
	virtual long GetBWallDowelSupportBarDiaIndex() { return 0; }
	virtual long GetBWallDowelSupportBarNum() { return 0; }
	virtual BOOL GetBWallDowelSupportPlaceType() { return FALSE; }
	virtual long GetBWallDowelBarSpliceWay() { return 0; }
	virtual BOOL GetBWallDowelBarAlterSpliceSet() { return FALSE; }
	virtual long GetBWallDowelBarAlterSpliceValue1() { return 0; }
	virtual long GetBWallDowelBarAlterSpliceValue2() { return 0; }
	virtual BOOL GetBWallDowelHookAnchorPlaceSpacingSet() { return FALSE; }
	virtual long GetBWallDowelHookAnchorPlaceSpacing() { return 0; }

	//Set Property
	virtual void SetBWallVerInfoSet(BOOL value) { }
	virtual void SetBWallPlaceEndBarSet(BOOL value) { }
	virtual void SetBWallEndVerAddBarCount(long value) { }
	virtual void SetBWallEndVerAddBarDist(long value) { }
	virtual void SetBWallVerBarTopLenType(long value) { }
	virtual void SetBWallCrossBarEAType(long value) { }
	virtual void SetBWallVerBarSpliceWay(long value) { }
	virtual void SetBWallVerBarAlterSpliceSet(BOOL value) { }
	virtual void SetBWallVerBarAlterSpliceValue1(long value) { }
	virtual void SetBWallVerBarAlterSpliceValue2(long value) { }
	virtual void SetBWallVerNumBarCrossBarIncludeSet(long value) { }

	virtual void SetBWallHorInfoSet(BOOL value) { }
	virtual void SetBWallPlaceUBar4ShortWall(BOOL value) { }
	virtual void SetBWallMaxUbarLegLength(long value) { }
	virtual void SetBWallPlaceStirrup4ShortWall(BOOL value) { }
	virtual void SetBWallHBarMaxStirrupLen(long value) { }
	virtual void SetBWallHorBarStartSet(BOOL value) { }
	virtual void SetBWallHorBarStartLength(long value) { }
	virtual void SetBWallStraightEndUBarPlaceSet(BOOL value) { }
	virtual void SetBWallDefaultUbarLen(BOOL value) { }
	virtual void SetBWallCrossBarUBarHalfPlace(BOOL value) { }

	virtual void SetBWallSpacerInfoSet(BOOL value) { }
	virtual void SetBWallSpacerDiaType(long value) { }
	virtual void SetBWallSpacerDiaIndex(long value) { }
	virtual void SetBWallSpacerHorSpacing(long value) { }
	virtual void SetBWallSpacerVerSpacingType(long value) { }
	virtual void SetBWallSpacerVerSpacing(long value) { }
	virtual void SetBWallSpacerVerSpacingRate(long value) { }
	virtual void SetBWallSpacerVerSpacingCount(long value) { }
	virtual void SetBWallDowelSpacerVerSpacingCount(long value) { }
	virtual void SetBWallSpacerLenType(long value) { }
	virtual void SetBWallSpacerLenCalWay(long value) { }
	virtual void SetBWallSpacerLen1(long value) { }
	virtual void SetBWallSpacerLen2(long value) { }
	virtual void SetBWallSpacerShape(long value) { }
	virtual void SetBWallSpacerRebarPlaceSet(BOOL value) { }
	virtual void SetBWallSpacerRebarPlaceCondition(long value) { }

	virtual void SetBWallDowelInfoSet(BOOL value) { }
	virtual void SetBWallUserDowelHookLen(BOOL value) { }
	virtual void SetBWallDowelBarAnchorType(long value) { }
	virtual void SetBWallDowelBarTopLenType(long value) { }
	virtual void SetBWallDowelSupportBarDiaIndex(long value) { }
	virtual void SetBWallDowelSupportBarNum(long value) { }
	virtual void SetBWallDowelSupportPlaceType(BOOL value) { }
	virtual void SetBWallDowelBarSpliceWay(long value) { }
	virtual void SetBWallDowelBarAlterSpliceSet(BOOL value) { }
	virtual void SetBWallDowelBarAlterSpliceValue1(long value) { }
	virtual void SetBWallDowelBarAlterSpliceValue2(long value) { }
	virtual void SetBWallDowelHookAnchorPlaceSpacingSet(BOOL value) { }
	virtual void SetBWallDowelHookAnchorPlaceSpacing(long value) { }

	//Function
	virtual map<long, long> GetBWallAddBarDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallEndBarDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallDefaultUbarLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallUserDowelSpliceLenMap() { map<long, long> value; return value; }

	virtual void SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex) { }
	virtual void SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex) { }
	virtual void SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen) { }
	virtual void SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }

	virtual double GetBWallAddBarDia(double dDia) { return 0; }
	virtual double GetBWallEndBarDia(double dDia) { return 0; }
	virtual double GetBWallDefaultUbarLen(double dDia) { return 0; }
	virtual double GetBWallUserDowelHookLen(double dDia) { return 0; }
	virtual double GetBWallUserDowelSpliceLen(double dDia) { return 0; }

	virtual void SetBWallAddBarDiaIndex(long key, long value) { }
	virtual void SetBWallEndBarDiaIndex(long key, long value) { }
	virtual void SetBWallDefaultUbarLen(long key, long value) { }
	virtual void SetBWallUserDowelHookLen(long key, long value) { }
	virtual void SetBWallUserDowelSpliceLen(long key, long value) { }

	//RWall
	//Get Property
	virtual BOOL GetRWallHorInfoSet() { return FALSE; }
	virtual BOOL GetRWallHorBarStartSet() { return FALSE; }
	virtual long GetRWallHorBarStartLength() { return 0; }
	virtual long GetRWallEndUbar() { return 0; }
	virtual BOOL GetRWallDefaultUbarLen() { return FALSE; }

	virtual BOOL GetRWallSpacerInfoSet() { return FALSE; }
	virtual long GetRWallSpacerDiaType() { return 0; }
	virtual long GetRWallSpacerDiaIndex() { return 0; }
	virtual long GetRWallSpacerHorSpacing() { return 0; }
	virtual long GetRWallSpacerVerSpacingType() { return 0; }
	virtual long GetRWallSpacerVerSpacing() { return 0; }
	virtual long GetRWallSpacerVerSpacingRate() { return 0; }
	virtual long GetRWallSpacerVerSpacingCount() { return 0; }
	virtual long GetRWallDowelSpacerVerSpacingCount() { return 0; }
	virtual long GetRWallSpacerLenType() { return 0; }
	virtual long GetRWallSpacerLenCalWay() { return 0; }
	virtual long GetRWallSpacerLen1() { return 0; }
	virtual long GetRWallSpacerLen2() { return 0; }
	virtual long GetRWallSpacerShape() { return 0; }
	virtual BOOL GetRWallSpacerRebarPlaceSet() { return FALSE; }
	virtual long GetRWallSpacerRebarPlaceCondition() { return 0; }

	virtual BOOL GetRWallVerInfoSet() { return FALSE; }
	virtual long GetRWallVerBarTopLenType() { return 0; }
	virtual long GetRWallVerBarSpliceWay() { return 0; }
	virtual BOOL GetRWallVerBarAlterSpliceSet() { return FALSE; }
	virtual long GetRWallVerBarAlterSpliceValue1() { return 0; }
	virtual long GetRWallVerBarAlterSpliceValue2() { return 0; }

	virtual BOOL GetRWallDowelInfoSet() { return FALSE; }
	virtual BOOL GetRWallUserDowelHookLen() { return FALSE; }
	virtual long GetRWallDowelBarAnchorType() { return 0; }
	virtual long GetRWallDowelBarTopLenType() { return 0; }
	virtual long GetRWallDowelSupportBarDiaIndex() { return 0; }
	virtual long GetRWallDowelSupportBarNum() { return 0; }
	virtual BOOL GetRWallDowelSupportPlaceType() { return FALSE; }
	virtual long GetRWallDowelBarSpliceWay() { return 0; }
	virtual BOOL GetRWallDowelBarAlterSpliceSet() { return FALSE; }
	virtual long GetRWallDowelBarAlterSpliceValue1() { return 0; }
	virtual long GetRWallDowelBarAlterSpliceValue2() { return 0; }
	virtual BOOL GetRWallDowelHookAnchorPlaceSpacingSet() { return FALSE; }
	virtual long GetRWallDowelHookAnchorPlaceSpacing() { return 0; }
	virtual BOOL GetRWallDowelBotAddBarIntegratedPlaceSet() { return FALSE; }

	//Set Property
	virtual void SetRWallHorInfoSet(BOOL value) { }
	virtual void SetRWallHorBarStartSet(BOOL value) { }
	virtual void SetRWallHorBarStartLength(long value) { }
	virtual void SetRWallEndUbar(long value) { }
	virtual void SetRWallDefaultUbarLen(BOOL value) { }

	virtual void SetRWallSpacerInfoSet(BOOL value) { }
	virtual void SetRWallSpacerDiaType(long value) { }
	virtual void SetRWallSpacerDiaIndex(long value) { }
	virtual void SetRWallSpacerHorSpacing(long value) { }
	virtual void SetRWallSpacerVerSpacingType(long value) { }
	virtual void SetRWallSpacerVerSpacing(long value) { }
	virtual void SetRWallSpacerVerSpacingRate(long value) { }
	virtual void SetRWallSpacerVerSpacingCount(long value) { }
	virtual void SetRWallDowelSpacerVerSpacingCount(long value) { }
	virtual void SetRWallSpacerLenType(long value) { }
	virtual void SetRWallSpacerLenCalWay(long value) { }
	virtual void SetRWallSpacerLen1(long value) { }
	virtual void SetRWallSpacerLen2(long value) { }
	virtual void SetRWallSpacerShape(long value) { }
	virtual void SetRWallSpacerRebarPlaceSet(BOOL value) { }
	virtual void SetRWallSpacerRebarPlaceCondition(long value) { }

	virtual void SetRWallVerInfoSet(BOOL value) { }
	virtual void SetRWallVerBarTopLenType(long value) { }
	virtual void SetRWallVerBarSpliceWay(long value) { }
	virtual void SetRWallVerBarAlterSpliceSet(BOOL value) { }
	virtual void SetRWallVerBarAlterSpliceValue1(long value) { }
	virtual void SetRWallVerBarAlterSpliceValue2(long value) { }

	virtual void SetRWallDowelInfoSet(BOOL value) { }
	virtual void SetRWallUserDowelHookLen(BOOL value) { }
	virtual void SetRWallDowelBarAnchorType(long value) { }
	virtual void SetRWallDowelBarTopLenType(long value) { }
	virtual void SetRWallDowelSupportBarDiaIndex(long value) { }
	virtual void SetRWallDowelSupportBarNum(long value) { }
	virtual void SetRWallDowelSupportPlaceType(BOOL value) { }
	virtual void SetRWallDowelBarSpliceWay(long value) { }
	virtual void SetRWallDowelBarAlterSpliceSet(BOOL value) { }
	virtual void SetRWallDowelBarAlterSpliceValue1(long value) { }
	virtual void SetRWallDowelBarAlterSpliceValue2(long value) { }
	virtual void SetRWallDowelHookAnchorPlaceSpacingSet(BOOL value) { }
	virtual void SetRWallDowelHookAnchorPlaceSpacing(long value) { }
	virtual void SetRWallDowelBotAddBarIntegratedPlaceSet(BOOL value) { }

	//Function
	virtual map<long, long> GetRWallDefaultUbarLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetRWallUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetRWallUserDowelSpliceLenMap() { map<long, long> value; return value; }

	virtual void SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen) { }
	virtual void SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }

	virtual double GetRWallUserDowelHookLen(double dDia) { return 0; }
	virtual double GetRWallUserDowelSpliceLen(double dDia) { return 0; }

	virtual void SetRWallDefaultUbarLen(long key, long value) { }
	virtual void SetRWallUserDowelHookLen(long key, long value) { }
	virtual void SetRWallUserDowelSpliceLen(long key, long value) { }

	//Pit
	virtual BOOL GetPitGeneralInfoSet() { return FALSE; }
	virtual long GetPitMaxLen2HookAnchorBar() { return 0; }

	virtual BOOL GetPitVerInfoSet() { return FALSE; }
	virtual long GetPitVerOutTopHookType() { return 0; }
	virtual long GetPitVerOutBotHookType() { return 0; }
	virtual long GetPitVerInTopHookType() { return 0; }
	virtual long GetPitVerInBotHookType() { return 0; }

	virtual BOOL GetPitHorInfoSet() { return FALSE; }
	virtual long GetPitHorOutHookType() { return 0; }
	virtual long GetPitHorInHookType() { return 0; }

	virtual BOOL GetPitTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetPitTopSpacerSet() { return FALSE; }
	virtual long GetPitTopSpacerDiaType() { return 0; }
	virtual long GetPitTopSpacerDiaIndex() { return 0; }
	virtual long GetPitTopSpacerSpacingType() { return 0; }
	virtual long GetPitTopSpacerSlabSpacing() { return 0; }
	virtual long GetPitTopSpacerUserSpacing1() { return 0; }
	virtual long GetPitTopSpacerUserSpacing2() { return 0; }
	virtual long GetPitTopSpacerHeightType() { return 0; }
	virtual long GetPitTopSpacerUserHeight() { return 0; }
	virtual long GetPitTopSpacerHookLenAType() { return 0; }
	virtual long GetPitTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetPitTopSpacerUserHookLenA() { return 0; }
	virtual long GetPitTopSpacerHookLenBType() { return 0; }
	virtual long GetPitTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetPitTopSpacerUserHookLenB() { return 0; }
	virtual long GetPitTopSpacerWidth() { return 0; }

	virtual BOOL GetPitBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetPitBotSpacerSet() { return FALSE; }
	virtual long GetPitBotSpacerDiaType() { return 0; }
	virtual long GetPitBotSpacerDiaIndex() { return 0; }
	virtual long GetPitBotSpacerSpacingType() { return 0; }
	virtual long GetPitBotSpacerSlabSpacing() { return 0; }
	virtual long GetPitBotSpacerUserSpacing1() { return 0; }
	virtual long GetPitBotSpacerUserSpacing2() { return 0; }
	virtual long GetPitBotSpacerHeightType() { return 0; }
	virtual long GetPitBotSpacerUserHeight() { return 0; }
	virtual long GetPitBotSpacerHookLenAType() { return 0; }
	virtual long GetPitBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetPitBotSpacerUserHookLenA() { return 0; }
	virtual long GetPitBotSpacerHookLenBType() { return 0; }
	virtual long GetPitBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetPitBotSpacerUserHookLenB() { return 0; }
	virtual long GetPitBotSpacerWidth() { return 0; }

	virtual void SetPitGeneralInfoSet(BOOL value) { }
	virtual void SetPitMaxLen2HookAnchorBar(long value) { }

	virtual void SetPitVerInfoSet(BOOL value) { }
	virtual void SetPitVerOutTopHookType(long value) { }
	virtual void SetPitVerOutBotHookType(long value) { }
	virtual void SetPitVerInTopHookType(long value) { }
	virtual void SetPitVerInBotHookType(long value) { }

	virtual void SetPitHorInfoSet(BOOL value) { }
	virtual void SetPitHorOutHookType(long value) { }
	virtual void SetPitHorInHookType(long value) { }

	virtual void SetPitTopSpacerInfoSet(BOOL value) { }
	virtual void SetPitTopSpacerSet(BOOL value) { }
	virtual void SetPitTopSpacerDiaType(long value) { }
	virtual void SetPitTopSpacerDiaIndex(long value) { }
	virtual void SetPitTopSpacerSpacingType(long value) { }
	virtual void SetPitTopSpacerSlabSpacing(long value) { }
	virtual void SetPitTopSpacerUserSpacing1(long value) { }
	virtual void SetPitTopSpacerUserSpacing2(long value) { }
	virtual void SetPitTopSpacerHeightType(long value) { }
	virtual void SetPitTopSpacerUserHeight(long value) { }
	virtual void SetPitTopSpacerHookLenAType(long value) { }
	virtual void SetPitTopSpacerMainBarHookLenA(long value) { }
	virtual void SetPitTopSpacerUserHookLenA(long value) { }
	virtual void SetPitTopSpacerHookLenBType(long value) { }
	virtual void SetPitTopSpacerMainBarHookLenB(long value) { }
	virtual void SetPitTopSpacerUserHookLenB(long value) { }
	virtual void SetPitTopSpacerWidth(long value) { }

	virtual void SetPitBotSpacerInfoSet(BOOL value) { }
	virtual void SetPitBotSpacerSet(BOOL value) { }
	virtual void SetPitBotSpacerDiaType(long value) { }
	virtual void SetPitBotSpacerDiaIndex(long value) { }
	virtual void SetPitBotSpacerSpacingType(long value) { }
	virtual void SetPitBotSpacerSlabSpacing(long value) { }
	virtual void SetPitBotSpacerUserSpacing1(long value) { }
	virtual void SetPitBotSpacerUserSpacing2(long value) { }
	virtual void SetPitBotSpacerHeightType(long value) { }
	virtual void SetPitBotSpacerUserHeight(long value) { }
	virtual void SetPitBotSpacerHookLenAType(long value) { }
	virtual void SetPitBotSpacerMainBarHookLenA(long value) { }
	virtual void SetPitBotSpacerUserHookLenA(long value) { }
	virtual void SetPitBotSpacerHookLenBType(long value) { }
	virtual void SetPitBotSpacerMainBarHookLenB(long value) { }
	virtual void SetPitBotSpacerUserHookLenB(long value) { }
	virtual void SetPitBotSpacerWidth(long value) { }

	// MAT Slab
	virtual BOOL GetMatLongMainBarInfoSet() { return FALSE; }
	virtual long GetMatLongMaxDualHookLength() { return 0; }
	virtual long GetMatLongSpliceWay() { return 0; }
	virtual long GetMatLongSpliceType() { return 0; }
	virtual BOOL GetMatLongAlterSpliceSet() { return FALSE; }
	virtual long GetMatLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetMatShortMainBarInfoSet() { return FALSE; }
	virtual long GetMatShortMaxDualHookLength() { return 0; }
	virtual long GetMatShortSpliceWay() { return 0; }
	virtual long GetMatShortSpliceType() { return 0; }
	virtual BOOL GetMatShortAlterSpliceSet() { return FALSE; }
	virtual long GetMatShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetMatEndInfoSet() { return FALSE; }
	virtual long GetMatTopEndHookType() { return 0; }
	virtual long GetMatBotEndHookType() { return 0; }
	virtual long GetMatIsoFootDevType() { return 0; }

	virtual BOOL GetMatLevelDifInfoSet() { return FALSE; }
	virtual long GetMatLevelDifPlaceShape() { return 0; }
	virtual long GetMatLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetMatRebarAreaInfoSet() { return FALSE; }
	virtual long GetMatRebarTolDist() { return 0; }
	virtual long GetMatRebarTolLenDif() { return 0; }

	virtual BOOL GetMatTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetMatTopSpacerSet() { return FALSE; }
	virtual long GetMatTopSpacerDiaType() { return 0; }
	virtual long GetMatTopSpacerDiaIndex() { return 0; }
	virtual long GetMatTopSpacerSpacingType() { return 0; }
	virtual long GetMatTopSpacerSlabSpacing() { return 0; }
	virtual long GetMatTopSpacerUserSpacing1() { return 0; }
	virtual long GetMatTopSpacerUserSpacing2() { return 0; }
	virtual long GetMatTopSpacerHeightType() { return 0; }
	virtual long GetMatTopSpacerUserHeight() { return 0; }
	virtual long GetMatTopSpacerHookLenAType() { return 0; }
	virtual long GetMatTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetMatTopSpacerUserHookLenA() { return 0; }
	virtual long GetMatTopSpacerHookLenBType() { return 0; }
	virtual long GetMatTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetMatTopSpacerUserHookLenB() { return 0; }
	virtual long GetMatTopSpacerWidth() { return 0; }

	virtual BOOL GetMatBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetMatBotSpacerSet() { return FALSE; }
	virtual long GetMatBotSpacerDiaType() { return 0; }
	virtual long GetMatBotSpacerDiaIndex() { return 0; }
	virtual long GetMatBotSpacerSpacingType() { return 0; }
	virtual long GetMatBotSpacerSlabSpacing() { return 0; }
	virtual long GetMatBotSpacerUserSpacing1() { return 0; }
	virtual long GetMatBotSpacerUserSpacing2() { return 0; }
	virtual long GetMatBotSpacerHeightType() { return 0; }
	virtual long GetMatBotSpacerUserHeight() { return 0; }
	virtual long GetMatBotSpacerHookLenAType() { return 0; }
	virtual long GetMatBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetMatBotSpacerUserHookLenA() { return 0; }
	virtual long GetMatBotSpacerHookLenBType() { return 0; }
	virtual long GetMatBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetMatBotSpacerUserHookLenB() { return 0; }
	virtual long GetMatBotSpacerWidth() { return 0; }

	virtual void SetMatLongMainBarInfoSet(BOOL value) { }
	virtual void SetMatLongMaxDualHookLength(long value) { }
	virtual void SetMatLongSpliceWay(long value) { }
	virtual void SetMatLongSpliceType(long value) { }
	virtual void SetMatLongAlterSpliceSet(BOOL value) { }
	virtual void SetMatLongAlterSpliceValue1(long value) { }

	virtual void SetMatShortMainBarInfoSet(BOOL value) { }
	virtual void SetMatShortMaxDualHookLength(long value) { }
	virtual void SetMatShortSpliceWay(long value) { }
	virtual void SetMatShortSpliceType(long value) { }
	virtual void SetMatShortAlterSpliceSet(BOOL value) { }
	virtual void SetMatShortAlterSpliceValue1(long value) { }

	virtual void SetMatEndInfoSet(BOOL value) { }
	virtual void SetMatTopEndHookType(long value) { }
	virtual void SetMatBotEndHookType(long value) { }
	virtual void SetMatIsoFootDevType(long value) { }

	virtual void SetMatLevelDifInfoSet(BOOL value) { }
	virtual void SetMatLevelDifPlaceShape(long value) { }
	virtual void SetMatLevelDifHaunchPlaceType(long value) { }

	virtual void SetMatRebarAreaInfoSet(BOOL value) { }
	virtual void SetMatRebarTolDist(long value) { }
	virtual void SetMatRebarTolLenDif(long value) { }

	virtual void SetMatTopSpacerInfoSet(BOOL value) { }
	virtual void SetMatTopSpacerSet(BOOL value) { }
	virtual void SetMatTopSpacerDiaType(long value) { }
	virtual void SetMatTopSpacerDiaIndex(long value) { }
	virtual void SetMatTopSpacerSpacingType(long value) { }
	virtual void SetMatTopSpacerSlabSpacing(long value) { }
	virtual void SetMatTopSpacerUserSpacing1(long value) { }
	virtual void SetMatTopSpacerUserSpacing2(long value) { }
	virtual void SetMatTopSpacerHeightType(long value) { }
	virtual void SetMatTopSpacerUserHeight(long value) { }
	virtual void SetMatTopSpacerHookLenAType(long value) { }
	virtual void SetMatTopSpacerMainBarHookLenA(long value) { }
	virtual void SetMatTopSpacerUserHookLenA(long value) { }
	virtual void SetMatTopSpacerHookLenBType(long value) { }
	virtual void SetMatTopSpacerMainBarHookLenB(long value) { }
	virtual void SetMatTopSpacerUserHookLenB(long value) { }
	virtual void SetMatTopSpacerWidth(long value) { }

	virtual void SetMatBotSpacerInfoSet(BOOL value) { }
	virtual void SetMatBotSpacerSet(BOOL value) { }
	virtual void SetMatBotSpacerDiaType(long value) { }
	virtual void SetMatBotSpacerDiaIndex(long value) { }
	virtual void SetMatBotSpacerSpacingType(long value) { }
	virtual void SetMatBotSpacerSlabSpacing(long value) { }
	virtual void SetMatBotSpacerUserSpacing1(long value) { }
	virtual void SetMatBotSpacerUserSpacing2(long value) { }
	virtual void SetMatBotSpacerHeightType(long value) { }
	virtual void SetMatBotSpacerUserHeight(long value) { }
	virtual void SetMatBotSpacerHookLenAType(long value) { }
	virtual void SetMatBotSpacerMainBarHookLenA(long value) { }
	virtual void SetMatBotSpacerUserHookLenA(long value) { }
	virtual void SetMatBotSpacerHookLenBType(long value) { }
	virtual void SetMatBotSpacerMainBarHookLenB(long value) { }
	virtual void SetMatBotSpacerUserHookLenB(long value) { }
	virtual void SetMatBotSpacerWidth(long value) { }

	// FLAT Slab
	virtual BOOL GetFlatLongMainBarInfoSet() { return FALSE; }
	virtual long GetFlatLongMaxDualHookLength() { return 0; }
	virtual long GetFlatLongSpliceWay() { return 0; }
	virtual long GetFlatLongSpliceType() { return 0; }
	virtual BOOL GetFlatLongAlterSpliceSet() { return FALSE; }
	virtual long GetFlatLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetFlatShortMainBarInfoSet() { return FALSE; }
	virtual long GetFlatShortMaxDualHookLength() { return 0; }
	virtual long GetFlatShortSpliceWay() { return 0; }
	virtual long GetFlatShortSpliceType() { return 0; }
	virtual BOOL GetFlatShortAlterSpliceSet() { return FALSE; }
	virtual long GetFlatShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetFlatEndInfoSet() { return FALSE; }
	virtual long GetFlatTopEndHookType() { return 0; }
	virtual long GetFlatBotEndHookType() { return 0; }
	virtual long GetFlatIsoFootDevType() { return 0; }
	virtual long GetFlatSpliceDevSlabType() { return 0; }

	virtual BOOL GetFlatLevelDifInfoSet() { return FALSE; }
	virtual long GetFlatLevelDifPlaceShape() { return 0; }
	virtual long GetFlatLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetFlatOpeningInfoSet() { return FALSE; }
	virtual long GetFlatOpeningTopHookType() { return 0; }
	virtual long GetFlatOpeningBotHookType() { return 0; }

	virtual BOOL GetFlatRebarAreaInfoSet() { return FALSE; }
	virtual long GetFlatRebarTolDist() { return 0; }
	virtual long GetFlatRebarTolLenDif() { return 0; }

	virtual BOOL GetFlatTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetFlatTopSpacerSet() { return FALSE; }
	virtual long GetFlatTopSpacerDiaType() { return 0; }
	virtual long GetFlatTopSpacerDiaIndex() { return 0; }
	virtual long GetFlatTopSpacerSpacingType() { return 0; }
	virtual long GetFlatTopSpacerSlabSpacing() { return 0; }
	virtual long GetFlatTopSpacerUserSpacing1() { return 0; }
	virtual long GetFlatTopSpacerUserSpacing2() { return 0; }
	virtual long GetFlatTopSpacerHeightType() { return 0; }
	virtual long GetFlatTopSpacerUserHeight() { return 0; }
	virtual long GetFlatTopSpacerHookLenAType() { return 0; }
	virtual long GetFlatTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetFlatTopSpacerUserHookLenA() { return 0; }
	virtual long GetFlatTopSpacerHookLenBType() { return 0; }
	virtual long GetFlatTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetFlatTopSpacerUserHookLenB() { return 0; }
	virtual long GetFlatTopSpacerWidth() { return 0; }

	virtual BOOL GetFlatBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetFlatBotSpacerSet() { return FALSE; }
	virtual long GetFlatBotSpacerDiaType() { return 0; }
	virtual long GetFlatBotSpacerDiaIndex() { return 0; }
	virtual long GetFlatBotSpacerSpacingType() { return 0; }
	virtual long GetFlatBotSpacerSlabSpacing() { return 0; }
	virtual long GetFlatBotSpacerUserSpacing1() { return 0; }
	virtual long GetFlatBotSpacerUserSpacing2() { return 0; }
	virtual long GetFlatBotSpacerHeightType() { return 0; }
	virtual long GetFlatBotSpacerUserHeight() { return 0; }
	virtual long GetFlatBotSpacerHookLenAType() { return 0; }
	virtual long GetFlatBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetFlatBotSpacerUserHookLenA() { return 0; }
	virtual long GetFlatBotSpacerHookLenBType() { return 0; }
	virtual long GetFlatBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetFlatBotSpacerUserHookLenB() { return 0; }
	virtual long GetFlatBotSpacerWidth() { return 0; }

	virtual void SetFlatLongMainBarInfoSet(BOOL value) { }
	virtual void SetFlatLongMaxDualHookLength(long value) { }
	virtual void SetFlatLongSpliceWay(long value) { }
	virtual void SetFlatLongSpliceType(long value) { }
	virtual void SetFlatLongAlterSpliceSet(BOOL value) { }
	virtual void SetFlatLongAlterSpliceValue1(long value) { }

	virtual void SetFlatShortMainBarInfoSet(BOOL value) { }
	virtual void SetFlatShortMaxDualHookLength(long value) { }
	virtual void SetFlatShortSpliceWay(long value) { }
	virtual void SetFlatShortSpliceType(long value) { }
	virtual void SetFlatShortAlterSpliceSet(BOOL value) { }
	virtual void SetFlatShortAlterSpliceValue1(long value) { }

	virtual void SetFlatEndInfoSet(BOOL value) { }
	virtual void SetFlatTopEndHookType(long value) { }
	virtual void SetFlatBotEndHookType(long value) { }
	virtual void SetFlatIsoFootDevType(long value) { }
	virtual void SetFlatSpliceDevSlabType(long value) { }

	virtual void SetFlatLevelDifInfoSet(BOOL value) { }
	virtual void SetFlatLevelDifPlaceShape(long value) { }
	virtual void SetFlatLevelDifHaunchPlaceType(long value) { }

	virtual void SetFlatOpeningInfoSet(BOOL value) { }
	virtual void SetFlatOpeningTopHookType(long value) { }
	virtual void SetFlatOpeningBotHookType(long value) { }

	virtual void SetFlatRebarAreaInfoSet(BOOL value) { }
	virtual void SetFlatRebarTolDist(long value) { }
	virtual void SetFlatRebarTolLenDif(long value) { }

	virtual void SetFlatTopSpacerInfoSet(BOOL value) { }
	virtual void SetFlatTopSpacerSet(BOOL value) { }
	virtual void SetFlatTopSpacerDiaType(long value) { }
	virtual void SetFlatTopSpacerDiaIndex(long value) { }
	virtual void SetFlatTopSpacerSpacingType(long value) { }
	virtual void SetFlatTopSpacerSlabSpacing(long value) { }
	virtual void SetFlatTopSpacerUserSpacing1(long value) { }
	virtual void SetFlatTopSpacerUserSpacing2(long value) { }
	virtual void SetFlatTopSpacerHeightType(long value) { }
	virtual void SetFlatTopSpacerUserHeight(long value) { }
	virtual void SetFlatTopSpacerHookLenAType(long value) { }
	virtual void SetFlatTopSpacerMainBarHookLenA(long value) { }
	virtual void SetFlatTopSpacerUserHookLenA(long value) { }
	virtual void SetFlatTopSpacerHookLenBType(long value) { }
	virtual void SetFlatTopSpacerMainBarHookLenB(long value) { }
	virtual void SetFlatTopSpacerUserHookLenB(long value) { }
	virtual void SetFlatTopSpacerWidth(long value) { }

	virtual void SetFlatBotSpacerInfoSet(BOOL value) { }
	virtual void SetFlatBotSpacerSet(BOOL value) { }
	virtual void SetFlatBotSpacerDiaType(long value) { }
	virtual void SetFlatBotSpacerDiaIndex(long value) { }
	virtual void SetFlatBotSpacerSpacingType(long value) { }
	virtual void SetFlatBotSpacerSlabSpacing(long value) { }
	virtual void SetFlatBotSpacerUserSpacing1(long value) { }
	virtual void SetFlatBotSpacerUserSpacing2(long value) { }
	virtual void SetFlatBotSpacerHeightType(long value) { }
	virtual void SetFlatBotSpacerUserHeight(long value) { }
	virtual void SetFlatBotSpacerHookLenAType(long value) { }
	virtual void SetFlatBotSpacerMainBarHookLenA(long value) { }
	virtual void SetFlatBotSpacerUserHookLenA(long value) { }
	virtual void SetFlatBotSpacerHookLenBType(long value) { }
	virtual void SetFlatBotSpacerMainBarHookLenB(long value) { }
	virtual void SetFlatBotSpacerUserHookLenB(long value) { }
	virtual void SetFlatBotSpacerWidth(long value) { }

	// 사변지지 SLAB
	virtual BOOL GetSlabLongMainBarInfoSet() { return FALSE; }
	virtual long GetSlabLongMaxDualHookLength() { return 0; }
	virtual long GetSlabLongSpliceWay() { return 0; }
	virtual long GetSlabLongSpliceType() { return 0; }
	virtual BOOL GetSlabLongAlterSpliceSet() { return FALSE; }
	virtual long GetSlabLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetSlabShortMainBarInfoSet() { return FALSE; }
	virtual long GetSlabShortMaxDualHookLength() { return 0; }
	virtual long GetSlabShortSpliceWay() { return 0; }
	virtual long GetSlabShortSpliceType() { return 0; }
	virtual BOOL GetSlabShortAlterSpliceSet() { return FALSE; }
	virtual long GetSlabShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetSlabStructuredEndInfoSet() { return FALSE; }
	virtual long GetSlabTopStructuredEndHookType() { return 0; }
	virtual long GetSlabBotStructuredEndHookType() { return 0; }

	virtual BOOL GetSlabTopCutBarLenInfoSet() { return FALSE; }
	virtual long GetSlabOneWayCutBarLenType() { return 0; }
	virtual long GetSlabTwoWayCutBarLenType() { return 0; }

	virtual BOOL GetSlabUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetSlabTopUnStructuredEndHookType() { return 0; }
	virtual long GetSlabBotUnStructuredEndHookType() { return 0; }
	virtual long GetSlabIsoFootUnStructuredDevType() { return 0; }

	virtual BOOL GetSlabLevelDifInfoSet() { return FALSE; }
	virtual long GetSlabLevelDifPlaceShape() { return 0; }
	virtual long GetSlabLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetSlabOpeningInfoSet() { return FALSE; }
	virtual long GetSlabOpeningTopHookType() { return 0; }
	virtual long GetSlabOpeningBotHookType() { return 0; }

	virtual BOOL GetSlabRebarAreaInfoSet() { return FALSE; }
	virtual long GetSlabRebarTolDist() { return 0; }
	virtual long GetSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetSlabTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSlabTopSpacerSet() { return FALSE; }
	virtual long GetSlabTopSpacerDiaType() { return 0; }
	virtual long GetSlabTopSpacerDiaIndex() { return 0; }
	virtual long GetSlabTopSpacerSpacingType() { return 0; }
	virtual long GetSlabTopSpacerSlabSpacing() { return 0; }
	virtual long GetSlabTopSpacerUserSpacing1() { return 0; }
	virtual long GetSlabTopSpacerUserSpacing2() { return 0; }
	virtual long GetSlabTopSpacerHeightType() { return 0; }
	virtual long GetSlabTopSpacerUserHeight() { return 0; }
	virtual long GetSlabTopSpacerHookLenAType() { return 0; }
	virtual long GetSlabTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetSlabTopSpacerUserHookLenA() { return 0; }
	virtual long GetSlabTopSpacerHookLenBType() { return 0; }
	virtual long GetSlabTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetSlabTopSpacerUserHookLenB() { return 0; }
	virtual long GetSlabTopSpacerWidth() { return 0; }

	virtual BOOL GetSlabBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSlabBotSpacerSet() { return FALSE; }
	virtual long GetSlabBotSpacerDiaType() { return 0; }
	virtual long GetSlabBotSpacerDiaIndex() { return 0; }
	virtual long GetSlabBotSpacerSpacingType() { return 0; }
	virtual long GetSlabBotSpacerSlabSpacing() { return 0; }
	virtual long GetSlabBotSpacerUserSpacing1() { return 0; }
	virtual long GetSlabBotSpacerUserSpacing2() { return 0; }
	virtual long GetSlabBotSpacerHeightType() { return 0; }
	virtual long GetSlabBotSpacerUserHeight() { return 0; }
	virtual long GetSlabBotSpacerHookLenAType() { return 0; }
	virtual long GetSlabBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetSlabBotSpacerUserHookLenA() { return 0; }
	virtual long GetSlabBotSpacerHookLenBType() { return 0; }
	virtual long GetSlabBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetSlabBotSpacerUserHookLenB() { return 0; }
	virtual long GetSlabBotSpacerWidth() { return 0; }

	virtual void SetSlabLongMainBarInfoSet(BOOL value) { }
	virtual void SetSlabLongMaxDualHookLength(long value) { }
	virtual void SetSlabLongSpliceWay(long value) { }
	virtual void SetSlabLongSpliceType(long value) { }
	virtual void SetSlabLongAlterSpliceSet(BOOL value) { }
	virtual void SetSlabLongAlterSpliceValue1(long value) { }

	virtual void SetSlabShortMainBarInfoSet(BOOL value) { }
	virtual void SetSlabShortMaxDualHookLength(long value) { }
	virtual void SetSlabShortSpliceWay(long value) { }
	virtual void SetSlabShortSpliceType(long value) { }
	virtual void SetSlabShortAlterSpliceSet(BOOL value) { }
	virtual void SetSlabShortAlterSpliceValue1(long value) { }
	
	virtual void SetSlabStructuredEndInfoSet(BOOL value) { }
	virtual void SetSlabTopStructuredEndHookType(long value) { }
	virtual void SetSlabBotStructuredEndHookType(long value) { }
	
	virtual void SetSlabTopCutBarLenInfoSet(BOOL value) { }
	virtual void SetSlabOneWayCutBarLenType(long value) { }
	virtual void SetSlabTwoWayCutBarLenType(long value) { }

	virtual void SetSlabUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetSlabTopUnStructuredEndHookType(long value) { }
	virtual void SetSlabBotUnStructuredEndHookType(long value) { }
	virtual void SetSlabIsoFootUnStructuredDevType(long value) { }

	virtual void SetSlabLevelDifInfoSet(BOOL value) { }
	virtual void SetSlabLevelDifPlaceShape(long value) { }
	virtual void SetSlabLevelDifHaunchPlaceType(long value) { }

	virtual void SetSlabOpeningInfoSet(BOOL value) { }
	virtual void SetSlabOpeningTopHookType(long value) { }
	virtual void SetSlabOpeningBotHookType(long value) { }

	virtual void SetSlabRebarAreaInfoSet(BOOL value) { }
	virtual void SetSlabRebarTolDist(long value) { }
	virtual void SetSlabRebarTolLenDif(long value) { }

	virtual void SetSlabTopSpacerInfoSet(BOOL value) { }
	virtual void SetSlabTopSpacerSet(BOOL value) { }
	virtual void SetSlabTopSpacerDiaType(long value) { }
	virtual void SetSlabTopSpacerDiaIndex(long value) { }
	virtual void SetSlabTopSpacerSpacingType(long value) { }
	virtual void SetSlabTopSpacerSlabSpacing(long value) { }
	virtual void SetSlabTopSpacerUserSpacing1(long value) { }
	virtual void SetSlabTopSpacerUserSpacing2(long value) { }
	virtual void SetSlabTopSpacerHeightType(long value) { }
	virtual void SetSlabTopSpacerUserHeight(long value) { }
	virtual void SetSlabTopSpacerHookLenAType(long value) { }
	virtual void SetSlabTopSpacerMainBarHookLenA(long value) { }
	virtual void SetSlabTopSpacerUserHookLenA(long value) { }
	virtual void SetSlabTopSpacerHookLenBType(long value) { }
	virtual void SetSlabTopSpacerMainBarHookLenB(long value) { }
	virtual void SetSlabTopSpacerUserHookLenB(long value) { }
	virtual void SetSlabTopSpacerWidth(long value) { }

	virtual void SetSlabBotSpacerInfoSet(BOOL value) { }
	virtual void SetSlabBotSpacerSet(BOOL value) { }
	virtual void SetSlabBotSpacerDiaType(long value) { }
	virtual void SetSlabBotSpacerDiaIndex(long value) { }
	virtual void SetSlabBotSpacerSpacingType(long value) { }
	virtual void SetSlabBotSpacerSlabSpacing(long value) { }
	virtual void SetSlabBotSpacerUserSpacing1(long value) { }
	virtual void SetSlabBotSpacerUserSpacing2(long value) { }
	virtual void SetSlabBotSpacerHeightType(long value) { }
	virtual void SetSlabBotSpacerUserHeight(long value) { }
	virtual void SetSlabBotSpacerHookLenAType(long value) { }
	virtual void SetSlabBotSpacerMainBarHookLenA(long value) { }
	virtual void SetSlabBotSpacerUserHookLenA(long value) { }
	virtual void SetSlabBotSpacerHookLenBType(long value) { }
	virtual void SetSlabBotSpacerMainBarHookLenB(long value) { }
	virtual void SetSlabBotSpacerUserHookLenB(long value) { }
	virtual void SetSlabBotSpacerWidth(long value) { }

	// SOG Slab
	virtual BOOL GetSOGLongMainBarInfoSet() { return FALSE; }
	virtual long GetSOGLongMaxDualHookLength() { return 0; }
	virtual long GetSOGLongSpliceWay() { return 0; }
	virtual long GetSOGLongSpliceType() { return 0; }
	virtual BOOL GetSOGLongAlterSpliceSet() { return FALSE; }
	virtual long GetSOGLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetSOGShortMainBarInfoSet() { return FALSE; }
	virtual long GetSOGShortMaxDualHookLength() { return 0; }
	virtual long GetSOGShortSpliceWay() { return 0; }
	virtual long GetSOGShortSpliceType() { return 0; }
	virtual BOOL GetSOGShortAlterSpliceSet() { return FALSE; }
	virtual long GetSOGShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetSOGEndInfoSet() { return FALSE; }
	virtual long GetSOGTopEndHookType() { return 0; }
	virtual long GetSOGBotEndHookType() { return 0; }
	virtual long GetSOGIsoFootDevType() { return 0; }
	virtual long GetSOGSpliceDevSlabType() { return 0; }

	virtual BOOL GetSOGLevelDifInfoSet() { return FALSE; }
	virtual long GetSOGLevelDifPlaceShape() { return 0; }
	virtual long GetSOGLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetSOGOpeningInfoSet() { return FALSE; }
	virtual long GetSOGOpeningTopHookType() { return 0; }
	virtual long GetSOGOpeningBotHookType() { return 0; }

	virtual BOOL GetSOGRebarAreaInfoSet() { return FALSE; }
	virtual long GetSOGRebarTolDist() { return 0; }
	virtual long GetSOGRebarTolLenDif() { return 0; }

	virtual BOOL GetSOGTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSOGTopSpacerSet() { return FALSE; }
	virtual long GetSOGTopSpacerDiaType() { return 0; }
	virtual long GetSOGTopSpacerDiaIndex() { return 0; }
	virtual long GetSOGTopSpacerSpacingType() { return 0; }
	virtual long GetSOGTopSpacerSlabSpacing() { return 0; }
	virtual long GetSOGTopSpacerUserSpacing1() { return 0; }
	virtual long GetSOGTopSpacerUserSpacing2() { return 0; }
	virtual long GetSOGTopSpacerHeightType() { return 0; }
	virtual long GetSOGTopSpacerUserHeight() { return 0; }
	virtual long GetSOGTopSpacerHookLenAType() { return 0; }
	virtual long GetSOGTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetSOGTopSpacerUserHookLenA() { return 0; }
	virtual long GetSOGTopSpacerHookLenBType() { return 0; }
	virtual long GetSOGTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetSOGTopSpacerUserHookLenB() { return 0; }
	virtual long GetSOGTopSpacerWidth() { return 0; }

	virtual BOOL GetSOGBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSOGBotSpacerSet() { return FALSE; }
	virtual long GetSOGBotSpacerDiaType() { return 0; }
	virtual long GetSOGBotSpacerDiaIndex() { return 0; }
	virtual long GetSOGBotSpacerSpacingType() { return 0; }
	virtual long GetSOGBotSpacerSlabSpacing() { return 0; }
	virtual long GetSOGBotSpacerUserSpacing1() { return 0; }
	virtual long GetSOGBotSpacerUserSpacing2() { return 0; }
	virtual long GetSOGBotSpacerHeightType() { return 0; }
	virtual long GetSOGBotSpacerUserHeight() { return 0; }
	virtual long GetSOGBotSpacerHookLenAType() { return 0; }
	virtual long GetSOGBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetSOGBotSpacerUserHookLenA() { return 0; }
	virtual long GetSOGBotSpacerHookLenBType() { return 0; }
	virtual long GetSOGBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetSOGBotSpacerUserHookLenB() { return 0; }
	virtual long GetSOGBotSpacerWidth() { return 0; }

	virtual void SetSOGLongMainBarInfoSet(BOOL value) { }
	virtual void SetSOGLongMaxDualHookLength(long value) { }
	virtual void SetSOGLongSpliceWay(long value) { }
	virtual void SetSOGLongSpliceType(long value) { }
	virtual void SetSOGLongAlterSpliceSet(BOOL value) { }
	virtual void SetSOGLongAlterSpliceValue1(long value) { }

	virtual void SetSOGShortMainBarInfoSet(BOOL value) { }
	virtual void SetSOGShortMaxDualHookLength(long value) { }
	virtual void SetSOGShortSpliceWay(long value) { }
	virtual void SetSOGShortSpliceType(long value) { }
	virtual void SetSOGShortAlterSpliceSet(BOOL value) { }
	virtual void SetSOGShortAlterSpliceValue1(long value) { }

	virtual void SetSOGEndInfoSet(BOOL value) { }
	virtual void SetSOGTopEndHookType(long value) { }
	virtual void SetSOGBotEndHookType(long value) { }
	virtual void SetSOGIsoFootDevType(long value) { }
	virtual void SetSOGSpliceDevSlabType(long value) { }

	virtual void SetSOGLevelDifInfoSet(BOOL value) { }
	virtual void SetSOGLevelDifPlaceShape(long value) { }
	virtual void SetSOGLevelDifHaunchPlaceType(long value) { }

	virtual void SetSOGOpeningInfoSet(BOOL value) { }
	virtual void SetSOGOpeningTopHookType(long value) { }
	virtual void SetSOGOpeningBotHookType(long value) { }

	virtual void SetSOGRebarAreaInfoSet(BOOL value) { }
	virtual void SetSOGRebarTolDist(long value) { }
	virtual void SetSOGRebarTolLenDif(long value) { }

	virtual void SetSOGTopSpacerInfoSet(BOOL value) { }
	virtual void SetSOGTopSpacerSet(BOOL value) { }
	virtual void SetSOGTopSpacerDiaType(long value) { }
	virtual void SetSOGTopSpacerDiaIndex(long value) { }
	virtual void SetSOGTopSpacerSpacingType(long value) { }
	virtual void SetSOGTopSpacerSlabSpacing(long value) { }
	virtual void SetSOGTopSpacerUserSpacing1(long value) { }
	virtual void SetSOGTopSpacerUserSpacing2(long value) { }
	virtual void SetSOGTopSpacerHeightType(long value) { }
	virtual void SetSOGTopSpacerUserHeight(long value) { }
	virtual void SetSOGTopSpacerHookLenAType(long value) { }
	virtual void SetSOGTopSpacerMainBarHookLenA(long value) { }
	virtual void SetSOGTopSpacerUserHookLenA(long value) { }
	virtual void SetSOGTopSpacerHookLenBType(long value) { }
	virtual void SetSOGTopSpacerMainBarHookLenB(long value) { }
	virtual void SetSOGTopSpacerUserHookLenB(long value) { }
	virtual void SetSOGTopSpacerWidth(long value) { }

	virtual void SetSOGBotSpacerInfoSet(BOOL value) { }
	virtual void SetSOGBotSpacerSet(BOOL value) { }
	virtual void SetSOGBotSpacerDiaType(long value) { }
	virtual void SetSOGBotSpacerDiaIndex(long value) { }
	virtual void SetSOGBotSpacerSpacingType(long value) { }
	virtual void SetSOGBotSpacerSlabSpacing(long value) { }
	virtual void SetSOGBotSpacerUserSpacing1(long value) { }
	virtual void SetSOGBotSpacerUserSpacing2(long value) { }
	virtual void SetSOGBotSpacerHeightType(long value) { }
	virtual void SetSOGBotSpacerUserHeight(long value) { }
	virtual void SetSOGBotSpacerHookLenAType(long value) { }
	virtual void SetSOGBotSpacerMainBarHookLenA(long value) { }
	virtual void SetSOGBotSpacerUserHookLenA(long value) { }
	virtual void SetSOGBotSpacerHookLenBType(long value) { }
	virtual void SetSOGBotSpacerMainBarHookLenB(long value) { }
	virtual void SetSOGBotSpacerUserHookLenB(long value) { }
	virtual void SetSOGBotSpacerWidth(long value) { }

	// Circular Ramp
	virtual BOOL GetCircularRampLongMainBarInfoSet() { return FALSE; }
	virtual long GetCircularRampLongMaxDualHookLength() { return 0; }
	virtual long GetCircularRampLongSpliceWay() { return 0; }
	virtual long GetCircularRampLongSpliceType() { return 0; }
	virtual BOOL GetCircularRampLongAlterSpliceSet() { return FALSE; }
	virtual long GetCircularRampLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetCircularRampShortMainBarInfoSet() { return FALSE; }
	virtual long GetCircularRampShortMaxDualHookLength() { return 0; }
	virtual long GetCircularRampShortSpliceWay() { return 0; }
	virtual long GetCircularRampShortSpliceType() { return 0; }
	virtual BOOL GetCircularRampShortAlterSpliceSet() { return FALSE; }
	virtual long GetCircularRampShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetCircularRampEndInfoSet() { return FALSE; }
	virtual long GetCircularRampTopEndHookType() { return 0; }
	virtual long GetCircularRampBotEndHookType() { return 0; }
	virtual long GetCircularRampIsoFootDevType() { return 0; }
	virtual long GetCircularRampSpliceDevSlabType() { return 0; }

	virtual BOOL GetCircularRampLevelDifInfoSet() { return FALSE; }
	virtual long GetCircularRampLevelDifPlaceShape() { return 0; }
	virtual long GetCircularRampLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetCircularRampOpeningInfoSet() { return FALSE; }
	virtual long GetCircularRampOpeningTopHookType() { return 0; }
	virtual long GetCircularRampOpeningBotHookType() { return 0; }

	virtual BOOL GetCircularRampRebarAreaInfoSet() { return FALSE; }
	virtual long GetCircularRampRebarTolDist() { return 0; }
	virtual long GetCircularRampRebarTolLenDif() { return 0; }

	virtual BOOL GetCircularRampTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetCircularRampTopSpacerSet() { return FALSE; }
	virtual long GetCircularRampTopSpacerDiaType() { return 0; }
	virtual long GetCircularRampTopSpacerDiaIndex() { return 0; }
	virtual long GetCircularRampTopSpacerSpacingType() { return 0; }
	virtual long GetCircularRampTopSpacerSlabSpacing() { return 0; }
	virtual long GetCircularRampTopSpacerUserSpacing1() { return 0; }
	virtual long GetCircularRampTopSpacerUserSpacing2() { return 0; }
	virtual long GetCircularRampTopSpacerHeightType() { return 0; }
	virtual long GetCircularRampTopSpacerUserHeight() { return 0; }
	virtual long GetCircularRampTopSpacerHookLenAType() { return 0; }
	virtual long GetCircularRampTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetCircularRampTopSpacerUserHookLenA() { return 0; }
	virtual long GetCircularRampTopSpacerHookLenBType() { return 0; }
	virtual long GetCircularRampTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetCircularRampTopSpacerUserHookLenB() { return 0; }
	virtual long GetCircularRampTopSpacerWidth() { return 0; }

	virtual BOOL GetCircularRampBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetCircularRampBotSpacerSet() { return FALSE; }
	virtual long GetCircularRampBotSpacerDiaType() { return 0; }
	virtual long GetCircularRampBotSpacerDiaIndex() { return 0; }
	virtual long GetCircularRampBotSpacerSpacingType() { return 0; }
	virtual long GetCircularRampBotSpacerSlabSpacing() { return 0; }
	virtual long GetCircularRampBotSpacerUserSpacing1() { return 0; }
	virtual long GetCircularRampBotSpacerUserSpacing2() { return 0; }
	virtual long GetCircularRampBotSpacerHeightType() { return 0; }
	virtual long GetCircularRampBotSpacerUserHeight() { return 0; }
	virtual long GetCircularRampBotSpacerHookLenAType() { return 0; }
	virtual long GetCircularRampBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetCircularRampBotSpacerUserHookLenA() { return 0; }
	virtual long GetCircularRampBotSpacerHookLenBType() { return 0; }
	virtual long GetCircularRampBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetCircularRampBotSpacerUserHookLenB() { return 0; }
	virtual long GetCircularRampBotSpacerWidth() { return 0; }

	virtual void SetCircularRampLongMainBarInfoSet(BOOL value) { }
	virtual void SetCircularRampLongMaxDualHookLength(long value) { }
	virtual void SetCircularRampLongSpliceWay(long value) { }
	virtual void SetCircularRampLongSpliceType(long value) { }
	virtual void SetCircularRampLongAlterSpliceSet(BOOL value) { }
	virtual void SetCircularRampLongAlterSpliceValue1(long value) { }

	virtual void SetCircularRampShortMainBarInfoSet(BOOL value) { }
	virtual void SetCircularRampShortMaxDualHookLength(long value) { }
	virtual void SetCircularRampShortSpliceWay(long value) { }
	virtual void SetCircularRampShortSpliceType(long value) { }
	virtual void SetCircularRampShortAlterSpliceSet(BOOL value) { }
	virtual void SetCircularRampShortAlterSpliceValue1(long value) { }

	virtual void SetCircularRampEndInfoSet(BOOL value) { }
	virtual void SetCircularRampTopEndHookType(long value) { }
	virtual void SetCircularRampBotEndHookType(long value) { }
	virtual void SetCircularRampIsoFootDevType(long value) { }
	virtual void SetCircularRampSpliceDevSlabType(long value) { }

	virtual void SetCircularRampLevelDifInfoSet(BOOL value) { }
	virtual void SetCircularRampLevelDifPlaceShape(long value) { }
	virtual void SetCircularRampLevelDifHaunchPlaceType(long value) { }

	virtual void SetCircularRampOpeningInfoSet(BOOL value) { }
	virtual void SetCircularRampOpeningTopHookType(long value) { }
	virtual void SetCircularRampOpeningBotHookType(long value) { }

	virtual void SetCircularRampRebarAreaInfoSet(BOOL value) { }
	virtual void SetCircularRampRebarTolDist(long value) { }
	virtual void SetCircularRampRebarTolLenDif(long value) { }

	virtual void SetCircularRampTopSpacerInfoSet(BOOL value) { }
	virtual void SetCircularRampTopSpacerSet(BOOL value) { }
	virtual void SetCircularRampTopSpacerDiaType(long value) { }
	virtual void SetCircularRampTopSpacerDiaIndex(long value) { }
	virtual void SetCircularRampTopSpacerSpacingType(long value) { }
	virtual void SetCircularRampTopSpacerSlabSpacing(long value) { }
	virtual void SetCircularRampTopSpacerUserSpacing1(long value) { }
	virtual void SetCircularRampTopSpacerUserSpacing2(long value) { }
	virtual void SetCircularRampTopSpacerHeightType(long value) { }
	virtual void SetCircularRampTopSpacerUserHeight(long value) { }
	virtual void SetCircularRampTopSpacerHookLenAType(long value) { }
	virtual void SetCircularRampTopSpacerMainBarHookLenA(long value) { }
	virtual void SetCircularRampTopSpacerUserHookLenA(long value) { }
	virtual void SetCircularRampTopSpacerHookLenBType(long value) { }
	virtual void SetCircularRampTopSpacerMainBarHookLenB(long value) { }
	virtual void SetCircularRampTopSpacerUserHookLenB(long value) { }
	virtual void SetCircularRampTopSpacerWidth(long value) { }

	virtual void SetCircularRampBotSpacerInfoSet(BOOL value) { }
	virtual void SetCircularRampBotSpacerSet(BOOL value) { }
	virtual void SetCircularRampBotSpacerDiaType(long value) { }
	virtual void SetCircularRampBotSpacerDiaIndex(long value) { }
	virtual void SetCircularRampBotSpacerSpacingType(long value) { }
	virtual void SetCircularRampBotSpacerSlabSpacing(long value) { }
	virtual void SetCircularRampBotSpacerUserSpacing1(long value) { }
	virtual void SetCircularRampBotSpacerUserSpacing2(long value) { }
	virtual void SetCircularRampBotSpacerHeightType(long value) { }
	virtual void SetCircularRampBotSpacerUserHeight(long value) { }
	virtual void SetCircularRampBotSpacerHookLenAType(long value) { }
	virtual void SetCircularRampBotSpacerMainBarHookLenA(long value) { }
	virtual void SetCircularRampBotSpacerUserHookLenA(long value) { }
	virtual void SetCircularRampBotSpacerHookLenBType(long value) { }
	virtual void SetCircularRampBotSpacerMainBarHookLenB(long value) { }
	virtual void SetCircularRampBotSpacerUserHookLenB(long value) { }
	virtual void SetCircularRampBotSpacerWidth(long value) { }

	// PC Slab
	virtual BOOL GetPCSlabLongMainBarInfoSet() { return FALSE; }
	virtual long GetPCSlabLongMaxDualHookLength() { return 0; }
	virtual long GetPCSlabLongSpliceWay() { return 0; }
	virtual long GetPCSlabLongSpliceType() { return 0; }
	virtual BOOL GetPCSlabLongAlterSpliceSet() { return FALSE; }
	virtual long GetPCSlabLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetPCSlabShortMainBarInfoSet() { return FALSE; }
	virtual long GetPCSlabShortMaxDualHookLength() { return 0; }
	virtual long GetPCSlabShortSpliceWay() { return 0; }
	virtual long GetPCSlabShortSpliceType() { return 0; }
	virtual BOOL GetPCSlabShortAlterSpliceSet() { return FALSE; }
	virtual long GetPCSlabShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetPCSlabStructuredEndInfoSet() { return FALSE; }
	virtual long GetPCSlabTopStructuredEndHookType() { return 0; }
	virtual long GetPCSlabBotStructuredEndHookType() { return 0; }

	virtual BOOL GetPCSlabDeckInfoSet() { return FALSE; }
	virtual long GetPCSlabDeckDifDiaConnectBarType() { return 0; }
	virtual long GetPCSlabDeckAnchorType() { return 0; }
	virtual long GetPCSlabDeckTopEndRatioType() { return 0; }
	virtual long GetPCSlabDeckBotEndRatioType() { return 0; }

	virtual BOOL GetPCSlabUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetPCSlabTopUnStructuredEndHookType() { return 0; }
	virtual long GetPCSlabBotUnStructuredEndHookType() { return 0; }
	virtual long GetPCSlabIsoFootUnStructuredDevType() { return 0; }

	virtual BOOL GetPCSlabLevelDifInfoSet() { return FALSE; }
	virtual long GetPCSlabLevelDifPlaceShape() { return 0; }
	virtual long GetPCSlabLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetPCSlabOpeningInfoSet() { return FALSE; }
	virtual long GetPCSlabOpeningTopHookType() { return 0; }
	virtual long GetPCSlabOpeningBotHookType() { return 0; }

	virtual BOOL GetPCSlabRebarAreaInfoSet() { return FALSE; }
	virtual long GetPCSlabRebarTolDist() { return 0; }
	virtual long GetPCSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetPCSlabTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetPCSlabTopSpacerSet() { return FALSE; }
	virtual long GetPCSlabTopSpacerDiaType() { return 0; }
	virtual long GetPCSlabTopSpacerDiaIndex() { return 0; }
	virtual long GetPCSlabTopSpacerSpacingType() { return 0; }
	virtual long GetPCSlabTopSpacerSlabSpacing() { return 0; }
	virtual long GetPCSlabTopSpacerUserSpacing1() { return 0; }
	virtual long GetPCSlabTopSpacerUserSpacing2() { return 0; }
	virtual long GetPCSlabTopSpacerHeightType() { return 0; }
	virtual long GetPCSlabTopSpacerUserHeight() { return 0; }
	virtual long GetPCSlabTopSpacerHookLenAType() { return 0; }
	virtual long GetPCSlabTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetPCSlabTopSpacerUserHookLenA() { return 0; }
	virtual long GetPCSlabTopSpacerHookLenBType() { return 0; }
	virtual long GetPCSlabTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetPCSlabTopSpacerUserHookLenB() { return 0; }
	virtual long GetPCSlabTopSpacerWidth() { return 0; }

	virtual BOOL GetPCSlabBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetPCSlabBotSpacerSet() { return FALSE; }
	virtual long GetPCSlabBotSpacerDiaType() { return 0; }
	virtual long GetPCSlabBotSpacerDiaIndex() { return 0; }
	virtual long GetPCSlabBotSpacerSpacingType() { return 0; }
	virtual long GetPCSlabBotSpacerSlabSpacing() { return 0; }
	virtual long GetPCSlabBotSpacerUserSpacing1() { return 0; }
	virtual long GetPCSlabBotSpacerUserSpacing2() { return 0; }
	virtual long GetPCSlabBotSpacerHeightType() { return 0; }
	virtual long GetPCSlabBotSpacerUserHeight() { return 0; }
	virtual long GetPCSlabBotSpacerHookLenAType() { return 0; }
	virtual long GetPCSlabBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetPCSlabBotSpacerUserHookLenA() { return 0; }
	virtual long GetPCSlabBotSpacerHookLenBType() { return 0; }
	virtual long GetPCSlabBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetPCSlabBotSpacerUserHookLenB() { return 0; }
	virtual long GetPCSlabBotSpacerWidth() { return 0; }

	virtual void SetPCSlabLongMainBarInfoSet(BOOL value) { }
	virtual void SetPCSlabLongMaxDualHookLength(long value) { }
	virtual void SetPCSlabLongSpliceWay(long value) { }
	virtual void SetPCSlabLongSpliceType(long value) { }
	virtual void SetPCSlabLongAlterSpliceSet(BOOL value) { }
	virtual void SetPCSlabLongAlterSpliceValue1(long value) { }

	virtual void SetPCSlabShortMainBarInfoSet(BOOL value) { }
	virtual void SetPCSlabShortMaxDualHookLength(long value) { }
	virtual void SetPCSlabShortSpliceWay(long value) { }
	virtual void SetPCSlabShortSpliceType(long value) { }
	virtual void SetPCSlabShortAlterSpliceSet(BOOL value) { }
	virtual void SetPCSlabShortAlterSpliceValue1(long value) { }

	virtual void SetPCSlabStructuredEndInfoSet(BOOL value) { }
	virtual void SetPCSlabTopStructuredEndHookType(long value) { }
	virtual void SetPCSlabBotStructuredEndHookType(long value) { }

	virtual void SetPCSlabDeckInfoSet(BOOL value) { }
	virtual void SetPCSlabDeckDifDiaConnectBarType(long value) { }
	virtual void SetPCSlabDeckAnchorType(long value) { }
	virtual void SetPCSlabDeckTopEndRatioType(long value) { }
	virtual void SetPCSlabDeckBotEndRatioType(long value) { }

	virtual void SetPCSlabUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetPCSlabTopUnStructuredEndHookType(long value) { }
	virtual void SetPCSlabBotUnStructuredEndHookType(long value) { }
	virtual void SetPCSlabIsoFootUnStructuredDevType(long value) { }

	virtual void SetPCSlabLevelDifInfoSet(BOOL value) { }
	virtual void SetPCSlabLevelDifPlaceShape(long value) { }
	virtual void SetPCSlabLevelDifHaunchPlaceType(long value) { }

	virtual void SetPCSlabOpeningInfoSet(BOOL value) { }
	virtual void SetPCSlabOpeningTopHookType(long value) { }
	virtual void SetPCSlabOpeningBotHookType(long value) { }

	virtual void SetPCSlabRebarAreaInfoSet(BOOL value) { }
	virtual void SetPCSlabRebarTolDist(long value) { }
	virtual void SetPCSlabRebarTolLenDif(long value) { }

	virtual void SetPCSlabTopSpacerInfoSet(BOOL value) { }
	virtual void SetPCSlabTopSpacerSet(BOOL value) { }
	virtual void SetPCSlabTopSpacerDiaType(long value) { }
	virtual void SetPCSlabTopSpacerDiaIndex(long value) { }
	virtual void SetPCSlabTopSpacerSpacingType(long value) { }
	virtual void SetPCSlabTopSpacerSlabSpacing(long value) { }
	virtual void SetPCSlabTopSpacerUserSpacing1(long value) { }
	virtual void SetPCSlabTopSpacerUserSpacing2(long value) { }
	virtual void SetPCSlabTopSpacerHeightType(long value) { }
	virtual void SetPCSlabTopSpacerUserHeight(long value) { }
	virtual void SetPCSlabTopSpacerHookLenAType(long value) { }
	virtual void SetPCSlabTopSpacerMainBarHookLenA(long value) { }
	virtual void SetPCSlabTopSpacerUserHookLenA(long value) { }
	virtual void SetPCSlabTopSpacerHookLenBType(long value) { }
	virtual void SetPCSlabTopSpacerMainBarHookLenB(long value) { }
	virtual void SetPCSlabTopSpacerUserHookLenB(long value) { }
	virtual void SetPCSlabTopSpacerWidth(long value) { }

	virtual void SetPCSlabBotSpacerInfoSet(BOOL value) { }
	virtual void SetPCSlabBotSpacerSet(BOOL value) { }
	virtual void SetPCSlabBotSpacerDiaType(long value) { }
	virtual void SetPCSlabBotSpacerDiaIndex(long value) { }
	virtual void SetPCSlabBotSpacerSpacingType(long value) { }
	virtual void SetPCSlabBotSpacerSlabSpacing(long value) { }
	virtual void SetPCSlabBotSpacerUserSpacing1(long value) { }
	virtual void SetPCSlabBotSpacerUserSpacing2(long value) { }
	virtual void SetPCSlabBotSpacerHeightType(long value) { }
	virtual void SetPCSlabBotSpacerUserHeight(long value) { }
	virtual void SetPCSlabBotSpacerHookLenAType(long value) { }
	virtual void SetPCSlabBotSpacerMainBarHookLenA(long value) { }
	virtual void SetPCSlabBotSpacerUserHookLenA(long value) { }
	virtual void SetPCSlabBotSpacerHookLenBType(long value) { }
	virtual void SetPCSlabBotSpacerMainBarHookLenB(long value) { }
	virtual void SetPCSlabBotSpacerUserHookLenB(long value) { }
	virtual void SetPCSlabBotSpacerWidth(long value) { }

	// 철근 일체형 데크 슬래브
	virtual BOOL GetEmbeddedDeckLongMainBarInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckLongMaxDualHookLength() { return 0; }
	virtual long GetEmbeddedDeckLongSpliceWay() { return 0; }
	virtual long GetEmbeddedDeckLongSpliceType() { return 0; }
	virtual BOOL GetEmbeddedDeckLongAlterSpliceSet() { return FALSE; }
	virtual long GetEmbeddedDeckLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetEmbeddedDeckShortMainBarInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckShortMaxDualHookLength() { return 0; }
	virtual long GetEmbeddedDeckShortSpliceWay() { return 0; }
	virtual long GetEmbeddedDeckShortSpliceType() { return 0; }
	virtual BOOL GetEmbeddedDeckShortAlterSpliceSet() { return FALSE; }
	virtual long GetEmbeddedDeckShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetEmbeddedDeckStructuredEndInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckTopStructuredEndHookType() { return 0; }
	virtual long GetEmbeddedDeckBotStructuredEndHookType() { return 0; }

	virtual BOOL GetEmbeddedDeckInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckDifDiaConnectBarType() { return 0; }
	virtual long GetEmbeddedDeckAnchorType() { return 0; }
	virtual long GetEmbeddedDeckTopEndRatioType() { return 0; }
	virtual long GetEmbeddedDeckBotEndRatioType() { return 0; }

	virtual BOOL GetEmbeddedDeckUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckTopUnStructuredEndHookType() { return 0; }
	virtual long GetEmbeddedDeckBotUnStructuredEndHookType() { return 0; }
	virtual long GetEmbeddedDeckIsoFootUnStructuredDevType() { return 0; }

	virtual BOOL GetEmbeddedDeckLevelDifInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckLevelDifPlaceShape() { return 0; }
	virtual long GetEmbeddedDeckLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetEmbeddedDeckOpeningInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckOpeningTopHookType() { return 0; }
	virtual long GetEmbeddedDeckOpeningBotHookType() { return 0; }

	virtual BOOL GetEmbeddedDeckRebarAreaInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckRebarTolDist() { return 0; }
	virtual long GetEmbeddedDeckRebarTolLenDif() { return 0; }

	virtual void SetEmbeddedDeckLongMainBarInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckLongMaxDualHookLength(long value) { }
	virtual void SetEmbeddedDeckLongSpliceWay(long value) { }
	virtual void SetEmbeddedDeckLongSpliceType(long value) { }
	virtual void SetEmbeddedDeckLongAlterSpliceSet(BOOL value) { }
	virtual void SetEmbeddedDeckLongAlterSpliceValue1(long value) { }

	virtual void SetEmbeddedDeckShortMainBarInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckShortMaxDualHookLength(long value) { }
	virtual void SetEmbeddedDeckShortSpliceWay(long value) { }
	virtual void SetEmbeddedDeckShortSpliceType(long value) { }
	virtual void SetEmbeddedDeckShortAlterSpliceSet(BOOL value) { }
	virtual void SetEmbeddedDeckShortAlterSpliceValue1(long value) { }

	virtual void SetEmbeddedDeckStructuredEndInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckTopStructuredEndHookType(long value) { }
	virtual void SetEmbeddedDeckBotStructuredEndHookType(long value) { }

	virtual void SetEmbeddedDeckInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckDifDiaConnectBarType(long value) { }
	virtual void SetEmbeddedDeckAnchorType(long value) { }
	virtual void SetEmbeddedDeckTopEndRatioType(long value) { }
	virtual void SetEmbeddedDeckBotEndRatioType(long value) { }

	virtual void SetEmbeddedDeckUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckTopUnStructuredEndHookType(long value) { }
	virtual void SetEmbeddedDeckBotUnStructuredEndHookType(long value) { }
	virtual void SetEmbeddedDeckIsoFootUnStructuredDevType(long value) { }

	virtual void SetEmbeddedDeckLevelDifInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckLevelDifPlaceShape(long value) { }
	virtual void SetEmbeddedDeckLevelDifHaunchPlaceType(long value) { }

	virtual void SetEmbeddedDeckOpeningInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckOpeningTopHookType(long value) { }
	virtual void SetEmbeddedDeckOpeningBotHookType(long value) { }

	virtual void SetEmbeddedDeckRebarAreaInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckRebarTolDist(long value) { }
	virtual void SetEmbeddedDeckRebarTolLenDif(long value) { }

	// 일반 데크 슬래브
	virtual BOOL GetGeneralDeckLongMainBarInfoSet() { return FALSE; }
	virtual long GetGeneralDeckLongMaxDualHookLength() { return 0; }
	virtual long GetGeneralDeckLongSpliceWay() { return 0; }
	virtual long GetGeneralDeckLongSpliceType() { return 0; }
	virtual BOOL GetGeneralDeckLongAlterSpliceSet() { return FALSE; }
	virtual long GetGeneralDeckLongAlterSpliceValue1() { return 0; }

	virtual BOOL GetGeneralDeckShortMainBarInfoSet() { return FALSE; }
	virtual long GetGeneralDeckShortMaxDualHookLength() { return 0; }
	virtual long GetGeneralDeckShortSpliceWay() { return 0; }
	virtual long GetGeneralDeckShortSpliceType() { return 0; }
	virtual BOOL GetGeneralDeckShortAlterSpliceSet() { return FALSE; }
	virtual long GetGeneralDeckShortAlterSpliceValue1() { return 0; }

	virtual BOOL GetGeneralDeckStructuredEndInfoSet() { return FALSE; }
	virtual long GetGeneralDeckTopStructuredEndHookType() { return 0; }
	virtual long GetGeneralDeckBotStructuredEndHookType() { return 0; }

	virtual BOOL GetGeneralDeckUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetGeneralDeckTopUnStructuredEndHookType() { return 0; }
	virtual long GetGeneralDeckBotUnStructuredEndHookType() { return 0; }
	virtual long GetGeneralDeckIsoFootUnStructuredDevType() { return 0; }

	virtual BOOL GetGeneralDeckLevelDifInfoSet() { return FALSE; }
	virtual long GetGeneralDeckLevelDifPlaceShape() { return 0; }
	virtual long GetGeneralDeckLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetGeneralDeckOpeningInfoSet() { return FALSE; }
	virtual long GetGeneralDeckOpeningTopHookType() { return 0; }
	virtual long GetGeneralDeckOpeningBotHookType() { return 0; }

	virtual BOOL GetGeneralDeckRebarAreaInfoSet() { return FALSE; }
	virtual long GetGeneralDeckRebarTolDist() { return 0; }
	virtual long GetGeneralDeckRebarTolLenDif() { return 0; }

	virtual void SetGeneralDeckLongMainBarInfoSet(BOOL value) { }
	virtual void SetGeneralDeckLongMaxDualHookLength(long value) { }
	virtual void SetGeneralDeckLongSpliceWay(long value) { }
	virtual void SetGeneralDeckLongSpliceType(long value) { }
	virtual void SetGeneralDeckLongAlterSpliceSet(BOOL value) { }
	virtual void SetGeneralDeckLongAlterSpliceValue1(long value) { }

	virtual void SetGeneralDeckShortMainBarInfoSet(BOOL value) { }
	virtual void SetGeneralDeckShortMaxDualHookLength(long value) { }
	virtual void SetGeneralDeckShortSpliceWay(long value) { }
	virtual void SetGeneralDeckShortSpliceType(long value) { }
	virtual void SetGeneralDeckShortAlterSpliceSet(BOOL value) { }
	virtual void SetGeneralDeckShortAlterSpliceValue1(long value) { }

	virtual void SetGeneralDeckStructuredEndInfoSet(BOOL value) { }
	virtual void SetGeneralDeckTopStructuredEndHookType(long value) { }
	virtual void SetGeneralDeckBotStructuredEndHookType(long value) { }

	virtual void SetGeneralDeckUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetGeneralDeckTopUnStructuredEndHookType(long value) { }
	virtual void SetGeneralDeckBotUnStructuredEndHookType(long value) { }
	virtual void SetGeneralDeckIsoFootUnStructuredDevType(long value) { }

	virtual void SetGeneralDeckLevelDifInfoSet(BOOL value) { }
	virtual void SetGeneralDeckLevelDifPlaceShape(long value) { }
	virtual void SetGeneralDeckLevelDifHaunchPlaceType(long value) { }

	virtual void SetGeneralDeckOpeningInfoSet(BOOL value) { }
	virtual void SetGeneralDeckOpeningTopHookType(long value) { }
	virtual void SetGeneralDeckOpeningBotHookType(long value) { }

	virtual void SetGeneralDeckRebarAreaInfoSet(BOOL value) { }
	virtual void SetGeneralDeckRebarTolDist(long value) { }
	virtual void SetGeneralDeckRebarTolLenDif(long value) { }

	//IsoFoot
	virtual BOOL GetIsoFootTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetIsoFootTopSpacerSet() { return FALSE; }
	virtual long GetIsoFootTopSpacerDiaType() { return 0; }
	virtual long GetIsoFootTopSpacerDiaIndex() { return 0; }
	virtual long GetIsoFootTopSpacerSpacingType() { return 0; }
	virtual long GetIsoFootTopSpacerSlabSpacing() { return 0; }
	virtual long GetIsoFootTopSpacerUserSpacing1() { return 0; }
	virtual long GetIsoFootTopSpacerUserSpacing2() { return 0; }
	virtual long GetIsoFootTopSpacerHeightType() { return 0; }
	virtual long GetIsoFootTopSpacerUserHeight() { return 0; }
	virtual long GetIsoFootTopSpacerHookLenAType() { return 0; }
	virtual long GetIsoFootTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetIsoFootTopSpacerUserHookLenA() { return 0; }
	virtual long GetIsoFootTopSpacerHookLenBType() { return 0; }
	virtual long GetIsoFootTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetIsoFootTopSpacerUserHookLenB() { return 0; }
	virtual long GetIsoFootTopSpacerWidth() { return 0; }

	virtual BOOL GetIsoFootBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetIsoFootBotSpacerSet() { return FALSE; }
	virtual long GetIsoFootBotSpacerDiaType() { return 0; }
	virtual long GetIsoFootBotSpacerDiaIndex() { return 0; }
	virtual long GetIsoFootBotSpacerSpacingType() { return 0; }
	virtual long GetIsoFootBotSpacerSlabSpacing() { return 0; }
	virtual long GetIsoFootBotSpacerUserSpacing1() { return 0; }
	virtual long GetIsoFootBotSpacerUserSpacing2() { return 0; }
	virtual long GetIsoFootBotSpacerHeightType() { return 0; }
	virtual long GetIsoFootBotSpacerUserHeight() { return 0; }
	virtual long GetIsoFootBotSpacerHookLenAType() { return 0; }
	virtual long GetIsoFootBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetIsoFootBotSpacerUserHookLenA() { return 0; }
	virtual long GetIsoFootBotSpacerHookLenBType() { return 0; }
	virtual long GetIsoFootBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetIsoFootBotSpacerUserHookLenB() { return 0; }
	virtual long GetIsoFootBotSpacerWidth() { return 0; }

	virtual void SetIsoFootTopSpacerInfoSet(BOOL value) { }
	virtual void SetIsoFootTopSpacerSet(BOOL value) { }
	virtual void SetIsoFootTopSpacerDiaType(long value) { }
	virtual void SetIsoFootTopSpacerDiaIndex(long value) { }
	virtual void SetIsoFootTopSpacerSpacingType(long value) { }
	virtual void SetIsoFootTopSpacerSlabSpacing(long value) { }
	virtual void SetIsoFootTopSpacerUserSpacing1(long value) { }
	virtual void SetIsoFootTopSpacerUserSpacing2(long value) { }
	virtual void SetIsoFootTopSpacerHeightType(long value) { }
	virtual void SetIsoFootTopSpacerUserHeight(long value) { }
	virtual void SetIsoFootTopSpacerHookLenAType(long value) { }
	virtual void SetIsoFootTopSpacerMainBarHookLenA(long value) { }
	virtual void SetIsoFootTopSpacerUserHookLenA(long value) { }
	virtual void SetIsoFootTopSpacerHookLenBType(long value) { }
	virtual void SetIsoFootTopSpacerMainBarHookLenB(long value) { }
	virtual void SetIsoFootTopSpacerUserHookLenB(long value) { }
	virtual void SetIsoFootTopSpacerWidth(long value) { }

	virtual void SetIsoFootBotSpacerInfoSet(BOOL value) { }
	virtual void SetIsoFootBotSpacerSet(BOOL value) { }
	virtual void SetIsoFootBotSpacerDiaType(long value) { }
	virtual void SetIsoFootBotSpacerDiaIndex(long value) { }
	virtual void SetIsoFootBotSpacerSpacingType(long value) { }
	virtual void SetIsoFootBotSpacerSlabSpacing(long value) { }
	virtual void SetIsoFootBotSpacerUserSpacing1(long value) { }
	virtual void SetIsoFootBotSpacerUserSpacing2(long value) { }
	virtual void SetIsoFootBotSpacerHeightType(long value) { }
	virtual void SetIsoFootBotSpacerUserHeight(long value) { }
	virtual void SetIsoFootBotSpacerHookLenAType(long value) { }
	virtual void SetIsoFootBotSpacerMainBarHookLenA(long value) { }
	virtual void SetIsoFootBotSpacerUserHookLenA(long value) { }
	virtual void SetIsoFootBotSpacerHookLenBType(long value) { }
	virtual void SetIsoFootBotSpacerMainBarHookLenB(long value) { }
	virtual void SetIsoFootBotSpacerUserHookLenB(long value) { }
	virtual void SetIsoFootBotSpacerWidth(long value) { }

	//WallFoot
	virtual BOOL GetWallFootTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetWallFootTopSpacerSet() { return FALSE; }
	virtual long GetWallFootTopSpacerDiaType() { return 0; }
	virtual long GetWallFootTopSpacerDiaIndex() { return 0; }
	virtual long GetWallFootTopSpacerSpacingType() { return 0; }
	virtual long GetWallFootTopSpacerSlabSpacing() { return 0; }
	virtual long GetWallFootTopSpacerUserSpacing1() { return 0; }
	virtual long GetWallFootTopSpacerUserSpacing2() { return 0; }
	virtual long GetWallFootTopSpacerHeightType() { return 0; }
	virtual long GetWallFootTopSpacerUserHeight() { return 0; }
	virtual long GetWallFootTopSpacerHookLenAType() { return 0; }
	virtual long GetWallFootTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetWallFootTopSpacerUserHookLenA() { return 0; }
	virtual long GetWallFootTopSpacerHookLenBType() { return 0; }
	virtual long GetWallFootTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetWallFootTopSpacerUserHookLenB() { return 0; }
	virtual long GetWallFootTopSpacerWidth() { return 0; }

	virtual BOOL GetWallFootBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetWallFootBotSpacerSet() { return FALSE; }
	virtual long GetWallFootBotSpacerDiaType() { return 0; }
	virtual long GetWallFootBotSpacerDiaIndex() { return 0; }
	virtual long GetWallFootBotSpacerSpacingType() { return 0; }
	virtual long GetWallFootBotSpacerSlabSpacing() { return 0; }
	virtual long GetWallFootBotSpacerUserSpacing1() { return 0; }
	virtual long GetWallFootBotSpacerUserSpacing2() { return 0; }
	virtual long GetWallFootBotSpacerHeightType() { return 0; }
	virtual long GetWallFootBotSpacerUserHeight() { return 0; }
	virtual long GetWallFootBotSpacerHookLenAType() { return 0; }
	virtual long GetWallFootBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetWallFootBotSpacerUserHookLenA() { return 0; }
	virtual long GetWallFootBotSpacerHookLenBType() { return 0; }
	virtual long GetWallFootBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetWallFootBotSpacerUserHookLenB() { return 0; }
	virtual long GetWallFootBotSpacerWidth() { return 0; }

	virtual void SetWallFootTopSpacerInfoSet(BOOL value) { }
	virtual void SetWallFootTopSpacerSet(BOOL value) { }
	virtual void SetWallFootTopSpacerDiaType(long value) { }
	virtual void SetWallFootTopSpacerDiaIndex(long value) { }
	virtual void SetWallFootTopSpacerSpacingType(long value) { }
	virtual void SetWallFootTopSpacerSlabSpacing(long value) { }
	virtual void SetWallFootTopSpacerUserSpacing1(long value) { }
	virtual void SetWallFootTopSpacerUserSpacing2(long value) { }
	virtual void SetWallFootTopSpacerHeightType(long value) { }
	virtual void SetWallFootTopSpacerUserHeight(long value) { }
	virtual void SetWallFootTopSpacerHookLenAType(long value) { }
	virtual void SetWallFootTopSpacerMainBarHookLenA(long value) { }
	virtual void SetWallFootTopSpacerUserHookLenA(long value) { }
	virtual void SetWallFootTopSpacerHookLenBType(long value) { }
	virtual void SetWallFootTopSpacerMainBarHookLenB(long value) { }
	virtual void SetWallFootTopSpacerUserHookLenB(long value) { }
	virtual void SetWallFootTopSpacerWidth(long value) { }

	virtual void SetWallFootBotSpacerInfoSet(BOOL value) { }
	virtual void SetWallFootBotSpacerSet(BOOL value) { }
	virtual void SetWallFootBotSpacerDiaType(long value) { }
	virtual void SetWallFootBotSpacerDiaIndex(long value) { }
	virtual void SetWallFootBotSpacerSpacingType(long value) { }
	virtual void SetWallFootBotSpacerSlabSpacing(long value) { }
	virtual void SetWallFootBotSpacerUserSpacing1(long value) { }
	virtual void SetWallFootBotSpacerUserSpacing2(long value) { }
	virtual void SetWallFootBotSpacerHeightType(long value) { }
	virtual void SetWallFootBotSpacerUserHeight(long value) { }
	virtual void SetWallFootBotSpacerHookLenAType(long value) { }
	virtual void SetWallFootBotSpacerMainBarHookLenA(long value) { }
	virtual void SetWallFootBotSpacerUserHookLenA(long value) { }
	virtual void SetWallFootBotSpacerHookLenBType(long value) { }
	virtual void SetWallFootBotSpacerMainBarHookLenB(long value) { }
	virtual void SetWallFootBotSpacerUserHookLenB(long value) { }
	virtual void SetWallFootBotSpacerWidth(long value) { }

	// [이성우]RebarTemplate 에서는 빠져 있는데 배근에서 사용되는 함수에 대해 사용할수 있게 처리
	virtual long GetMatLevelDifTopHookType() { return 1; }
	virtual long GetMatLevelDifBotHookType() { return 1; }
	virtual long GetFlatLevelDifTopHookType() { return 1; }
	virtual long GetFlatLevelDifBotHookType() { return 1; }

	void InitUserDowelSpliceLenMap(map<long, long> &mapUserDowelSpliceLen);
	void InitSubStirrupHookLenMap(map<long, long> &mapHookLen);
	void InitShearBarHookLenMap(map<long, long> &mapHookLenA, map<long, long> &mapHookLenB);
	void InitAddBarDiaIndexMap(map<long, long> &mapAddBarDiaIndex);
	void InitEndBarDiaIndexMap(map<long, long> &mapEndBarDiaIndex);
	void InitDefaultUbarLenMap(map<long, long> &mapDefaultUbarLen);
	void InitUserDowelHookLenMap(map<long, long> &mapUserDowelHookLen);
	void InitSubStirrupDiaIndexMap(map<long, long> &mapSubStirrupDiaIndex);
	
	void UserDowelSpliceLenMapDataCopy(map<long, long> &mapSrcUserDowelSpliceLen, map<long, long> &mapDesUserDowelSpliceLen);
	void SubStirrupHookLenMapDataCopy(map<long, long> &mapSrcHookLen, map<long, long> &mapDesHookLen);
	void ShearBarHookLenMapDataCopy(map<long, long> &mapSrcHookLenA, map<long, long> &mapSrcHookLenB, map<long, long> &mapDesHookLenA, map<long, long> &mapDesHookLenB);
	void AddBarDiaIndexMapDataCopy(map<long, long> &mapSrcAddBarDiaIndex, map<long, long> &mapDesAddBarDiaIndex);
	void EndBarDiaIndexMapDataCopy(map<long, long> &mapSrcEndBarDiaIndex, map<long, long> &mapDesEndBarDiaIndex);
	void DefaultUbarLenMapDataCopy(map<long, long> &mapSrcDefaultUbarLen, map<long, long> &mapDesDefaultUbarLen);
	void UserDowelHookLenMapDataCopy(map<long, long> &mapSrcUserDowelHookLen, map<long, long> &mapDesUserDowelHookLen);
	void SubStirrupDiaIndexMapDataCopy(map<long, long> &mapSrcSubStirrupDiaIndex, map<long, long> &mapDesSubStirrupDiaIndex);
};

