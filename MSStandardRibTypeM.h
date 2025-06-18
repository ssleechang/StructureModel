#pragma once
#include "msribtypem.h"
class StructureModel_CLASS MSStandardRibTypeM :
	public MSRibTypeM
{
public:
	MSStandardRibTypeM(void);
	~MSStandardRibTypeM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSStandardRibTypeM)
	virtual void CopyFromMe(MSStandardRibTypeM* pSource);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	double GetParamA()	{ return m_ParamA;};
	double GetParamB()	{ return m_ParamB;};
	double GetParamC()	{ return m_ParamC;};

	void SetParamA(double paramA)	{ m_ParamA = paramA;};
	void SetParamB(double paramB)	{ m_ParamB = paramB;};
	void SetParamC(double paramC)	{ m_ParamC = paramC;};
protected:
	double m_ParamA;	// 상부 길이	|			 |
	double m_ParamB;	// 깊이				 |			|
	double m_ParamC;	// 하부 길이	  |ㅡㅡ|
														 	
};

