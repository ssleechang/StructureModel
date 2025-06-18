#pragma once
using namespace std;

#include "MemberLengthItem.h"
#include "CommonLengthItem.h"
#include "HookLengthItem.h"

namespace RebarOption
{
	enum DevSpl_Type
	{
		DEVSPL_CALC_DETAIL,
		DEVSPL_CALC_SIMPLE,
		DEVSPL_USER_GEN,
		DEVSPL_USER_UNDEFIND = 1000
	};

	enum MemberLen_Type
	{
		MEMBERTYPE_FOUNDATION = 0,
		MEMBERTYPE_COLUMN,
		MEMBERTYPE_WALL,
		MEMBERTYPE_RWALL,
		MEMBERTYPE_LOWWALL,
		MEMBERTYPE_BEAM,
		MEMBERTYPE_SLAB,
		MEMBERTYPE_DECKSLAB,
		MEMBERTYPE_SRCCOLUMN,
		MEMBERTYPE_SRCBEAM,
		MEMBERTYPE_COMMON,
		MEMBERTYPE_HOOK
	};
}
#define STR_MEMBERTYPE_FOUNDATION	_T("FOUNDATION")
#define STR_MEMBERTYPE_COLUMN			_T("COLUMN")
#define STR_MEMBERTYPE_WALL				_T("WALL")
#define STR_MEMBERTYPE_RWALL			_T("RWALL")
#define STR_MEMBERTYPE_LOWWALL		_T("LOWWALL")
#define STR_MEMBERTYPE_BEAM				_T("BEAM")
#define STR_MEMBERTYPE_SLAB				_T("SLAB")
#define STR_MEMBERTYPE_DECKSLAB			_T("DECKSLAB")
#define STR_MEMBERTYPE_SRCCOLUMN		_T("SRCCOLUMN")
#define STR_MEMBERTYPE_SRCBEAM			_T("SRCBEAM")
#define STR_MEMBERTYPE_COMMON			_T("COMMON")
#define STR_MEMBERTYPE_HOOK				_T("HOOK")

#define SHOP_CONMAT21 21
#define SHOP_CONMAT24 24
#define SHOP_CONMAT27 27
#define SHOP_CONMAT30 30
#define SHOP_CONMAT35 35
#define SHOP_CONMAT40 40
#define SHOP_CONMAT45 45
#define SHOP_CONMAT50 50

#define CODESTRESSUNIT 9.806 //kgf->N

class MSCompoG;
class MSMaterial;
class StructureModel_CLASS DevSpliceLengthSet
{
public:
	DevSpliceLengthSet();
	DevSpliceLengthSet(CString name);
	~DevSpliceLengthSet(void);
	void Init();
	void Serialize(CArchive& ar);
	void Initialize(bool bFullUpdate = true);

	CString GetDiaName(long index);
	
	double GetHookLength(double dDia, bool IsStirrup = false, long nDeg = 90) const;
	double GetDevLengthTension(MSCompoG* pSupportG,MSCompoG* pMSCompoG,double dDia, long IsTop, double dBarDist,long nMaterialID) const;
	double GetDevLengthComp(MSCompoG* pSupportG,MSCompoG* pMSCompoG,double dDia, long IsTop=0, double TieBarDist=200, double TieBarDia=0) const;
	double GetSpliceLengthA(MSCompoG* pMSCompoG,double dDia, long IsTop=0, double dBarDist = 50) const;
	double GetSpliceLengthB(MSCompoG* pMSCompoG, double dDia, long IsTop, double dBarDist, double dFckSupportG, long nMaterialID) const;
	double GetSpliceLengthComp(MSCompoG* pMSCompoG, double dDia, double dFckSupportG,long nMaterialID) const;
	double GetDevLength4Hook(MSCompoG* pSupportG,MSCompoG* pMSCompoG, double dDia, double EndCover=20, double StrpDist = 200) const;


	CString GetHookKey(double dDia, bool IsStirrup = false) const;
	CString GetDevSplKey(CString strMember, double dDia, double dFck, long IsTop, double dFy) const;
	CString GetCommonKey(CString strMember,double dDia, double dFck,double dFy) const;

