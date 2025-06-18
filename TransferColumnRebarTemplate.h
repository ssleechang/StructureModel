#pragma once
#include "BaseRebarTemplate.h"

class ColumnVerBarRebarTemplate;
class ColumnDowelRebarTemplate;
class VerMemberLinkDowelTemplate;
class StructureModel_CLASS TransferColumnRebarTemplate : public BaseRebarTemplate
{
public:
	TransferColumnRebarTemplate();
	~TransferColumnRebarTemplate(void);

	void DataInit();

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void Serialize(CArchive& ar);

	//Get Property
	virtual long GetTransferColumnVerBarPlaceTypeDiaIndex() { return m_nTransferColumnVerBarPlaceTypeDiaIndex; }
	virtual long GetTransferColumnDowelPlaceTypeDiaIndex() { return m_nTransferColumnDowelPlaceTypeDiaIndex; }

	virtual long GetTransferColumnVerBarSpliceType(double dDia);
	virtual long GetTransferColumnVerBarTopLenType(double dDia);
	virtual long GetTransferColumnVerBarTopFloorDevType(double dDia);
	virtual long GetTransferColumnVerBarConstructionHeight(double dDia);
	virtual BOOL GetTransferColumnVerBarAlterSpliceSet(double dDia);
	virtual long GetTransferColumnVerBarAlterSpliceDist(double dDia);

	virtual long GetTransferColumnVerBarAlterSpliceType(double dDia);
	virtual long GetTransferColumnDowelAlterSpliceType(double dDia);

	virtual BOOL GetTransferColumnEndHoopBarShapeSet() { return m_bTransferColumnEndHoopBarShapeSet; }
	virtual long GetTransferColumnHoopBarShapeIdx() { return m_nTransferColumnHoopBarShapeIdx; }
	virtual long GetTransferColumnTieBarShapeIdx() { return m_nTransferColumnTieBarShapeIdx; }
	virtual BOOL GetTransferColumnHoopBarStartLengthSet() { return m_bTransferColumnHoopBarStartLengthSet; }
	virtual long GetTransferColumnHoopBarStartLength() { return m_nTransferColumnHoopBarStartLength; }
	virtual long GetTransferColumnTopMemberCover() { return m_nTransferColumnTopMemberCover; }
	virtual BOOL GetTransferColumnTieAddLengthSet() { return m_bTransferColumnTieAddLengthSet; }
	virtual long GetTransferColumnTieAddLength() { return m_nTransferColumnTieAddLength; }
	virtual BOOL GetTransferColumnOutsideColumnTieBarDelete() { return m_bTransferColumnOutsideColumnTieBarDelete; }

	virtual long GetTransferColumnDowelBarSpliceType(double dDia);
	virtual long GetTransferColumnDowelTopLenType(double dDia);
	virtual long GetTransferColumnDowelConstructionHeight(double dDia);
	virtual BOOL GetTransferColumnDowelAlterSpliceSet(double dDia);
	virtual long GetTransferColumnDowelAlterSpliceDist(double dDia);
	virtual long GetTransferColumnDowelHoopRebarType(double dDia);
	virtual long GetTransferColumnDowelHoopBySpace(double dDia);
	virtual long GetTransferColumnDowelHoopByNum(double dDia);
	virtual long GetTransferColumnDowelSupportBarDiaIndex(double dDia);
	virtual BOOL GetTransferColumnDowelSupportBarUserSteelGradeS(double dDia);
	virtual long GetTransferColumnDowelSupportBarLength(double dDia);
	virtual long GetTransferColumnDowelSupportBarLengthType(double dDia);
	virtual double GetTransferColumnDowelSupportBarFootingThickMultiplicationValue(double dDia);
	virtual long GetTransferColumnDowelSupportBarCorrectiveLength(double dDia);
	virtual long GetTransferColumnDowelSupportBarNum(double dDia);
	virtual long GetTransferColumnDowelBotPlaceType(double dDia);
	virtual BOOL GetTransferColumnDowelFootingPercentSet(double dDia);
	virtual long GetTransferColumnDowelFootingPercent(double dDia);
	virtual BOOL GetTransferColumnDowelFootingCoverSet(double dDia);
	virtual long GetTransferColumnDowelHoopBarShapeIdx() { return m_nTransferColumnDowelHoopBarShapeIdx; };
	virtual long GetTransferColumnDowelFootingCover(double dDia);
	virtual BOOL GetTransferColumnUserDowelHookLenSet(double dDia);

	//Set Property
	virtual void SetTransferColumnVerBarPlaceTypeDiaIndex(long value) { m_nTransferColumnVerBarPlaceTypeDiaIndex = value; }
	virtual void SetTransferColumnDowelPlaceTypeDiaIndex(long value) { m_nTransferColumnDowelPlaceTypeDiaIndex = value; }
		
	virtual void SetTransferColumnEndHoopBarShapeSet(BOOL value) { m_bTransferColumnEndHoopBarShapeSet = value; }
	virtual void SetTransferColumnHoopBarShapeIdx(long value) { m_nTransferColumnHoopBarShapeIdx = value; }
	virtual void SetTransferColumnTieBarShapeIdx(long value) { m_nTransferColumnTieBarShapeIdx = value; }
	virtual void SetTransferColumnHoopBarStartLengthSet(BOOL value) { m_bTransferColumnHoopBarStartLengthSet = value; }
	virtual void SetTransferColumnHoopBarStartLength(long value) { m_nTransferColumnHoopBarStartLength = value; }
	virtual void SetTransferColumnTopMemberCover(long value) { m_nTransferColumnTopMemberCover = value; }
	virtual void SetTransferColumnTieAddLengthSet(BOOL value) { m_bTransferColumnTieAddLengthSet = value; }
	virtual void SetTransferColumnTieAddLength(long value) { m_nTransferColumnTieAddLength = value; }
	virtual void SetTransferColumnOutsideColumnTieBarDelete(BOOL value) { m_bTransferColumnOutsideColumnTieBarDelete = value; }
	virtual void SetTransferColumnDowelHoopBarShapeIdx(int value) { m_nTransferColumnDowelHoopBarShapeIdx = value; };


