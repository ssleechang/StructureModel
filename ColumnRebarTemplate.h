#pragma once
#include "BaseRebarTemplate.h"

class ButtressColumnRebarTemplate;
class ColumnVerBarRebarTemplate;
class ColumnDowelRebarTemplate;
class VerMemberLinkDowelTemplate;
class StructureModel_CLASS ColumnRebarTemplate : public BaseRebarTemplate
{
public:
	ColumnRebarTemplate();
	~ColumnRebarTemplate(void);

	void DataInit();

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void Serialize(CArchive& ar);

	//Get Property
	virtual long GetColumnVerBarPlaceTypeDiaIndex() { return m_nColumnVerBarPlaceTypeDiaIndex; }
	virtual long GetColumnDowelPlaceTypeDiaIndex() { return m_nColumnDowelPlaceTypeDiaIndex; }

	virtual long GetColumnVerBarSpliceType(double dDia);
	virtual long GetColumnVerBarTopLenType(double dDia);
	virtual long GetColumnVerBarTopFloorDevType(double dDia);
	virtual long GetColumnVerBarConstructionHeight(double dDia);
	virtual BOOL GetColumnVerBarAlterSpliceSet(double dDia);
	virtual long GetColumnVerBarAlterSpliceDist(double dDia);
	virtual long GetColumnVerBarAlterSpliceType(double dDia);
	virtual long GetColumnDowelAlterSpliceType(double dDia);

	virtual BOOL GetColumnEndHoopBarShapeSet() { return m_bColumnEndHoopBarShapeSet; }
	virtual long GetColumnHoopBarShapeIdx() { return m_nColumnHoopBarShapeIdx; }
	virtual long GetColumnTieBarShapeIdx() { return m_nColumnTieBarShapeIdx; }
	virtual BOOL GetColumnHoopBarStartLengthSet() { return m_bColumnHoopBarStartLengthSet; }
	virtual long GetColumnHoopBarStartLength() { return m_nColumnHoopBarStartLength; }
	virtual long GetColumnTopMemberCover() { return m_nColumnTopMemberCover; }
	virtual BOOL GetColumnTieAddLengthSet() { return m_bColumnTieAddLengthSet; }
	virtual long GetColumnTieAddLength() { return m_nColumnTieAddLength; }
	virtual BOOL GetColumnOutsideColumnTieBarDelete() { return m_bColumnOutsideColumnTieBarDelete; }

	virtual long GetColumnDowelBarSpliceType(double dDia);
	virtual long GetColumnDowelTopLenType(double dDia);
	virtual long GetColumnDowelConstructionHeight(double dDia);
	virtual BOOL GetColumnDowelAlterSpliceSet(double dDia);
	virtual long GetColumnDowelAlterSpliceDist(double dDia);
	virtual long GetColumnDowelHoopRebarType(double dDia);
	virtual long GetColumnDowelHoopBySpace(double dDia);
	virtual long GetColumnDowelHoopByNum(double dDia);
	virtual long GetColumnDowelSupportBarDiaIndex(double dDia);
	virtual BOOL GetColumnDowelSupportBarUserSteelGradeS(double dDia);
	virtual long GetColumnDowelSupportBarLength(double dDia);
	virtual long GetColumnDowelSupportBarLengthType(double dDia);
	virtual double GetColumnDowelSupportBarFootingThickMultiplicationValue(double dDia);
	virtual long GetColumnDowelSupportBarCorrectiveLength(double dDia);
	virtual long GetColumnDowelSupportBarNum(double dDia);
	virtual long GetColumnDowelBotPlaceType(double dDia);
	virtual BOOL GetColumnDowelFootingPercentSet(double dDia);
	virtual long GetColumnDowelFootingPercent(double dDia);
	virtual BOOL GetColumnDowelFootingCoverSet(double dDia);
	virtual long GetColumnDowelHoopBarShapeIdx() { return m_nColumnDowelHoopBarShapeIdx; };
	virtual long GetColumnDowelFootingCover(double dDia);
	virtual BOOL GetColumnUserDowelHookLenSet(double dDia);

	//Set Property
	virtual void SetColumnVerBarPlaceTypeDiaIndex(long value) { m_nColumnVerBarPlaceTypeDiaIndex = value; }
	virtual void SetColumnDowelPlaceTypeDiaIndex(long value) { m_nColumnDowelPlaceTypeDiaIndex = value; }
		
	virtual void SetColumnEndHoopBarShapeSet(BOOL value) { m_bColumnEndHoopBarShapeSet = value; }
	virtual void SetColumnHoopBarShapeIdx(long value) { m_nColumnHoopBarShapeIdx = value; }
	virtual void SetColumnTieBarShapeIdx(long value) { m_nColumnTieBarShapeIdx = value; }
	virtual void SetColumnHoopBarStartLengthSet(BOOL value) { m_bColumnHoopBarStartLengthSet = value; }
	virtual void SetColumnHoopBarStartLength(long value) { m_nColumnHoopBarStartLength = value; }
	virtual void SetColumnTopMemberCover(long value) { m_nColumnTopMemberCover = value; }
	virtual void SetColumnTieAddLengthSet(BOOL value) { m_bColumnTieAddLengthSet = value; }
	virtual void SetColumnTieAddLength(long value) { m_nColumnTieAddLength = value; }
	virtual void SetColumnOutsideColumnTieBarDelete(BOOL value) { m_bColumnOutsideColumnTieBarDelete = value; }