	double SqrtFck(double Fck) const;
	double GetFck(MSCompoG* pMSCompoG) const;
	double GetFck(MSMaterial* pMat) const;
	double GetCover(MSCompoG* pMSCompoG) const;
	double GetDepthBelow(MSCompoG* pMSCompoG) const;

	////120215.ES.약산식/정밀식 반영-표준갈고리를 갖는 인장이형철근의 정착-KBC2009.0508.2.5
	double CalcDevLength4Hook(double Dia, double Fck, double Fy, double CorrectionFactor =1.0) const;
	
	//120215.ES.약산식/정밀식 반영-표준갈고리-KBC0505.2 표준갈고리
	double CalcHookLength(double Dia, bool IsStirrup = false, double dDeg = 90) const;
	
	//120215.ES.약산식/정밀식 반영-인장정착길이--KBC2009.0508.2.2
	double CalcDevLength(double Dia, double Fck, double Fy,double CorrectionFactor=1.0, long Simplified =0,
		double DistFactor=1.0, double DepthBelow=301.0) const;
	//120215.ES.약산식/정밀식 반영-압축이형철근의 정착-KBC2009.0508.2.3 
	double CalcDevLengthComp(double Dia, double Fck, double Fy, double CorrectiveFactor=1.0) const;

	//120215.ES.약산식/정밀식 반영-압축이형철근의 A급이음-KBC2009.0508.6.2
	double CalcSpliceLengthA(double Dia, double Fck, double Fy,double CorrectionFactor=1.0, long Simplified =0,
		double DistFactor=1.0, double DepthBelow=301.0) const;

	//120215.ES.약산식/정밀식 반영-압축이형철근의 B급이음-KBC2009.0508.6.2
	double CalcSpliceLengthB(double Dia, double Fck, double Fy,double CorrectionFactor=1.0, long Simplified =0,
		double DistFactor=1.0, double DepthBelow=301.0) const;

	//120215.ES.약산식/정밀식 반영-압축이형철근의이음--KBC2009.0508.6.3
	double CalcSpliceLengthComp(double Dia, double Fck, double Fy) const;

	CString GetMemberType(MSCompoG* pMSCompoG) const;
	CString GetConMatKeyName(double dFck) const;
	//CLI
	CString GetHookKey4DiaIndex(long nDiaIndex);
	CString GetDevSplKey(long nDiaIndex, long IsTop, double dFy, double dFck);
	CString GetCommonKey(long nDiaIndex, double dFck, double dFy);
	long GetNumDia();
	long GetDevLength(long nDiaIndex, long nLoc, double dFy, double dFck);
	void SetDevLength(long nDiaIndex, long nLoc, long nVal, double dFy, double dFck);
	long GetSpliceLength(long nDiaIndex, long nLoc, double dFy, double dFck);
	void SetSpliceLength(long nDiaIndex, long nLoc, long nVal, double dFy, double dFck);
	long GetCompSpliceLength(long nDiaIndex, double dFck, double dFy);
	void SetCompSpliceLength(long nDiaIndex, double dFck, double dFy, long nVal);
	long GetLDHLength(long nDiaIndex, double dFck, double dFy);
	void SetLDHLength(long nDiaIndex, double dFck, double dFy, long nVal);
	HookLengthItem *GetHookLengthItem(long nDiaIndex);
	CString DevSpliceLengthKeyParsing(CString key);

	CString GetMemberName(long MemberType);
	double GetFy(MSCompoG* pMSCompoG,double dDia) const;
	double GetFy(MSMaterial* pMat, double dDia) const;
	long GetFckNameList(CStringArray& arNameList);
	long GetFyNameList(CStringArray& arNameList);
	CString GetFyKeyName(double dFy) const;
	CString m_sName;

	CString m_sReinforceBarSetting;//선택된 FyIndex
	long m_nMemberType;//선택된 MemberLen_Type

	RebarOption::DevSpl_Type m_nDevSplType;
	long m_nCeilDigit;
	BOOL m_bEpoxyFactor;
	BOOL m_LightWeightConc;
	BOOL m_SimpleCalculation;



	map<CString,MemberLengthItem> mm_MemberLength;
	map<CString,CommonLengthItem> mm_CommonLength;
	map<CString,HookLengthItem> mm_HookLength;

	void Copy(shared_ptr<DevSpliceLengthSet> desDevSpliceLengthSet);

	void MakeLowWallFromBWall();
};

