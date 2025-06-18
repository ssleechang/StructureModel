#pragma once
#include "dtobject.h"
class StructureModel_CLASS DTOpeningWall :
	public DTObject
{
public:
	DTOpeningWall();
	virtual~DTOpeningWall();
	void Init();

	static long MakeTestDefaultSample(vector<DTOpeningWall*>& DTOpenningWallArr);

private:
// 	수평단부처리
	EndType m_nHorEndType;//0:별도처리없음  1:표준후크 2:Ubar
// 	수직단부처리
	EndType m_nVerEndType;//0:별도처리없음  1:표준후크 2:Ubar
// 	단부보강근
//	-보강근 개수 산정방법
	bool m_bUserEndBarNum;//false:끊어지는 주근의 수 /2   true:사용자 입력
	long m_numVerEndAddbar;//수직보강근개수
	long m_numHorEndAddbar;//수평보강근개수

//	모서리보강근 갯수
	//0:보강하지 않음.
	long m_numCornerAddbar;

//	보강근의 철근 지름 입력방식
	long m_nAddBarDiaType;//0:끊어지는 철근의 지름과 동일 1:사용자 지정
	long m_nAddBarDia;
	long m_nCoverBarDia;	

public:
	void SetHorEndType(EndType val){m_nHorEndType = val;};
	EndType GetHorEndType(){return m_nHorEndType;};
	void SetVerEndType(EndType val){m_nVerEndType = val;};
	EndType GetVerEndType(){return m_nVerEndType;};
	void SetbUserEndBarNum(bool val){m_bUserEndBarNum = val;};
	bool GetbUserEndBarNum(){return m_bUserEndBarNum;};
	void SetNumVerEndAddbar(long val){m_numVerEndAddbar = val;};
	long GetNumVerEndAddbar(){return m_numVerEndAddbar;};
	void SetNumHorEndAddbar(long val){m_numHorEndAddbar = val;};
	long GetNumHorEndAddbar(){return m_numHorEndAddbar;};
	void SetNumCornerAddbar(long val){m_numCornerAddbar = val;};
	long GetNumCornerAddbar(){return m_numCornerAddbar;};
	void SetAddBarDiaType(long val){m_nAddBarDiaType = val;};
	long GetAddBarDiaType(){return m_nAddBarDiaType;};
	void SetAddBarDia(long val){m_nAddBarDia = val;};
	long GetAddBarDia(){return m_nAddBarDia;};
	void SetCoverBarDia(long val){m_nCoverBarDia = val;};
	long GetCoverBarDia(){return m_nCoverBarDia;};

};