	virtual void SetColumnDowelHoopBarShapeIdx(int value) { m_nColumnDowelHoopBarShapeIdx = value; };


	//Function
	virtual map<long, long> GetColumnUserDowelHookLenMap() { return mm_ColumnUserDowelHookLen; }
	virtual map<long, long> GetColumnUserDowelSpliceLenMap() { return mm_ColumnUserDowelSpliceLen; }
	virtual map<long, long> GetColumnDowelMainBarUserDiaIndexMap() { return mm_ColumnDowelMainBarUserDiaIndex; }
	virtual map<long, long> GetColumnDowelHoopBarUserDiaIndexMap() { return mm_ColumnDowelHoopBarUserDiaIndex; }

	virtual void SetColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap);
	virtual void SetColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap);

	virtual double GetColumnUserDowelHookLen(double dDia);
	virtual long GetColumnUserDowelSpliceLen(double dDia);
	virtual double GetColumnDowelMainBarUserDia(double dDia);
	virtual double GetColumnDowelHoopBarUserDia(double dDia);

	virtual void SetColumnUserDowelHookLen(long key, long value) { mm_ColumnUserDowelHookLen[key] = value; }
	virtual void SetColumnUserDowelSpliceLen(long key, long value) { mm_ColumnUserDowelSpliceLen[key] = value; }
	virtual void SetColumnDowelMainBarUserDiaIndex(long key, long value) { mm_ColumnDowelMainBarUserDiaIndex[key] = value; }
	virtual void SetColumnDowelHoopBarUserDiaIndex(long key, long value) { mm_ColumnDowelHoopBarUserDiaIndex[key] = value; }

	virtual ColumnVerBarRebarTemplate *GetColumnVerBarTemplateBelowDia();
	virtual ColumnVerBarRebarTemplate *GetColumnVerBarTemplateExcessDia();
	virtual ColumnDowelRebarTemplate *GetColumnDowelTemplateBelowDia();
	virtual ColumnDowelRebarTemplate *GetColumnDowelTemplateExcessDia();

	
	virtual BOOL GetColumnDowelMainBarUserDiaSet();	
	virtual void SetColumnDowelMainBarUserDiaSet(BOOL value);
	virtual BOOL GetColumnDowelHoopBarUserDiaSet();	
	virtual void SetColumnDowelHoopBarUserDiaSet(BOOL value);

	virtual BOOL GetColumnDowelMainBarUserSteelGradeS();
	virtual void SetColumnDowelMainBarUserSteelGradeS(BOOL value);
	virtual BOOL GetColumnDowelHoopBarUserSteelGradeS();
	virtual void SetColumnDowelHoopBarUserSteelGradeS(BOOL value);

	//Buttress
	virtual long GetButtressColumnVerBarPlaceTypeDiaIndex();
	virtual long GetButtressColumnDowelPlaceTypeDiaIndex();

	virtual long GetButtressColumnVerBarSpliceType(double dDia);
	virtual long GetButtressColumnVerBarTopLenType(double dDia);
	virtual long GetButtressColumnVerBarTopFloorDevType(double dDia);
	virtual long GetButtressColumnVerBarConstructionHeight(double dDia);
	virtual BOOL GetButtressColumnVerBarAlterSpliceSet(double dDia);
	virtual long GetButtressColumnVerBarAlterSpliceDist(double dDia);
	
	virtual long GetButtressColumnDowelBarSpliceType(double dDia);
	virtual long GetButtressColumnDowelTopLenType(double dDia);
	virtual long GetButtressColumnDowelConstructionHeight(double dDia);
	virtual BOOL GetButtressColumnDowelAlterSpliceSet(double dDia);
	virtual long GetButtressColumnDowelAlterSpliceDist(double dDia);
	virtual long GetButtressColumnDowelHoopRebarType(double dDia);
	virtual long GetButtressColumnDowelHoopBySpace(double dDia);
	virtual long GetButtressColumnDowelHoopByNum(double dDia);
	virtual long GetButtressColumnDowelSupportBarDiaIndex(double dDia);
	virtual BOOL GetButtressColumnDowelSupportBarUserSteelGradeS(double dDia);
	virtual long GetButtressColumnDowelSupportBarLength(double dDia);
	virtual long GetButtressColumnDowelSupportBarLengthType(double dDia);
	virtual long GetButtressColumnDowelSupportBarCorrectiveLength(double dDia);
	virtual double GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(double dDia);
	virtual long GetButtressColumnDowelSupportBarNum(double dDia);
	virtual long GetButtressColumnDowelBotPlaceType(double dDia);
	virtual BOOL GetButtressColumnDowelFootingPercentSet(double dDia);
	virtual long GetButtressColumnDowelFootingPercent(double dDia);
	virtual BOOL GetButtressColumnDowelFootingCoverSet(double dDia);
	virtual long GetButtressColumnDowelFootingCover(double dDia);
	virtual BOOL GetButtressColumnUserDowelHookLenSet(double dDia);

	//Set Property
	virtual void SetButtressColumnVerBarPlaceTypeDiaIndex(long value);
	virtual void SetButtressColumnDowelPlaceTypeDiaIndex(long value);
	
	//Function
	virtual map<long, long> GetButtressColumnUserDowelHookLenMap();
	virtual map<long, long> GetButtressColumnUserDowelSpliceLenMap();
	virtual map<long, long> GetButtressColumnDowelMainBarUserDiaIndexMap();
	virtual map<long, long> GetButtressColumnDowelHoopBarUserDiaIndexMap();

	virtual void SetButtressColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetButtressColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetButtressColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap);
	virtual void SetButtressColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap);

	virtual double GetButtressColumnUserDowelHookLen(double dDia);
	virtual long GetButtressColumnUserDowelSpliceLen(double dDia);
	virtual double GetButtressColumnDowelMainBarUserDia(double dDia);
	virtual double GetButtressColumnDowelHoopBarUserDia(double dDia);

	virtual void SetButtressColumnUserDowelHookLen(long key, long value);
	virtual void SetButtressColumnUserDowelSpliceLen(long key, long value);
	virtual void SetButtressColumnDowelMainBarUserDiaIndex(long key, long value);
	virtual void SetButtressColumnDowelHoopBarUserDiaIndex(long key, long value);

	virtual ColumnVerBarRebarTemplate *GetButtressColumnVerBarTemplateBelowDia();
	virtual ColumnVerBarRebarTemplate *GetButtressColumnVerBarTemplateExcessDia();
	virtual ColumnDowelRebarTemplate *GetButtressColumnDowelTemplateBelowDia();
	virtual ColumnDowelRebarTemplate *GetButtressColumnDowelTemplateExcessDia();
		
	virtual BOOL GetButtressColumnDowelMainBarUserDiaSet();
	virtual void SetButtressColumnDowelMainBarUserDiaSet(BOOL value);
	virtual BOOL GetButtressColumnDowelHoopBarUserDiaSet();
	virtual void SetButtressColumnDowelHoopBarUserDiaSet(BOOL value);

	virtual BOOL GetButtressColumnDowelMainBarUserSteelGradeS();
	virtual void SetButtressColumnDowelMainBarUserSteelGradeS(BOOL value);
	virtual BOOL GetButtressColumnDowelHoopBarUserSteelGradeS();
	virtual void SetButtressColumnDowelHoopBarUserSteelGradeS(BOOL value);

	virtual VerMemberLinkDowelTemplate *GetColumnVerMemberLinkDowelTemplate();
