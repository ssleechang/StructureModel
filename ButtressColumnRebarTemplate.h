#pragma once
#include "BaseRebarTemplate.h"

class ColumnVerBarRebarTemplate;
class ColumnDowelRebarTemplate;
class StructureModel_CLASS ButtressColumnRebarTemplate : public BaseRebarTemplate
{
public:
	ButtressColumnRebarTemplate();
	~ButtressColumnRebarTemplate(void);

	void DataInit();

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void Serialize(CArchive& ar);

	//Get Property
	virtual long GetButtressColumnVerBarPlaceTypeDiaIndex() { return m_nButtressColumnVerBarPlaceTypeDiaIndex; }
	virtual long GetButtressColumnDowelPlaceTypeDiaIndex() { return m_nButtressColumnDowelPlaceTypeDiaIndex; }

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
	virtual double GetButtressColumnDowelSupportBarFootingThickMultiplicationValue(double dDia);
	virtual long GetButtressColumnDowelSupportBarCorrectiveLength(double dDia);
	virtual long GetButtressColumnDowelSupportBarNum(double dDia);
	virtual long GetButtressColumnDowelBotPlaceType(double dDia);
	virtual BOOL GetButtressColumnDowelFootingPercentSet(double dDia);
	virtual long GetButtressColumnDowelFootingPercent(double dDia);
	virtual BOOL GetButtressColumnDowelFootingCoverSet(double dDia);
	virtual long GetButtressColumnDowelFootingCover(double dDia);
	virtual BOOL GetButtressColumnUserDowelHookLenSet(double dDia);

	//Set Property
	virtual void SetButtressColumnVerBarPlaceTypeDiaIndex(long value) { m_nButtressColumnVerBarPlaceTypeDiaIndex = value; }
	virtual void SetButtressColumnDowelPlaceTypeDiaIndex(long value) { m_nButtressColumnDowelPlaceTypeDiaIndex = value; }
	
	//Function
	virtual map<long, long> GetButtressColumnUserDowelHookLenMap() { return mm_ButtressColumnUserDowelHookLen; }
	virtual map<long, long> GetButtressColumnUserDowelSpliceLenMap() { return mm_ButtressColumnUserDowelSpliceLen; }
	virtual map<long, long> GetButtressColumnDowelMainBarUserDiaIndexMap() { return mm_ButtressColumnDowelMainBarUserDiaIndex; }
	virtual map<long, long> GetButtressColumnDowelHoopBarUserDiaIndexMap() { return mm_ButtressColumnDowelHoopBarUserDiaIndex; }

	virtual void SetButtressColumnUserDowelHookLenMap(map<long, long> mapUserDowelHookLen);
	virtual void SetButtressColumnUserDowelSpliceLenMap(map<long, long> mapUserDowelSpliceLen);
	virtual void SetButtressColumnDowelMainBarUserDiaIndexMap(map<long, long> dowelMainBarUserDiaIndexMap);
	virtual void SetButtressColumnDowelHoopBarUserDiaIndexMap(map<long, long> dowelHoopBarUserDiaIndexMap);

	virtual double GetButtressColumnUserDowelHookLen(double dDia);
	virtual long GetButtressColumnUserDowelSpliceLen(double dDia);
	virtual double GetButtressColumnDowelMainBarUserDia(double dDia);
	virtual double GetButtressColumnDowelHoopBarUserDia(double dDia);

	virtual void SetButtressColumnUserDowelHookLen(long key, long value) { mm_ButtressColumnUserDowelHookLen[key] = value; }
	virtual void SetButtressColumnUserDowelSpliceLen(long key, long value) { mm_ButtressColumnUserDowelSpliceLen[key] = value; }
	virtual void SetButtressColumnDowelMainBarUserDiaIndex(long key, long value) { mm_ButtressColumnDowelMainBarUserDiaIndex[key] = value; }
	virtual void SetButtressColumnDowelHoopBarUserDiaIndex(long key, long value) { mm_ButtressColumnDowelHoopBarUserDiaIndex[key] = value; }

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
private:		
	long m_nButtressColumnVerBarPlaceTypeDiaIndex;
	long m_nButtressColumnDowelPlaceTypeDiaIndex;
	ColumnVerBarRebarTemplate *m_pVerBarTemplateBelowDia, *m_pVerBarTemplateExcessDia;
	ColumnDowelRebarTemplate *m_pDowelTemplateBelowDia, *m_pDowelTemplateExcessDia;


	BOOL m_bButtressColumnDowelMainBarUserDiaSet;
	BOOL m_bButtressColumnDowelHoopBarUserDiaSet;
	BOOL m_bButtressColumnDowelMainBarUserSteelGradeS;
	BOOL m_bButtressColumnDowelHoopBarUserSteelGradeS;

	map<long, long> mm_ButtressColumnUserDowelHookLen;	//DiaIndex별 다월후크길이설정 값
	map<long, long> mm_ButtressColumnUserDowelSpliceLen;	//DiaIndex별 이음타입 사용자설정 값
		
	map<long, long> mm_ButtressColumnDowelMainBarUserDiaIndex;
	map<long, long> mm_ButtressColumnDowelHoopBarUserDiaIndex;
};

