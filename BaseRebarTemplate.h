#pragma once
class ColumnVerBarRebarTemplate;
class ColumnDowelRebarTemplate;
class BWallVerBarRebarTemplate;
class BWallDowelRebarTemplate;
class RWallVerBarRebarTemplate;
class RWallDowelRebarTemplate;
class SlabHorSupportRebarsTemplate;
class ShearFrictionBarTemplate;
class VerMemberLinkDowelTemplate;

class StructureModel_CLASS BaseRebarTemplate
{
public:
	BaseRebarTemplate();
	~BaseRebarTemplate(void);
	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) {};
	virtual void Serialize(CArchive& ar) {};

	virtual VerMemberLinkDowelTemplate *GetColumnVerMemberLinkDowelTemplate() { return nullptr; }
	virtual VerMemberLinkDowelTemplate *GetBWallVerMemberLinkDowelTemplate() { return nullptr; }
	virtual VerMemberLinkDowelTemplate *GetRWallVerMemberLinkDowelTemplate() { return nullptr; }
	//Column
	//Get Property	

	virtual long GetColumnVerBarPlaceTypeDiaIndex() { return 0; }
	virtual ColumnVerBarRebarTemplate *GetColumnVerBarTemplateBelowDia() { return NULL; }
	virtual ColumnVerBarRebarTemplate *GetColumnVerBarTemplateExcessDia() { return NULL; }

	virtual long GetColumnDowelPlaceTypeDiaIndex() { return 0; }
	virtual ColumnDowelRebarTemplate *GetColumnDowelTemplateBelowDia() { return NULL; }
	virtual ColumnDowelRebarTemplate *GetColumnDowelTemplateExcessDia() { return NULL; }

	virtual long GetColumnVerBarSpliceType(double dDia) { return 0; }
	virtual long GetColumnVerBarTopLenType(double dDia) { return 0; }
	virtual long GetColumnVerBarTopFloorDevType(double dDia) { return 0; }
	virtual long GetColumnVerBarConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetColumnVerBarAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetColumnVerBarAlterSpliceDist(double dDia) { return 0; }

	virtual long GetColumnVerBarAlterSpliceType(double dDia) { return 0; }
	virtual long GetColumnDowelAlterSpliceType(double dDia) { return 0; }

	virtual BOOL GetColumnEndHoopBarShapeSet() { return FALSE; }
	virtual long GetColumnHoopBarShapeIdx() { return 0; }
	virtual long GetColumnTieBarShapeIdx() { return 0; }
	virtual BOOL GetColumnHoopBarStartLengthSet() { return FALSE; }
	virtual long GetColumnHoopBarStartLength() { return 0; }
	virtual long GetColumnTopMemberCover() { return 0; }
	virtual BOOL GetColumnTieAddLengthSet() { return FALSE; }
	virtual long GetColumnTieAddLength() { return 0; }
	virtual BOOL GetColumnOutsideColumnTieBarDelete() { return FALSE; }

	virtual long GetColumnDowelBarSpliceType(double dDia) { return 0; }
	virtual long GetColumnDowelTopLenType(double dDia) { return 0; }
	virtual long GetColumnDowelConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetColumnDowelAlterSpliceSet(double dDia) { return false; }
	virtual long GetColumnDowelAlterSpliceDist(double dDia) { return 0; }
	virtual long GetColumnDowelHoopRebarType(double dDia) { return 0; }
	virtual long GetColumnDowelHoopBySpace(double dDia) { return 0; }
	virtual long GetColumnDowelHoopByNum(double dDia) { return 0; }
	virtual long GetColumnDowelSupportBarDiaIndex(double dDia) { return 0; }
	virtual BOOL GetColumnDowelSupportBarUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetColumnDowelSupportBarLengthType(double dDia) { return 0; }
	virtual double GetColumnDowelSupportBarFootingThickMultiplicationValue(double dDia) { return 0; }
	virtual long GetColumnDowelSupportBarLength(double dDia) { return 0; }
	virtual long GetColumnDowelSupportBarCorrectiveLength(double dDia) { return 0; }
	virtual long GetColumnDowelSupportBarNum(double dDia) { return 0; }
	virtual long GetColumnDowelBotPlaceType(double dDia) { return 0; }
	virtual BOOL GetColumnDowelFootingPercentSet(double dDia) { return FALSE; }
	virtual long GetColumnDowelFootingPercent(double dDia) { return 0; }
	virtual BOOL GetColumnDowelFootingCoverSet(double dDia) { return FALSE; }
	virtual long GetColumnDowelHoopBarShapeIdx() { return 0; };
	virtual long GetColumnDowelFootingCover(double dDia) { return 0; }
	virtual BOOL GetColumnUserDowelHookLenSet(double dDia) { return FALSE; }

	//Set Property
	virtual void SetColumnVerBarPlaceTypeDiaIndex(long value) {}
	virtual void SetColumnDowelPlaceTypeDiaIndex(long value) {}

	virtual void SetColumnEndHoopBarShapeSet(BOOL value) { }
	virtual void SetColumnHoopBarShapeIdx(long value) { }
	virtual void SetColumnTieBarShapeIdx(long value) { }
	virtual void SetColumnHoopBarStartLengthSet(BOOL value) { }
	virtual void SetColumnHoopBarStartLength(long value) { }
	virtual void SetColumnTopMemberCover(long value) { }
	virtual void SetColumnTieAddLengthSet(BOOL value) { }
	virtual void SetColumnTieAddLength(long value) { }
	virtual void SetColumnOutsideColumnTieBarDelete(BOOL value) { }

	virtual void SetColumnDowelHoopBarShapeIdx(long value) {}

	virtual BOOL GetColumnDowelMainBarUserDiaSet() { return FALSE; }
	virtual void SetColumnDowelMainBarUserDiaSet(BOOL value) { }
	virtual BOOL GetColumnDowelHoopBarUserDiaSet() { return FALSE; }
	virtual void SetColumnDowelHoopBarUserDiaSet(BOOL value) { }

	virtual BOOL GetColumnDowelMainBarUserSteelGradeS() { return FALSE; }
	virtual void SetColumnDowelMainBarUserSteelGradeS(BOOL value) { }
	virtual BOOL GetColumnDowelHoopBarUserSteelGradeS() { return FALSE; }
	virtual void SetColumnDowelHoopBarUserSteelGradeS(BOOL value) { }
	//Function
	virtual map<long, long> GetColumnUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetColumnUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetColumnDowelMainBarUserDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetColumnDowelHoopBarUserDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }
	virtual void SetColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap) { }
	virtual void SetColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap) { }

	virtual double GetColumnUserDowelHookLen(double dDia) { return 0; }
	virtual long GetColumnUserDowelSpliceLen(double dDia) { return 0; }
	virtual double GetColumnDowelMainBarUserDia(double dDia) { return 0; }
	virtual double GetColumnDowelHoopBarUserDia(double dDia) { return 0; }

	virtual void SetColumnUserDowelHookLen(long key, long value) { }
	virtual void SetColumnUserDowelSpliceLen(long key, long value) { }
	virtual void SetColumnDowelMainBarUserDiaIndex(long key, long value) { }
	virtual void SetColumnDowelHoopBarUserDiaIndex(long key, long value) { }


	//TransferColumn
	//Get Property	
	virtual long GetTransferColumnVerBarPlaceTypeDiaIndex() { return 0; }
	virtual long GetTransferColumnDowelPlaceTypeDiaIndex() { return 0; }

	virtual long GetTransferColumnVerBarSpliceType(double dDia) { return 0; }
	virtual long GetTransferColumnVerBarTopLenType(double dDia) { return 0; }
	virtual long GetTransferColumnVerBarTopFloorDevType(double dDia) { return 0; }
	virtual long GetTransferColumnVerBarConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetTransferColumnVerBarAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetTransferColumnVerBarAlterSpliceDist(double dDia) { return 0; }

	virtual long GetTransferColumnVerBarAlterSpliceType(double dDia) { return 0; }
	virtual long GetTransferColumnDowelAlterSpliceType(double dDia) { return 0; }

	virtual BOOL GetTransferColumnEndHoopBarShapeSet() { return FALSE; }
	virtual long GetTransferColumnHoopBarShapeIdx() { return 0; }
	virtual long GetTransferColumnTieBarShapeIdx() { return 0; }
	virtual BOOL GetTransferColumnHoopBarStartLengthSet() { return FALSE; }
	virtual long GetTransferColumnHoopBarStartLength() { return 0; }
	virtual long GetTransferColumnTopMemberCover() { return 0; }
	virtual BOOL GetTransferColumnTieAddLengthSet() { return FALSE; }
	virtual long GetTransferColumnTieAddLength() { return 0; }
	virtual BOOL GetTransferColumnOutsideColumnTieBarDelete() { return FALSE; }

	virtual long GetTransferColumnDowelBarSpliceType(double dDia) { return 0; }
	virtual long GetTransferColumnDowelTopLenType(double dDia) { return 0; }
	virtual long GetTransferColumnDowelConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetTransferColumnDowelAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetTransferColumnDowelAlterSpliceDist(double dDia) { return 0; }
	virtual long GetTransferColumnDowelHoopRebarType(double dDia) { return 0; }
	virtual long GetTransferColumnDowelHoopBySpace(double dDia) { return 0; }
	virtual long GetTransferColumnDowelHoopByNum(double dDia) { return 0; }
	virtual long GetTransferColumnDowelSupportBarDiaIndex(double dDia) { return 0; }
	virtual BOOL GetTransferColumnDowelSupportBarUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetTransferColumnDowelSupportBarLength(double dDia) { return 0; }
	virtual long GetTransferColumnDowelSupportBarLengthType(double dDia) { return 0; }
	virtual double GetTransferColumnDowelSupportBarFootingThickMultiplicationValue(double dDia) { return 0; }
	virtual long GetTransferColumnDowelSupportBarCorrectiveLength(double dDia) { return 0; }
	virtual long GetTransferColumnDowelSupportBarNum(double dDia) { return 0; }
	virtual long GetTransferColumnDowelBotPlaceType(double dDia) { return 0; }
	virtual BOOL GetTransferColumnDowelFootingPercentSet(double dDia) { return FALSE; }
	virtual long GetTransferColumnDowelFootingPercent(double dDia) { return 0; }
	virtual BOOL GetTransferColumnDowelFootingCoverSet(double dDia) { return FALSE; }
	virtual long GetTransferColumnDowelHoopBarShapeIdx() { return 0; };
	virtual long GetTransferColumnDowelFootingCover(double dDia) { return 0; }
	virtual BOOL GetTransferColumnUserDowelHookLenSet(double dDia) { return FALSE; }

	//Set Property
	virtual void SetTransferColumnVerBarPlaceTypeDiaIndex(long value) { }
	virtual void SetTransferColumnDowelPlaceTypeDiaIndex(long value) { }
		
	virtual void SetTransferColumnEndHoopBarShapeSet(BOOL value) { }
	virtual void SetTransferColumnHoopBarShapeIdx(long value) { }
	virtual void SetTransferColumnTieBarShapeIdx(long value) { }
	virtual void SetTransferColumnHoopBarStartLengthSet(BOOL value) { }
	virtual void SetTransferColumnHoopBarStartLength(long value) { }
	virtual void SetTransferColumnTopMemberCover(long value) { }
	virtual void SetTransferColumnTieAddLengthSet(BOOL value) { }
	virtual void SetTransferColumnTieAddLength(long value) { }
	virtual void SetTransferColumnOutsideColumnTieBarDelete(BOOL value) { }
	virtual void SetTransferColumnDowelHoopBarShapeIdx(int value) { }

	//Function
	virtual map<long, long> GetTransferColumnUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferColumnUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferColumnDowelMainBarUserDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferColumnDowelHoopBarUserDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetTransferColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetTransferColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }
	virtual void SetTransferColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap) { }
	virtual void SetTransferColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap) { }

	virtual double GetTransferColumnUserDowelHookLen(double dDia) { return 0; }
	virtual long GetTransferColumnUserDowelSpliceLen(double dDia) { return 0; }
	virtual double GetTransferColumnDowelMainBarUserDia(double dDia) { return 0; }
	virtual double GetTransferColumnDowelHoopBarUserDia(double dDia) { return 0; }

	virtual void SetTransferColumnUserDowelHookLen(long key, long value) { }
	virtual void SetTransferColumnUserDowelSpliceLen(long key, long value) { }
	virtual void SetTransferColumnDowelMainBarUserDiaIndex(long key, long value) { }
	virtual void SetTransferColumnDowelHoopBarUserDiaIndex(long key, long value) { }

	virtual ColumnVerBarRebarTemplate *GetTransferColumnVerBarTemplateBelowDia() { return nullptr; }
	virtual ColumnVerBarRebarTemplate *GetTransferColumnVerBarTemplateExcessDia() { return nullptr; }
	virtual ColumnDowelRebarTemplate *GetTransferColumnDowelTemplateBelowDia() { return nullptr; }
	virtual ColumnDowelRebarTemplate *GetTransferColumnDowelTemplateExcessDia() { return nullptr; }
	
	virtual BOOL GetTransferColumnDowelMainBarUserDiaSet() { return FALSE; }
	virtual void SetTransferColumnDowelMainBarUserDiaSet(BOOL value) { }
	virtual BOOL GetTransferColumnDowelHoopBarUserDiaSet() { return FALSE; }
	virtual void SetTransferColumnDowelHoopBarUserDiaSet(BOOL value) { }

	virtual BOOL GetTransferColumnDowelMainBarUserSteelGradeS() { return FALSE; }
	virtual void SetTransferColumnDowelMainBarUserSteelGradeS(BOOL value) { }
	virtual BOOL GetTransferColumnDowelHoopBarUserSteelGradeS() { return FALSE; }
	virtual void SetTransferColumnDowelHoopBarUserSteelGradeS(BOOL value) { }

	virtual VerMemberLinkDowelTemplate *GetTransferColumnVerMemberLinkDowelTemplate() { return nullptr; }

	//ButtressColumn
	//Get Property
	virtual long GetButtressColumnVerBarPlaceTypeDiaIndex() { return 0; }
	virtual long GetButtressColumnDowelPlaceTypeDiaIndex() { return 0; }

	virtual long GetButtressColumnVerBarSpliceType(double dDia) { return 0; }
	virtual long GetButtressColumnVerBarTopLenType(double dDia) { return 0; }
	virtual long GetButtressColumnVerBarTopFloorDevType(double dDia) { return 0; }
	virtual long GetButtressColumnVerBarConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetButtressColumnVerBarAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetButtressColumnVerBarAlterSpliceDist(double dDia) { return 0; }

	virtual long GetButtressColumnDowelBarSpliceType(double dDia) { return 0; }
	virtual long GetButtressColumnDowelTopLenType(double dDia) { return 0; }
	virtual long GetButtressColumnDowelConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetButtressColumnDowelAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetButtressColumnDowelAlterSpliceDist(double dDia) { return 0; }
	virtual long GetButtressColumnDowelHoopRebarType(double dDia) { return 0; }
	virtual long GetButtressColumnDowelHoopBySpace(double dDia) { return 0; }
	virtual long GetButtressColumnDowelHoopByNum(double dDia) { return 0; }
	virtual long GetButtressColumnDowelSupportBarDiaIndex(double dDia) { return 0; }
	virtual BOOL GetButtressColumnDowelSupportBarUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetButtressColumnDowelSupportBarLength(double dDia) { return 0; }
	virtual long GetButtressColumnDowelSupportBarLengthType(double dDia) { return 0; }
	virtual double GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(double dDia) { return 0; }
	virtual long GetButtressColumnDowelSupportBarCorrectiveLength(double dDia) { return 0; }
	virtual long GetButtressColumnDowelSupportBarNum(double dDia) { return 0; }
	virtual long GetButtressColumnDowelBotPlaceType(double dDia) { return 0; }
	virtual BOOL GetButtressColumnDowelFootingPercentSet(double dDia) { return FALSE; }
	virtual long GetButtressColumnDowelFootingPercent(double dDia) { return 0; }
	virtual BOOL GetButtressColumnDowelFootingCoverSet(double dDia) { return FALSE; }
	virtual long GetButtressColumnDowelFootingCover(double dDia) { return 0; }
	virtual BOOL GetButtressColumnUserDowelHookLenSet(double dDia) { return FALSE; }

	//Set Property
	virtual void SetButtressColumnVerBarPlaceTypeDiaIndex(long value) { }
	virtual void SetButtressColumnDowelPlaceTypeDiaIndex(long value) { }

	//Function
	virtual map<long, long> GetButtressColumnUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetButtressColumnUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetButtressColumnDowelMainBarUserDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetButtressColumnDowelHoopBarUserDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetButtressColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetButtressColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }
	virtual void SetButtressColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap) { }
	virtual void SetButtressColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap) { }

	virtual double GetButtressColumnUserDowelHookLen(double dDia) { return 0; }
	virtual long GetButtressColumnUserDowelSpliceLen(double dDia) { return 0; }
	virtual double GetButtressColumnDowelMainBarUserDia(double dDia) { return 0; }
	virtual double GetButtressColumnDowelHoopBarUserDia(double dDia) { return 0; }

	virtual void SetButtressColumnUserDowelHookLen(long key, long value) { }
	virtual void SetButtressColumnUserDowelSpliceLen(long key, long value) { }
	virtual void SetButtressColumnDowelMainBarUserDiaIndex(long key, long value) { }
	virtual void SetButtressColumnDowelHoopBarUserDiaIndex(long key, long value) { }

	virtual ColumnVerBarRebarTemplate *GetButtressColumnVerBarTemplateBelowDia() { return NULL; }
	virtual ColumnVerBarRebarTemplate *GetButtressColumnVerBarTemplateExcessDia() { return NULL; }
	virtual ColumnDowelRebarTemplate *GetButtressColumnDowelTemplateBelowDia() { return NULL; }
	virtual ColumnDowelRebarTemplate *GetButtressColumnDowelTemplateExcessDia() { return NULL; }

	virtual BOOL GetButtressColumnDowelMainBarUserDiaSet() { return FALSE; }
	virtual void SetButtressColumnDowelMainBarUserDiaSet(BOOL value) { }
	virtual BOOL GetButtressColumnDowelHoopBarUserDiaSet() { return FALSE; }
	virtual void SetButtressColumnDowelHoopBarUserDiaSet(BOOL value) { }

	virtual BOOL GetButtressColumnDowelMainBarUserSteelGradeS() { return FALSE; }
	virtual void SetButtressColumnDowelMainBarUserSteelGradeS(BOOL value) { }
	virtual BOOL GetButtressColumnDowelHoopBarUserSteelGradeS() { return FALSE; }
	virtual void SetButtressColumnDowelHoopBarUserSteelGradeS(BOOL value) { }
	//Beam
	//Get Property
	virtual BOOL GetBeamGeneralInfoSet() { return FALSE; }
	virtual long GetBeamTopMaxLen2HookAnchorBar() { return 0; }
	virtual long GetBeamBotMaxLen2HookAnchorBar() { return 0; }
	virtual long GetBeamLineContinuityHorEcc() { return 0; }
	virtual long GetBeamLineContinuityVerEcc() { return 0; }
	virtual long GetBeamBarLayerGap() { return 0; }
	virtual long GetBeamMainBarConnectionTolerance() { return 0; }
	
	virtual BOOL GetBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetBeamSubBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetBeamSubBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetBeamSpliceDevConditionSet() { return 0; }
	virtual long GetBeamSpliceDevConditionLength() { return 0; }

	virtual BOOL GetBeamStirrupInfoSet() { return FALSE; }
	virtual long GetBeamShearBarType() { return 0; }
	virtual long GetBeamHorShearBarType() { return 0; }
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
	virtual long GetBeamSubStirrupStartSpacing() { return 0; }
	virtual long GetBeamSubTopStirrupType() { return 0; }
	virtual long GetBeamSubTopStirrupNum() { return 0; }
	virtual long GetBeamSubTopStirrupSpacing() { return 0; }
	virtual long GetBeamSubBotStirrupType() { return 0; }
	virtual long GetBeamSubBotStirrupNum() { return 0; }
	virtual long GetBeamSubBotStirrupSpacing() { return 0; }
	virtual long GetBeamSubStirrupTopLen() { return 0; }
	virtual long GetBeamSubStirrupBotLen() { return 0; }
	virtual BOOL GetBeamSubStirrupUserHookLen() { return FALSE; }
	virtual long GetBeamSubStirrupBarType() { return 0; }

	virtual BOOL GetBeamSupportRebarsSet() { return FALSE; }
	virtual long GetBeamSupportRebarsDiaIndex() { return 0; }
	virtual double GetBeamSupportRebarsTotalLengthRate() { return 0.0; }
	virtual long GetBeamSupportRebarsOrderLength() { return 0; }
	virtual long GetBeamSupportRebarsSpliceLength() { return 0; }
	virtual long GetBeamSupportRebarsCorrectiveLength() { return 0; }

	virtual long GetBeamCapBarPlaceType() { return 0; }
	virtual long GetBeamCapBarNum() { return 0; }

	virtual long GetSubBeamCapBarPlaceType() { return 0; }
	virtual long GetSubBeamCapBarNum() { return 0; }
	virtual BOOL GetBeamSupportRebarUserSteelGradeS() { return FALSE; }
	//Set Property
	virtual void SetBeamGeneralInfoSet(BOOL value) { }
	virtual void SetBeamTopMaxLen2HookAnchorBar(long value) { }
	virtual void SetBeamBotMaxLen2HookAnchorBar(long value) { }
	virtual void SetBeamLineContinuityHorEcc(long value) { }
	virtual void SetBeamLineContinuityVerEcc(long value) { }
	virtual void SetBeamBarLayerGap(long value) { }
	virtual void SetBeamMainBarConnectionTolerance(long value) { }
	
	virtual void SetBeamBottomBarEnd150Dev(BOOL value) { }
	virtual void SetBeamBottomBarLink150Dev(BOOL value) { }

	virtual void SetBeamSubBeamBottomBarEnd150Dev(BOOL value) {  }
	virtual void SetBeamSubBeamBottomBarLink150Dev(BOOL value) {  }

	virtual void SetBeamSpliceDevConditionSet(BOOL value) { }
	virtual void SetBeamSpliceDevConditionLength(long value) { }

	virtual void SetBeamStirrupInfoSet(BOOL value) { }
	virtual void SetBeamShearBarType(long value) { }
	virtual void SetBeamHorShearBarType(long value) { }
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
	virtual void SetBeamSubStirrupStartSpacing(long value) { }
	virtual void SetBeamSubTopStirrupType(long value) { }
	virtual void SetBeamSubTopStirrupNum(long value) { }
	virtual void SetBeamSubTopStirrupSpacing(long value) { }
	virtual void SetBeamSubBotStirrupType(long value) { }
	virtual void SetBeamSubBotStirrupNum(long value) { }
	virtual void SetBeamSubBotStirrupSpacing(long value) { }
	virtual void SetBeamSubStirrupTopLen(long value) { }
	virtual void SetBeamSubStirrupBotLen(long value) { }
	virtual void SetBeamSubStirrupUserHookLen(BOOL value) { }
	virtual void SetBeamSubStirrupBarType(long value) { }

	virtual void SetBeamSupportRebarsSet(BOOL value) { }
	virtual void SetBeamSupportRebarsDiaIndex(long value) { }
	virtual void SetBeamSupportRebarsTotalLengthRate(double value) { }
	virtual void SetBeamSupportRebarsOrderLength(long value) { }
	virtual void SetBeamSupportRebarsSpliceLength(long value) { }
	virtual void SetBeamSupportRebarsCorrectiveLength(long value) { }

	virtual void SetBeamCapBarPlaceType(long value) { }
	virtual void SetBeamCapBarNum(long value) { }
	virtual void SetSubBeamCapBarPlaceType(long value) { }
	virtual void SetSubBeamCapBarNum(long value) { }
	virtual void SetBeamSupportRebarUserSteelGradeS(BOOL value) { }
	//Function
	virtual map<long, long> GetBeamTopSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamBotSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamShearBarHookLenAMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamShearBarHookLenBMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamTopSubStirrupDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBeamBotSubStirrupDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA) { }
	virtual void SetBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB) { }
	virtual void SetBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }
	virtual void SetBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }


	virtual double GetBeamTopSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetBeamBotSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetBeamShearBarHookLenA(double dDia) { return 0; }
	virtual double GetBeamShearBarHookLenB(double dDia) { return 0; }
	virtual double GetBeamTopSubStirrupDia(double dDia) { return 0; }
	virtual double GetBeamBotSubStirrupDia(double dDia) { return 0; }

	virtual void SetBeamTopSubStirrupHookLen(long key, long value) { }
	virtual void SetBeamBotSubStirrupHookLen(long key, long value) { }
	virtual void SetBeamShearBarHookLenA(long key, long value) { }
	virtual void SetBeamShearBarHookLenB(long key, long value) { }
	virtual void SetBeamTopSubStirrupDiaIndex(long key, long value) { }
	virtual void SetBeamBotSubStirrupDiaIndex(long key, long value) { }

	virtual BOOL GetBeamSupportRebarsPlaceConditionSet() { return FALSE; }
	virtual long GetBeamSupportRebarsPlaceCondition() { return 0; }
	virtual void SetBeamSupportRebarsPlaceConditionSet(BOOL value) { }
	virtual void SetBeamSupportRebarsPlaceCondition(long value) { }

	virtual long GetBeamPlaceTypeDiaIndex() { return 0; }
	virtual void SetBeamPlaceTypeDiaIndex(long value) { }

	virtual long GetBeamSpliceTypeBelowDia() { return 0; }
	virtual long GetBeamSpliceWayBelowDia() { return 0; }
	virtual long GetBeamCZoneSpliceWayBelowDia() { return 0; }
	virtual long GetBeamCZoneSpliceLengthBelowDia() { return 0; }

	virtual long GetBeamSpliceTypeExcessDia() { return 0; }
	virtual long GetBeamSpliceWayExcessDia() { return 0; }
	virtual long GetBeamCZoneSpliceWayExcessDia() { return 0; }
	virtual long GetBeamCZoneSpliceLengthExcessDia() { return 0; }

	virtual void SetBeamSpliceTypeBelowDia(long value) { }
	virtual void SetBeamSpliceWayBelowDia(long value) { }
	virtual void SetBeamCZoneSpliceWayBelowDia(long value) { }
	virtual void SetBeamCZoneSpliceLengthBelowDia(long value) { }

	virtual void SetBeamSpliceTypeExcessDia(long value) { }
	virtual void SetBeamSpliceWayExcessDia(long value) { }
	virtual void SetBeamCZoneSpliceWayExcessDia(long value) { }
	virtual void SetBeamCZoneSpliceLengthExcessDia(long value) { }

	virtual long GetBeamSpliceType(double dDia) { return 0; }
	virtual long GetBeamSpliceWay(double dDia) { return 0; }
	virtual long GetBeamCZoneSpliceWay(double dDia) { return 0; }
	virtual long GetBeamCZoneSpliceLength(double dDia) { return 0; }
	
	//Transfer Beam
	//Get Property
	virtual BOOL GetTransferBeamGeneralInfoSet() { return FALSE; }
	virtual long GetTransferBeamTopMaxLen2HookAnchorBar() { return 0; }
	virtual long GetTransferBeamBotMaxLen2HookAnchorBar() { return 0; }
	virtual long GetTransferBeamLineContinuityHorEcc() { return 0; }
	virtual long GetTransferBeamLineContinuityVerEcc() { return 0; }
	virtual long GetTransferBeamBarLayerGap() { return 0; }
	virtual long GetTransferBeamMainBarConnectionTolerance() { return 0; }
	
	virtual BOOL GetTransferBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetTransferBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetTransferBeamSubBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetTransferBeamSubBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetTransferBeamSpliceDevConditionSet() { return 0; }
	virtual long GetTransferBeamSpliceDevConditionLength() { return 0; }

	virtual BOOL GetTransferBeamStirrupInfoSet() { return FALSE; }
	virtual long GetTransferBeamShearBarType() { return 0; }
	virtual long GetTransferBeamHorShearBarType() { return 0; }
	virtual long GetTransferBeamCapbarRatio() { return 0; }
	virtual long GetTransferBeamStirrupType() { return 0; }
	virtual long GetTransferBeamCoverBarType() { return 0; }
	virtual BOOL GetTransferBeamDeepDualUBARPlaceSet() { return FALSE; }
	virtual long GetTransferBeamDeepDepth() { return 0; }
	virtual long GetTransferBeamDeepSpliceType() { return 0; }
	virtual long GetTransferBeamStartSpacingType() { return 0; }

	virtual BOOL GetTransferBeamSubBeamInfoSet() { return FALSE; }
	virtual long GetTransferBeamSubBeamStirrupType() { return 0; }
	virtual long GetTransferBeamSubBeamCapbarRatio() { return 0; }
	virtual long GetTransferBeamSubBeamStirrupShape() { return 0; }
	virtual BOOL GetTransferBeamSubBeamOnlyCenterStirrupPlace() { return FALSE; }
	virtual long GetTransferBeamSubBeamStartSpacingType() { return 0; }

	virtual BOOL GetTransferBeamSubBarInfoSet() { return FALSE; }
	virtual long GetTransferBeamTopSubStirrupBarShape() { return 0; }
	virtual long GetTransferBeamBotSubStirrupBarShape() { return 0; }
	virtual long GetTransferBeamSubStirrupStartSpacing() { return 0; }
	virtual long GetTransferBeamSubTopStirrupType() { return 0; }
	virtual long GetTransferBeamSubTopStirrupNum() { return 0; }
	virtual long GetTransferBeamSubTopStirrupSpacing() { return 0; }
	virtual long GetTransferBeamSubBotStirrupType() { return 0; }
	virtual long GetTransferBeamSubBotStirrupNum() { return 0; }
	virtual long GetTransferBeamSubBotStirrupSpacing() { return 0; }
	virtual long GetTransferBeamSubStirrupTopLen() { return 0; }
	virtual long GetTransferBeamSubStirrupBotLen() { return 0; }
	virtual BOOL GetTransferBeamSubStirrupUserHookLen() { return FALSE; }
	virtual long GetTransferBeamSubStirrupBarType() { return 0; }

	virtual BOOL GetTransferBeamSupportRebarsSet() { return FALSE; }
	virtual long GetTransferBeamSupportRebarsDiaIndex() { return 0; }
	virtual double GetTransferBeamSupportRebarsTotalLengthRate() { return 0.0; }
	virtual long GetTransferBeamSupportRebarsOrderLength() { return 0; }
	virtual long GetTransferBeamSupportRebarsSpliceLength() { return 0; }
	virtual long GetTransferBeamSupportRebarsCorrectiveLength() { return 0; }

	virtual long GetTransferBeamCapBarPlaceType() { return 0; }
	virtual long GetTransferBeamCapBarNum() { return 0; }

	virtual long GetTransferBeamSubBeamCapBarPlaceType() { return 0; }
	virtual long GetTransferBeamSubBeamCapBarNum() { return 0; }
	virtual BOOL GetTransferBeamSupportRebarUserSteelGradeS() { return FALSE; }

	virtual long GetTransferBeamPlaceTypeDiaIndex() { return 0; }
	virtual void SetTransferBeamPlaceTypeDiaIndex(long value) { }

	virtual long GetTransferBeamSpliceTypeBelowDia() { return 0; }
	virtual long GetTransferBeamSpliceWayBelowDia() { return 0; }
	virtual long GetTransferBeamCZoneSpliceWayBelowDia() { return 0; }
	virtual long GetTransferBeamCZoneSpliceLengthBelowDia() { return 0; }

	virtual long GetTransferBeamSpliceTypeExcessDia() { return 0; }
	virtual long GetTransferBeamSpliceWayExcessDia() { return 0; }
	virtual long GetTransferBeamCZoneSpliceWayExcessDia() { return 0; }
	virtual long GetTransferBeamCZoneSpliceLengthExcessDia() { return 0; }

	virtual void SetTransferBeamSpliceTypeBelowDia(long value) { }
	virtual void SetTransferBeamSpliceWayBelowDia(long value) { }
	virtual void SetTransferBeamCZoneSpliceWayBelowDia(long value) { }
	virtual void SetTransferBeamCZoneSpliceLengthBelowDia(long value) { }

	virtual void SetTransferBeamSpliceTypeExcessDia(long value) { }
	virtual void SetTransferBeamSpliceWayExcessDia(long value) { }
	virtual void SetTransferBeamCZoneSpliceWayExcessDia(long value) { }
	virtual void SetTransferBeamCZoneSpliceLengthExcessDia(long value) { }

	virtual long GetTransferBeamSpliceType(double dDia) { return 0; }
	virtual long GetTransferBeamSpliceWay(double dDia) { return 0; }
	virtual long GetTransferBeamCZoneSpliceWay(double dDia) { return 0; }
	virtual long GetTransferBeamCZoneSpliceLength(double dDia) { return 0; }
	//Set Property
	virtual void SetTransferBeamGeneralInfoSet(BOOL value) { }
	virtual void SetTransferBeamTopMaxLen2HookAnchorBar(long value) { }
	virtual void SetTransferBeamBotMaxLen2HookAnchorBar(long value) { }
	virtual void SetTransferBeamLineContinuityHorEcc(long value) { }
	virtual void SetTransferBeamLineContinuityVerEcc(long value) { }
	virtual void SetTransferBeamBarLayerGap(long value) { }
	virtual void SetTransferBeamMainBarConnectionTolerance(long value) { }
	virtual void SetTransferBeamTopSpliceType(long value) { }
	virtual void SetTransferBeamTopSpliceWay(long value) { }
	virtual void SetTransferBeamCZoneSpliceWay(long value) {  }
	virtual void SetTransferBeamCZoneSpliceLength(long value) {  }
	virtual void SetTransferBeamBottomBarEnd150Dev(BOOL value) { }
	virtual void SetTransferBeamBottomBarLink150Dev(BOOL value) { }

	virtual void SetTransferBeamSubBeamBottomBarEnd150Dev(BOOL value) {  }
	virtual void SetTransferBeamSubBeamBottomBarLink150Dev(BOOL value) {  }

	virtual void SetTransferBeamSpliceDevConditionSet(BOOL value) { }
	virtual void SetTransferBeamSpliceDevConditionLength(long value) { }

	virtual void SetTransferBeamStirrupInfoSet(BOOL value) { }
	virtual void SetTransferBeamShearBarType(long value) { }
	virtual void SetTransferBeamHorShearBarType(long value) { }
	virtual void SetTransferBeamCapbarRatio(long value) { }
	virtual void SetTransferBeamStirrupType(long value) { }
	virtual void SetTransferBeamCoverBarType(long value) { }
	virtual void SetTransferBeamDeepDualUBARPlaceSet(BOOL value) { }
	virtual void SetTransferBeamDeepDepth(long value) { }
	virtual void SetTransferBeamDeepSpliceType(long value) { }
	virtual void SetTransferBeamStartSpacingType(long value) { }

	virtual void SetTransferBeamSubBeamInfoSet(BOOL value) { }
	virtual void SetTransferBeamSubBeamStirrupType(long value) { }
	virtual void SetTransferBeamSubBeamCapbarRatio(long value) { }
	virtual void SetTransferBeamSubBeamStirrupShape(long value) { }
	virtual void SetTransferBeamSubBeamOnlyCenterStirrupPlace(BOOL value) { }
	virtual void SetTransferBeamSubBeamStartSpacingType(long value) { }

	virtual void SetTransferBeamSubBarInfoSet(BOOL value) { }
	virtual void SetTransferBeamTopSubStirrupBarShape(long value) { }
	virtual void SetTransferBeamBotSubStirrupBarShape(long value) { }
	virtual void SetTransferBeamSubStirrupStartSpacing(long value) { }
	virtual void SetTransferBeamSubTopStirrupType(long value) { }
	virtual void SetTransferBeamSubTopStirrupNum(long value) { }
	virtual void SetTransferBeamSubTopStirrupSpacing(long value) { }
	virtual void SetTransferBeamSubBotStirrupType(long value) { }
	virtual void SetTransferBeamSubBotStirrupNum(long value) { }
	virtual void SetTransferBeamSubBotStirrupSpacing(long value) { }
	virtual void SetTransferBeamSubStirrupTopLen(long value) { }
	virtual void SetTransferBeamSubStirrupBotLen(long value) { }
	virtual void SetTransferBeamSubStirrupUserHookLen(BOOL value) { }
	virtual void SetTransferBeamSubStirrupBarType(long value) { }

	virtual void SetTransferBeamSupportRebarsSet(BOOL value) { }
	virtual void SetTransferBeamSupportRebarsDiaIndex(long value) { }
	virtual void SetTransferBeamSupportRebarsTotalLengthRate(double value) { }
	virtual void SetTransferBeamSupportRebarsOrderLength(long value) { }
	virtual void SetTransferBeamSupportRebarsSpliceLength(long value) { }
	virtual void SetTransferBeamSupportRebarsCorrectiveLength(long value) { }

	virtual void SetTransferBeamCapBarPlaceType(long value) { }
	virtual void SetTransferBeamCapBarNum(long value) { }
	virtual void SetTransferBeamSubBeamCapBarPlaceType(long value) { }
	virtual void SetTransferBeamSubBeamCapBarNum(long value) { }
	virtual void SetTransferBeamSupportRebarUserSteelGradeS(BOOL value) { }
	//Function
	virtual map<long, long> GetTransferBeamTopSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferBeamBotSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferBeamShearBarHookLenAMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferBeamShearBarHookLenBMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferBeamTopSubStirrupDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetTransferBeamBotSubStirrupDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetTransferBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetTransferBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetTransferBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA) { }
	virtual void SetTransferBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB) { }
	virtual void SetTransferBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }
	virtual void SetTransferBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }

	virtual double GetTransferBeamTopSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetTransferBeamBotSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetTransferBeamShearBarHookLenA(double dDia) { return 0; }
	virtual double GetTransferBeamShearBarHookLenB(double dDia) { return 0; }
	virtual double GetTransferBeamTopSubStirrupDia(double dDia) { return 0; }
	virtual double GetTransferBeamBotSubStirrupDia(double dDia) { return 0; }

	virtual void SetTransferBeamTopSubStirrupHookLen(long key, long value) { }
	virtual void SetTransferBeamBotSubStirrupHookLen(long key, long value) { }
	virtual void SetTransferBeamShearBarHookLenA(long key, long value) { }
	virtual void SetTransferBeamShearBarHookLenB(long key, long value) { }
	virtual void SetTransferBeamTopSubStirrupDiaIndex(long key, long value) { }
	virtual void SetTransferBeamBotSubStirrupDiaIndex(long key, long value) { }

	virtual BOOL GetTransferBeamSupportRebarsPlaceConditionSet() { return FALSE; }
	virtual long GetTransferBeamSupportRebarsPlaceCondition() { return 0; }
	virtual void SetTransferBeamSupportRebarsPlaceConditionSet(BOOL value) { }
	virtual void SetTransferBeamSupportRebarsPlaceCondition(long value) { }
	//DH Beam
	virtual BOOL GetDeckBeamGeneralInfoSet() { return FALSE; }
	virtual long GetDeckBeamTopMaxLen2HookAnchorBar() { return 0; }
	virtual long GetDeckBeamBotMaxLen2HookAnchorBar() { return 0; }
	virtual long GetDeckBeamLineContinuityHorEcc() { return 0; }
	virtual long GetDeckBeamLineContinuityVerEcc() { return 0; }
	virtual long GetDeckBeamBarLayerGap() { return 0; }
	virtual long GetDeckBeamMainBarConnectionTolerance() { return 0; }
	
	virtual BOOL GetDeckBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetDeckBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetDeckBeamSubBeamBottomBarEnd150Dev() { return  FALSE; }
	virtual BOOL GetDeckBeamSubBeamBottomBarLink150Dev() { return  FALSE; }

	virtual BOOL GetDeckBeamSpliceDevConditionSet() { return 0; }
	virtual long GetDeckBeamSpliceDevConditionLength() { return 0; }

	virtual BOOL GetDeckBeamStirrupInfoSet() { return FALSE; }
	virtual long GetDeckBeamShearBarType() { return 0; }
	virtual long GetDeckBeamHorShearBarType() { return 0; }
	virtual long GetDeckBeamCapbarRatio() { return 0; }
	virtual long GetDeckBeamStirrupType() { return 0; }
	virtual long GetDeckBeamCoverBarType() { return 0; }
	
	virtual long GetDeckBeamStartSpacingType() { return 0; }

	virtual BOOL GetDeckBeamSubBeamInfoSet() { return FALSE; }
	virtual long GetDeckBeamSubBeamStirrupType() { return 0; }
	virtual long GetDeckBeamSubBeamCapbarRatio() { return 0; }
	virtual long GetDeckBeamSubBeamStirrupShape() { return 0; }
	virtual BOOL GetDeckBeamSubBeamOnlyCenterStirrupPlace() { return FALSE; }
	virtual long GetDeckBeamSubBeamStartSpacingType() { return 0; }

	virtual BOOL GetDeckBeamSubBarInfoSet() { return FALSE; }
	virtual long GetDeckBeamTopSubStirrupBarShape() { return 0; }
	virtual long GetDeckBeamBotSubStirrupBarShape() { return 0; }
	virtual long GetDeckBeamSubStirrupStartSpacing() { return 0; }
	virtual long GetDeckBeamSubTopStirrupType() { return 0; }
	virtual long GetDeckBeamSubTopStirrupNum() { return 0; }
	virtual long GetDeckBeamSubTopStirrupSpacing() { return 0; }
	virtual long GetDeckBeamSubBotStirrupType() { return 0; }
	virtual long GetDeckBeamSubBotStirrupNum() { return 0; }
	virtual long GetDeckBeamSubBotStirrupSpacing() { return 0; }
	virtual long GetDeckBeamSubStirrupTopLen() { return 0; }
	virtual long GetDeckBeamSubStirrupBotLen() { return 0; }
	virtual BOOL GetDeckBeamSubStirrupUserHookLen() { return FALSE; }
	virtual long GetDeckBeamSubStirrupBarType() { return 0; }

	virtual long GetDeckBeamCapBarPlaceType() { return 0; }
	virtual long GetDeckBeamCapBarNum() { return 0; }

	virtual long GetDeckBeamSubBeamCapBarPlaceType() { return 0; }
	virtual long GetDeckBeamSubBeamCapBarNum() { return 0; }
	
	virtual long GetDeckBeamPlaceTypeDiaIndex() { return 0; }
	virtual void SetDeckBeamPlaceTypeDiaIndex(long value) { }

	virtual long GetDeckBeamSpliceTypeBelowDia() { return 0; }
	virtual long GetDeckBeamSpliceWayBelowDia() { return 0; }
	virtual long GetDeckBeamCZoneSpliceWayBelowDia() { return 0; }
	virtual long GetDeckBeamCZoneSpliceLengthBelowDia() { return 0; }

	virtual long GetDeckBeamSpliceTypeExcessDia() { return 0; }
	virtual long GetDeckBeamSpliceWayExcessDia() { return 0; }
	virtual long GetDeckBeamCZoneSpliceWayExcessDia() { return 0; }
	virtual long GetDeckBeamCZoneSpliceLengthExcessDia() { return 0; }

	virtual void SetDeckBeamSpliceTypeBelowDia(long value) { }
	virtual void SetDeckBeamSpliceWayBelowDia(long value) { }
	virtual void SetDeckBeamCZoneSpliceWayBelowDia(long value) { }
	virtual void SetDeckBeamCZoneSpliceLengthBelowDia(long value) { }

	virtual void SetDeckBeamSpliceTypeExcessDia(long value) { }
	virtual void SetDeckBeamSpliceWayExcessDia(long value) { }
	virtual void SetDeckBeamCZoneSpliceWayExcessDia(long value) { }
	virtual void SetDeckBeamCZoneSpliceLengthExcessDia(long value) { }

	virtual long GetDeckBeamSpliceType(double dDia) { return 0; }
	virtual long GetDeckBeamSpliceWay(double dDia) { return 0; }
	virtual long GetDeckBeamCZoneSpliceWay(double dDia) { return 0; }
	virtual long GetDeckBeamCZoneSpliceLength(double dDia) { return 0; }
	//Set Property
	virtual void SetDeckBeamGeneralInfoSet(BOOL value) { }
	virtual void SetDeckBeamTopMaxLen2HookAnchorBar(long value) { }
	virtual void SetDeckBeamBotMaxLen2HookAnchorBar(long value) { }
	virtual void SetDeckBeamLineContinuityHorEcc(long value) { }
	virtual void SetDeckBeamLineContinuityVerEcc(long value) { }
	virtual void SetDeckBeamBarLayerGap(long value) { }
	virtual void SetDeckBeamMainBarConnectionTolerance(long value) { }
	virtual void SetDeckBeamTopSpliceType(long value) { }
	virtual void SetDeckBeamTopSpliceWay(long value) { }
	virtual void SetDeckBeamCZoneSpliceWay(long value) {  }
	virtual void SetDeckBeamCZoneSpliceLength(long value) {  }
	virtual void SetDeckBeamBottomBarEnd150Dev(BOOL value) { }
	virtual void SetDeckBeamBottomBarLink150Dev(BOOL value) { }

	virtual void SetDeckBeamSubBeamBottomBarEnd150Dev(BOOL value) {  }
	virtual void SetDeckBeamSubBeamBottomBarLink150Dev(BOOL value) {  }

	virtual void SetDeckBeamSpliceDevConditionSet(BOOL value) { }
	virtual void SetDeckBeamSpliceDevConditionLength(long value) { }

	virtual void SetDeckBeamStirrupInfoSet(BOOL value) { }
	virtual void SetDeckBeamShearBarType(long value) { }
	virtual void SetDeckBeamHorShearBarType(long value) { }
	virtual void SetDeckBeamCapbarRatio(long value) { }
	virtual void SetDeckBeamStirrupType(long value) { }
	virtual void SetDeckBeamCoverBarType(long value) { }
	
	virtual void SetDeckBeamStartSpacingType(long value) { }

	virtual void SetDeckBeamSubBeamInfoSet(BOOL value) { }
	virtual void SetDeckBeamSubBeamStirrupType(long value) { }
	virtual void SetDeckBeamSubBeamCapbarRatio(long value) { }
	virtual void SetDeckBeamSubBeamStirrupShape(long value) { }
	virtual void SetDeckBeamSubBeamOnlyCenterStirrupPlace(BOOL value) { }
	virtual void SetDeckBeamSubBeamStartSpacingType(long value) { }

	virtual void SetDeckBeamSubBarInfoSet(BOOL value) { }
	virtual void SetDeckBeamTopSubStirrupBarShape(long value) { }
	virtual void SetDeckBeamBotSubStirrupBarShape(long value) { }
	virtual void SetDeckBeamSubStirrupStartSpacing(long value) { }
	virtual void SetDeckBeamSubTopStirrupType(long value) { }
	virtual void SetDeckBeamSubTopStirrupNum(long value) { }
	virtual void SetDeckBeamSubTopStirrupSpacing(long value) { }
	virtual void SetDeckBeamSubBotStirrupType(long value) { }
	virtual void SetDeckBeamSubBotStirrupNum(long value) { }
	virtual void SetDeckBeamSubBotStirrupSpacing(long value) { }
	virtual void SetDeckBeamSubStirrupTopLen(long value) { }
	virtual void SetDeckBeamSubStirrupBotLen(long value) { }
	virtual void SetDeckBeamSubStirrupUserHookLen(BOOL value) { }
	virtual void SetDeckBeamSubStirrupBarType(long value) { }

	virtual void SetDeckBeamCapBarPlaceType(long value) { }
	virtual void SetDeckBeamCapBarNum(long value) { }
	virtual void SetDeckBeamSubBeamCapBarPlaceType(long value) { }
	virtual void SetDeckBeamSubBeamCapBarNum(long value) { }
	
	//Function
	virtual map<long, long> GetDeckBeamTopSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetDeckBeamBotSubStirrupHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetDeckBeamShearBarHookLenAMap() { map<long, long> value; return value; }
	virtual map<long, long> GetDeckBeamShearBarHookLenBMap() { map<long, long> value; return value; }
	virtual map<long, long> GetDeckBeamTopSubStirrupDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetDeckBeamBotSubStirrupDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetDeckBeamTopSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetDeckBeamBotSubStirrupHookLenMap(map<long, long> mapSubStirrupHookLen) { }
	virtual void SetDeckBeamShearBarHookLenAMap(map<long, long> mapShearBarHookLenA) { }
	virtual void SetDeckBeamShearBarHookLenBMap(map<long, long> mapShearBarHookLenB) { }
	virtual void SetDeckBeamTopSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }
	virtual void SetDeckBeamBotSubStirrupDiaIndexMap(map<long, long> mapSubStirrupDiaIndex) { }

	virtual double GetDeckBeamTopSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetDeckBeamBotSubStirrupHookLen(double dDia) { return 0; }
	virtual double GetDeckBeamShearBarHookLenA(double dDia) { return 0; }
	virtual double GetDeckBeamShearBarHookLenB(double dDia) { return 0; }
	virtual double GetDeckBeamTopSubStirrupDia(double dDia) { return 0; }
	virtual double GetDeckBeamBotSubStirrupDia(double dDia) { return 0; }

	virtual void SetDeckBeamTopSubStirrupHookLen(long key, long value) { }
	virtual void SetDeckBeamBotSubStirrupHookLen(long key, long value) { }
	virtual void SetDeckBeamShearBarHookLenA(long key, long value) { }
	virtual void SetDeckBeamShearBarHookLenB(long key, long value) { }
	virtual void SetDeckBeamTopSubStirrupDiaIndex(long key, long value) { }
	virtual void SetDeckBeamBotSubStirrupDiaIndex(long key, long value) { }

	
	//BWall
	//Get Property

	virtual long GetBWallVerBarPlaceTypeDiaIndex() { return 0; }
	virtual BWallVerBarRebarTemplate *GetBWallVerBarTemplateBelowDia() { return NULL; }
	virtual BWallVerBarRebarTemplate *GetBWallVerBarTemplateExcessDia() { return NULL; }

	virtual long GetBWallDowelPlaceTypeDiaIndex() { return 0; }
	virtual BWallDowelRebarTemplate *GetBWallDowelTemplateBelowDia() { return NULL; }
	virtual BWallDowelRebarTemplate *GetBWallDowelTemplateExcessDia() { return NULL; }

	virtual long GetBWallVerBarSpliceType(double dDia) { return 0; }
	virtual long GetBWallVerBarTopLenType(double dDia) { return 0; }
	virtual long GetBWallVerBarTopFloorDevType(double dDia) { return 0; }
	virtual long GetBWallVerBarConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetBWallVerBarAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetBWallVerBarAlterSpliceDist(double dDia) { return 0; }
	virtual long GetBWallVerNumBarCrossBarIncludeSet() { return 0; }

	virtual long GetBWallVerBarAlterSpliceType(double dDia) { return 0; }
	virtual long GetBWallDowelAlterSpliceType(double dDia) { return 0; }

	virtual long GetBWallDowelBarSpliceType(double dDia) { return 0; }
	virtual long GetBWallDowelBarTopLenType(double dDia) { return 0; }
	virtual long GetBWallDowelDowelShape(double dDia) { return 0; }
	virtual long GetBWallDowelConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetBWallDowelAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetBWallDowelAlterSpliceDist(double dDia) { return 0; }
	virtual long GetBWallDowelSupportBarDiaIndex(double dDia) { return 0; }
	virtual BOOL GetBWallDowelSupportBarUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetBWallDowelSupportBarNum(double dDia) { return 0; }
	virtual long GetBWallDowelBarAnchorType(double dDia) { return 0; }
	virtual BOOL GetBWallDowelHookAnchorPlaceSpacingSet(double dDia) { return FALSE; }
	virtual long GetBWallDowelHookAnchorPlaceSpacing(double dDia) { return 0; }
	virtual BOOL GetBWallUserDowelHookLenSet(double dDia) { return FALSE; }

	virtual long GetBWallCrossBarEAType() { return 0; }
	virtual BOOL GetBWallCrossBarEATypeExcessThickSet() { return FALSE; }
	virtual long GetBWallCrossBarEATypeExcessThick() { return 0; }
	virtual long GetBWallCrossBarStartSpacingType() { return 0; }
	virtual BOOL GetBWallPlaceEndBarSet() { return FALSE; }
	virtual long GetBWallEndVerAddBarCount() { return 0; }
	virtual long GetBWallEndVerAddBarDist() { return 0; }
	virtual long GetBWallEndVerStartSpacingType() { return 0; }

	virtual BOOL GetBWallPlaceBraceSupportBar() { return FALSE; }
	virtual long GetBWallBraceSupportBarOverDiaIndex() { return 0; }
	virtual BOOL GetBWallBraceSupportBarUserSteelGradeS() { return FALSE; }
	virtual long GetBWallBraceSupportBarWallLengthCondition() { return 0; }
	virtual long GetBWallBraceSupportBarWallLengthConditionOverLength() { return 0; }
	virtual long GetBWallBraceSupportBarWallLengthConditionUnderLength() { return 0; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionDiaSet() { return FALSE; }
	virtual long GetBWallBraceSupportNotPlaceConditionDiaIndex() { return 0; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionLengthSet() { return FALSE; }
	virtual long GetBWallBraceSupportNotPlaceConditionLength() { return 0; }
	virtual BOOL GetBWallBraceSupportNotPlaceConditionBottomDowel() { return FALSE; }

	virtual BOOL GetBWallBraceSupportNotPlaceConditionHeightSet() { return FALSE; }
	virtual long GetBWallBraceSupportNotPlaceConditionHeight() { return 0; }

	virtual BOOL GetBWallPlaceUBar4ShortWall() { return FALSE; }
	virtual long GetBWallMaxUbarLegLength() { return 0; }
	virtual BOOL GetBWallPlaceStirrup4ShortWall() { return FALSE; }
	virtual long GetBWallHBarMaxStirrupLen() { return 0; }
	virtual BOOL GetBWallHorBarStartSet() { return FALSE; }
	virtual long GetBWallHorBarStartLength() { return 0; }
	virtual BOOL GetBWallDefaultUbarLen() { return FALSE; }
	virtual BOOL GetBWallCrossBarUBarHalfPlace() { return FALSE; }
	virtual BOOL GetBWallStraightEndUBarPlaceSet() { return FALSE; }

	virtual long GetBWallSpacerDiaType() { return 0; }
	virtual long GetBWallSpacerDiaIndex() { return 0; }
	virtual BOOL GetBWallSpacerUserSteelGradeS() { return FALSE; }
	virtual long GetBWallSpacerHorSpacing() { return 0; }
	virtual long GetBWallSpacerVerSpacingType() { return 0; }
	virtual long GetBWallSpacerVerSpacing() { return 0; }
	virtual long GetBWallSpacerVerSpacingRate() { return 0; }
	virtual long GetBWallSpacerVerSpacingCount() { return 0; }
	virtual BOOL GetBWallSpacerVerEndBarPlaceSet() { return FALSE; }
	virtual long GetBWallSpacerVerEndBarSpacingType() { return 0; }
	virtual long GetBWallSpacerVerEndBarSpacing() { return 0; }
	virtual long GetBWallSpacerVerEndBarSpacingRate() { return 0; }
	virtual long GetBWallSpacerVerEndBarSpacingCount() { return 0; }
	virtual long GetBWallDowelSpacerVerSpacingCount() { return 0; }
	virtual long GetBWallSpacerLenType() { return 0; }
	virtual long GetBWallSpacerLenCalWayA() { return 0; }
	virtual long GetBWallSpacerLenCalWayB() { return 0; }
	virtual long GetBWallSpacerLenA() { return 0; }
	virtual long GetBWallSpacerLenB() { return 0; }
	virtual long GetBWallSpacerShape() { return 0; }
	virtual BOOL GetBWallSpacerRebarPlaceSet() { return FALSE; }
	virtual long GetBWallSpacerRebarPlaceCondition() { return 0; }

	virtual BOOL GetBWallSupportRebarsSet() { return FALSE; }
	virtual long GetBWallSupportRebarsDiaIndex() { return 0; }
	virtual BOOL GetBWallSupportRebarsUserSteelGradeS() { return FALSE; }
	virtual double GetBWallSupportRebarsTotalLengthRate() { return 0.0; }
	virtual long GetBWallSupportRebarsOrderLength() { return 0; }
	virtual long GetBWallSupportRebarsSpliceLength() { return 0; }
	virtual long GetBWallSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetBWallAddBarsSet() { return FALSE; }
	virtual long GetBWallAddBarsDiaIndex() { return 0; }
	virtual BOOL GetBWallAddBarsUserSteelGradeS() { return FALSE; }
	virtual long GetBWallAddBarsNumber() { return 0; }
	virtual long GetBWallAddBarsLengthType() { return 0; }
	virtual long GetBWallAddBarsTensionLengthType() { return 0; }
	virtual long GetBWallAddBarsUserLength() { return 0; }
	virtual BOOL GetBWallDowelSupportPlaceType() { return FALSE; }

	virtual BOOL GetBWallShearBarSet() { return FALSE; }
	virtual long GetBWallShearBarDiaType() { return 0; }
	virtual long GetBWallShearBarDiaIndex() { return 0; }
	virtual BOOL GetBWallShearBarUserSteelGradeS() { return FALSE; }
	virtual long GetBWallShearBarUserDiaType() { return 0; }

	virtual long GetBWallShearBarShapeType() { return 0; }

	virtual long GetBWallShearBarVerSpacingCondition() { return 0; }
	virtual long GetBWallShearBarVerBelowType() { return 0; }
	virtual long GetBWallShearBarVerBelowSpacing() { return 0; }
	virtual long GetBWallShearBarVerBelowRatio() { return 0; }
	virtual long GetBWallShearBarVerBelowNum() { return 0; }
	virtual long GetBWallShearBarVerOverType() { return 0; }
	virtual long GetBWallShearBarVerOverSpacing() { return 0; }
	virtual long GetBWallShearBarVerOverRatio() { return 0; }
	virtual long GetBWallShearBarVerOverNum() { return 0; }

	virtual long GetBWallShearBarHorSpacingCondition() { return 0; }
	virtual long GetBWallShearBarHorBelowType() { return 0; }
	virtual long GetBWallShearBarHorBelowSpacing() { return 0; }
	virtual long GetBWallShearBarHorBelowRatio() { return 0; }
	virtual long GetBWallShearBarHorBelowNum() { return 0; }
	virtual long GetBWallShearBarHorOverType() { return 0; }
	virtual long GetBWallShearBarHorOverSpacing() { return 0; }
	virtual long GetBWallShearBarHorOverRatio() { return 0; }
	virtual long GetBWallShearBarHorOverNum() { return 0; }

	virtual BOOL GetBWallUBarAddLengthSet() { return FALSE; }
	virtual long GetBWallUBarAddLength() { return 0; }
	virtual BOOL GetBWallSpacerAddLengthSet() { return FALSE; }
	virtual long GetBWallSpacerAddLength() { return 0; }

	//Set Property
	virtual void SetBWallVerBarPlaceTypeDiaIndex(long value) {}
	virtual void SetBWallDowelPlaceTypeDiaIndex(long value) {}

	virtual void SetBWallVerNumBarCrossBarIncludeSet(long value) { }
	virtual void SetBWallCrossBarEAType(long value) { }
	virtual void SetBWallCrossBarEATypeExcessThickSet(BOOL value) { }
	virtual void SetBWallCrossBarEATypeExcessThick(long value) { }
	virtual void SetBWallCrossBarStartSpacingType(long value) { }
	virtual void SetBWallPlaceEndBarSet(BOOL value) { }
	virtual void SetBWallEndVerAddBarCount(long value) { }
	virtual void SetBWallEndVerAddBarDist(long value) { }
	virtual void SetBWallEndVerStartSpacingType(long value) { }

	virtual void SetBWallPlaceBraceSupportBar(BOOL value) { }
	virtual void SetBWallBraceSupportBarOverDiaIndex(long value) { }
	virtual void SetBWallBraceSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetBWallBraceSupportBarWallLengthCondition(long value) { }
	virtual void SetBWallBraceSupportBarWallLengthConditionOverLength(long value) { }
	virtual void SetBWallBraceSupportBarWallLengthConditionUnderLength(long value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionDiaSet(BOOL value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionDiaIndex(long value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionLengthSet(BOOL value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionLength(long value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionBottomDowel(BOOL value) { }

	virtual void SetBWallBraceSupportNotPlaceConditionHeightSet(BOOL value) { }
	virtual void SetBWallBraceSupportNotPlaceConditionHeight(long value) { }

	virtual void SetBWallPlaceUBar4ShortWall(BOOL value) { }
	virtual void SetBWallMaxUbarLegLength(long value) { }
	virtual void SetBWallPlaceStirrup4ShortWall(BOOL value) { }
	virtual void SetBWallHBarMaxStirrupLen(long value) { }
	virtual void SetBWallHorBarStartSet(BOOL value) { }
	virtual void SetBWallHorBarStartLength(long value) { }
	virtual void SetBWallDefaultUbarLen(BOOL value) { }
	virtual void SetBWallCrossBarUBarHalfPlace(BOOL value) { }
	virtual void SetBWallStraightEndUBarPlaceSet(BOOL value) { }

	virtual void SetBWallSpacerDiaType(long value) { }
	virtual void SetBWallSpacerDiaIndex(long value) { }
	virtual void SetBWallSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetBWallSpacerHorSpacing(long value) { }
	virtual void SetBWallSpacerVerSpacingType(long value) { }
	virtual void SetBWallSpacerVerSpacing(long value) { }
	virtual void SetBWallSpacerVerSpacingRate(long value) { }
	virtual void SetBWallSpacerVerSpacingCount(long value) { }
	virtual void SetBWallSpacerVerEndBarPlaceSet(BOOL value) { }
	virtual void SetBWallSpacerVerEndBarSpacingType(long value) { }
	virtual void SetBWallSpacerVerEndBarSpacing(long value) { }
	virtual void SetBWallSpacerVerEndBarSpacingRate(long value) { }
	virtual void SetBWallSpacerVerEndBarSpacingCount(long value) { }
	virtual void SetBWallDowelSpacerVerSpacingCount(long value) { }
	virtual void SetBWallSpacerLenType(long value) { }
	virtual void SetBWallSpacerLenCalWayA(long value) { }
	virtual void SetBWallSpacerLenCalWayB(long value) { }
	virtual void SetBWallSpacerLenA(long value) { }
	virtual void SetBWallSpacerLenB(long value) { }
	virtual void SetBWallSpacerShape(long value) { }
	virtual void SetBWallSpacerRebarPlaceSet(BOOL value) { }
	virtual void SetBWallSpacerRebarPlaceCondition(long value) { }

	virtual void SetBWallSupportRebarsSet(BOOL value) { }
	virtual void SetBWallSupportRebarsDiaIndex(long value) { }
	virtual void SetBWallSupportRebarsUserSteelGradeS(BOOL value) { }
	virtual void SetBWallSupportRebarsTotalLengthRate(double value) { }
	virtual void SetBWallSupportRebarsOrderLength(long value) { }
	virtual void SetBWallSupportRebarsSpliceLength(long value) { }
	virtual void SetBWallSupportRebarsCorrectiveLength(long value) { }

	virtual void SetBWallAddBarsSet(BOOL value) { }
	virtual void SetBWallAddBarsDiaIndex(long value) { }
	virtual void SetBWallAddBarsUserSteelGradeS(BOOL value) { }
	virtual void SetBWallAddBarsNumber(long value) { }
	virtual void SetBWallAddBarsLengthType(long value) { }
	virtual void SetBWallAddBarsTensionLengthType(long value) { }
	virtual void SetBWallAddBarsUserLength(long value) { }
	virtual void SetBWallDowelSupportPlaceType(BOOL value) { }

	virtual void SetBWallShearBarSet(long value) { }
	virtual void SetBWallShearBarDiaType(long value) { }
	virtual void SetBWallShearBarDiaIndex(long value) { }
	virtual void SetBWallShearBarUserSteelGradeS(BOOL value) { }
	virtual void SetBWallShearBarUserDiaType(long value) { }

	virtual void SetBWallShearBarShapeType(long value) { }

	virtual void SetBWallShearBarVerSpacingCondition(long value) { }
	virtual void SetBWallShearBarVerBelowType(long value) { }
	virtual void SetBWallShearBarVerBelowSpacing(long value) { }
	virtual void SetBWallShearBarVerBelowRatio(long value) { }
	virtual void SetBWallShearBarVerBelowNum(long value) { }
	virtual void SetBWallShearBarVerOverType(long value) { }
	virtual void SetBWallShearBarVerOverSpacing(long value) { }
	virtual void SetBWallShearBarVerOverRatio(long value) { }
	virtual void SetBWallShearBarVerOverNum(long value) { }

	virtual void SetBWallShearBarHorSpacingCondition(long value) { }
	virtual void SetBWallShearBarHorBelowType(long value) { }
	virtual void SetBWallShearBarHorBelowSpacing(long value) { }
	virtual void SetBWallShearBarHorBelowRatio(long value) { }
	virtual void SetBWallShearBarHorBelowNum(long value) { }
	virtual void SetBWallShearBarHorOverType(long value) { }
	virtual void SetBWallShearBarHorOverSpacing(long value) { }
	virtual void SetBWallShearBarHorOverRatio(long value) { }
	virtual void SetBWallShearBarHorOverNum(long value) { }

	virtual void SetBWallUBarAddLengthSet(BOOL value) { }
	virtual void SetBWallUBarAddLength(long value) { }
	virtual void SetBWallSpacerAddLengthSet(BOOL value) { }
	virtual void SetBWallSpacerAddLength(long value) { }

	virtual BOOL GetBWallDowelVerBarUserDiaSet() { return FALSE; }
	virtual void SetBWallDowelVerBarUserDiaSet(BOOL value) { }

	virtual BOOL GetBWallDowelVerBarUserSteelGradeS() { return FALSE; }
	virtual void SetBWallDowelVerBarUserSteelGradeS(BOOL value){ }
	//Function
	virtual map<long, long> GetBWallAddBarDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallEndBarDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallDefaultUbarLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallShearBarVerUserDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallShearBarHorUserDiaIndexMap() { map<long, long> value; return value; }
	virtual map<long, long> GetBWallDowelVerBarUserDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetBWallAddBarDiaIndexMap(map<long, long> mapAddBarDiaIndex) { }
	virtual void SetBWallEndBarDiaIndexMap(map<long, long> mapEndBarDiaIndex) { }
	virtual void SetBWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen) { }
	virtual void SetBWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetBWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }
	virtual void SetBWallShearBarVerUserDiaIndexMap(map<long, long> mapAddBarDiaIndex) { }
	virtual void SetBWallShearBarHorUserDiaIndexMap(map<long, long> mapAddBarDiaIndex) { }
	virtual void SetBWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex) { }

	virtual double GetBWallAddBarDia(double dDia) { return 0; }
	virtual double GetBWallEndBarDia(double dDia) { return 0; }
	virtual double GetBWallDefaultUbarLen(double dDia) { return 0; }
	virtual double GetBWallUserDowelHookLen(double dDia) { return 0; }
	virtual double GetBWallUserDowelSpliceLen(double dDia) { return 0; }
	virtual double GetBWallShearBarVerUserDia(double dDia) { return 0; }
	virtual double GetBWallShearBarHorUserDia(double dDia) { return 0; }
	virtual double GetBWallDowelVerBarUserDia(double dDia) { return 0; }

	virtual void SetBWallAddBarDiaIndex(long key, long value) { }
	virtual void SetBWallEndBarDiaIndex(long key, long value) { }
	virtual void SetBWallDefaultUbarLen(long key, long value) { }
	virtual void SetBWallUserDowelHookLen(long key, long value) { }
	virtual void SetBWallUserDowelSpliceLen(long key, long value) { }
	virtual void SetBWallShearBarVerUserDiaIndex(long key, long value) { }
	virtual void SetBWallShearBarHorUserDiaIndex(long key, long value) { }
	virtual void SetBWallDowelVerBarUserDiaIndex(long key, long value) { }

	//RWall
	//Get Property
	virtual long GetRWallVerBarPlaceTypeDiaIndex() { return 0; }
	virtual RWallVerBarRebarTemplate *GetRWallVerBarTemplateBelowDia() { return NULL; }
	virtual RWallVerBarRebarTemplate *GetRWallVerBarTemplateExcessDia() { return NULL; }

	virtual long GetRWallDowelPlaceTypeDiaIndex() { return 0; }
	virtual RWallDowelRebarTemplate *GetRWallDowelTemplateBelowDia() { return NULL; }
	virtual RWallDowelRebarTemplate *GetRWallDowelTemplateExcessDia() { return NULL; }

	virtual BOOL GetRWallPlaceCrossBar() { return FALSE; }
	virtual long GetRWallCrossBarEAType() { return 0; }
	virtual BOOL GetRWallCrossBarEATypeExcessThickSet() { return FALSE; }
	virtual long GetRWallCrossBarEATypeExcessThick() { return 0; }
	virtual long GetRWallCrossBarStartSpacingType() { return 0; }

	virtual long GetRWallVerBarTopLenType(double dDia) { return 0; }
	virtual long GetRWallVerBarSpliceType(double dDia) { return 0; }
	virtual long GetRWallVerBarTopFloorDevType(double dDia) { return 0; }
	virtual long GetRWallVerBarConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetRWallVerBarAlterSpliceSet(double dDia) { return 0; }
	virtual long GetRWallVerBarAlterSpliceDist(double dDia) { return 0; }

	virtual long GetRWallVerBarAlterSpliceType(double dDia) { return 0; }
	virtual long GetRWallDowelAlterSpliceType(double dDia) { return 0; }

	virtual BOOL GetRWallUserDowelHookLenSet(double dDia) { return FALSE; }
	virtual long GetRWallDowelBarAnchorType(double dDia) { return 0; }
	virtual long GetRWallDowelBarTopLenType(double dDia) { return 0; }
	virtual long GetRWallDowelDowelShape(double dDia) { return 0; }
	virtual long GetRWallDowelSupportBarDiaIndex(double dDia) { return 0; }
	virtual BOOL GetRWallDowelSupportBarUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetRWallDowelSupportBarNum(double dDia) { return 0; }
	virtual long GetRWallDowelBarSpliceType(double dDia) { return 0; }
	virtual long GetRWallDowelConstructionHeight(double dDia) { return 0; }
	virtual BOOL GetRWallDowelAlterSpliceSet(double dDia) { return FALSE; }
	virtual long GetRWallDowelAlterSpliceDist(double dDia) { return 0; }
	virtual BOOL GetRWallDowelHookAnchorPlaceSpacingSet(double dDia) { return FALSE; }
	virtual long GetRWallDowelHookAnchorPlaceSpacing(double dDia) { return 0; }
	virtual BOOL GetRWallDowelBotAddBarIntegratedPlaceSet(double dDia) { return FALSE; }
	virtual BOOL GetRWallDowelAdjustRatioSet(double dDia) { return FALSE; }
	virtual int GetRWallDowelAdjustRatio(double dDia) { return 0; }
	virtual long GetRWallDowelBotAddBarIntegratedPlaceLengthConditionType(double dDia) { return 0; }
	virtual long GetRWallDowelBotAddBarIntegratedPlaceLengthCondition(double dDia) { return 0; }

	virtual BOOL GetRWallPlaceBraceSupportBar() { return FALSE; }
	virtual long GetRWallBraceSupportBarOverDiaIndex() { return 0; }
	virtual BOOL GetRWallBraceSupportBarUserSteelGradeS() { return FALSE; }
	virtual long GetRWallBraceSupportBarWallLengthCondition() { return 0; }
	virtual long GetRWallBraceSupportBarWallLengthConditionOverLength() { return 0; }
	virtual long GetRWallBraceSupportBarWallLengthConditionUnderLength() { return 0; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionDiaSet() { return FALSE; }
	virtual long GetRWallBraceSupportNotPlaceConditionDiaIndex() { return 0; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionLengthSet() { return FALSE; }
	virtual long GetRWallBraceSupportNotPlaceConditionLength() { return 0; }
	virtual BOOL GetRWallBraceSupportNotPlaceConditionBottomDowel() { return FALSE; }

	virtual BOOL GetRWallBraceSupportNotPlaceConditionHeightSet() { return FALSE; }
	virtual long GetRWallBraceSupportNotPlaceConditionHeight() { return 0; }

	virtual BOOL GetRWallHorInfoSet() { return FALSE; }
	virtual BOOL GetRWallHorBarStartSet() { return FALSE; }
	virtual long GetRWallHorBarStartLength() { return 0; }
	virtual long GetRWallEndUbar() { return 0; }
	virtual BOOL GetRWallCrossEndUBarPlace() { return FALSE; }
	virtual BOOL GetRWallDefaultUbarLen() { return FALSE; }
	virtual BOOL GetRWallCrossBarUBarHalfPlace() { return FALSE; }

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
	virtual long GetRWallSpacerLenCalWayA() { return 0; }
	virtual long GetRWallSpacerLenCalWayB() { return 0; }
	virtual long GetRWallSpacerLenA() { return 0; }
	virtual long GetRWallSpacerLenB() { return 0; }
	virtual long GetRWallSpacerShape() { return 0; }
	virtual BOOL GetRWallSpacerRebarPlaceSet() { return FALSE; }
	virtual long GetRWallSpacerRebarPlaceCondition() { return 0; }

	virtual BOOL GetRWallSupportRebarsSet() { return FALSE; }
	virtual long GetRWallSupportRebarsDiaIndex() { return 0; }
	virtual double GetRWallSupportRebarsTotalLengthRate() { return 0.0; }
	virtual long GetRWallSupportRebarsOrderLength() { return 0; }
	virtual long GetRWallSupportRebarsSpliceLength() { return 0; }
	virtual long GetRWallSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetRWallAddBarsSet() { return FALSE; }
	virtual long GetRWallAddBarsDiaIndex() { return 0; }
	virtual long GetRWallAddBarsNumber() { return 0; }
	virtual long GetRWallAddBarsLengthType() { return 0; }
	virtual long GetRWallAddBarsTensionLengthType() { return 0; }
	virtual long GetRWallAddBarsUserLength() { return 0; }

	virtual BOOL GetRWallVerInfoSet() { return FALSE; }
	virtual BOOL GetRWallDowelSupportPlaceType() { return FALSE; }
	virtual BOOL GetRWallDowelInfoSet() { return FALSE; }


	virtual long GetRWallShearBarShapeType() { return 0; }

	virtual BOOL GetRWallUBarAddLengthSet() { return FALSE; }
	virtual long GetRWallUBarAddLength() { return 0; }
	virtual BOOL GetRWallSpacerAddLengthSet() { return FALSE; }
	virtual long GetRWallSpacerAddLength() { return 0; }

	virtual BOOL GetRWallAddBarsUserSteelGradeS() { return FALSE; }
	virtual void SetRWallAddBarsUserSteelGradeS(BOOL value) { }
	virtual BOOL GetRWallSupportRebarsUserSteelGradeS() { return FALSE; }
	virtual void SetRWallSupportRebarsUserSteelGradeS(BOOL value) { }
	virtual BOOL GetRWallSpacerUserSteelGradeS() { return FALSE; }
	virtual void SetRWallSpacerUserSteelGradeS(BOOL value) { }
	//Set Property
	virtual void SetRWallVerBarPlaceTypeDiaIndex(long value) {}
	virtual void SetRWallDowelPlaceTypeDiaIndex(long value) {}

	virtual void SetRWallPlaceCrossBar(BOOL value) { }
	virtual void SetRWallCrossBarEAType(long value) { }
	virtual void SetRWallCrossBarEATypeExcessThickSet(BOOL value) { }
	virtual void SetRWallCrossBarEATypeExcessThick(long value) { }
	virtual void SetRWallCrossBarStartSpacingType(long value) { }

	virtual void SetRWallPlaceBraceSupportBar(BOOL value) { }
	virtual void SetRWallBraceSupportBarOverDiaIndex(long value) { }
	virtual void SetRWallBraceSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetRWallBraceSupportBarWallLengthCondition(long value) { }
	virtual void SetRWallBraceSupportBarWallLengthConditionOverLength(long value) { }
	virtual void SetRWallBraceSupportBarWallLengthConditionUnderLength(long value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionDiaSet(BOOL value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionDiaIndex(long value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionLengthSet(BOOL value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionLength(long value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionBottomDowel(BOOL value) { }

	virtual void SetRWallBraceSupportNotPlaceConditionHeightSet(BOOL value) { }
	virtual void SetRWallBraceSupportNotPlaceConditionHeight(long value) { }

	virtual void SetRWallHorInfoSet(BOOL value) { }
	virtual void SetRWallHorBarStartSet(BOOL value) { }
	virtual void SetRWallHorBarStartLength(long value) { }
	virtual void SetRWallEndUbar(long value) { }
	virtual void SetRWallCrossEndUBarPlace(BOOL value) { }
	virtual void SetRWallDefaultUbarLen(BOOL value) { }
	virtual void SetRWallCrossBarUBarHalfPlace(BOOL value) {  }

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
	virtual void SetRWallSpacerLenCalWayA(long value) { }
	virtual void SetRWallSpacerLenCalWayB(long value) { }
	virtual void SetRWallSpacerLenA(long value) { }
	virtual void SetRWallSpacerLenB(long value) { }
	virtual void SetRWallSpacerShape(long value) { }
	virtual void SetRWallSpacerRebarPlaceSet(BOOL value) { }
	virtual void SetRWallSpacerRebarPlaceCondition(long value) { }

	virtual void SetRWallSupportRebarsSet(BOOL value) { }
	virtual void SetRWallSupportRebarsDiaIndex(long value) { }
	virtual void SetRWallSupportRebarsTotalLengthRate(double value) { }
	virtual void SetRWallSupportRebarsOrderLength(long value) { }
	virtual void SetRWallSupportRebarsSpliceLength(long value) { }
	virtual void SetRWallSupportRebarsCorrectiveLength(long value) { }

	virtual void SetRWallAddBarsSet(BOOL value) { }
	virtual void SetRWallAddBarsDiaIndex(long value) { }
	virtual void SetRWallAddBarsNumber(long value) { }
	virtual void SetRWallAddBarsLengthType(long value) { }
	virtual void SetRWallAddBarsTensionLengthType(long value) { }
	virtual void SetRWallAddBarsUserLength(long value) { }

	virtual void SetRWallVerInfoSet(BOOL value) { }
	virtual void SetRWallDowelSupportPlaceType(BOOL value) { }
	virtual void SetRWallDowelInfoSet(BOOL value) { }

	virtual void SetRWallShearBarShapeType(long value) { }

	virtual void SetRWallUBarAddLengthSet(BOOL value) { }
	virtual void SetRWallUBarAddLength(long value) { }
	virtual void SetRWallSpacerAddLengthSet(BOOL value) { }
	virtual void SetRWallSpacerAddLength(long value) { }

	virtual double GetRWallCrossBarDia(double dDia) { return 0; }
	virtual map<long, long> GetRWallCrossBarDiaIndexMap() { map<long, long> value; return value; }
	virtual void SetRWallCrossBarDiaIndexMap(map<long, long> mapCrossBarDiaIndex) { }
	virtual void SetRWallCrossBarDiaIndex(long key, long value) { }

	virtual BOOL GetRWallDowelVerBarUserDiaSet() { return FALSE; }
	virtual void SetRWallDowelVerBarUserDiaSet(BOOL value) { }

	virtual BOOL GetRWallDowelVerBarUserSteelGradeS() { return FALSE; }
	virtual void SetRWallDowelVerBarUserSteelGradeS(BOOL value){ }
	//Function
	virtual map<long, long> GetRWallDefaultUbarLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetRWallUserDowelHookLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetRWallUserDowelSpliceLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetRWallDowelVerBarUserDiaIndexMap() { map<long, long> value; return value; }

	virtual void SetRWallDefaultUbarLenMap(map<long, long> mapDefaultUbarLen) { }
	virtual void SetRWallUserDowelHookLenMap(map<long, long> mapUserDowelHookLen) { }
	virtual void SetRWallUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen) { }
	virtual void SetRWallDowelVerBarUserDiaIndexMap(map<long, long> dowelVerBarUserDiaIndex) { }

	virtual double GetRWallDefaultUbarLen(double dDia) { return 0; }
	virtual double GetRWallUserDowelHookLen(double dDia) { return 0; }
	virtual double GetRWallUserDowelSpliceLen(double dDia) { return 0; }
	virtual double GetRWallDowelVerBarUserDia(double dDia) { return 0; }

	virtual void SetRWallDefaultUbarLen(long key, long value) { }
	virtual void SetRWallUserDowelHookLen(long key, long value) { }
	virtual void SetRWallUserDowelSpliceLen(long key, long value) { }
	virtual void SetRWallDowelVerBarUserDiaIndex(long key, long value) { }

	//Pit
	virtual BOOL GetPitGeneralInfoSet() { return FALSE; }
	virtual long GetPitMaxLen2HookAnchorBar() { return 0; }

	virtual BOOL GetPitSpliceDevConditionSet() { return 0; }
	virtual long GetPitSpliceDevConditionLength() { return 0; }

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
	virtual long GetPitTopSpacerSlabThick() { return 0; }
	virtual long GetPitTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetPitTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetPitSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetPitSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetPitSpacerSupportBarsCondition() { return 0; }
	virtual long GetPitSpacerSupportBarsDiaType() { return 0; }
	virtual long GetPitSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetPitSpacerSupportBarsNumType() { return 0; }
	virtual double GetPitSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetPitSpacerSupportBarsUserNums() { return 0; }
	virtual long GetPitSpacerSupportBarsLengthType() { return 0; }
	virtual double GetPitSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetPitSpacerSupportBarsUserLength() { return 0; }

	virtual BOOL GetPitSupportRebarsSet() { return FALSE; }
	virtual long GetPitSupportRebarsDiaIndex() { return 0; }
	virtual double GetPitSupportRebarsSlabLengthRate() { return 0; }
	virtual long GetPitSupportRebarsOrderLength() { return 0; }
	virtual long GetPitSupportRebarsSpliceLength() { return 0; }
	virtual long GetPitSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetPitBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetPitTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetPitSpacerSupportBarUserSteelGradeS() { return FALSE; }
	virtual BOOL GetPitSupportRebarUserSteelGradeS() { return FALSE; }

	virtual void SetPitGeneralInfoSet(BOOL value) { }
	virtual void SetPitMaxLen2HookAnchorBar(long value) { }

	virtual void SetPitSpliceDevConditionSet(BOOL value) { }
	virtual void SetPitSpliceDevConditionLength(long value) { }

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
	virtual void SetPitTopSpacerSlabThick(long value) { }
	virtual void SetPitTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetPitTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetPitSpacerSupportBarsSet(BOOL value) { }
	virtual void SetPitSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetPitSpacerSupportBarsCondition(long value) { }
	virtual void SetPitSpacerSupportBarsDiaType(long value) { }
	virtual void SetPitSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetPitSpacerSupportBarsNumType(long value) { }
	virtual void SetPitSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetPitSpacerSupportBarsUserNums(long value) { }
	virtual void SetPitSpacerSupportBarsLengthType(long value) { }
	virtual void SetPitSpacerSupportBarsUserRatio(double value) { }
	virtual void SetPitSpacerSupportBarsUserLength(long value) { }

	virtual void SetPitSupportRebarsSet(BOOL value) { }
	virtual void SetPitSupportRebarsDiaIndex(long value) { }
	virtual void SetPitSupportRebarsSlabLengthRate(double value) { }
	virtual void SetPitSupportRebarsOrderLength(long value) { }
	virtual void SetPitSupportRebarsSpliceLength(long value) { }
	virtual void SetPitSupportRebarsCorrectiveLength(long value) { }

	virtual void SetPitBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetPitTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetPitSpacerSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetPitSupportRebarUserSteelGradeS(BOOL value) { }

	virtual long GetPitHorOutHookLengthType() { return 0; }
	virtual void SetPitHorOutHookLengthType(long value) { }

	virtual map<long, long> GetPitUserOutHorBarHookLenMap() { map<long, long> value; return value; }
	virtual void SetPitUserOutHorBarHookLenMap(map<long, long> newVal) { }

	virtual long GetPitUserOutHorBarHookLen(double dDia) { return 0; }
	virtual void SetPitUserOutHorBarHookLen(long key, long value) {  }

	virtual BOOL GetPitWellAddBarSet() { return FALSE; }
	virtual long GetPitWellAddBarDiaIndex() { return 0; }
	virtual BOOL GetPitWellAddBarUserSteelGradeS() { return FALSE; }
	virtual long GetPitWellAddBarNum() { return 0; }
	virtual long GetPitWellAddBarDevType() { return 0; }
	virtual long GetPitWellAddBarDevUserLength() { return 0; }
	
	virtual void SetPitWellAddBarSet(BOOL value) { }
	virtual void SetPitWellAddBarDiaIndex(long value) { }
	virtual void SetPitWellAddBarUserSteelGradeS(BOOL value) { }
	virtual void SetPitWellAddBarNum(long value) { }
	virtual void SetPitWellAddBarDevType(long value) { }
	virtual void SetPitWellAddBarDevUserLength(long value) { }

	virtual BOOL GetPitWallSpacerInfoSet() { return FALSE; }
	virtual long GetPitWallSpacerDiaType() { return 0; }
	virtual long GetPitWallSpacerDiaIndex() { return 0; }
	virtual BOOL GetPitWallSpacerUserSteelGradeS() { return FALSE; }
	virtual long GetPitWallSpacerHorSpacing() { return 0; }
	virtual long GetPitWallSpacerVerSpacingType() { return 0; }
	virtual long GetPitWallSpacerVerSpacing() { return 0; }
	virtual long GetPitWallSpacerVerSpacingRate() { return 0; }
	virtual long GetPitWallSpacerVerSpacingCount() { return 0; }
	virtual long GetPitWallSpacerLenType() { return 0; }
	virtual long GetPitWallSpacerLenCalWayA() { return 0; }
	virtual long GetPitWallSpacerLenCalWayB() { return 0; }
	virtual long GetPitWallSpacerLenA() { return 0; }
	virtual long GetPitWallSpacerLenB() { return 0; }
	virtual long GetPitWallSpacerShape() { return 0; }

	virtual void SetPitWallSpacerInfoSet(BOOL value) { }
	virtual void SetPitWallSpacerDiaType(long value) { }
	virtual void SetPitWallSpacerDiaIndex(long value) { }
	virtual void SetPitWallSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetPitWallSpacerHorSpacing(long value) { }
	virtual void SetPitWallSpacerVerSpacingType(long value) { }
	virtual void SetPitWallSpacerVerSpacing(long value) { }
	virtual void SetPitWallSpacerVerSpacingRate(long value) { }
	virtual void SetPitWallSpacerVerSpacingCount(long value) { }
	virtual void SetPitWallSpacerLenType(long value) { }
	virtual void SetPitWallSpacerLenCalWayA(long value) { }
	virtual void SetPitWallSpacerLenCalWayB(long value) { }
	virtual void SetPitWallSpacerLenA(long value) { }
	virtual void SetPitWallSpacerLenB(long value) { }
	virtual void SetPitWallSpacerShape(long value) { }

	virtual BOOL GetPitPlaceBraceSupportBar() { return FALSE; }
	virtual long GetPitBraceSupportBarDiaIndex() { return 0; }
	virtual BOOL GetPitBraceSupportBarUserSteelGradeS() { return FALSE; }
	virtual long GetPitBraceSupportBarNumIndex() { return 0; }

	virtual void SetPitPlaceBraceSupportBar(BOOL value) { }
	virtual void SetPitBraceSupportBarDiaIndex(long value) { }
	virtual void SetPitBraceSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetPitBraceSupportBarNumIndex(long value) { }

	// MAT Slab
	virtual BOOL GetMatMainBarInfoSet() { return FALSE; }
	virtual long GetMatMaxDualHookLength() { return 0; }
	virtual long GetMatSpliceWay() { return 0; }
	virtual long GetMatSpliceType() { return 0; }

	virtual BOOL GetMatEndInfoSet() { return FALSE; }
	virtual long GetMatTopEndHookType() { return 0; }
	virtual long GetMatBotEndHookType() { return 0; }
	virtual long GetMatIsoFootDevType() { return 0; }

	virtual long GetMatTopEndDevTypeUserLength() { return 0; }
	virtual long GetMatBotEndDevTypeUserLength() { return 0; }
	virtual long GetMatOpeningTopDevType() { return 0; }
	virtual long GetMatOpeningBotDevType() { return 0; }

	virtual BOOL GetMatLevelDifInfoSet() { return FALSE; }
	virtual long GetMatLevelDifPlaceShape() { return 0; }
	virtual long GetMatLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetMatSupportRebarsSet() { return FALSE; }
	virtual long GetMatSupportRebarsDiaIndex() { return 0; }
	virtual long GetMatSupportRebarsLengthType() { return 0; }
	virtual double GetMatSupportRebarsRWallLengthRate() { return 0.0; }
	virtual double GetMatSupportRebarsSlabLengthRate() { return 0.0; }
	virtual long GetMatSupportRebarsOrderLength() { return 0; }
	virtual long GetMatSupportRebarsSpliceLength() { return 0; }
	virtual long GetMatSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetMatRebarAreaInfoSet() { return FALSE; }
	virtual long GetMatRebarTolDist() { return 0; }
	virtual long GetMatRebarTolLenDif() { return 0; }

	virtual BOOL GetMatSpliceDevConditionSet() { return 0; }
	virtual long GetMatSpliceDevConditionLength() { return 0; }

	virtual BOOL GetMatSpliceAddLengthSet() { return FALSE; }
	virtual long GetMatSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetMatTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetMatTopSpacerSet() { return FALSE; }
	virtual long GetMatTopSpacerDiaType() { return 0; }
	virtual long GetMatTopSpacerSlabThick() { return 0; }
	virtual long GetMatTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetMatTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetMatSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetMatSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetMatSpacerSupportBarsCondition() { return 0; }
	virtual long GetMatSpacerSupportBarsDiaType() { return 0; }
	virtual long GetMatSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetMatSpacerSupportBarsNumType() { return 0; }
	virtual double GetMatSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetMatSpacerSupportBarsUserNums() { return 0; }
	virtual long GetMatSpacerSupportBarsLengthType() { return 0; }
	virtual double GetMatSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetMatSpacerSupportBarsUserLength() { return 0; }

	virtual long GetMatCZoneSpliceWay() { return 0; }
	virtual long GetMatCZoneSpliceLength() { return 0; }

	virtual void SetMatMainBarInfoSet(BOOL value) { }
	virtual void SetMatMaxDualHookLength(long value) { }
	virtual void SetMatSpliceWay(long value) { }
	virtual void SetMatSpliceType(long value) { }

	virtual void SetMatEndInfoSet(BOOL value) { }
	virtual void SetMatTopEndHookType(long value) { }
	virtual void SetMatBotEndHookType(long value) { }
	virtual void SetMatIsoFootDevType(long value) { }

	virtual void SetMatTopEndDevTypeUserLength(long value) {  }
	virtual void SetMatBotEndDevTypeUserLength(long value) {  }
	virtual void SetMatOpeningTopDevType(long value) {  }
	virtual void SetMatOpeningBotDevType(long value) {  }

	virtual void SetMatLevelDifInfoSet(BOOL value) { }
	virtual void SetMatLevelDifPlaceShape(long value) { }
	virtual void SetMatLevelDifHaunchPlaceType(long value) { }

	virtual void SetMatSupportRebarsSet(BOOL value) { }
	virtual void SetMatSupportRebarsDiaIndex(long value) { }
	virtual void SetMatSupportRebarsLengthType(long value) { }
	virtual void SetMatSupportRebarsRWallLengthRate(double value) { }
	virtual void SetMatSupportRebarsSlabLengthRate(double value) { }
	virtual void SetMatSupportRebarsOrderLength(long value) { }
	virtual void SetMatSupportRebarsSpliceLength(long value) { }
	virtual void SetMatSupportRebarsCorrectiveLength(long value) { }

	virtual void SetMatRebarAreaInfoSet(BOOL value) { }
	virtual void SetMatRebarTolDist(long value) { }
	virtual void SetMatRebarTolLenDif(long value) { }

	virtual void SetMatSpliceDevConditionSet(BOOL value) { }
	virtual void SetMatSpliceDevConditionLength(long value) { }

	virtual void SetMatSpliceAddLengthSet(BOOL value) {  }
	virtual void SetMatSpliceAddLengthNumSplice(long value) {  }

	virtual void SetMatTopSpacerInfoSet(BOOL value) { }
	virtual void SetMatTopSpacerSet(BOOL value) { }
	virtual void SetMatTopSpacerDiaType(long value) { }
	virtual void SetMatTopSpacerSlabThick(long value) { }
	virtual void SetMatTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetMatTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetMatSpacerSupportBarsSet(BOOL value) { }
	virtual void SetMatSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetMatSpacerSupportBarsCondition(long value) { }
	virtual void SetMatSpacerSupportBarsDiaType(long value) { }
	virtual void SetMatSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetMatSpacerSupportBarsNumType(long value) { }
	virtual void SetMatSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetMatSpacerSupportBarsUserNums(long value) { }
	virtual void SetMatSpacerSupportBarsLengthType(long value) { }
	virtual void SetMatSpacerSupportBarsUserRatio(double value) { }
	virtual void SetMatSpacerSupportBarsUserLength(long value) { }

	virtual void SetMatCZoneSpliceWay(long value) {  }
	virtual void SetMatCZoneSpliceLength(long value) {  }

	// FLAT Slab
	virtual BOOL GetFlatMainBarInfoSet() { return FALSE; }
	virtual long GetFlatMaxDualHookLength() { return 0; }
	virtual long GetFlatSpliceWay() { return 0; }
	virtual long GetFlatSpliceType() { return 0; }

	virtual BOOL GetFlatEndInfoSet() { return FALSE; }
	virtual long GetFlatTopEndHookType() { return 0; }
	virtual long GetFlatBotEndHookType() { return 0; }
	virtual long GetFlatIsoFootDevType() { return 0; }
	virtual long GetFlatSpliceDevSlabType() { return 0; }

	virtual long GetFlatTopEndDevTypeUserLength() { return 0; }
	virtual long GetFlatBotEndDevTypeUserLength() { return 0; }
	virtual long GetFlatOpeningTopDevType() { return 0; }
	virtual long GetFlatOpeningBotDevType() { return 0; }

	virtual BOOL GetFlatLevelDifInfoSet() { return FALSE; }
	virtual long GetFlatLevelDifPlaceShape() { return 0; }
	virtual long GetFlatLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetFlatOpeningInfoSet() { return FALSE; }

	virtual BOOL GetFlatRebarAreaInfoSet() { return FALSE; }
	virtual long GetFlatRebarTolDist() { return 0; }
	virtual long GetFlatRebarTolLenDif() { return 0; }

	virtual BOOL GetFlatSpliceDevConditionSet() { return 0; }
	virtual long GetFlatSpliceDevConditionLength() { return 0; }

	virtual BOOL GetFlatSpliceAddLengthSet() { return FALSE; }
	virtual long GetFlatSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetFlatTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetFlatTopSpacerSet() { return FALSE; }
	virtual long GetFlatTopSpacerDiaType() { return 0; }
	virtual long GetFlatTopSpacerSlabThick() { return 0; }
	virtual long GetFlatTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetFlatTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetFlatSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetFlatSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetFlatSpacerSupportBarsCondition() { return 0; }
	virtual long GetFlatSpacerSupportBarsDiaType() { return 0; }
	virtual long GetFlatSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetFlatSpacerSupportBarsNumType() { return 0; }
	virtual double GetFlatSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetFlatSpacerSupportBarsUserNums() { return 0; }
	virtual long GetFlatSpacerSupportBarsLengthType() { return 0; }
	virtual double GetFlatSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetFlatSpacerSupportBarsUserLength() { return 0; }

	virtual long GetFlatCZoneSpliceWay() { return 0; }
	virtual long GetFlatCZoneSpliceLength() { return 0; }

	virtual void SetFlatMainBarInfoSet(BOOL value) { }
	virtual void SetFlatMaxDualHookLength(long value) { }
	virtual void SetFlatSpliceWay(long value) { }
	virtual void SetFlatSpliceType(long value) { }

	virtual void SetFlatEndInfoSet(BOOL value) { }
	virtual void SetFlatTopEndHookType(long value) { }
	virtual void SetFlatBotEndHookType(long value) { }
	virtual void SetFlatIsoFootDevType(long value) { }
	virtual void SetFlatSpliceDevSlabType(long value) { }

	virtual void SetFlatTopEndDevTypeUserLength(long value) { }
	virtual void SetFlatBotEndDevTypeUserLength(long value) { }
	virtual void SetFlatOpeningTopDevType(long value) { }
	virtual void SetFlatOpeningBotDevType(long value) { }

	virtual void SetFlatLevelDifInfoSet(BOOL value) { }
	virtual void SetFlatLevelDifPlaceShape(long value) { }
	virtual void SetFlatLevelDifHaunchPlaceType(long value) { }

	virtual void SetFlatOpeningInfoSet(BOOL value) { }

	virtual void SetFlatRebarAreaInfoSet(BOOL value) { }
	virtual void SetFlatRebarTolDist(long value) { }
	virtual void SetFlatRebarTolLenDif(long value) { }

	virtual void SetFlatSpliceDevConditionSet(BOOL value) { }
	virtual void SetFlatSpliceDevConditionLength(long value) { }

	virtual void SetFlatSpliceAddLengthSet(BOOL value) { }
	virtual void SetFlatSpliceAddLengthNumSplice(long value) { }

	virtual void SetFlatTopSpacerInfoSet(BOOL value) { }
	virtual void SetFlatTopSpacerSet(BOOL value) { }
	virtual void SetFlatTopSpacerDiaType(long value) { }
	virtual void SetFlatTopSpacerSlabThick(long value) { }
	virtual void SetFlatTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetFlatTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetFlatSpacerSupportBarsSet(BOOL value) { }
	virtual void SetFlatSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetFlatSpacerSupportBarsCondition(long value) { }
	virtual void SetFlatSpacerSupportBarsDiaType(long value) { }
	virtual void SetFlatSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetFlatSpacerSupportBarsNumType(long value) { }
	virtual void SetFlatSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetFlatSpacerSupportBarsUserNums(long value) { }
	virtual void SetFlatSpacerSupportBarsLengthType(long value) { }
	virtual void SetFlatSpacerSupportBarsUserRatio(double value) { }
	virtual void SetFlatSpacerSupportBarsUserLength(long value) { }

	virtual void SetFlatCZoneSpliceWay(long value) { }
	virtual void SetFlatCZoneSpliceLength(long value) { }

	virtual ShearFrictionBarTemplate *GetMatShearFrictionBarTemplate() { return nullptr; }
	virtual ShearFrictionBarTemplate *GetFlatShearFrictionBarTemplate() { return nullptr; }
	virtual ShearFrictionBarTemplate *GetBeamShearFrictionBarTemplate() { return nullptr; }
	virtual ShearFrictionBarTemplate *GetTransferBeamShearFrictionBarTemplate() { return nullptr; }

	//Slab
	virtual SlabHorSupportRebarsTemplate *GetPCSlabHorSupportRebarsTemplate() { return nullptr; }
	virtual SlabHorSupportRebarsTemplate *GetPCIMTSlabHorSupportRebarsTemplate() { return nullptr; }
	virtual SlabHorSupportRebarsTemplate *GetPCHCSSlabHorSupportRebarsTemplate() { return nullptr; }
	virtual SlabHorSupportRebarsTemplate *GetPCRPSSlabHorSupportRebarsTemplate() { return nullptr; }
	virtual SlabHorSupportRebarsTemplate *GetGeneralDeckHorSupportRebarsTemplate() { return nullptr; }
	virtual SlabHorSupportRebarsTemplate *GetEmbeddedDeckHorSupportRebarsTemplate() { return nullptr; }
	// 사변지지 SLAB
	virtual BOOL GetSlabMainBarInfoSet() { return FALSE; }
	virtual long GetSlabMaxDualHookLength() { return 0; }
	virtual long GetSlabSpliceWay() { return 0; }
	virtual long GetSlabSpliceType() { return 0; }

	virtual BOOL GetSlabTopCutBarLenInfoSet() { return FALSE; }
	virtual long GetSlabOneWayCutBarLenType() { return 0; }
	virtual long GetSlabTwoWayCutBarLenType() { return 0; }

	virtual BOOL GetSlabUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetSlabTopUnStructuredEndHookType() { return 0; }
	virtual long GetSlabBotUnStructuredEndHookType() { return 0; }

	virtual long GetSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetSlabBotEndDevTypeUserLength() { return 0; }
	virtual long GetSlabOpeningTopDevType() { return 0; }
	virtual long GetSlabOpeningBotDevType() { return 0; }

	virtual BOOL GetSlabLevelDifInfoSet() { return FALSE; }
	virtual long GetSlabLevelDifPlaceShape() { return 0; }
	virtual long GetSlabLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetSlabOpeningInfoSet() { return FALSE; }

	virtual BOOL GetSlabRebarAreaInfoSet() { return FALSE; }
	virtual long GetSlabRebarTolDist() { return 0; }
	virtual long GetSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetSlabSpliceDevConditionSet() { return 0; }
	virtual long GetSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetSlabSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetSlabTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSlabTopSpacerSet() { return FALSE; }
	virtual long GetSlabTopSpacerDiaType() { return 0; }
	virtual long GetSlabTopSpacerSlabThick() { return 0; }
	virtual long GetSlabTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetSlabTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetSlabSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetSlabSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetSlabSpacerSupportBarsCondition() { return 0; }
	virtual long GetSlabSpacerSupportBarsDiaType() { return 0; }
	virtual long GetSlabSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetSlabSpacerSupportBarsNumType() { return 0; }
	virtual double GetSlabSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetSlabSpacerSupportBarsUserNums() { return 0; }
	virtual long GetSlabSpacerSupportBarsLengthType() { return 0; }
	virtual double GetSlabSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetSlabSpacerSupportBarsUserLength() { return 0; }

	virtual long GetSlabCZoneSpliceWay() { return 0; }
	virtual long GetSlabCZoneSpliceLength() { return 0; }

	virtual void SetSlabMainBarInfoSet(BOOL value) { }
	virtual void SetSlabMaxDualHookLength(long value) { }
	virtual void SetSlabSpliceWay(long value) { }
	virtual void SetSlabSpliceType(long value) { }


	virtual void SetSlabTopCutBarLenInfoSet(BOOL value) { }
	virtual void SetSlabOneWayCutBarLenType(long value) { }
	virtual void SetSlabTwoWayCutBarLenType(long value) { }

	virtual void SetSlabUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetSlabTopUnStructuredEndHookType(long value) { }
	virtual void SetSlabBotUnStructuredEndHookType(long value) { }

	virtual void SetSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetSlabBotEndDevTypeUserLength(long value) { }
	virtual void SetSlabOpeningTopDevType(long value) { }
	virtual void SetSlabOpeningBotDevType(long value) { }

	virtual void SetSlabLevelDifInfoSet(BOOL value) { }
	virtual void SetSlabLevelDifPlaceShape(long value) { }
	virtual void SetSlabLevelDifHaunchPlaceType(long value) { }

	virtual void SetSlabOpeningInfoSet(BOOL value) { }

	virtual void SetSlabRebarAreaInfoSet(BOOL value) { }
	virtual void SetSlabRebarTolDist(long value) { }
	virtual void SetSlabRebarTolLenDif(long value) { }

	virtual void SetSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetSlabSpliceDevConditionLength(long value) { }

	virtual void SetSlabSpliceAddLengthSet(BOOL value) {  }
	virtual void SetSlabSpliceAddLengthNumSplice(long value) {  }

	virtual void SetSlabTopSpacerInfoSet(BOOL value) { }
	virtual void SetSlabTopSpacerSet(BOOL value) { }
	virtual void SetSlabTopSpacerDiaType(long value) { }
	virtual void SetSlabTopSpacerSlabThick(long value) { }
	virtual void SetSlabTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetSlabTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetSlabSpacerSupportBarsSet(BOOL value) { }
	virtual void SetSlabSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetSlabSpacerSupportBarsCondition(long value) { }
	virtual void SetSlabSpacerSupportBarsDiaType(long value) { }
	virtual void SetSlabSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetSlabSpacerSupportBarsNumType(long value) { }
	virtual void SetSlabSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetSlabSpacerSupportBarsUserNums(long value) { }
	virtual void SetSlabSpacerSupportBarsLengthType(long value) { }
	virtual void SetSlabSpacerSupportBarsUserRatio(double value) { }
	virtual void SetSlabSpacerSupportBarsUserLength(long value) { }

	virtual void SetSlabCZoneSpliceWay(long value) { }
	virtual void SetSlabCZoneSpliceLength(long value) { }

	// SOG Slab
	virtual BOOL GetSOGMainBarInfoSet() { return FALSE; }
	virtual long GetSOGMaxDualHookLength() { return 0; }
	virtual long GetSOGSpliceWay() { return 0; }
	virtual long GetSOGSpliceType() { return 0; }

	virtual BOOL GetSOGEndInfoSet() { return FALSE; }
	virtual long GetSOGTopEndHookType() { return 0; }
	virtual long GetSOGBotEndHookType() { return 0; }
	virtual long GetSOGIsoFootDevType() { return 0; }
	virtual long GetSOGSpliceDevSlabType() { return 0; }

	virtual long GetSOGTopEndDevTypeUserLength() { return 0; }
	virtual long GetSOGBotEndDevTypeUserLength() { return 0; }
	virtual long GetSOGOpeningTopDevType() { return 0; }
	virtual long GetSOGOpeningBotDevType() { return 0; }


	virtual BOOL GetSOGLevelDifInfoSet() { return FALSE; }
	virtual long GetSOGLevelDifPlaceShape() { return 0; }
	virtual long GetSOGLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetSOGSupportRebarsSet() { return FALSE; }
	virtual long GetSOGSupportRebarsDiaIndex() { return 0; }
	virtual long GetSOGSupportRebarsLengthType() { return 0; }
	virtual double GetSOGSupportRebarsRWallLengthRate() { return 0.0; }
	virtual double GetSOGSupportRebarsSlabLengthRate() { return 0.0; }
	virtual long GetSOGSupportRebarsOrderLength() { return 0; }
	virtual long GetSOGSupportRebarsSpliceLength() { return 0; }
	virtual long GetSOGSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetSOGOpeningInfoSet() { return FALSE; }

	virtual BOOL GetSOGRebarAreaInfoSet() { return FALSE; }
	virtual long GetSOGRebarTolDist() { return 0; }
	virtual long GetSOGRebarTolLenDif() { return 0; }

	virtual BOOL GetSOGSpliceDevConditionSet() { return 0; }
	virtual long GetSOGSpliceDevConditionLength() { return 0; }

	virtual BOOL GetSOGSpliceAddLengthSet() { return FALSE; }
	virtual long GetSOGSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetSOGTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetSOGTopSpacerSet() { return FALSE; }
	virtual long GetSOGTopSpacerDiaType() { return 0; }
	virtual long GetSOGTopSpacerSlabThick() { return 0; }
	virtual long GetSOGTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetSOGTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetSOGSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetSOGSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetSOGSpacerSupportBarsCondition() { return 0; }
	virtual long GetSOGSpacerSupportBarsDiaType() { return 0; }
	virtual long GetSOGSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetSOGSpacerSupportBarsNumType() { return 0; }
	virtual double GetSOGSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetSOGSpacerSupportBarsUserNums() { return 0; }
	virtual long GetSOGSpacerSupportBarsLengthType() { return 0; }
	virtual double GetSOGSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetSOGSpacerSupportBarsUserLength() { return 0; }

	virtual long GetSOGCZoneSpliceWay() { return 0; }
	virtual long GetSOGCZoneSpliceLength() { return 0; }

	virtual void SetSOGMainBarInfoSet(BOOL value) { }
	virtual void SetSOGMaxDualHookLength(long value) { }
	virtual void SetSOGSpliceWay(long value) { }
	virtual void SetSOGSpliceType(long value) { }

	virtual void SetSOGEndInfoSet(BOOL value) { }
	virtual void SetSOGTopEndHookType(long value) { }
	virtual void SetSOGBotEndHookType(long value) { }
	virtual void SetSOGIsoFootDevType(long value) { }
	virtual void SetSOGSpliceDevSlabType(long value) { }

	virtual void SetSOGTopEndDevTypeUserLength(long value) {  }
	virtual void SetSOGBotEndDevTypeUserLength(long value) {  }
	virtual void SetSOGOpeningTopDevType(long value) {  }
	virtual void SetSOGOpeningBotDevType(long value) {  }

	virtual void SetSOGLevelDifInfoSet(BOOL value) { }
	virtual void SetSOGLevelDifPlaceShape(long value) { }
	virtual void SetSOGLevelDifHaunchPlaceType(long value) { }

	virtual void SetSOGSupportRebarsSet(BOOL value) { }
	virtual void SetSOGSupportRebarsDiaIndex(long value) { }
	virtual void SetSOGSupportRebarsLengthType(long value) { }
	virtual void SetSOGSupportRebarsRWallLengthRate(double value) { }
	virtual void SetSOGSupportRebarsSlabLengthRate(double value) { }
	virtual void SetSOGSupportRebarsOrderLength(long value) { }
	virtual void SetSOGSupportRebarsSpliceLength(long value) { }
	virtual void SetSOGSupportRebarsCorrectiveLength(long value) { }

	virtual void SetSOGOpeningInfoSet(BOOL value) { }

	virtual void SetSOGRebarAreaInfoSet(BOOL value) { }
	virtual void SetSOGRebarTolDist(long value) { }
	virtual void SetSOGRebarTolLenDif(long value) { }

	virtual void SetSOGSpliceDevConditionSet(BOOL value) { }
	virtual void SetSOGSpliceDevConditionLength(long value) { }

	virtual void SetSOGSpliceAddLengthSet(BOOL value) { }
	virtual void SetSOGSpliceAddLengthNumSplice(long value) { }

	virtual void SetSOGTopSpacerInfoSet(BOOL value) { }
	virtual void SetSOGTopSpacerSet(BOOL value) { }
	virtual void SetSOGTopSpacerDiaType(long value) { }
	virtual void SetSOGTopSpacerSlabThick(long value) { }
	virtual void SetSOGTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetSOGTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetSOGSpacerSupportBarsSet(BOOL value) { }
	virtual void SetSOGSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetSOGSpacerSupportBarsCondition(long value) { }
	virtual void SetSOGSpacerSupportBarsDiaType(long value) { }
	virtual void SetSOGSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetSOGSpacerSupportBarsNumType(long value) { }
	virtual void SetSOGSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetSOGSpacerSupportBarsUserNums(long value) { }
	virtual void SetSOGSpacerSupportBarsLengthType(long value) { }
	virtual void SetSOGSpacerSupportBarsUserRatio(double value) { }
	virtual void SetSOGSpacerSupportBarsUserLength(long value) { }

	virtual void SetSOGCZoneSpliceWay(long value) { }
	virtual void SetSOGCZoneSpliceLength(long value) { }

	// Circular Ramp
	virtual BOOL GetCircularRampMainBarInfoSet() { return FALSE; }
	virtual long GetCircularRampMaxDualHookLength() { return 0; }
	virtual long GetCircularRampSpliceWay() { return 0; }
	virtual long GetCircularRampSpliceType() { return 0; }

	virtual BOOL GetCircularRampEndInfoSet() { return FALSE; }
	virtual long GetCircularRampTopEndHookType() { return 0; }
	virtual long GetCircularRampBotEndHookType() { return 0; }
	virtual long GetCircularRampIsoFootDevType() { return 0; }
	virtual long GetCircularRampSpliceDevSlabType() { return 0; }

	virtual long GetCircularRampTopEndDevTypeUserLength() { return 0; }
	virtual long GetCircularRampBotEndDevTypeUserLength() { return 0; }
	virtual long GetCircularRampOpeningTopDevType() { return 0; }
	virtual long GetCircularRampOpeningBotDevType() { return 0; }

	virtual BOOL GetCircularRampLevelDifInfoSet() { return FALSE; }
	virtual long GetCircularRampLevelDifPlaceShape() { return 0; }
	virtual long GetCircularRampLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetCircularRampOpeningInfoSet() { return FALSE; }

	virtual BOOL GetCircularRampRebarAreaInfoSet() { return FALSE; }
	virtual long GetCircularRampRebarTolDist() { return 0; }
	virtual long GetCircularRampRebarTolLenDif() { return 0; }

	virtual BOOL GetCircularRampSpliceDevConditionSet() { return 0; }
	virtual long GetCircularRampSpliceDevConditionLength() { return 0; }

	virtual BOOL GetCircularRampSpliceAddLengthSet() { return FALSE; }
	virtual long GetCircularRampSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetCircularRampTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetCircularRampTopSpacerSet() { return FALSE; }
	virtual long GetCircularRampTopSpacerDiaType() { return 0; }
	virtual long GetCircularRampTopSpacerSlabThick() { return 0; }
	virtual long GetCircularRampTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetCircularRampTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetCircularRampSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetCircularRampSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetCircularRampSpacerSupportBarsCondition() { return 0; }
	virtual long GetCircularRampSpacerSupportBarsDiaType() { return 0; }
	virtual long GetCircularRampSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetCircularRampSpacerSupportBarsNumType() { return 0; }
	virtual double GetCircularRampSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetCircularRampSpacerSupportBarsUserNums() { return 0; }
	virtual long GetCircularRampSpacerSupportBarsLengthType() { return 0; }
	virtual double GetCircularRampSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetCircularRampSpacerSupportBarsUserLength() { return 0; }

	virtual long GetCircularRampCZoneSpliceWay() { return 0; }
	virtual long GetCircularRampCZoneSpliceLength() { return 0; }

	virtual void SetCircularRampMainBarInfoSet(BOOL value) { }
	virtual void SetCircularRampMaxDualHookLength(long value) { }
	virtual void SetCircularRampSpliceWay(long value) { }
	virtual void SetCircularRampSpliceType(long value) { }

	virtual void SetCircularRampEndInfoSet(BOOL value) { }
	virtual void SetCircularRampTopEndHookType(long value) { }
	virtual void SetCircularRampBotEndHookType(long value) { }
	virtual void SetCircularRampIsoFootDevType(long value) { }
	virtual void SetCircularRampSpliceDevSlabType(long value) { }

	virtual void SetCircularRampTopEndDevTypeUserLength(long value) { }
	virtual void SetCircularRampBotEndDevTypeUserLength(long value) { }
	virtual void SetCircularRampOpeningTopDevType(long value) { }
	virtual void SetCircularRampOpeningBotDevType(long value) { }

	virtual void SetCircularRampLevelDifInfoSet(BOOL value) { }
	virtual void SetCircularRampLevelDifPlaceShape(long value) { }
	virtual void SetCircularRampLevelDifHaunchPlaceType(long value) { }

	virtual void SetCircularRampOpeningInfoSet(BOOL value) { }

	virtual void SetCircularRampRebarAreaInfoSet(BOOL value) { }
	virtual void SetCircularRampRebarTolDist(long value) { }
	virtual void SetCircularRampRebarTolLenDif(long value) { }

	virtual void SetCircularRampSpliceDevConditionSet(BOOL value) { }
	virtual void SetCircularRampSpliceDevConditionLength(long value) { }

	virtual void SetCircularRampSpliceAddLengthSet(BOOL value) {  }
	virtual void SetCircularRampSpliceAddLengthNumSplice(long value) {  }

	virtual void SetCircularRampTopSpacerInfoSet(BOOL value) { }
	virtual void SetCircularRampTopSpacerSet(BOOL value) { }
	virtual void SetCircularRampTopSpacerDiaType(long value) { }
	virtual void SetCircularRampTopSpacerSlabThick(long value) { }
	virtual void SetCircularRampTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetCircularRampTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetCircularRampSpacerSupportBarsSet(BOOL value) { }
	virtual void SetCircularRampSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetCircularRampSpacerSupportBarsCondition(long value) { }
	virtual void SetCircularRampSpacerSupportBarsDiaType(long value) { }
	virtual void SetCircularRampSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetCircularRampSpacerSupportBarsNumType(long value) { }
	virtual void SetCircularRampSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetCircularRampSpacerSupportBarsUserNums(long value) { }
	virtual void SetCircularRampSpacerSupportBarsLengthType(long value) { }
	virtual void SetCircularRampSpacerSupportBarsUserRatio(double value) { }
	virtual void SetCircularRampSpacerSupportBarsUserLength(long value) { }

	virtual void SetCircularRampCZoneSpliceWay(long value) { }
	virtual void SetCircularRampCZoneSpliceLength(long value) { }

	// PC Slab
	virtual BOOL GetPCSlabMainBarInfoSet() { return FALSE; }
	virtual long GetPCSlabMaxDualHookLength() { return 0; }
	virtual long GetPCSlabSpliceWay() { return 0; }
	virtual long GetPCSlabSpliceType() { return 0; }

	virtual long GetPCSlabTopEndAddBarLxLnxType() { return  0; }
	virtual long GetPCSlabTopEndAddBarLengthType() { return  0; }
	virtual long GetPCSlabTopCenAddBarLxLnxType() { return  0; }
	virtual long GetPCSlabTopCenAddBarLengthType() { return  0; }

	virtual long GetPCSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetPCSlabBotEndDevTypeUserLength() { return 0; }

	virtual BOOL GetPCSlabShortCenSpliceBarPlace() { return  FALSE; }
	virtual long GetPCSlabShortCenSpliceBarLengthType() { return  0; }
	virtual BOOL GetPCSlabShortEndSpliceBarPlace() { return  FALSE; }
	virtual long GetPCSlabShortEndSpliceBarLengthType() { return  0; }
	virtual BOOL GetPCSlabLongCenSpliceBarPlace() { return  FALSE; }
	virtual long GetPCSlabLongCenSpliceBarLengthType() { return  0; }
	virtual BOOL GetPCSlabLongEndSpliceBarPlace() { return  FALSE; }
	virtual long GetPCSlabLongEndSpliceBarLengthType() { return  0; }

	virtual BOOL GetPCSlabUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetPCSlabTopUnStructuredEndHookType() { return 0; }
	virtual long GetPCSlabBotUnStructuredEndHookType() { return 0; }

	virtual BOOL GetPCSlabLevelDifInfoSet() { return FALSE; }
	virtual long GetPCSlabLevelDifPlaceShape() { return 0; }
	virtual long GetPCSlabLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetPCSlabOpeningInfoSet() { return FALSE; }
	virtual long GetPCSlabOpeningTopDevType() { return 0; }
	virtual long GetPCSlabOpeningBotDevType() { return 0; }

	virtual BOOL GetPCSlabRebarAreaInfoSet() { return FALSE; }
	virtual long GetPCSlabRebarTolDist() { return 0; }
	virtual long GetPCSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetPCSlabSpliceDevConditionSet() { return FALSE; }
	virtual long GetPCSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetPCSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetPCSlabSpliceAddLengthNumSplice() { return 0; }

	virtual void SetPCSlabMainBarInfoSet(BOOL value) { }
	virtual void SetPCSlabMaxDualHookLength(long value) { }
	virtual void SetPCSlabSpliceWay(long value) { }
	virtual void SetPCSlabSpliceType(long value) { }

	virtual void SetPCSlabTopEndAddBarLxLnxType(long value) { }
	virtual void SetPCSlabTopEndAddBarLengthType(long value) { }
	virtual void SetPCSlabTopCenAddBarLxLnxType(long value) { }
	virtual void SetPCSlabTopCenAddBarLengthType(long value) { }

	virtual void SetPCSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetPCSlabBotEndDevTypeUserLength(long value) {  }

	virtual void SetPCSlabShortCenSpliceBarPlace(BOOL value) { }
	virtual void SetPCSlabShortCenSpliceBarLengthType(long value) { }
	virtual void SetPCSlabShortEndSpliceBarPlace(BOOL value) { }
	virtual void SetPCSlabShortEndSpliceBarLengthType(long value) { }
	virtual void SetPCSlabLongCenSpliceBarPlace(BOOL value) { }
	virtual void SetPCSlabLongCenSpliceBarLengthType(long value) { }
	virtual void SetPCSlabLongEndSpliceBarPlace(BOOL value) { }
	virtual void SetPCSlabLongEndSpliceBarLengthType(long value) { }

	virtual void SetPCSlabUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetPCSlabTopUnStructuredEndHookType(long value) { }
	virtual void SetPCSlabBotUnStructuredEndHookType(long value) { }

	virtual void SetPCSlabLevelDifInfoSet(BOOL value) { }
	virtual void SetPCSlabLevelDifPlaceShape(long value) { }
	virtual void SetPCSlabLevelDifHaunchPlaceType(long value) { }

	virtual void SetPCSlabOpeningInfoSet(BOOL value) { }
	virtual void SetPCSlabOpeningTopDevType(long value) { }
	virtual void SetPCSlabOpeningBotDevType(long value) { }

	virtual void SetPCSlabRebarAreaInfoSet(BOOL value) { }
	virtual void SetPCSlabRebarTolDist(long value) { }
	virtual void SetPCSlabRebarTolLenDif(long value) { }

	virtual void SetPCSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetPCSlabSpliceDevConditionLength(long value) { }

	virtual void SetPCSlabSpliceAddLengthSet(BOOL value) {  }
	virtual void SetPCSlabSpliceAddLengthNumSplice(long value) {  }

	virtual map<long, long> GetPCSlabTopEndAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabTopEndAddBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabTopEndAddBarUserLen(long key, long value) { }

	virtual map<long, long> GetPCSlabTopCenAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabTopCenAddBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabTopCenAddBarUserLen(long key, long value) { }

	virtual map<long, long> GetPCSlabShortCenSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabShortCenSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabShortCenSpliceBarUserLen(long key, long value) { }

	virtual map<long, long> GetPCSlabShortEndSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabShortEndSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabShortEndSpliceBarUserLen(long key, long value) { }

	virtual map<long, long> GetPCSlabLongCenSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabLongCenSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabLongCenSpliceBarUserLen(long key, long value) { }

	virtual map<long, long> GetPCSlabLongEndSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetPCSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual long GetPCSlabLongEndSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetPCSlabLongEndSpliceBarUserLen(long key, long value) { }
	// 슬래브 보강근
	virtual BOOL GetSlabRebarRebarAreaInfoSet() { return FALSE; }
	virtual long GetSlabRebarRebarTolDist() { return 0; }
	virtual long GetSlabRebarRebarTolLenDif() { return 0; }

	virtual void SetSlabRebarRebarAreaInfoSet(BOOL value) { }
	virtual void SetSlabRebarRebarTolDist(long value) { }
	virtual void SetSlabRebarRebarTolLenDif(long value) { }

	// 철근 일체형 데크 슬래브
	virtual BOOL GetEmbeddedDeckMainBarInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckMaxDualHookLength() { return 0; }
	virtual long GetEmbeddedDeckSpliceWay() { return 0; }
	virtual long GetEmbeddedDeckSpliceType() { return 0; }

	virtual long GetEmbeddedDeckTopEndAddBarLxLnxType() { return 0; }
	virtual long GetEmbeddedDeckTopEndAddBarLengthType() { return 0; }
	virtual long GetEmbeddedDeckTopCenAddBarLxLnxType() { return 0; }
	virtual long GetEmbeddedDeckTopCenAddBarLengthType() { return 0; }

	virtual BOOL GetEmbeddedDeckSpliceBarPlace() { return FALSE; }
	virtual long GetEmbeddedDeckSpliceBarLengthType() { return 0; }

	virtual long GetEmbeddedDeckTopEndDevTypeUserLength() { return 0; }
	virtual long GetEmbeddedDeckBotEndDevTypeUserLength() { return 0; }

	virtual BOOL GetEmbeddedDeckUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckTopUnStructuredEndHookType() { return 0; }
	virtual long GetEmbeddedDeckBotUnStructuredEndHookType() { return 0; }

	virtual BOOL GetEmbeddedDeckLevelDifInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckLevelDifPlaceShape() { return 0; }
	virtual long GetEmbeddedDeckLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetEmbeddedDeckOpeningInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckOpeningTopDevType() { return 0; }
	virtual long GetEmbeddedDeckOpeningBotDevType() { return 0; }

	virtual BOOL GetEmbeddedDeckRebarAreaInfoSet() { return FALSE; }
	virtual long GetEmbeddedDeckRebarTolDist() { return 0; }
	virtual long GetEmbeddedDeckRebarTolLenDif() { return 0; }

	virtual BOOL GetEmbeddedDeckSpliceDevConditionSet() { return 0; }
	virtual long GetEmbeddedDeckSpliceDevConditionLength() { return 0; }

	virtual BOOL GetEmbeddedDeckSpliceAddLengthSet() { return FALSE; }
	virtual long GetEmbeddedDeckSpliceAddLengthNumSplice() { return 0; }

	virtual void SetEmbeddedDeckMainBarInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckMaxDualHookLength(long value) { }
	virtual void SetEmbeddedDeckSpliceWay(long value) { }
	virtual void SetEmbeddedDeckSpliceType(long value) { }

	virtual void SetEmbeddedDeckTopEndAddBarLxLnxType(long value) { }
	virtual void SetEmbeddedDeckTopEndAddBarLengthType(long value) { }
	virtual void SetEmbeddedDeckTopCenAddBarLxLnxType(long value) { }
	virtual void SetEmbeddedDeckTopCenAddBarLengthType(long value) { }

	virtual void SetEmbeddedDeckSpliceBarPlace(BOOL value) { }
	virtual void SetEmbeddedDeckSpliceBarLengthType(long value) { }

	virtual void SetEmbeddedDeckTopEndDevTypeUserLength(long value) { }
	virtual void SetEmbeddedDeckBotEndDevTypeUserLength(long value) { }

	virtual void SetEmbeddedDeckUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckTopUnStructuredEndHookType(long value) { }
	virtual void SetEmbeddedDeckBotUnStructuredEndHookType(long value) { }

	virtual void SetEmbeddedDeckLevelDifInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckLevelDifPlaceShape(long value) { }
	virtual void SetEmbeddedDeckLevelDifHaunchPlaceType(long value) { }

	virtual void SetEmbeddedDeckOpeningInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckOpeningTopDevType(long value) { }
	virtual void SetEmbeddedDeckOpeningBotDevType(long value) { }

	virtual void SetEmbeddedDeckRebarAreaInfoSet(BOOL value) { }
	virtual void SetEmbeddedDeckRebarTolDist(long value) { }
	virtual void SetEmbeddedDeckRebarTolLenDif(long value) { }

	virtual void SetEmbeddedDeckSpliceDevConditionSet(BOOL value) { }
	virtual void SetEmbeddedDeckSpliceDevConditionLength(long value) { }

	virtual void SetEmbeddedDeckSpliceAddLengthSet(BOOL value) {	}
	virtual void SetEmbeddedDeckSpliceAddLengthNumSplice(long value) { }

	virtual map<long, long> GetEmbeddedDeckTopEndAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetEmbeddedDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetEmbeddedDeckTopEndAddBarUserLen(double dDia) { return 0; }
	virtual void SetEmbeddedDeckTopEndAddBarUserLen(long key, long value) {  }

	virtual map<long, long> GetEmbeddedDeckTopCenAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetEmbeddedDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetEmbeddedDeckTopCenAddBarUserLen(double dDia) { return 0; }
	virtual void SetEmbeddedDeckTopCenAddBarUserLen(long key, long value) {  }

	virtual map<long, long> GetEmbeddedDeckSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetEmbeddedDeckSpliceBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetEmbeddedDeckSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetEmbeddedDeckSpliceBarUserLen(long key, long value) {  }

	// 일반 데크 슬래브
	virtual BOOL GetGeneralDeckMainBarInfoSet() { return FALSE; }
	virtual long GetGeneralDeckMaxDualHookLength() { return 0; }
	virtual long GetGeneralDeckSpliceWay() { return 0; }
	virtual long GetGeneralDeckSpliceType() { return 0; }

	virtual long GetGeneralDeckTopEndAddBarLxLnxType() { return 0; }
	virtual long GetGeneralDeckTopEndAddBarLengthType() { return 0; }
	virtual long GetGeneralDeckTopCenAddBarLxLnxType() { return 0; }
	virtual long GetGeneralDeckTopCenAddBarLengthType() { return 0; }

	virtual BOOL GetGeneralDeckSpliceBarPlace() { return FALSE; }
	virtual long GetGeneralDeckSpliceBarLengthType() { return 0; }

	virtual long GetGeneralDeckTopEndDevTypeUserLength() { return 0; }
	virtual long GetGeneralDeckBotEndDevTypeUserLength() { return 0; }

	virtual BOOL GetGeneralDeckUnStructuredEndInfoSet() { return FALSE; }
	virtual long GetGeneralDeckTopUnStructuredEndHookType() { return 0; }
	virtual long GetGeneralDeckBotUnStructuredEndHookType() { return 0; }

	virtual BOOL GetGeneralDeckLevelDifInfoSet() { return FALSE; }
	virtual long GetGeneralDeckLevelDifPlaceShape() { return 0; }
	virtual long GetGeneralDeckLevelDifHaunchPlaceType() { return 0; }

	virtual BOOL GetGeneralDeckOpeningInfoSet() { return FALSE; }
	virtual long GetGeneralDeckOpeningTopDevType() { return 0; }
	virtual long GetGeneralDeckOpeningBotDevType() { return 0; }

	virtual BOOL GetGeneralDeckRebarAreaInfoSet() { return FALSE; }
	virtual long GetGeneralDeckRebarTolDist() { return 0; }
	virtual long GetGeneralDeckRebarTolLenDif() { return 0; }

	virtual BOOL GetGeneralDeckSpliceDevConditionSet() { return 0; }
	virtual long GetGeneralDeckSpliceDevConditionLength() { return 0; }

	virtual BOOL GetGeneralDeckSpliceAddLengthSet() { return FALSE; }
	virtual long GetGeneralDeckSpliceAddLengthNumSplice() { return 0; }

	virtual void SetGeneralDeckMainBarInfoSet(BOOL value) { }
	virtual void SetGeneralDeckMaxDualHookLength(long value) { }
	virtual void SetGeneralDeckSpliceWay(long value) { }
	virtual void SetGeneralDeckSpliceType(long value) { }

	virtual void SetGeneralDeckTopEndAddBarLxLnxType(long value) { }
	virtual void SetGeneralDeckTopEndAddBarLengthType(long value) { }
	virtual void SetGeneralDeckTopCenAddBarLxLnxType(long value) { }
	virtual void SetGeneralDeckTopCenAddBarLengthType(long value) { }

	virtual void SetGeneralDeckSpliceBarPlace(BOOL value) { }
	virtual void SetGeneralDeckSpliceBarLengthType(long value) { }

	virtual void SetGeneralDeckTopEndDevTypeUserLength(long value) { }
	virtual void SetGeneralDeckBotEndDevTypeUserLength(long value) { }

	virtual void SetGeneralDeckUnStructuredEndInfoSet(BOOL value) { }
	virtual void SetGeneralDeckTopUnStructuredEndHookType(long value) { }
	virtual void SetGeneralDeckBotUnStructuredEndHookType(long value) { }

	virtual void SetGeneralDeckLevelDifInfoSet(BOOL value) { }
	virtual void SetGeneralDeckLevelDifPlaceShape(long value) { }
	virtual void SetGeneralDeckLevelDifHaunchPlaceType(long value) { }

	virtual void SetGeneralDeckOpeningInfoSet(BOOL value) { }
	virtual void SetGeneralDeckOpeningTopDevType(long value) { }
	virtual void SetGeneralDeckOpeningBotDevType(long value) { }

	virtual void SetGeneralDeckRebarAreaInfoSet(BOOL value) { }
	virtual void SetGeneralDeckRebarTolDist(long value) { }
	virtual void SetGeneralDeckRebarTolLenDif(long value) { }

	virtual void SetGeneralDeckSpliceDevConditionSet(BOOL value) { }
	virtual void SetGeneralDeckSpliceDevConditionLength(long value) { }

	virtual void SetGeneralDeckSpliceAddLengthSet(BOOL value) {	}
	virtual void SetGeneralDeckSpliceAddLengthNumSplice(long value) { }

	virtual map<long, long> GetGeneralDeckTopEndAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetGeneralDeckTopEndAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetGeneralDeckTopEndAddBarUserLen(double dDia) { return 0; }
	virtual void SetGeneralDeckTopEndAddBarUserLen(long key, long value) {  }

	virtual map<long, long> GetGeneralDeckTopCenAddBarUserLenMap() { return map<long, long>(); }
	virtual void SetGeneralDeckTopCenAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetGeneralDeckTopCenAddBarUserLen(double dDia) { return 0; }
	virtual void SetGeneralDeckTopCenAddBarUserLen(long key, long value) {  }

	virtual map<long, long> GetGeneralDeckSpliceBarUserLenMap() { return map<long, long>(); }
	virtual void SetGeneralDeckSpliceBarUserLenMap(map<long, long> mapUserLen) {}
	virtual long GetGeneralDeckSpliceBarUserLen(double dDia) { return 0; }
	virtual void SetGeneralDeckSpliceBarUserLen(long key, long value) {  }

	// PC IMT Slab
	virtual BOOL GetPCIMTSlabSpliceDevConditionSet() { return FALSE; }
	virtual long GetPCIMTSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetPCIMTSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetPCIMTSlabSpliceAddLengthNumSplice() { return 0; };

	virtual long GetPCIMTSlabMaxDualHookLength() { return 0; }
	virtual long GetPCIMTSlabSpliceWay() { return 0; }
	virtual long GetPCIMTSlabSpliceType() { return 0; }

	virtual long GetPCIMTSlabTopHookType() { return 0; }
	virtual long GetPCIMTSlabBotHookType() { return 0; }
	virtual long GetPCIMTSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetPCIMTSlabBotEndDevTypeUserLength() { return 0; }
	virtual long GetPCIMTSlabOpeningTopDevType() { return 0; }
	virtual long GetPCIMTSlabOpeningBotDevType() { return 0; }
	virtual long GetPCIMTSlabTopEndAddBarLxLnxType() { return 0; }
	virtual long GetPCIMTSlabTopEndAddBarLengthType() { return 0; }
	virtual long GetPCIMTSlabTopCenAddBarLxLnxType() { return 0; }
	virtual long GetPCIMTSlabTopCenAddBarLengthType() { return 0; }
	virtual BOOL GetPCIMTSlabShortSpliceBarPlace() { return FALSE; }
	virtual long GetPCIMTSlabShortSpliceBarLengthType() { return 0; }
	virtual BOOL GetPCIMTSlabLongSpliceBarPlace() { return FALSE; }

	virtual long GetPCIMTSlabRebarTolDist() { return 0; }
	virtual long GetPCIMTSlabRebarTolLenDif() { return 0; }

	virtual long GetPCIMTSlabLongSpliceBarType() { return 0; }


	virtual void SetPCIMTSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetPCIMTSlabSpliceDevConditionLength(long value) { }

	virtual void SetPCIMTSlabSpliceAddLengthSet(BOOL value) {  }
	virtual void SetPCIMTSlabSpliceAddLengthNumSplice(long value) { };

	virtual void SetPCIMTSlabMaxDualHookLength(long value) { }
	virtual void SetPCIMTSlabSpliceWay(long value) { }
	virtual void SetPCIMTSlabSpliceType(long value) { }

	virtual void SetPCIMTSlabTopHookType(long value) { }
	virtual void SetPCIMTSlabBotHookType(long value) { }
	virtual void SetPCIMTSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetPCIMTSlabBotEndDevTypeUserLength(long value) { }
	virtual void SetPCIMTSlabOpeningTopDevType(long value) { }
	virtual void SetPCIMTSlabOpeningBotDevType(long value) { }
	virtual void SetPCIMTSlabTopEndAddBarLxLnxType(long value) { }
	virtual void SetPCIMTSlabTopEndAddBarLengthType(long value) { }
	virtual void SetPCIMTSlabTopCenAddBarLxLnxType(long value) { }
	virtual void SetPCIMTSlabTopCenAddBarLengthType(long value) { }
	virtual void SetPCIMTSlabShortSpliceBarPlace(BOOL value) { }
	virtual void SetPCIMTSlabShortSpliceBarLengthType(long value) { }
	virtual void SetPCIMTSlabLongSpliceBarPlace(BOOL value) { }

	virtual void SetPCIMTSlabRebarTolDist(long value) { }
	virtual void SetPCIMTSlabRebarTolLenDif(long value) { }

	virtual void SetPCIMTSlabLongSpliceBarType(long value) { }
	//Function
	virtual map<long, long> GetPCIMTSlabTopEndAddBarUserLenMap() { return map<long, long>(); }
	virtual map<long, long> GetPCIMTSlabTopCenAddBarUserLenMap() { return map<long, long>(); }
	virtual map<long, long> GetPCIMTSlabShortSpliceBarUserLenMap() { return map<long, long>(); }

	virtual void SetPCIMTSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual void SetPCIMTSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen) {}
	virtual void SetPCIMTSlabShortSpliceBarUserLenMap(map<long, long> mapUserLen) {}

	virtual long GetPCIMTSlabTopEndAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCIMTSlabTopCenAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCIMTSlabShortSpliceBarUserLen(double dDia) { return 0; }

	virtual void SetPCIMTSlabTopEndAddBarUserLen(long key, long value) { }
	virtual void SetPCIMTSlabTopCenAddBarUserLen(long key, long value) { }
	virtual void SetPCIMTSlabShortSpliceBarUserLen(long key, long value) { }

	//PC HCS Slab
	virtual BOOL GetPCHCSSlabSpliceDevConditionSet() { return FALSE; }
	virtual long GetPCHCSSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetPCHCSSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetPCHCSSlabSpliceAddLengthNumSplice() { return 0; };

	virtual long GetPCHCSSlabMaxDualHookLength() { return 0; }
	virtual long GetPCHCSSlabSpliceWay() { return 0; }
	virtual long GetPCHCSSlabSpliceType() { return 0; }

	virtual long GetPCHCSSlabTopHookType() { return 0; }
	virtual long GetPCHCSSlabBotHookType() { return 0; }
	virtual long GetPCHCSSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetPCHCSSlabBotEndDevTypeUserLength() { return 0; }
	virtual long GetPCHCSSlabOpeningTopDevType() { return 0; }
	virtual long GetPCHCSSlabOpeningBotDevType() { return 0; }

	virtual long GetPCHCSSlabTopEndAddBarLxLnxType() { return 0; }
	virtual long GetPCHCSSlabTopEndAddBarLengthType() { return 0; }
	virtual long GetPCHCSSlabTopCenAddBarLxLnxType() { return 0; }
	virtual long GetPCHCSSlabTopCenAddBarLengthType() { return 0; }
	virtual long GetPCHCSSlabDevSubBarLengthType() { return 0; }
	virtual long GetPCHCSSlabShortCenSpliceBarLengthType() { return 0; }
	virtual long GetPCHCSSlabShortEndSpliceBarLengthType() { return 0; }
	virtual long GetPCHCSSlabLongCenSpliceBarLengthType() { return 0; }
	virtual long GetPCHCSSlabLongEndSpliceBarLengthType() { return 0; }

	virtual long GetPCHCSSlabRebarTolDist() { return 0; }
	virtual long GetPCHCSSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetPCHCSSlabShortCenSpliceBarPlace() { return FALSE; }
	virtual BOOL GetPCHCSSlabShortEndSpliceBarPlace() { return FALSE; }
	virtual BOOL GetPCHCSSlabLongCenSpliceBarPlace() { return FALSE; }
	virtual BOOL GetPCHCSSlabLongEndSpliceBarPlace() { return FALSE; }

	virtual void SetPCHCSSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetPCHCSSlabSpliceDevConditionLength(long value) { }

	virtual void SetPCHCSSlabSpliceAddLengthSet(BOOL value) { }
	virtual void SetPCHCSSlabSpliceAddLengthNumSplice(long value) { }

	virtual void SetPCHCSSlabMaxDualHookLength(long value) { }
	virtual void SetPCHCSSlabSpliceWay(long value) { }
	virtual void SetPCHCSSlabSpliceType(long value) { }

	virtual void SetPCHCSSlabTopHookType(long value) { }
	virtual void SetPCHCSSlabBotHookType(long value) { }
	virtual void SetPCHCSSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetPCHCSSlabBotEndDevTypeUserLength(long value) { }
	virtual void SetPCHCSSlabOpeningTopDevType(long value) { }
	virtual void SetPCHCSSlabOpeningBotDevType(long value) { }

	virtual void SetPCHCSSlabTopEndAddBarLxLnxType(long value) { }
	virtual void SetPCHCSSlabTopEndAddBarLengthType(long value) { }
	virtual void SetPCHCSSlabTopCenAddBarLxLnxType(long value) { }
	virtual void SetPCHCSSlabTopCenAddBarLengthType(long value) { }
	virtual void SetPCHCSSlabDevSubBarLengthType(long value) { }
	virtual void SetPCHCSSlabShortCenSpliceBarLengthType(long value) { }
	virtual void SetPCHCSSlabShortEndSpliceBarLengthType(long value) { }
	virtual void SetPCHCSSlabLongCenSpliceBarLengthType(long value) { }
	virtual void SetPCHCSSlabLongEndSpliceBarLengthType(long value) { }

	virtual void SetPCHCSSlabRebarTolDist(long value) { }
	virtual void SetPCHCSSlabRebarTolLenDif(long value) { }

	virtual void SetPCHCSSlabShortCenSpliceBarPlace(BOOL value) { }
	virtual void SetPCHCSSlabShortEndSpliceBarPlace(BOOL value) { }
	virtual void SetPCHCSSlabLongCenSpliceBarPlace(BOOL value) { }
	virtual void SetPCHCSSlabLongEndSpliceBarPlace(BOOL value) { }

	//Function
	virtual map<long, long> GetPCHCSSlabTopEndAddBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabTopCenAddBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabDevSubBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabShortCenSpliceBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabShortEndSpliceBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabLongCenSpliceBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCHCSSlabLongEndSpliceBarUserLenMap() { map<long, long> value; return value; }

	virtual void SetPCHCSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabDevSubBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabShortCenSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabShortEndSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCHCSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen) { }

	virtual long GetPCHCSSlabTopEndAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabTopCenAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabDevSubBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabShortCenSpliceBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabShortEndSpliceBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabLongCenSpliceBarUserLen(double dDia) { return 0; }
	virtual long GetPCHCSSlabLongEndSpliceBarUserLen(double dDia) { return 0; }

	virtual void SetPCHCSSlabTopEndAddBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabTopCenAddBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabDevSubBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabShortCenSpliceBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabShortEndSpliceBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabLongCenSpliceBarUserLen(long key, long value) { }
	virtual void SetPCHCSSlabLongEndSpliceBarUserLen(long key, long value) { }

	//PC RPS Slab
	virtual BOOL GetPCRPSSlabSpliceDevConditionSet() { return FALSE; }
	virtual long GetPCRPSSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetPCRPSSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetPCRPSSlabSpliceAddLengthNumSplice() { return 0; };

	virtual long GetPCRPSSlabMaxDualHookLength() { return 0; }
	virtual long GetPCRPSSlabSpliceWay() { return 0; }
	virtual long GetPCRPSSlabSpliceType() { return 0; }

	virtual long GetPCRPSSlabTopHookType() { return 0; }
	virtual long GetPCRPSSlabBotHookType() { return 0; }
	virtual long GetPCRPSSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetPCRPSSlabBotEndDevTypeUserLength() { return 0; }
	virtual long GetPCRPSSlabOpeningTopDevType() { return 0; }
	virtual long GetPCRPSSlabOpeningBotDevType() { return 0; }

	virtual long GetPCRPSSlabTopEndAddBarLxLnxType() { return 0; }
	virtual long GetPCRPSSlabTopEndAddBarLengthType() { return 0; }
	virtual long GetPCRPSSlabTopCenAddBarLxLnxType() { return 0; }
	virtual long GetPCRPSSlabTopCenAddBarLengthType() { return 0; }
	virtual long GetPCRPSSlabDevSubBarLengthType() { return 0; }
	virtual long GetPCRPSSlabLongCenSpliceBarLengthType() { return 0; }
	virtual long GetPCRPSSlabLongEndSpliceBarLengthType() { return 0; }

	virtual long GetPCRPSSlabRebarTolDist() { return 0; }
	virtual long GetPCRPSSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetPCRPSSlabLongCenSpliceBarPlace() { return FALSE; }
	virtual BOOL GetPCRPSSlabLongEndSpliceBarPlace() { return FALSE; }

	virtual void SetPCRPSSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetPCRPSSlabSpliceDevConditionLength(long value) { }

	virtual void SetPCRPSSlabSpliceAddLengthSet(BOOL value) { }
	virtual void SetPCRPSSlabSpliceAddLengthNumSplice(long value) { }

	virtual void SetPCRPSSlabMaxDualHookLength(long value) { }
	virtual void SetPCRPSSlabSpliceWay(long value) { }
	virtual void SetPCRPSSlabSpliceType(long value) { }

	virtual void SetPCRPSSlabTopHookType(long value) { }
	virtual void SetPCRPSSlabBotHookType(long value) { }
	virtual void SetPCRPSSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetPCRPSSlabBotEndDevTypeUserLength(long value) { }
	virtual void SetPCRPSSlabOpeningTopDevType(long value) { }
	virtual void SetPCRPSSlabOpeningBotDevType(long value) { }

	virtual void SetPCRPSSlabTopEndAddBarLxLnxType(long value) { }
	virtual void SetPCRPSSlabTopEndAddBarLengthType(long value) { }
	virtual void SetPCRPSSlabTopCenAddBarLxLnxType(long value) { }
	virtual void SetPCRPSSlabTopCenAddBarLengthType(long value) { }
	virtual void SetPCRPSSlabDevSubBarLengthType(long value) { }
	virtual void SetPCRPSSlabLongCenSpliceBarLengthType(long value) { }
	virtual void SetPCRPSSlabLongEndSpliceBarLengthType(long value) { }

	virtual void SetPCRPSSlabRebarTolDist(long value) { }
	virtual void SetPCRPSSlabRebarTolLenDif(long value) { }

	virtual void SetPCRPSSlabLongCenSpliceBarPlace(BOOL value) { }
	virtual void SetPCRPSSlabLongEndSpliceBarPlace(BOOL value) { }

	//Function
	virtual map<long, long> GetPCRPSSlabTopEndAddBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCRPSSlabTopCenAddBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCRPSSlabDevSubBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCRPSSlabLongCenSpliceBarUserLenMap() { map<long, long> value; return value; }
	virtual map<long, long> GetPCRPSSlabLongEndSpliceBarUserLenMap() { map<long, long> value; return value; }

	virtual void SetPCRPSSlabTopEndAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCRPSSlabTopCenAddBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCRPSSlabDevSubBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCRPSSlabLongCenSpliceBarUserLenMap(map<long, long> mapUserLen) { }
	virtual void SetPCRPSSlabLongEndSpliceBarUserLenMap(map<long, long> mapUserLen) { }

	virtual long GetPCRPSSlabTopEndAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCRPSSlabTopCenAddBarUserLen(double dDia) { return 0; }
	virtual long GetPCRPSSlabDevSubBarUserLen(double dDia) { return 0; }
	virtual long GetPCRPSSlabLongCenSpliceBarUserLen(double dDia) { return 0; }
	virtual long GetPCRPSSlabLongEndSpliceBarUserLen(double dDia) { return 0; }

	virtual void SetPCRPSSlabTopEndAddBarUserLen(long key, long value) { }
	virtual void SetPCRPSSlabTopCenAddBarUserLen(long key, long value) { }
	virtual void SetPCRPSSlabDevSubBarUserLen(long key, long value) { }
	virtual void SetPCRPSSlabLongCenSpliceBarUserLen(long key, long value) { }
	virtual void SetPCRPSSlabLongEndSpliceBarUserLen(long key, long value) { }

	virtual BOOL GetMatBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetMatTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetMatSpacerSupportBarUserSteelGradeS() { return FALSE; }
	virtual BOOL GetMatSupportRebarUserSteelGradeS() { return FALSE; }

	virtual BOOL GetFlatBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetFlatTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetFlatSpacerSupportBarUserSteelGradeS() { return FALSE; }

	virtual BOOL GetSlabBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetSlabTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetSlabSpacerSupportBarUserSteelGradeS() { return FALSE; }

	virtual BOOL GetSOGBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetSOGTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetSOGSpacerSupportBarUserSteelGradeS() { return FALSE; }
	virtual BOOL GetSOGSupportRebarUserSteelGradeS() { return FALSE; }

	virtual BOOL GetCircularRampBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetCircularRampTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetCircularRampSpacerSupportBarUserSteelGradeS() { return FALSE; }

	//
	virtual void SetMatBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetMatTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetMatSpacerSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetMatSupportRebarUserSteelGradeS(BOOL value) { }

	virtual void SetFlatBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetFlatTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetFlatSpacerSupportBarUserSteelGradeS(BOOL value) { }

	virtual void SetSlabBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetSlabTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetSlabSpacerSupportBarUserSteelGradeS(BOOL value) { }

	virtual void SetSOGBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetSOGTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetSOGSpacerSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetSOGSupportRebarUserSteelGradeS(BOOL value) { }

	virtual void SetCircularRampBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetCircularRampTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetCircularRampSpacerSupportBarUserSteelGradeS(BOOL value) { }

	virtual long GetEmbeddedDeckCZoneSpliceWay() { return 0; }
	virtual long GetEmbeddedDeckCZoneSpliceLength() { return 0; }
	virtual long GetGeneralDeckCZoneSpliceWay() { return 0; }
	virtual long GetGeneralDeckCZoneSpliceLength() { return 0; }
	virtual long GetPCSlabCZoneSpliceWay() { return 0; }
	virtual long GetPCSlabCZoneSpliceLength() { return 0; }
	virtual long GetPCIMTSlabCZoneSpliceWay() { return 0; }
	virtual long GetPCIMTSlabCZoneSpliceLength() { return 0; }
	virtual long GetPCHCSSlabCZoneSpliceWay() { return 0; }
	virtual long GetPCHCSSlabCZoneSpliceLength() { return 0; }
	virtual long GetPCRPSSlabCZoneSpliceWay() { return 0; }
	virtual long GetPCRPSSlabCZoneSpliceLength() { return 0; }

	virtual void SetEmbeddedDeckCZoneSpliceWay(long value) { }
	virtual void SetEmbeddedDeckCZoneSpliceLength(long value) { }
	virtual void SetGeneralDeckCZoneSpliceWay(long value) { }
	virtual void SetGeneralDeckCZoneSpliceLength(long value) { }
	virtual void SetPCSlabCZoneSpliceWay(long value) { }
	virtual void SetPCSlabCZoneSpliceLength(long value) { }
	virtual void SetPCIMTSlabCZoneSpliceWay(long value) { }
	virtual void SetPCIMTSlabCZoneSpliceLength(long value) { }
	virtual void SetPCHCSSlabCZoneSpliceWay(long value) { }
	virtual void SetPCHCSSlabCZoneSpliceLength(long value) { }
	virtual void SetPCRPSSlabCZoneSpliceWay(long value) { }
	virtual void SetPCRPSSlabCZoneSpliceLength(long value) { }


	//IsoFoot
	virtual BOOL GetIsoFootSpliceDevConditionSet() { return 0; }
	virtual long GetIsoFootSpliceDevConditionLength() { return 0; }

	virtual long GetIsoFootDevType() { return 0; }

	virtual BOOL GetIsoFootTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetIsoFootTopSpacerSet() { return FALSE; }
	virtual long GetIsoFootTopSpacerDiaType() { return 0; }
	virtual long GetIsoFootTopSpacerSlabThick() { return 0; }
	virtual long GetIsoFootTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetIsoFootTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetIsoFootSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetIsoFootSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetIsoFootSpacerSupportBarsCondition() { return 0; }
	virtual long GetIsoFootSpacerSupportBarsDiaType() { return 0; }
	virtual long GetIsoFootSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetIsoFootSpacerSupportBarsNumType() { return 0; }
	virtual double GetIsoFootSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetIsoFootSpacerSupportBarsUserNums() { return 0; }
	virtual long GetIsoFootSpacerSupportBarsLengthType() { return 0; }
	virtual double GetIsoFootSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetIsoFootSpacerSupportBarsUserLength() { return 0; }

	virtual BOOL GetIsoFootSupportRebarsSet() { return FALSE; }
	virtual long GetIsoFootSupportRebarsDiaIndex() { return 0; }
	virtual double GetIsoFootSupportRebarsSlabLengthRate() { return 0; }
	virtual long GetIsoFootSupportRebarsOrderLength() { return 0; }
	virtual long GetIsoFootSupportRebarsSpliceLength() { return 0; }
	virtual long GetIsoFootSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetIsoFootBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetIsoFootTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetIsoFootSpacerSupportBarUserSteelGradeS() { return FALSE; }
	virtual BOOL GetIsoFootSupportRebarUserSteelGradeS() { return FALSE; }


	virtual void SetIsoFootSpliceDevConditionSet(BOOL value) { }
	virtual void SetIsoFootSpliceDevConditionLength(long value) { }

	virtual void SetIsoFootDevType(long value) { }

	virtual void SetIsoFootTopSpacerInfoSet(BOOL value) { }
	virtual void SetIsoFootTopSpacerSet(BOOL value) { }
	virtual void SetIsoFootTopSpacerDiaType(long value) { }
	virtual void SetIsoFootTopSpacerSlabThick(long value) { }
	virtual void SetIsoFootTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetIsoFootTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetIsoFootSpacerSupportBarsSet(BOOL value) { }
	virtual void SetIsoFootSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetIsoFootSpacerSupportBarsCondition(long value) { }
	virtual void SetIsoFootSpacerSupportBarsDiaType(long value) { }
	virtual void SetIsoFootSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetIsoFootSpacerSupportBarsNumType(long value) { }
	virtual void SetIsoFootSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetIsoFootSpacerSupportBarsUserNums(long value) { }
	virtual void SetIsoFootSpacerSupportBarsLengthType(long value) { }
	virtual void SetIsoFootSpacerSupportBarsUserRatio(double value) { }
	virtual void SetIsoFootSpacerSupportBarsUserLength(long value) { }

	virtual void SetIsoFootSupportRebarsSet(BOOL value) { }
	virtual void SetIsoFootSupportRebarsDiaIndex(long value) { }
	virtual void SetIsoFootSupportRebarsSlabLengthRate(double value) { }
	virtual void SetIsoFootSupportRebarsOrderLength(long value) { }
	virtual void SetIsoFootSupportRebarsSpliceLength(long value) { }
	virtual void SetIsoFootSupportRebarsCorrectiveLength(long value) { }

	virtual void SetIsoFootBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetIsoFootTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetIsoFootSpacerSupportBarUserSteelGradeS(BOOL value) { }
	virtual void SetIsoFootSupportRebarUserSteelGradeS(BOOL value) { }

	//WallFoot
	virtual BOOL GetWallFootSpliceDevConditionSet() { return 0; }
	virtual long GetWallFootSpliceDevConditionLength() { return 0; }

	virtual long GetWallFootDevType() { return 0; }

	virtual BOOL GetWallFootTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetWallFootTopSpacerSet() { return FALSE; }
	virtual long GetWallFootTopSpacerDiaType() { return 0; }
	virtual long GetWallFootTopSpacerSlabThick() { return 0; }
	virtual long GetWallFootTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetWallFootTopSpacerSlabThickUnderDiaIndex() { return 0; }
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

	virtual BOOL GetWallFootSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetWallFootSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetWallFootSpacerSupportBarsCondition() { return 0; }
	virtual long GetWallFootSpacerSupportBarsDiaType() { return 0; }
	virtual long GetWallFootSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetWallFootSpacerSupportBarsNumType() { return 0; }
	virtual double GetWallFootSpacerSupportBarsTopSpacerNums() { return 0.0; }
	virtual long GetWallFootSpacerSupportBarsUserNums() { return 0; }
	virtual long GetWallFootSpacerSupportBarsLengthType() { return 0; }
	virtual double GetWallFootSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetWallFootSpacerSupportBarsUserLength() { return 0; }

	virtual BOOL GetWallFootBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetWallFootTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetWallFootSpacerSupportBarUserSteelGradeS() { return FALSE; }

	virtual void SetWallFootSpliceDevConditionSet(BOOL value) { }
	virtual void SetWallFootSpliceDevConditionLength(long value) { }

	virtual void SetWallFootDevType(long value) { }

	virtual void SetWallFootTopSpacerInfoSet(BOOL value) { }
	virtual void SetWallFootTopSpacerSet(BOOL value) { }
	virtual void SetWallFootTopSpacerDiaType(long value) { }
	virtual void SetWallFootTopSpacerSlabThick(long value) { }
	virtual void SetWallFootTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetWallFootTopSpacerSlabThickUnderDiaIndex(long value) { }
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

	virtual void SetWallFootSpacerSupportBarsSet(BOOL value) { }
	virtual void SetWallFootSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetWallFootSpacerSupportBarsCondition(long value) { }
	virtual void SetWallFootSpacerSupportBarsDiaType(long value) { }
	virtual void SetWallFootSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetWallFootSpacerSupportBarsNumType(long value) { }
	virtual void SetWallFootSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetWallFootSpacerSupportBarsUserNums(long value) { }
	virtual void SetWallFootSpacerSupportBarsLengthType(long value) { }
	virtual void SetWallFootSpacerSupportBarsUserRatio(double value) { }
	virtual void SetWallFootSpacerSupportBarsUserLength(long value) { }

	virtual void SetWallFootBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetWallFootTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetWallFootSpacerSupportBarUserSteelGradeS(BOOL value) { }

	virtual SlabHorSupportRebarsTemplate *GetWallFootSupportRebarsTemplate() { return nullptr; }
	//LowWall
	//Get Property
	virtual long GetLowWallVerBarTopLenType() { return 0; }
	virtual BOOL GetLowWallVerBarStirrup() { return FALSE; }
	virtual BOOL GetLowWallVerBarDoubleUbar() { return FALSE; }
	virtual BOOL GetLowWallVerBarDowelIntegrated() { return FALSE; }
	virtual long GetLowWallVerBarStirrupMaxLength() { return 0; }
	virtual long GetLowWallVerBarDoubleUbarMaxLength() { return 0; }
	virtual long GetLowWallVerBarDowelIntegratedLength() { return 0; }

	virtual BOOL GetLowWallHorBarStartSet() { return FALSE; }
	virtual long GetLowWallHorBarStartLength() { return 0; }

	virtual BOOL GetLowWallSupportRebarsSet() { return FALSE; }
	virtual long GetLowWallSupportRebarsDiaIndex() { return 0; }
	virtual double GetLowWallSupportRebarsTotalLengthRate() { return 0.0; }
	virtual long GetLowWallSupportRebarsOrderLength() { return 0; }
	virtual long GetLowWallSupportRebarsSpliceLength() { return 0; }
	virtual long GetLowWallSupportRebarsCorrectiveLength() { return 0; }

	virtual BOOL GetLowWallAddBarsSet() { return FALSE; }
	virtual long GetLowWallAddBarsDiaIndex() { return 0; }
	virtual long GetLowWallAddBarsNumber() { return 0; }
	virtual long GetLowWallAddBarsLengthType() { return 0; }
	virtual long GetLowWallAddBarsTensionLengthType() { return 0; }
	virtual long GetLowWallAddBarsUserLength() { return 0; }

	virtual BOOL GetLowWallSupportRebarUserSteelGradeS() { return FALSE; }
	virtual BOOL GetLowWallAddBarUserSteelGradeS() { return FALSE; }
	//Set Property
	virtual void SetLowWallVerBarTopLenType(long value) { }
	virtual void SetLowWallVerBarStirrup(BOOL value) { }
	virtual void SetLowWallVerBarDoubleUbar(BOOL value) {  }
	virtual void SetLowWallVerBarDowelIntegrated(BOOL value) { }
	virtual void SetLowWallVerBarStirrupMaxLength(long value) {  }
	virtual void SetLowWallVerBarDoubleUbarMaxLength(long value) { }
	virtual void SetLowWallVerBarDowelIntegratedLength(long value) {  }

	virtual void SetLowWallHorBarStartSet(BOOL value) { }
	virtual void SetLowWallHorBarStartLength(long value) { }

	virtual void SetLowWallSupportRebarsSet(BOOL value) { }
	virtual void SetLowWallSupportRebarsDiaIndex(long value) { }
	virtual void SetLowWallSupportRebarsTotalLengthRate(double value) { }
	virtual void SetLowWallSupportRebarsOrderLength(long value) { }
	virtual void SetLowWallSupportRebarsSpliceLength(long value) { }
	virtual void SetLowWallSupportRebarsCorrectiveLength(long value) { }

	virtual void SetLowWallAddBarsSet(BOOL value) { }
	virtual void SetLowWallAddBarsDiaIndex(long value) { }
	virtual void SetLowWallAddBarsNumber(long value) { }
	virtual void SetLowWallAddBarsLengthType(long value) { }
	virtual void SetLowWallAddBarsTensionLengthType(long value) { }
	virtual void SetLowWallAddBarsUserLength(long value) { }

	virtual void SetLowWallSupportRebarUserSteelGradeS(BOOL value) { }
	virtual void SetLowWallAddBarUserSteelGradeS(BOOL value) {  }

	// [이성우]RebarTemplate 에서는 빠져 있는데 배근에서 사용되는 함수에 대해 사용할수 있게 처리
	virtual long GetMatLevelDifTopHookType() { return 1; }
	virtual long GetMatLevelDifBotHookType() { return 1; }
	virtual long GetFlatLevelDifTopHookType() { return 1; }
	virtual long GetFlatLevelDifBotHookType() { return 1; }

	static void InitUserDowelSpliceLenMap(map<long, long> &mapUserDowelSpliceLen);
	void InitSubStirrupHookLenMap(map<long, long> &mapHookLen);
	void InitShearBarHookLenMap(map<long, long> &mapHookLenA, map<long, long> &mapHookLenB);
	void InitAddBarDiaIndexMap(map<long, long> &mapAddBarDiaIndex, bool bSameDia = false);
	void InitEndBarDiaIndexMap(map<long, long> &mapEndBarDiaIndex, bool bSameDia = false);
	void InitDefaultUbarLenMap(map<long, long> &mapDefaultUbarLen);
	static void InitUserDowelHookLenMap(map<long, long> &mapUserDowelHookLen);
	void InitSubStirrupDiaIndexMap(map<long, long> &mapSubStirrupDiaIndex);
	void InitShearBarVerUserDiaIndexMap(map<long, long> &mapShearBarVerUserDiaIndex);
	void InitShearBarHorUserDiaIndexMap(map<long, long> &mapShearBarHorUserDiaIndex);
	void InitUserLenMap(map<long, long> &mapUserLen, long value = 500);
	void InitUserDiaIndexMap(map<long, long> &mapBarDiaIndex);

	static void UserDowelSpliceLenMapDataCopy(map<long, long> &mapSrcUserDowelSpliceLen, map<long, long> &mapDesUserDowelSpliceLen);
	void SubStirrupHookLenMapDataCopy(map<long, long> &mapSrcHookLen, map<long, long> &mapDesHookLen);
	void ShearBarHookLenMapDataCopy(map<long, long> &mapSrcHookLenA, map<long, long> &mapSrcHookLenB, map<long, long> &mapDesHookLenA, map<long, long> &mapDesHookLenB);
	void AddBarDiaIndexMapDataCopy(map<long, long> &mapSrcAddBarDiaIndex, map<long, long> &mapDesAddBarDiaIndex);
	void EndBarDiaIndexMapDataCopy(map<long, long> &mapSrcEndBarDiaIndex, map<long, long> &mapDesEndBarDiaIndex);
	void DefaultUbarLenMapDataCopy(map<long, long> &mapSrcDefaultUbarLen, map<long, long> &mapDesDefaultUbarLen);
	static void UserDowelHookLenMapDataCopy(map<long, long> &mapSrcUserDowelHookLen, map<long, long> &mapDesUserDowelHookLen);
	void SubStirrupDiaIndexMapDataCopy(map<long, long> &mapSrcSubStirrupDiaIndex, map<long, long> &mapDesSubStirrupDiaIndex);
	void ShearBarVerUserDiaIndexMapDataCopy(map<long, long> &mapSrcShearBarVerUserDiaIndex, map<long, long> &mapDesShearBarVerUserDiaIndex);
	void ShearBarHorUserDiaIndexMapDataCopy(map<long, long> &mapSrcShearBarHorUserDiaIndex, map<long, long> &mapDesShearBarHorUserDiaIndex);
	void UserMapDataCopy(map<long, long> &mapSrcUser, map<long, long> &mapDesUser);

	//슬래브 상부 우마 배근조건──────────────────────
	virtual BOOL GetWallFootTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetWallFootTopSpacerPlaceCondition() { return 0; }
	virtual void SetWallFootTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetWallFootTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetIsoFootTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetIsoFootTopSpacerPlaceCondition() { return 0; }
	virtual void SetIsoFootTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetIsoFootTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetPitTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetPitTopSpacerPlaceCondition() { return 0; }
	virtual void SetPitTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetPitTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetMatTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetMatTopSpacerPlaceCondition() { return 0; }
	virtual void SetMatTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetMatTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetFlatTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetFlatTopSpacerPlaceCondition() { return 0; }
	virtual void SetFlatTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetFlatTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetSlabTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetSlabTopSpacerPlaceCondition() { return 0; }
	virtual void SetSlabTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetSlabTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetSOGTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetSOGTopSpacerPlaceCondition() { return 0; }
	virtual void SetSOGTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetSOGTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetCircularRampTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetCircularRampTopSpacerPlaceCondition() { return 0; }
	virtual void SetCircularRampTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetCircularRampTopSpacerPlaceCondition(long value) { }
	//────────────────────────────────────
	//벽체 선조립 우마 및 다월 UBar 고임철근 개발───────────────
	virtual BOOL GetBWallStraightCenterUmaShape() { return FALSE; }
	virtual void SetBWallStraightCenterUmaShape(BOOL value) { }

	virtual long GetBWallUmaSpacerDiaType() { return 0; }
	virtual long GetBWallUmaSpacerWallThick() { return 0; }
	virtual long GetBWallUmaSpacerWallThickOverDiaIndex() { return 0; }
	virtual long GetBWallUmaSpacerWallThickUnderDiaIndex() { return 0; }
	virtual BOOL GetBWallUmaSpacerUserSteelGradeS() { return FALSE; }
	virtual long GetBWallUmaSpacerSpacingType() { return 0; }
	virtual long GetBWallUmaSpacerWallSpacing() { return 0; }
	virtual long GetBWallUmaSpacerUserSpacing1() { return 0; }
	virtual long GetBWallUmaSpacerUserSpacing2() { return 0; }
	virtual long GetBWallUmaSpacerHeightType() { return 0; }
	virtual long GetBWallUmaSpacerUserHeight() { return 0; }
	virtual long GetBWallUmaSpacerHookLenAType() { return 0; }
	virtual long GetBWallUmaSpacerMainBarHookLenA() { return 0; }
	virtual long GetBWallUmaSpacerUserHookLenA() { return 0; }
	virtual long GetBWallUmaSpacerHookLenBType() { return 0; }
	virtual long GetBWallUmaSpacerMainBarHookLenB() { return 0; }
	virtual long GetBWallUmaSpacerUserHookLenB() { return 0; }
	virtual long GetBWallUmaSpacerWidth() { return 0; }

	virtual BOOL GetBWallUShapeTopSpacer() { return FALSE; }
	virtual BOOL GetBWallUShapeBotSpacer() { return FALSE; }
	virtual long GetBWallUShapeSpacerDiaType() { return 0; }
	virtual long GetBWallUShapeSpacerDiaIndex() { return 0; }
	virtual BOOL GetBWallUShapeSpacerUserSteelGradeS() { return FALSE; }
	virtual long GetBWallUShapeSpacerHorSpacingType() { return 0; }
	virtual long GetBWallUShapeSpacerHorSpacing() { return 0; }
	virtual long GetBWallUShapeSpacerHorSpacingRate() { return 0; }
	virtual long GetBWallUShapeSpacerHorSpacingCount() { return 0; }
	virtual long GetBWallUShapeSpacerLenType() { return 0; }
	virtual long GetBWallUShapeSpacerUserLen() { return 0; }

	virtual void SetBWallUmaSpacerDiaType(long value) { }
	virtual void SetBWallUmaSpacerWallThick(long value) { }
	virtual void SetBWallUmaSpacerWallThickOverDiaIndex(long value) { }
	virtual void SetBWallUmaSpacerWallThickUnderDiaIndex(long value) { }
	virtual void SetBWallUmaSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetBWallUmaSpacerSpacingType(long value) { }
	virtual void SetBWallUmaSpacerWallSpacing(long value) { }
	virtual void SetBWallUmaSpacerUserSpacing1(long value) { }
	virtual void SetBWallUmaSpacerUserSpacing2(long value) { }
	virtual void SetBWallUmaSpacerHeightType(long value) { }
	virtual void SetBWallUmaSpacerUserHeight(long value) { }
	virtual void SetBWallUmaSpacerHookLenAType(long value) { }
	virtual void SetBWallUmaSpacerMainBarHookLenA(long value) { }
	virtual void SetBWallUmaSpacerUserHookLenA(long value) { }
	virtual void SetBWallUmaSpacerHookLenBType(long value) { }
	virtual void SetBWallUmaSpacerMainBarHookLenB(long value) { }
	virtual void SetBWallUmaSpacerUserHookLenB(long value) { }
	virtual void SetBWallUmaSpacerWidth(long value) { }

	virtual void SetBWallUShapeTopSpacer(BOOL value) { }
	virtual void SetBWallUShapeBotSpacer(BOOL value) { }
	virtual void SetBWallUShapeSpacerDiaType(long value) { }
	virtual void SetBWallUShapeSpacerDiaIndex(long value) { }
	virtual void SetBWallUShapeSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetBWallUShapeSpacerHorSpacingType(long value) { }
	virtual void SetBWallUShapeSpacerHorSpacing(long value) { }
	virtual void SetBWallUShapeSpacerHorSpacingRate(long value) { }
	virtual void SetBWallUShapeSpacerHorSpacingCount(long value) { }
	virtual void SetBWallUShapeSpacerLenType(long value) { }
	virtual void SetBWallUShapeSpacerUserLen(long value) { }

	virtual BOOL GetRWallStraightCenterUmaShape() { return FALSE; }
	virtual void SetRWallStraightCenterUmaShape(BOOL value) { }

	virtual long GetRWallUmaSpacerDiaType() { return 0; }
	virtual long GetRWallUmaSpacerWallThick() { return 0; }
	virtual long GetRWallUmaSpacerWallThickOverDiaIndex() { return 0; }
	virtual long GetRWallUmaSpacerWallThickUnderDiaIndex() { return 0; }
	virtual BOOL GetRWallUmaSpacerUserSteelGradeS() { return 0; }
	virtual long GetRWallUmaSpacerSpacingType() { return 0; }
	virtual long GetRWallUmaSpacerWallSpacing() { return 0; }
	virtual long GetRWallUmaSpacerUserSpacing1() { return 0; }
	virtual long GetRWallUmaSpacerUserSpacing2() { return 0; }
	virtual long GetRWallUmaSpacerHeightType() { return 0; }
	virtual long GetRWallUmaSpacerUserHeight() { return 0; }
	virtual long GetRWallUmaSpacerHookLenAType() { return 0; }
	virtual long GetRWallUmaSpacerMainBarHookLenA() { return 0; }
	virtual long GetRWallUmaSpacerUserHookLenA() { return 0; }
	virtual long GetRWallUmaSpacerHookLenBType() { return 0; }
	virtual long GetRWallUmaSpacerMainBarHookLenB() { return 0; }
	virtual long GetRWallUmaSpacerUserHookLenB() { return 0; }
	virtual long GetRWallUmaSpacerWidth() { return 0; }

	virtual BOOL GetRWallUShapeTopSpacer() { return FALSE; }
	virtual BOOL GetRWallUShapeBotSpacer() { return FALSE; }
	virtual long GetRWallUShapeSpacerDiaType() { return 0; }
	virtual long GetRWallUShapeSpacerDiaIndex() { return 0; }
	virtual BOOL GetRWallUShapeSpacerUserSteelGradeS() { return FALSE; }
	virtual long GetRWallUShapeSpacerHorSpacingType() { return 0; }
	virtual long GetRWallUShapeSpacerHorSpacing() { return 0; }
	virtual long GetRWallUShapeSpacerHorSpacingRate() { return 0; }
	virtual long GetRWallUShapeSpacerHorSpacingCount() { return 0; }
	virtual long GetRWallUShapeSpacerLenType() { return 0; }
	virtual long GetRWallUShapeSpacerUserLen() { return 0; }
	
	virtual void SetRWallUmaSpacerDiaType(long value) { }
	virtual void SetRWallUmaSpacerWallThick(long value) { }
	virtual void SetRWallUmaSpacerWallThickOverDiaIndex(long value) { }
	virtual void SetRWallUmaSpacerWallThickUnderDiaIndex(long value) { }
	virtual void SetRWallUmaSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetRWallUmaSpacerSpacingType(long value) { }
	virtual void SetRWallUmaSpacerWallSpacing(long value) { }
	virtual void SetRWallUmaSpacerUserSpacing1(long value) { }
	virtual void SetRWallUmaSpacerUserSpacing2(long value) { }
	virtual void SetRWallUmaSpacerHeightType(long value) { }
	virtual void SetRWallUmaSpacerUserHeight(long value) { }
	virtual void SetRWallUmaSpacerHookLenAType(long value) { }
	virtual void SetRWallUmaSpacerMainBarHookLenA(long value) { }
	virtual void SetRWallUmaSpacerUserHookLenA(long value) { }
	virtual void SetRWallUmaSpacerHookLenBType(long value) { }
	virtual void SetRWallUmaSpacerMainBarHookLenB(long value) { }
	virtual void SetRWallUmaSpacerUserHookLenB(long value) { }
	virtual void SetRWallUmaSpacerWidth(long value) { }

	virtual void SetRWallUShapeTopSpacer(BOOL value) { }
	virtual void SetRWallUShapeBotSpacer(BOOL value) { }
	virtual void SetRWallUShapeSpacerDiaType(long value) { }
	virtual void SetRWallUShapeSpacerDiaIndex(long value) { }
	virtual void SetRWallUShapeSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetRWallUShapeSpacerHorSpacingType(long value) { }
	virtual void SetRWallUShapeSpacerHorSpacing(long value) { }
	virtual void SetRWallUShapeSpacerHorSpacingRate(long value) { }
	virtual void SetRWallUShapeSpacerHorSpacingCount(long value) { }
	virtual void SetRWallUShapeSpacerLenType(long value) { }
	virtual void SetRWallUShapeSpacerUserLen(long value) { }

	virtual BOOL GetBWallDowelPlaceSpacer(double dDia) { return FALSE; }
	virtual long GetBWallDowelSpacerDiaIndex(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerSpacing(double dDia) { return 0; }
	virtual BOOL GetBWallDowelSpacerUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetBWallDowelSpacerHeightType(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerHeightSpliceAddLength(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerUserHeight(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerLengthA(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerLengthB(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerWidthType(double dDia) { return 0; }
	virtual long GetBWallDowelSpacerUserWidth(double dDia) { return 0; }

	virtual BOOL GetRWallDowelPlaceSpacer(double dDia) { return FALSE; }
	virtual long GetRWallDowelSpacerDiaIndex(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerSpacing(double dDia) { return 0; }
	virtual BOOL GetRWallDowelSpacerUserSteelGradeS(double dDia) { return FALSE; }
	virtual long GetRWallDowelSpacerHeightType(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerHeightSpliceAddLength(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerUserHeight(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerLengthA(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerLengthB(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerWidthType(double dDia) { return 0; }
	virtual long GetRWallDowelSpacerUserWidth(double dDia) { return 0; }
	//────────────────────────────────────

	//────────────────────────────────────
	//오프닝 보강근 공제───────────────────────────
	virtual BOOL GetSlabOpeningAddBarDeductionCondition() { return FALSE; }
	virtual void SetSlabOpeningAddBarDeductionCondition(BOOL value) { }
	virtual long GetSlabOpeningAddBarDeductionPercent() { return 0; }
	virtual void SetSlabOpeningAddBarDeductionPercent(long value) { }

	virtual BOOL GetBWallOpeningAddBarDeductionCondition() { return FALSE; }
	virtual void SetBWallOpeningAddBarDeductionCondition(BOOL value) { }

	virtual BOOL GetRWallOpeningAddBarDeductionCondition() { return FALSE; }
	virtual void SetRWallOpeningAddBarDeductionCondition(BOOL value) { }
	//────────────────────────────────────
	virtual BOOL GetBWallBraceSupportBarDiaConditionHeight() { return FALSE; }
	virtual void SetBWallBraceSupportBarDiaConditionHeight(BOOL value) { }
	virtual long GetBWallBraceSupportBarUnderDiaIndex() { return 0; }
	virtual void SetBWallBraceSupportBarUnderDiaIndex(long value) { }

	virtual BOOL GetRWallBraceSupportBarDiaConditionHeight() { return FALSE; }
	virtual void SetRWallBraceSupportBarDiaConditionHeight(BOOL value) { }
	virtual long GetRWallBraceSupportBarUnderDiaIndex() { return 0; }
	virtual void SetRWallBraceSupportBarUnderDiaIndex(long value) { }

	virtual BOOL GetBWallSpacerVerStartSpacingSet() { return FALSE; }
	virtual void SetBWallSpacerVerStartSpacingSet(BOOL value) { }
	virtual long GetBWallSpacerVerStartSpacing() { return 0; }
	virtual void SetBWallSpacerVerStartSpacing(long value) { }

	virtual BOOL GetRWallSpacerVerStartSpacingSet() { return FALSE; }
	virtual void SetRWallSpacerVerStartSpacingSet(BOOL value) { }
	virtual long GetRWallSpacerVerStartSpacing() { return 0; }
	virtual void SetRWallSpacerVerStartSpacing(long value) { }
	//TransferSlab───────────────────────────
	virtual BOOL GetTransferSlabMainBarInfoSet() { return FALSE; }
	virtual long GetTransferSlabMaxDualHookLength() { return 0; }
	virtual long GetTransferSlabSpliceWay() { return 0; }
	virtual long GetTransferSlabSpliceType() { return 0; }

	virtual BOOL GetTransferSlabEndInfoSet() { return FALSE; }
	virtual long GetTransferSlabTopEndHookType() { return 0; }
	virtual long GetTransferSlabBotEndHookType() { return 0; }
	virtual long GetTransferSlabIsoFootDevType() { return 0; }
	virtual long GetTransferSlabSpliceDevSlabType() { return 0; }

	virtual long GetTransferSlabTopEndDevTypeUserLength() { return 0; }
	virtual long GetTransferSlabBotEndDevTypeUserLength() { return 0; }
	virtual long GetTransferSlabOpeningTopDevType() { return 0; }
	virtual long GetTransferSlabOpeningBotDevType() { return 0; }

	virtual BOOL GetTransferSlabLevelDifInfoSet() { return FALSE; }
	virtual long GetTransferSlabLevelDifPlaceShape() { return 0; }
	virtual long GetTransferSlabLevelDifHaunchPlaceType() { return 0; }


	virtual BOOL GetTransferSlabOpeningInfoSet() { return FALSE; }
	
	virtual BOOL GetTransferSlabRebarAreaInfoSet() { return FALSE; }
	virtual long GetTransferSlabRebarTolDist() { return 0; }
	virtual long GetTransferSlabRebarTolLenDif() { return 0; }

	virtual BOOL GetTransferSlabSpliceDevConditionSet() { return FALSE; }
	virtual long GetTransferSlabSpliceDevConditionLength() { return 0; }

	virtual BOOL GetTransferSlabSpliceAddLengthSet() { return FALSE; }
	virtual long GetTransferSlabSpliceAddLengthNumSplice() { return 0; }

	virtual BOOL GetTransferSlabTopSpacerInfoSet() { return FALSE; }
	virtual BOOL GetTransferSlabTopSpacerSet() { return FALSE; }
	virtual long GetTransferSlabTopSpacerDiaType() { return 0; }
	virtual long GetTransferSlabTopSpacerSlabThick() { return 0; }
	virtual long GetTransferSlabTopSpacerSlabThickOverDiaIndex() { return 0; }
	virtual long GetTransferSlabTopSpacerSlabThickUnderDiaIndex() { return 0; }
	virtual long GetTransferSlabTopSpacerSpacingType() { return 0; }
	virtual long GetTransferSlabTopSpacerSlabSpacing() { return 0; }
	virtual long GetTransferSlabTopSpacerUserSpacing1() { return 0; }
	virtual long GetTransferSlabTopSpacerUserSpacing2() { return 0; }
	virtual long GetTransferSlabTopSpacerHeightType() { return 0; }
	virtual long GetTransferSlabTopSpacerUserHeight() { return 0; }
	virtual long GetTransferSlabTopSpacerHookLenAType() { return 0; }
	virtual long GetTransferSlabTopSpacerMainBarHookLenA() { return 0; }
	virtual long GetTransferSlabTopSpacerUserHookLenA() { return 0; }
	virtual long GetTransferSlabTopSpacerHookLenBType() { return 0; }
	virtual long GetTransferSlabTopSpacerMainBarHookLenB() { return 0; }
	virtual long GetTransferSlabTopSpacerUserHookLenB() { return 0; }
	virtual long GetTransferSlabTopSpacerWidth() { return 0; }

	virtual BOOL GetTransferSlabBotSpacerInfoSet() { return FALSE; }
	virtual BOOL GetTransferSlabBotSpacerSet() { return FALSE; }
	virtual long GetTransferSlabBotSpacerDiaType() { return 0; }
	virtual long GetTransferSlabBotSpacerDiaIndex() { return 0; }
	virtual long GetTransferSlabBotSpacerSpacingType() { return 0; }
	virtual long GetTransferSlabBotSpacerSlabSpacing() { return 0; }
	virtual long GetTransferSlabBotSpacerUserSpacing1() { return 0; }
	virtual long GetTransferSlabBotSpacerUserSpacing2() { return 0; }
	virtual long GetTransferSlabBotSpacerHeightType() { return 0; }
	virtual long GetTransferSlabBotSpacerUserHeight() { return 0; }
	virtual long GetTransferSlabBotSpacerHookLenAType() { return 0; }
	virtual long GetTransferSlabBotSpacerMainBarHookLenA() { return 0; }
	virtual long GetTransferSlabBotSpacerUserHookLenA() { return 0; }
	virtual long GetTransferSlabBotSpacerHookLenBType() { return 0; }
	virtual long GetTransferSlabBotSpacerMainBarHookLenB() { return 0; }
	virtual long GetTransferSlabBotSpacerUserHookLenB() { return 0; }
	virtual long GetTransferSlabBotSpacerWidth() { return 0; }

	virtual BOOL GetTransferSlabSpacerSupportBarsSet() { return FALSE; }
	virtual BOOL GetTransferSlabSpacerSupportBarsConditionSet() { return FALSE; }
	virtual long GetTransferSlabSpacerSupportBarsCondition() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsDiaType() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsUserDiaIndex() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsNumType() { return 0; }
	virtual double GetTransferSlabSpacerSupportBarsTopSpacerNums() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsUserNums() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsLengthType() { return 0; }
	virtual double GetTransferSlabSpacerSupportBarsUserRatio() { return 0; }
	virtual long GetTransferSlabSpacerSupportBarsUserLength() { return 0; }

	virtual long GetTransferSlabCZoneSpliceWay() { return 0; }
	virtual long GetTransferSlabCZoneSpliceLength() { return 0; }

	virtual void SetTransferSlabMainBarInfoSet(BOOL value) { }
	virtual void SetTransferSlabMaxDualHookLength(long value) { }
	virtual void SetTransferSlabSpliceWay(long value) { }
	virtual void SetTransferSlabSpliceType(long value) { }

	virtual void SetTransferSlabEndInfoSet(BOOL value) { }
	virtual void SetTransferSlabTopEndHookType(long value) { }
	virtual void SetTransferSlabBotEndHookType(long value) { }
	virtual void SetTransferSlabIsoFootDevType(long value) { }
	virtual void SetTransferSlabSpliceDevSlabType(long value) { }

	virtual void SetTransferSlabTopEndDevTypeUserLength(long value) { }
	virtual void SetTransferSlabBotEndDevTypeUserLength(long value) { }
	virtual void SetTransferSlabOpeningTopDevType(long value) { }
	virtual void SetTransferSlabOpeningBotDevType(long value) { }

	virtual void SetTransferSlabLevelDifInfoSet(BOOL value) { }
	virtual void SetTransferSlabLevelDifPlaceShape(long value) { }
	virtual void SetTransferSlabLevelDifHaunchPlaceType(long value) { }


	virtual void SetTransferSlabOpeningInfoSet(BOOL value) { }
	
	virtual void SetTransferSlabRebarAreaInfoSet(BOOL value) { }
	virtual void SetTransferSlabRebarTolDist(long value) { }
	virtual void SetTransferSlabRebarTolLenDif(long value) { }

	virtual void SetTransferSlabSpliceDevConditionSet(BOOL value) { }
	virtual void SetTransferSlabSpliceDevConditionLength(long value) { }

	virtual void SetTransferSlabSpliceAddLengthSet(BOOL value) { }
	virtual void SetTransferSlabSpliceAddLengthNumSplice(long value) { }

	virtual void SetTransferSlabTopSpacerInfoSet(BOOL value) { }
	virtual void SetTransferSlabTopSpacerSet(BOOL value) { }
	virtual void SetTransferSlabTopSpacerDiaType(long value) { }
	virtual void SetTransferSlabTopSpacerSlabThick(long value) { }
	virtual void SetTransferSlabTopSpacerSlabThickOverDiaIndex(long value) { }
	virtual void SetTransferSlabTopSpacerSlabThickUnderDiaIndex(long value) { }
	virtual void SetTransferSlabTopSpacerSpacingType(long value) { }
	virtual void SetTransferSlabTopSpacerSlabSpacing(long value) { }
	virtual void SetTransferSlabTopSpacerUserSpacing1(long value) { }
	virtual void SetTransferSlabTopSpacerUserSpacing2(long value) { }
	virtual void SetTransferSlabTopSpacerHeightType(long value) { }
	virtual void SetTransferSlabTopSpacerUserHeight(long value) { }
	virtual void SetTransferSlabTopSpacerHookLenAType(long value) { }
	virtual void SetTransferSlabTopSpacerMainBarHookLenA(long value) { }
	virtual void SetTransferSlabTopSpacerUserHookLenA(long value) { }
	virtual void SetTransferSlabTopSpacerHookLenBType(long value) { }
	virtual void SetTransferSlabTopSpacerMainBarHookLenB(long value) { }
	virtual void SetTransferSlabTopSpacerUserHookLenB(long value) { }
	virtual void SetTransferSlabTopSpacerWidth(long value) { }

	virtual void SetTransferSlabBotSpacerInfoSet(BOOL value) { }
	virtual void SetTransferSlabBotSpacerSet(BOOL value) { }
	virtual void SetTransferSlabBotSpacerDiaType(long value) { }
	virtual void SetTransferSlabBotSpacerDiaIndex(long value) { }
	virtual void SetTransferSlabBotSpacerSpacingType(long value) { }
	virtual void SetTransferSlabBotSpacerSlabSpacing(long value) { }
	virtual void SetTransferSlabBotSpacerUserSpacing1(long value) { }
	virtual void SetTransferSlabBotSpacerUserSpacing2(long value) { }
	virtual void SetTransferSlabBotSpacerHeightType(long value) { }
	virtual void SetTransferSlabBotSpacerUserHeight(long value) { }
	virtual void SetTransferSlabBotSpacerHookLenAType(long value) { }
	virtual void SetTransferSlabBotSpacerMainBarHookLenA(long value) { }
	virtual void SetTransferSlabBotSpacerUserHookLenA(long value) { }
	virtual void SetTransferSlabBotSpacerHookLenBType(long value) { }
	virtual void SetTransferSlabBotSpacerMainBarHookLenB(long value) { }
	virtual void SetTransferSlabBotSpacerUserHookLenB(long value) { }
	virtual void SetTransferSlabBotSpacerWidth(long value) { }

	virtual void SetTransferSlabSpacerSupportBarsSet(BOOL value) { }
	virtual void SetTransferSlabSpacerSupportBarsConditionSet(BOOL value) { }
	virtual void SetTransferSlabSpacerSupportBarsCondition(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsDiaType(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsUserDiaIndex(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsNumType(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsTopSpacerNums(double value) { }
	virtual void SetTransferSlabSpacerSupportBarsUserNums(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsLengthType(long value) { }
	virtual void SetTransferSlabSpacerSupportBarsUserRatio(double value) { }
	virtual void SetTransferSlabSpacerSupportBarsUserLength(long value) { }

	virtual void SetTransferSlabCZoneSpliceWay(long value) { }
	virtual void SetTransferSlabCZoneSpliceLength(long value) { }
			
	virtual ShearFrictionBarTemplate *GetTransferSlabShearFrictionBarTemplate() { return nullptr; }
	
	virtual BOOL GetTransferSlabBotSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetTransferSlabTopSpacerUserSteelGradeS() { return FALSE; }
	virtual BOOL GetTransferSlabSpacerSupportBarUserSteelGradeS() { return FALSE; }

	virtual void SetTransferSlabBotSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetTransferSlabTopSpacerUserSteelGradeS(BOOL value) { }
	virtual void SetTransferSlabSpacerSupportBarUserSteelGradeS(BOOL value) { }
	
	virtual BOOL GetTransferSlabTopSpacerPlaceConditionSet() { return FALSE; }
	virtual long GetTransferSlabTopSpacerPlaceCondition() { return 0; }
	virtual void SetTransferSlabTopSpacerPlaceConditionSet(BOOL value) { }
	virtual void SetTransferSlabTopSpacerPlaceCondition(long value) { }

	virtual BOOL GetTransferSlabEndAddBarSet() { return FALSE; }
	virtual long GetTransferSlabEndVerAddBarDiaIndex() { return 0; }
	virtual long GetTransferSlabEndVerAddBarSpacing() { return 0; }
	virtual long GetTransferSlabEndHorAddBarPlaceCondition() { return 0; }
	virtual long GetTransferSlabEndHorAddBarDiaIndex() { return 0; }
	virtual long GetTransferSlabEndHorAddBarSpacing() { return 0; }

	virtual void SetTransferSlabEndAddBarSet(BOOL value) {  }
	virtual void SetTransferSlabEndVerAddBarDiaIndex(long value) {  }
	virtual void SetTransferSlabEndVerAddBarSpacing(long value) {  }
	virtual void SetTransferSlabEndHorAddBarPlaceCondition(long value) {  }
	virtual void SetTransferSlabEndHorAddBarDiaIndex(long value) {  }
	virtual void SetTransferSlabEndHorAddBarSpacing(long value) {  }
	//────────────────────────────────────
	// Q24-528 지하 선조립 우마 적용 시 벽체 높이 5000이상인 경우에만 반영되므로, 높이 제한 입력
	virtual BOOL GetBWallUmaSpacerConditionSet() { return FALSE; }
	virtual long GetBWallUmaSpacerCondition() { return 0; }
	virtual void SetBWallUmaSpacerConditionSet(BOOL value) { }
	virtual void SetBWallUmaSpacerCondition(long value) { }

	virtual BOOL GetRWallUmaSpacerConditionSet() { return FALSE; }
	virtual long GetRWallUmaSpacerCondition() { return 0; }
	virtual void SetRWallUmaSpacerConditionSet(BOOL value) { }
	virtual void SetRWallUmaSpacerCondition(long value) { }

	//────────────────────────────────────
	// Q24-864 이음여유치수
	virtual BOOL GetWallFootSpliceAddLengthSet() { return FALSE; }
	virtual void SetWallFootSpliceAddLengthSet(BOOL value) { }
	virtual long GetWallFootSpliceAddLengthNumSplice() { return 0; }
	virtual void SetWallFootSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetIsoFootSpliceAddLengthSet() { return FALSE; }
	virtual void SetIsoFootSpliceAddLengthSet(BOOL value) { }
	virtual long GetIsoFootSpliceAddLengthNumSplice() { return 0; }
	virtual void SetIsoFootSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetPitSpliceAddLengthSet() { return FALSE; }
	virtual void SetPitSpliceAddLengthSet(BOOL value) { }
	virtual long GetPitSpliceAddLengthNumSplice() { return 0; }
	virtual void SetPitSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetBeamSpliceAddLengthSet() { return FALSE; }
	virtual void SetBeamSpliceAddLengthSet(BOOL value) { }
	virtual long GetBeamSpliceAddLengthNumSplice() { return 0; }
	virtual void SetBeamSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetTransferBeamSpliceAddLengthSet() { return FALSE; }
	virtual void SetTransferBeamSpliceAddLengthSet(BOOL value) { }
	virtual long GetTransferBeamSpliceAddLengthNumSplice() { return 0; }
	virtual void SetTransferBeamSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetDeckBeamSpliceAddLengthSet() { return FALSE; }
	virtual void SetDeckBeamSpliceAddLengthSet(BOOL value) { }
	virtual long GetDeckBeamSpliceAddLengthNumSplice() { return 0; }
	virtual void SetDeckBeamSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetLowWallSpliceAddLengthSet() { return FALSE; }
	virtual void SetLowWallSpliceAddLengthSet(BOOL value) { }
	virtual long GetLowWallSpliceAddLengthNumSplice() { return 0; }
	virtual void SetLowWallSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetLowWallVerBarStartSet() { return FALSE; }
	virtual void SetLowWallVerBarStartSet(BOOL value) { }
	virtual long GetLowWallVerBarStartLength() { return 0; }
	virtual void SetLowWallVerBarStartLength(long value) { }

	virtual BOOL GetBWallSpliceAddLengthSet() { return FALSE; }
	virtual void SetBWallSpliceAddLengthSet(BOOL value) { }
	virtual long GetBWallSpliceAddLengthNumSplice() { return 0; }
	virtual void SetBWallSpliceAddLengthNumSplice(long value) { }

	virtual BOOL GetRWallSpliceAddLengthSet() { return FALSE; }
	virtual void SetRWallSpliceAddLengthSet(BOOL value) { }
	virtual long GetRWallSpliceAddLengthNumSplice() { return 0; }
	virtual void SetRWallSpliceAddLengthNumSplice(long value) { }

	virtual long GetDeckBeamSidePressureBarDiaIndex() { return 0; }
	virtual long GetDeckBeamSidePressureBarNum() { return 0; }
	virtual long GetDeckBeamSidePressureBarSpacing() { return 0; }

	virtual void SetDeckBeamSidePressureBarDiaIndex(long value) { }
	virtual void SetDeckBeamSidePressureBarNum(long value) { }
	virtual void SetDeckBeamSidePressureBarSpacing(long value) { }

	virtual map<CString, long> GetMatThickToDiaIndex() { return map<CString, long>(); }
	virtual map<CString, long> GetFlatThickToDiaIndex() { return map<CString, long>(); }
	virtual map<CString, long> GetSlabThickToDiaIndex() { return map<CString, long>(); }
	virtual map<CString, long> GetSOGThickToDiaIndex() { return map<CString, long>(); }
	virtual map<CString, long> GetCircularRampThickToDiaIndex() { return map<CString, long>(); }
	virtual map<CString, long> GetTransferSlabThickToDiaIndex() { return map<CString, long>(); }

	virtual void SetMatThickToDiaIndex(map<CString, long> value) { }
	virtual void SetFlatThickToDiaIndex(map<CString, long> value) { }
	virtual void SetSlabThickToDiaIndex(map<CString, long> value) { }
	virtual void SetSOGThickToDiaIndex(map<CString, long> value) { }
	virtual void SetCircularRampThickToDiaIndex(map<CString, long> value) { }
	virtual void SetTransferSlabThickToDiaIndex(map<CString, long> value) { }

	virtual long GetBWallPlusCrossShapeType() { return 0; }
	virtual long GetBWallTCrossShapeType() { return 0; }
	virtual long GetBWallLCrossShapeType() { return 0; }

	virtual void SetBWallPlusCrossShapeType(long value) { }
	virtual void SetBWallTCrossShapeType(long value) { }
	virtual void SetBWallLCrossShapeType(long value) { }

	virtual long GetRWallLCrossShapeType() { return 0; }

	virtual void SetRWallLCrossShapeType(long value) { }

	virtual BOOL GetSlabLongEndPlaceAreaSet() { return FALSE; }
	virtual long GetSlabLongEndPlaceAreaOneWaySlabType() { return 0; }
	virtual long GetSlabLongEndPlaceAreaTwoWaySlabType() { return 0; }
	virtual BOOL GetSlabBotAddBarLengthSet() { return FALSE; }
	virtual long GetSlabBotAddBarLengthType() { return 0; }
	virtual BOOL GetSlabBotAddBarCutBarLenExclusionSet() { return FALSE; }

	virtual void  SetSlabLongEndPlaceAreaSet(BOOL value) { }
	virtual void  SetSlabLongEndPlaceAreaOneWaySlabType(long value) { }
	virtual void  SetSlabLongEndPlaceAreaTwoWaySlabType(long value) { }
	virtual void  SetSlabBotAddBarLengthSet(BOOL value) { }
	virtual void  SetSlabBotAddBarLengthType(long value) { }
	virtual void  SetSlabBotAddBarCutBarLenExclusionSet(BOOL value) { }

	virtual long GetSlabOpeningAddBarDevLengthType() { return 0; }
	virtual void SetSlabOpeningAddBarDevLengthType(long value) { }

	virtual long GetBWallStraightCenterUmaPlaceType() { return 0; }
	virtual void SetBWallStraightCenterUmaPlaceType(long value) { }

	virtual long GetRWallStraightCenterUmaPlaceType() { return 0; }
	virtual void SetRWallStraightCenterUmaPlaceType(long value) { }

	virtual BOOL GetBeamPlaceHorShearBar() { return FALSE; }
	virtual long GetBeamHorShearBarMainDirDiaIndex() { return 0; }
	virtual long GetBeamHorShearBarMainDirPlaceType() { return 0; }
	virtual long GetBeamHorShearBarMainDirSpacing() { return 0; }
	virtual long GetBeamHorShearBarMainDirNumBar() { return 0; }
	virtual long GetBeamHorShearBarStirrupDirDiaIndex() { return 0; }
	virtual long GetBeamHorShearBarStirrupDirSpacing() { return 0; }
	virtual BOOL GetBeamHorShearBarMainDirUserSteelGradeS() { return FALSE; }
	virtual BOOL GetBeamHorShearBarStirrupDirUserSteelGradeS() { return FALSE; }

	virtual void SetBeamPlaceHorShearBar(BOOL value) { }
	virtual void SetBeamHorShearBarMainDirDiaIndex(long value) { }
	virtual void SetBeamHorShearBarMainDirPlaceType(long value) { }
	virtual void SetBeamHorShearBarMainDirSpacing(long value) { }
	virtual void SetBeamHorShearBarMainDirNumBar(long value) { }
	virtual void SetBeamHorShearBarStirrupDirDiaIndex(long value) { }
	virtual void SetBeamHorShearBarStirrupDirSpacing(long value) { }
	virtual void SetBeamHorShearBarMainDirUserSteelGradeS(BOOL value) { }
	virtual void SetBeamHorShearBarStirrupDirUserSteelGradeS(BOOL value) { }

	virtual BOOL GetTransferBeamPlaceHorShearBar() { return FALSE; }
	virtual long GetTransferBeamHorShearBarMainDirDiaIndex() { return 0; }
	virtual long GetTransferBeamHorShearBarMainDirPlaceType() { return 0; }
	virtual long GetTransferBeamHorShearBarMainDirSpacing() { return 0; }
	virtual long GetTransferBeamHorShearBarMainDirNumBar() { return 0; }
	virtual long GetTransferBeamHorShearBarStirrupDirDiaIndex() { return 0; }
	virtual long GetTransferBeamHorShearBarStirrupDirSpacing() { return 0; }
	virtual BOOL GetTransferBeamHorShearBarMainDirUserSteelGradeS() { return FALSE; }
	virtual BOOL GetTransferBeamHorShearBarStirrupDirUserSteelGradeS() { return FALSE; }

	virtual void SetTransferBeamPlaceHorShearBar(BOOL value) {}
	virtual void SetTransferBeamHorShearBarMainDirDiaIndex(long value) {}
	virtual void SetTransferBeamHorShearBarMainDirPlaceType(long value) {}
	virtual void SetTransferBeamHorShearBarMainDirSpacing(long value) {}
	virtual void SetTransferBeamHorShearBarMainDirNumBar(long value) {}
	virtual void SetTransferBeamHorShearBarStirrupDirDiaIndex(long value) {}
	virtual void SetTransferBeamHorShearBarStirrupDirSpacing(long value) {}
	virtual void SetTransferBeamHorShearBarMainDirUserSteelGradeS(BOOL value) { }
	virtual void SetTransferBeamHorShearBarStirrupDirUserSteelGradeS(BOOL value) {}
};