private:		
	long m_nColumnVerBarPlaceTypeDiaIndex;
	long m_nColumnDowelPlaceTypeDiaIndex;
	ColumnVerBarRebarTemplate *m_pVerBarTemplateBelowDia, *m_pVerBarTemplateExcessDia;
	ColumnDowelRebarTemplate *m_pDowelTemplateBelowDia, *m_pDowelTemplateExcessDia;

	BOOL m_bColumnEndHoopBarShapeSet;		//단부 대근에 135+135 형상 후크 적용
	long m_nColumnHoopBarShapeIdx;			//대근 후프
	long m_nColumnTieBarShapeIdx;			//보조대근 후프
	BOOL m_bColumnHoopBarStartLengthSet;	//시작간격 설정
	long m_nColumnHoopBarStartLength;		//시작간격
	long m_nColumnTopMemberCover;			//마지막 간격
	BOOL m_bColumnTieAddLengthSet;			//보조대근 추가 길이 설정
	long m_nColumnTieAddLength;				//보조대근 추가 길이
	BOOL m_bColumnOutsideColumnTieBarDelete;//외부기둥 접합부 보조대근 삭제

	BOOL m_bColumnDowelMainBarUserDiaSet;
	BOOL m_bColumnDowelHoopBarUserDiaSet;
	BOOL m_bColumnDowelMainBarUserSteelGradeS;
	BOOL m_bColumnDowelHoopBarUserSteelGradeS;
	// 추가 BH-4305 배근도 작성 프로토타입
	long m_nColumnDowelHoopBarShapeIdx;	// 다월 수평근 형상
	map<long, long> mm_ColumnUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_ColumnUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
		
	map<long, long> mm_ColumnDowelMainBarUserDiaIndex;
	map<long, long> mm_ColumnDowelHoopBarUserDiaIndex;

	ButtressColumnRebarTemplate *m_pButtressColumnRebarTemplate;

	VerMemberLinkDowelTemplate *m_pColumnVerMemberLinkDowelTemplate;
};