	//Function
	virtual map<long, long> GetTransferColumnUserDowelHookLenMap() { return mm_TransferColumnUserDowelHookLen; }
	virtual map<long, long> GetTransferColumnUserDowelSpliceLenMap() { return mm_TransferColumnUserDowelSpliceLen; }
	virtual map<long, long> GetTransferColumnDowelMainBarUserDiaIndexMap() { return mm_TransferColumnDowelMainBarUserDiaIndex; }
	virtual map<long, long> GetTransferColumnDowelHoopBarUserDiaIndexMap() { return mm_TransferColumnDowelHoopBarUserDiaIndex; }

	virtual void SetTransferColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetTransferColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetTransferColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap);
	virtual void SetTransferColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap);

	virtual double GetTransferColumnUserDowelHookLen(double dDia);
	virtual long GetTransferColumnUserDowelSpliceLen(double dDia);
	virtual double GetTransferColumnDowelMainBarUserDia(double dDia);
	virtual double GetTransferColumnDowelHoopBarUserDia(double dDia);

	virtual void SetTransferColumnUserDowelHookLen(long key, long value) { mm_TransferColumnUserDowelHookLen[key] = value; }
	virtual void SetTransferColumnUserDowelSpliceLen(long key, long value) { mm_TransferColumnUserDowelSpliceLen[key] = value; }
	virtual void SetTransferColumnDowelMainBarUserDiaIndex(long key, long value) { mm_TransferColumnDowelMainBarUserDiaIndex[key] = value; }
	virtual void SetTransferColumnDowelHoopBarUserDiaIndex(long key, long value) { mm_TransferColumnDowelHoopBarUserDiaIndex[key] = value; }

	virtual ColumnVerBarRebarTemplate *GetTransferColumnVerBarTemplateBelowDia();
	virtual ColumnVerBarRebarTemplate *GetTransferColumnVerBarTemplateExcessDia();
	virtual ColumnDowelRebarTemplate *GetTransferColumnDowelTemplateBelowDia();
	virtual ColumnDowelRebarTemplate *GetTransferColumnDowelTemplateExcessDia();
	
	virtual BOOL GetTransferColumnDowelMainBarUserDiaSet();	
	virtual void SetTransferColumnDowelMainBarUserDiaSet(BOOL value);
	virtual BOOL GetTransferColumnDowelHoopBarUserDiaSet();	
	virtual void SetTransferColumnDowelHoopBarUserDiaSet(BOOL value);

	virtual BOOL GetTransferColumnDowelMainBarUserSteelGradeS();
	virtual void SetTransferColumnDowelMainBarUserSteelGradeS(BOOL value);
	virtual BOOL GetTransferColumnDowelHoopBarUserSteelGradeS();
	virtual void SetTransferColumnDowelHoopBarUserSteelGradeS(BOOL value);

	virtual VerMemberLinkDowelTemplate *GetTransferColumnVerMemberLinkDowelTemplate();
private:		
	long m_nTransferColumnVerBarPlaceTypeDiaIndex;
	long m_nTransferColumnDowelPlaceTypeDiaIndex;
	ColumnVerBarRebarTemplate *m_pVerBarTemplateBelowDia, *m_pVerBarTemplateExcessDia;
	ColumnDowelRebarTemplate *m_pDowelTemplateBelowDia, *m_pDowelTemplateExcessDia;

	BOOL m_bTransferColumnEndHoopBarShapeSet;		//단부 대근에 135+135 형상 후크 적용
	long m_nTransferColumnHoopBarShapeIdx;			//대근 후프
	long m_nTransferColumnTieBarShapeIdx;			//보조대근 후프
	BOOL m_bTransferColumnHoopBarStartLengthSet;	//시작간격 설정
	long m_nTransferColumnHoopBarStartLength;		//시작간격
	long m_nTransferColumnTopMemberCover;			//마지막 간격
	BOOL m_bTransferColumnTieAddLengthSet;			//보조대근 추가 길이 설정
	long m_nTransferColumnTieAddLength;				//보조대근 추가 길이
	BOOL m_bTransferColumnOutsideColumnTieBarDelete;//외부기둥 접합부 보조대근 삭제

	BOOL m_bTransferColumnDowelMainBarUserDiaSet;
	BOOL m_bTransferColumnDowelHoopBarUserDiaSet;
	BOOL m_bTransferColumnDowelMainBarUserSteelGradeS;
	BOOL m_bTransferColumnDowelHoopBarUserSteelGradeS;
	// 추가 BH-4305 배근도 작성 프로토타입
	long m_nTransferColumnDowelHoopBarShapeIdx;	// 다월 수평근 형상
	map<long, long> mm_TransferColumnUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_TransferColumnUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
		
	map<long, long> mm_TransferColumnDowelMainBarUserDiaIndex;
	map<long, long> mm_TransferColumnDowelHoopBarUserDiaIndex;
	
	VerMemberLinkDowelTemplate *m_pTransferColumnVerMemberLinkDowelTemplate;
};

