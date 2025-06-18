#pragma once
#include "MDMember.h"

#include "MSStirrupBar.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSRcBWallM :
	public MDMember
{
public:
	MSRcBWallM(void);
	~MSRcBWallM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcBWallM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	// 철근정보
	MSStirrupBar m_HBars;					//간격
	MSStirrupBar m_VBars;	
	MSMainBar m_MainVBar;
	MSMainBar m_MainHBar;

	//전단 보강근
	MSStirrupBar m_TopShearHBars;
	MSStirrupBar m_TopShearVBars;
	MSStirrupBar m_BotShearHBars;
	MSStirrupBar m_BotShearVBars;

	BOOL m_bTopShearAddBar;					//상부근 유무
	BOOL m_bBotShearAddBar;					//하부근 유무

	long m_nTopShearAddHBarLength;	//상부근 보강구간 길이
	long m_nBotShearAddVBarLength;  //하부근 보강구간 길이

	// Add Code - by Gracias(2013.09.09)
	BOOL m_bStartShearAddBar;					//시작근 유무
	BOOL m_bEndShearAddBar;					//끝근 유무
	MSStirrupBar m_StartShearHBars;
	MSStirrupBar m_StartShearVBars;
	MSStirrupBar m_EndShearHBars;
	MSStirrupBar m_EndShearVBars;
	long m_nStartShearAddBarLength;
	long m_nEndShearAddBarLength;
	// End(2013.09.09)

	BOOL m_bAllShearAddBar;
	MSStirrupBar m_AllShearHBars;
	MSStirrupBar m_AllShearVBars;

	//////////////////////////////////////////////////////////////////////////
	BOOL m_bIsSinglePlaceBar;				//배근 타입 // 디폴트 = 1 // 0 = 복배근 1 = 단배근
	BOOL m_bIsNumPlaceVBar;						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	BOOL m_bIsNumPlaceHBar;						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetVerBarDiaAllLayer();
	virtual vector<double> GetHorBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();

	double getVBarMaxDeformedBarDiameter();
	double getVBarMinDeformedBarDiameter();
	double getHBarMaxDeformedBarDiameter();

	double GetMaxDia();
	double GetMaxDia_Ver();
	double GetMaxDia_Hor();
	bool GetVerBarUserSteelGradeS();

	double GetMaxDia_EndVerBar();
	bool GetEndVerBarUserSteelGradeS();
private:
	long m_nShearBarPlaceType;		//전단 보강근 배근 타입 // 디폴트 = 0 // 0 : 간격 배근 1 : 비율 배근
	double m_dShearBarRateDiaIndex;

	long m_nHorShearBarRateSpacing;
	long m_nHorShearBarRate;

	long m_nVerShearBarRateSpacing;
	long m_nVerShearBarRate;

#pragma region 특수 전단벽 DB
	
	// 특수 경계 요소
	bool m_IsSpecialBoundary;
	// 경계 길이(유형) : 전체(0) / 구간(1)
	int m_nSpecialBoundaryType;
	// 경계 길이
	int m_nBoundaryLength;
	
	// 단부 수직근
	// 갯수배근(true) / 간격 배근(false)
	bool m_IsNumPlaceEndVerBar;
	// 단부 수직근(갯수)
	MSMainBar m_EndVerMainBar;
	// 단부 수직근(간격)
	MSStirrupBar m_EndVerBar;
	
	// 대근
	MSStirrupBar m_HoopBar;
	
	// 보조대근 Lx
	// 갯수배근(true) / 간격 배근(false)
	bool m_IsNumPlaceTieBarLx;
	// 보조대근 Lx(갯수)
	MSStirrupBar m_TieBarLx;
	// 보조대근 Lx(간격), m_HoopBar 데이타 값사용
	long m_nTieBarLxNum;
	
	// 보조대근 Ly 
	// 갯수배근(true) / 간격 배근(false)
	bool m_IsNumPlaceTieBarLy;
	// 보조대근 Ly(갯수)
	MSStirrupBar m_TieBarLy;
	// 보조대근 Ly(간격), m_HoopBar 데이타 값사용
	long m_nTieBarLyNum;
		
public:
	bool GetIsSpecialBoundary() { return m_IsSpecialBoundary; };
	void SetIsSpecialBoundary(bool IsSpecialBoundary) { m_IsSpecialBoundary = IsSpecialBoundary; };

	int GetSpecialBoundaryType() { return m_nSpecialBoundaryType; };
	void SetSpecialBoundaryType(int SpecialBoundaryType) { m_nSpecialBoundaryType = SpecialBoundaryType; };
	
	int GetBoundaryLength() { return m_nBoundaryLength; };
	void SetBoundaryLength(int BoundaryLength) { m_nBoundaryLength = BoundaryLength; };

	// 단부 수직근
	bool GetIsNumPlaceEndVerBar() { return m_IsNumPlaceEndVerBar; };
	void SetIsNumPlaceEndVerBar(bool IsNumPlaceEndVerBar) { m_IsNumPlaceEndVerBar = IsNumPlaceEndVerBar; };
	
	MSMainBar* GetEndVerMainBar() { return &m_EndVerMainBar; };
	MSStirrupBar* GetEndVerBar() { return &m_EndVerBar; };

	// 대근
	MSStirrupBar* GetHoopBar() { return &m_HoopBar; };

	// 보조대근 Lx
	bool GetIsNumPlaceTieBarLx() { return m_IsNumPlaceTieBarLx; };
	void SetIsNumPlaceTieBarLx(bool IsNumPlaceTieBarLx) { m_IsNumPlaceTieBarLx = IsNumPlaceTieBarLx; };
	MSStirrupBar* GetTieBarLx() { return &m_TieBarLx; };
	int GetTieBarLxNum() { return m_nTieBarLxNum; };
	void SetTieBarLxNum(int TieBarLxNum) { m_nTieBarLxNum = TieBarLxNum; };

	// 보조대근 Ly 
	bool GetIsNumPlaceTieBarLy() { return m_IsNumPlaceTieBarLy; };
	void SetIsNumPlaceTieBarLy(bool IsNumPlaceTieBarLy) { m_IsNumPlaceTieBarLy = IsNumPlaceTieBarLy; };
	MSStirrupBar* GetTieBarLy() { return &m_TieBarLy; };
	int GetTieBarLyNum() { return m_nTieBarLyNum; };
	void SetTieBarLyNum(int TieBarLyNum) { m_nTieBarLyNum = TieBarLyNum; };

	
#pragma endregion


public:
	long GetShearBarPlaceType() { return m_nShearBarPlaceType; }
	void SetShearBarPlaceType(long nShearBarPlaceType) { m_nShearBarPlaceType = nShearBarPlaceType; }

	long GetShearBarRateDiaIndex() { return m_dShearBarRateDiaIndex; }
	void SetShearBarRateDiaIndex(double dShearBarRateDiaIndex) { m_dShearBarRateDiaIndex = dShearBarRateDiaIndex; }

	long GetHorShearBarRateSpacing() { return m_nHorShearBarRateSpacing; }
	void SetHorShearBarRateSpacing(long nHorShearBarRateSpacing) { m_nHorShearBarRateSpacing = nHorShearBarRateSpacing; }
	long GetHorShearBarRate() { return m_nHorShearBarRate; }
	void SetHorShearBarRate(long nHorShearBarRate) { m_nHorShearBarRate = nHorShearBarRate; }

	long GetVerShearBarRateSpacing() { return m_nVerShearBarRateSpacing; }
	void SetVerShearBarRateSpacing(long nVerShearBarRateSpacing) { m_nVerShearBarRateSpacing = nVerShearBarRateSpacing; }
	long GetVerShearBarRate() { return m_nVerShearBarRate; }
	void SetVerShearBarRate(long nVerShearBarRate) { m_nVerShearBarRate = nVerShearBarRate; }
};
