#pragma once
#include "BaseRebarTemplate.h"

class StructureModel_CLASS LowWallRebarTemplate : public BaseRebarTemplate
{
public:
	LowWallRebarTemplate();
	~LowWallRebarTemplate(void);

	void DataInit();

	virtual void CopyFromMe(BaseRebarTemplate* pBaseRebarTemplate) override;

	void Serialize(CArchive& ar);

	//Get Property
	virtual long GetLowWallVerBarTopLenType() { return m_nLowWallVerBarTopLenType; }
	virtual BOOL GetLowWallVerBarStirrup() { return m_bLowWallVerBarStirrup; }
	virtual BOOL GetLowWallVerBarDoubleUbar() { return m_bLowWallVerBarDoubleUbar; }
	virtual BOOL GetLowWallVerBarDowelIntegrated() { return m_bLowWallVerBarDowelIntegrated; }

	virtual long GetLowWallVerBarStirrupMaxLength() { return m_nLowWallVerBarStirrupMaxLength; }
	virtual long GetLowWallVerBarDoubleUbarMaxLength() { return m_nLowWallVerBarDoubleUbarMaxLength; }
	virtual long GetLowWallVerBarDowelIntegratedLength() { return m_nLowWallVerBarDowelIntegratedLength; }

	virtual BOOL GetLowWallHorBarStartSet() { return m_bLowWallHorBarStartSet; }
	virtual long GetLowWallHorBarStartLength() { return m_nLowWallHorBarStartLength; }

	virtual BOOL GetLowWallSupportRebarsSet() { return m_bLowWallSupportRebarsSet; }
	virtual long GetLowWallSupportRebarsDiaIndex() { return m_nLowWallSupportRebarsDiaIndex; }
	virtual double GetLowWallSupportRebarsTotalLengthRate() { return m_dLowWallSupportRebarsTotalLengthRate; }
	virtual long GetLowWallSupportRebarsOrderLength() { return m_nLowWallSupportRebarsOrderLength; }
	virtual long GetLowWallSupportRebarsSpliceLength() { return m_nLowWallSupportRebarsSpliceLength; }
	virtual long GetLowWallSupportRebarsCorrectiveLength() { return m_nLowWallSupportRebarsCorrectiveLength; }

	virtual BOOL GetLowWallAddBarsSet() { return m_bLowWallAddBarsSet; }
	virtual long GetLowWallAddBarsDiaIndex() { return m_nLowWallAddBarsDiaIndex; }
	virtual long GetLowWallAddBarsNumber() { return m_nLowWallAddBarsNumber; }
	virtual long GetLowWallAddBarsLengthType() { return m_nLowWallAddBarsLengthType; }
	virtual long GetLowWallAddBarsTensionLengthType() { return m_nLowWallAddBarsTensionLengthType; }
	virtual long GetLowWallAddBarsUserLength() { return m_nLowWallAddBarsUserLength; }

	virtual BOOL GetLowWallSupportRebarUserSteelGradeS() { return m_bLowWallSupportRebarUserSteelGradeS; }
	virtual BOOL GetLowWallAddBarUserSteelGradeS() { return m_bLowWallAddBarUserSteelGradeS; }

	//Set Property
	virtual void SetLowWallVerBarTopLenType(long value) { m_nLowWallVerBarTopLenType = value; }
	virtual void SetLowWallVerBarStirrup(BOOL value) { m_bLowWallVerBarStirrup = value; }
	virtual void SetLowWallVerBarDoubleUbar(BOOL value) { m_bLowWallVerBarDoubleUbar = value; }
	virtual void SetLowWallVerBarDowelIntegrated(BOOL value) { m_bLowWallVerBarDowelIntegrated = value; }
	virtual void SetLowWallVerBarStirrupMaxLength(long value) { m_nLowWallVerBarStirrupMaxLength = value; }
	virtual void SetLowWallVerBarDoubleUbarMaxLength(long value) { m_nLowWallVerBarDoubleUbarMaxLength = value; }
	virtual void SetLowWallVerBarDowelIntegratedLength(long value) { m_nLowWallVerBarDowelIntegratedLength = value; }


	virtual void SetLowWallHorBarStartSet(BOOL value) { m_bLowWallHorBarStartSet = value; }
	virtual void SetLowWallHorBarStartLength(long value) { m_nLowWallHorBarStartLength = value; }

