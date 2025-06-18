#pragma once
#include "MDMember.h"

#include "MSMainBar.h"
#include "MSStirrupBar.h"
#include "MSSubMainBar.h"
#include "MSMainBarSet.h"

#include "GMLib/GMPolygon.h"
class MSSpecificTieBar;
class StructureModel_CLASS MSRcColumnM :
	public MDMember
{
public:
	MSRcColumnM(void);
	~MSRcColumnM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcColumnM)
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long TotalBarCount();
	
	//형상정보

	//단면정보
	MSMainBarSet m_TBars;
	MSMainBarSet m_BBars;
	MSMainBarSet m_LBars;
	MSMainBarSet m_RBars;
	MSStirrupBar m_EndHoopBar;	
	MSStirrupBar m_CenHoopBar;
	BOOL m_nDoubleHoopType;             // 0 :일반,  1:이중HOOP배근    
	BOOL m_nHoopType;              //long m_nHoopType; // 0 Hoop:   1 : 쪽후프
	long m_nTieRowNum;                  //long m_XStirrupNum;  X축방향 Stirrup수
	long m_nTieColNum;									//long m_YStirrupNum;  Y축방향 Stirrup수
	// 20130903 추가 - by Gracias
	MSStirrupBar m_RowEndTieBar;
	MSStirrupBar m_RowCenTieBar;
	MSStirrupBar m_ColEndTieBar;
	MSStirrupBar m_ColCenTieBar;
	                   
	//배근정보
	long m_nDowelSpliceType;                 // 0:몰이음, 1: 엇이음
	long m_nDowelSpliceDetail;               // 0: 커플러  1: 가스용접
	long m_nSpliceLengthA;                   // 엇이음 길이A
	long m_nSpliceLengthB;                   // 엇이음 길이B

	vector<MSMainBar*> ma_MainBar;
	// 20120730
	void SetPC(BOOL IsPc){m_IsPC = IsPc;};
	BOOL GetPC(){return m_IsPC;};
	virtual BOOL IsPC();
	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
	double GetMaxMainBarDia();
	vector<MSSpecificTieBar*> ma_SpecificTieBar;
	void DeleteSpecificTieBars();
	// 보조주근
	MSSubMainBar m_SubMainBar;

	bool GetIsCornerPlacing()								{ return m_IsCornerPlacing; };
	void SetIsCornerPlacing(bool IsCornerPlacing)	{ m_IsCornerPlacing = IsCornerPlacing; };

	CString GetLeftRightBarDefaultValue();
	long GetColTieBarCountDefaultValue();
	CString GetColRowTieBarDefaultValue();
	double GetColRowTieBarSpacingDefaultValue();

protected:
	BOOL m_IsPC;
	bool m_IsCornerPlacing; // true: 코너 기준 배근 / false: 등간격 배근

#pragma region 전이 부재
public:
	enum ColumnM_TYPE { General = 0, Transfer = 1 };

protected:
	int m_nColumnMType;
public:
	int GetColumnMType();
	// COLUMNM_TYPE { GENERAL = 0, TRANSFER };
	void SetColumnMType(int nType);
	bool IsTransfer();

#pragma endregion
};
