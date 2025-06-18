#pragma once
#include "MDMember.h"

#include "MSRcBeamSec.h"


class StructureModel_CLASS MSRcBeamM : public MDMember
{
public:
	enum SEC_COMBI {ALL=0, SMALL_BIGGER, ALL_DIFF, BOTH_MID, SMALL_MID};
	//모두 같은 단면;외단부 = 내단부;모두 다른 단면;내단부 = 중앙부;외단부 = 중앙부;

public:
	MSRcBeamM(void);
	~MSRcBeamM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRcBeamM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	SEC_COMBI me_SecCombi;

	void CreateSections();
	MSRcBeamSec* mp_SmallSec;
	MSRcBeamSec* mp_MidSec;
	MSRcBeamSec* mp_LargeSec;

	BOOL m_bCrossBar;

	// Add Code - by Gracias

	void SetSecCombi(long Type);
	// 20120730
	void SetPC(BOOL IsPc){m_IsPC = IsPc;};
	// 현재 사용안함 셋팅도 안함
	BOOL GetPC(){return m_IsPC;};
	virtual BOOL IsPC();
	double GetMainBarDia();
	double GetMainBarDiaByPC();
	double getStrpBarDia();

	int RegenerateSectionCombination();

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	bool GetIsSubBeam()								{ return m_IsSubBeam;};
	void SetIsSubBeam(bool isSubBeam)	{ m_IsSubBeam = isSubBeam;};

	bool GetIsCornerPlacing()								{ return m_IsCornerPlacing; };
	void SetIsCornerPlacing(bool IsCornerPlacing)	{ m_IsCornerPlacing = IsCornerPlacing; };

	vector<CString> GetLinkedExteriors();
	vector<CString> GetLinkedInteriors();
	void SetLinkedExteriors(vector<CString>& linkeds);
	void SetLinkedInteriors(vector<CString>& linkeds);

	SEC_COMBI GetSecCombi();
protected:
	BOOL m_IsPC;
	bool m_IsSubBeam;
	bool m_IsCornerPlacing; // true: 코너 기준 배근 / false: 등간격 배근

	vector<CString> ma_LinkedExterior;
	vector<CString> ma_LinkedInterior;

#pragma region 특수 전단벽 DB
protected:

	// 대각보강근
	bool m_IsDiagonalAddBar;
	
	// 보강유형 : 케이지(0) / 묶음(1)
	int m_nDiagonalAddBarType;
	// 대각근
	MSMainBar m_DiagonalBar;
	
	// 스터럽
	MSStirrupBar m_DiagonalStirrupBar;
	// 스터럽 갯수
	int m_nDiagonalStirrupBarNum;

	// STR폭
	int m_nStrHor;
	// STR춤
	int m_nStrVer;

public:
	// 대각보강근
	bool GetIsDiagonalAddBar() { return m_IsDiagonalAddBar; };
	void SetIsDiagonalAddBar(bool IsDiagonalAddBar) { m_IsDiagonalAddBar = IsDiagonalAddBar; };

	// 보강유형 : 케이지(0) / 묶음(1)
	int GetDiagonalAddBarType() { return m_nDiagonalAddBarType; };
	void SetDiagonalAddBarType(int DiagonalAddBarType) { m_nDiagonalAddBarType = DiagonalAddBarType; };

	// 대각근
	MSMainBar* GetDiagonalBar() { return &m_DiagonalBar; };
	
	// 스터럽
	MSStirrupBar* GetDiagonalStirrupBar() { return &m_DiagonalStirrupBar; };
	// 스터럽 갯수
	int GetDiagonalStirrupBarNum() { return m_nDiagonalStirrupBarNum; };
	void SetDiagonalStirrupBarNum(int DiagonalStirrupBarNum) { m_nDiagonalStirrupBarNum = DiagonalStirrupBarNum; };


	// STR폭
	int GetSTRHor() { return m_nStrHor; };
	void SetSTRHor(int nStrHor) { m_nStrHor = nStrHor; };
	// STR춤
	int GetSTRVer() { return m_nStrVer; };
	void SetSTRVer(int nStrVer) { m_nStrVer = nStrVer; };
#pragma endregion

#pragma region 전이 부재
public:
	enum BeamM_TYPE { General = 0, Transfer = 1, Deck };

protected:
	int m_nBeamMType;
public:
	int GetBeamMType();
	// BEAMM_TYPE { GENERAL = 0, TRANSFER };
	void SetBeamMType(int nType);
	virtual bool IsTransfer() override;
	virtual bool IsDeck() override;
#pragma endregion
};
