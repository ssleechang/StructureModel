#pragma once
#include "mscompom.h"
#include "MSMainBar.h"
#include "MSStirrupBar.h"
class StructureModel_CLASS MSColumnHeaderM : public MSCompoM
{
public:
	MSColumnHeaderM(void);
	~MSColumnHeaderM(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSColumnHeaderM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	int GetHeadType()				{ return m_HeaderType;};
	int		   GetDirection()		{ return m_Direction;};
	double GetL1()							{ return m_L1; };
	double GetL2()							{ return m_L2; };
	double GetL3()							{ return m_L3; };
	double GetL4()							{ return m_L4; };
	double GetL5()							{ return m_L5; };
	double GetT1()							{ return m_T1; };
	double GetT2()							{ return m_T2; };
	double GetT3()							{ return m_T3; };
	double GetT4()							{ return m_T4; };
	double GetT5()							{ return m_T5; };
	bool GetNumPlaceBar()					{ return m_IsNumPlaceBar; };

	void SetHeadType(int type)			{ m_HeaderType = type;};
	void SetDirection(int direction)	{ m_Direction = direction;};
	void SetL1(double value)	{ m_L1 = value;};
	void SetL2(double value)	{ m_L2 = value;};
	void SetL3(double value)	{ m_L3 = value;};
	void SetL4(double value)	{ m_L4 = value;};
	void SetL5(double value)	{ m_L5 = value;};
	void SetT1(double value)		{ m_T1 = value;};
	void SetT2(double value)		{ m_T2 = value;};
	void SetT3(double value)		{ m_T3 = value;};
	void SetT4(double value)		{ m_T4 = value;};
	void SetT5(double value)		{ m_T5 = value;};
	void SetNumPlaceBar(bool value)	{ m_IsNumPlaceBar = value; };

	bool GetPlaceRebar() { return m_PlaceRebar; };
	void SetPlaceRebar(bool value) { m_PlaceRebar = value; };

	MSMainBar m_LxMainBar;
	MSMainBar m_LxAddBar;
	MSMainBar m_LyMainBar;
	MSMainBar m_LyAddBar;
	MSStirrupBar m_StirrupBar;
	MSMainBar m_StirrupMainBar;

protected:
	int m_HeaderType;
	int m_Direction;
	double m_L1, m_L2;
	double m_L3, m_L4;
	double m_T1, m_T2;
	double m_T3, m_T4;
	double m_L5, m_T5;
	bool m_IsNumPlaceBar;

	bool m_PlaceRebar;
};

