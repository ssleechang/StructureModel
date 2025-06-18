#pragma once
#include "msmaterial.h"

class StructureModel_CLASS MSSteel :
	public MSMaterial
{
public:
	MSSteel(void);
	~MSSteel(void);

	enum STEEL_CODE
	{
		STL_CODE_USER = 0, STL_CODE_KS
	};

	enum STEEL_GRADE
	{
		//일반구조용 압연강재(KS D 3503)
		STL_GRADE_SS400 = 0, STL_GRADE_SS490, STL_GRADE_SS540,
		//용접구조용 압연강재(KS D 3515)
		STL_GRADE_SM400A, STL_GRADE_SM400B, STL_GRADE_SM400C, 
		STL_GRADE_SM490A, STL_GRADE_SM490B, STL_GRADE_SM490C, 
		STL_GRADE_SM490YA, STL_GRADE_SM490YB, STL_GRADE_SM490TMC,
		STL_GRADE_SM520B, STL_GRADE_SM520C, STL_GRADE_SM520TMC, 
		STL_GRADE_SM570, STL_GRADE_SM570TMC,
		//일반구조용 탄소강관(KS D 3566)
		STL_GRADE_SPS290, STL_GRADE_SPS400, STL_GRADE_SPS490, STL_GRADE_SPS500, STL_GRADE_SPS540,
		//일반구조용 각형강관(KS D 3568)
		STL_GRADE_SPSR400, STL_GRADE_SPSR490,
		//건축구조용 탄소강관(KS D 3632)
		STL_GRADE_STKN400W, STL_GRADE_STKN400B, STL_GRADE_STKN490B,
		//건축구조용 압연강재(KS D 3861)
		STL_GRADE_SN400A, STL_GRADE_SN400B, STL_GRADE_SN400C, STL_GRADE_SN490B, STL_GRADE_SN490C, 
		//건축구조용 열간압연H형강(KS D 3866)
		STL_GRADE_SHN400, STL_GRADE_SHN490, STL_GRADE_SHN520,
		//내진 건축구조용 냉간성형 각형강관(KS D 3864)
		STL_GRADE_SPAR295, STL_GRADE_SPAR360, 
		STL_GRADE_SPAP235, STL_GRADE_SPAP325,
		//H형강말뚝(KS F 4603)
		STL_GRADE_SHK400, STL_GRADE_SHK400M, STL_GRADE_SHK490M,
		//강널말뚝(KS F 4604)
		STL_GRADE_SY30, STL_GRADE_SY30A, STL_GRADE_SY40, STL_GRADE_SY40A,
	};
	void Serialize( CArchive& archive );
	virtual void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSSteel)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	double m_Fy;
	STEEL_CODE m_Code;
	STEEL_GRADE m_Grade;
};