	virtual void SetLowWallSupportRebarsSet(BOOL value) { m_bLowWallSupportRebarsSet = value; }
	virtual void SetLowWallSupportRebarsDiaIndex(long value) { m_nLowWallSupportRebarsDiaIndex = value; }
	virtual void SetLowWallSupportRebarsTotalLengthRate(double value) { m_dLowWallSupportRebarsTotalLengthRate = value; }
	virtual void SetLowWallSupportRebarsOrderLength(long value) { m_nLowWallSupportRebarsOrderLength = value; }
	virtual void SetLowWallSupportRebarsSpliceLength(long value) { m_nLowWallSupportRebarsSpliceLength = value; }
	virtual void SetLowWallSupportRebarsCorrectiveLength(long value) { m_nLowWallSupportRebarsCorrectiveLength = value; }

	virtual void SetLowWallAddBarsSet(BOOL value) { m_bLowWallAddBarsSet = value; }
	virtual void SetLowWallAddBarsDiaIndex(long value) { m_nLowWallAddBarsDiaIndex = value; }
	virtual void SetLowWallAddBarsNumber(long value) { m_nLowWallAddBarsNumber = value; }
	virtual void SetLowWallAddBarsLengthType(long value) { m_nLowWallAddBarsLengthType = value; }
	virtual void SetLowWallAddBarsTensionLengthType(long value) { m_nLowWallAddBarsTensionLengthType = value; }
	virtual void SetLowWallAddBarsUserLength(long value) { m_nLowWallAddBarsUserLength = value; }

	virtual void SetLowWallSupportRebarUserSteelGradeS(BOOL value) { m_bLowWallSupportRebarUserSteelGradeS = value; }
	virtual void SetLowWallAddBarUserSteelGradeS(BOOL value) {  m_bLowWallAddBarUserSteelGradeS = value; }

	virtual BOOL GetLowWallSpliceAddLengthSet() { return m_bLowWallSpliceAddLengthSet; }
	virtual void SetLowWallSpliceAddLengthSet(BOOL value) { m_bLowWallSpliceAddLengthSet = value; }
	virtual long GetLowWallSpliceAddLengthNumSplice() { return m_nLowWallSpliceAddLengthNumSplice; }
	virtual void SetLowWallSpliceAddLengthNumSplice(long value) { m_nLowWallSpliceAddLengthNumSplice = value; }

	virtual BOOL GetLowWallVerBarStartSet() { return m_bLowWallVerBarStartSet; }
	virtual void SetLowWallVerBarStartSet(BOOL value) { m_bLowWallVerBarStartSet = value; }
	virtual long GetLowWallVerBarStartLength() { return m_nLowWallVerBarStartLength; }
	virtual void SetLowWallVerBarStartLength(long value) { m_nLowWallVerBarStartLength = value; }
private:
	long m_nLowWallVerBarTopLenType;			//겹침이음 길이 0:인장이음 1:압축이음

	BOOL m_bLowWallVerBarStirrup;//폐쇄형 스터럽 배근 여부
	BOOL m_bLowWallVerBarDoubleUbar;//양유바+일자 배근 여부
	BOOL m_bLowWallVerBarDowelIntegrated;//다월 일체 배근 여부
	long m_nLowWallVerBarStirrupMaxLength;//이하면 폐쇄형 스터럽 
	long m_nLowWallVerBarDoubleUbarMaxLength;//초과면,양유바+일자배근
	long m_nLowWallVerBarDowelIntegratedLength;//이하면, 다월 일체 배근

	BOOL m_bLowWallHorBarStartSet;			//시작 간격 설정
	long m_nLowWallHorBarStartLength;			//시작 간격

	BOOL m_bLowWallSupportRebarsSet;					//수평 도리근 설정
	long m_nLowWallSupportRebarsDiaIndex;				//직경
	double m_dLowWallSupportRebarsTotalLengthRate;	//벽 길이 비율
	long m_nLowWallSupportRebarsOrderLength;			//주문길이
	long m_nLowWallSupportRebarsSpliceLength;			//이음길이
	long m_nLowWallSupportRebarsCorrectiveLength;		//보정길이
	BOOL m_bLowWallSupportRebarUserSteelGradeS;

	BOOL m_bLowWallAddBarsSet;						//보강근 설정
	long m_nLowWallAddBarsDiaIndex;					//보강근 직경
	long m_nLowWallAddBarsNumber;						//보강근 개수
	long m_nLowWallAddBarsLengthType;					//보강근 길이 타입
	long m_nLowWallAddBarsTensionLengthType;			//보강근 인장길이 타입
	long m_nLowWallAddBarsUserLength;			//보강근 사용자 길이
	BOOL m_bLowWallAddBarUserSteelGradeS;

	BOOL m_bLowWallSpliceAddLengthSet;
	long m_nLowWallSpliceAddLengthNumSplice;

	BOOL m_bLowWallVerBarStartSet;			//시작 간격 설정
	long m_nLowWallVerBarStartLength;			//시작 간격
};

