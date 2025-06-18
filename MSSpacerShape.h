#pragma once
#include "MSElement.h"
class StructureModel_CLASS MSSpacerShape : public MSElement
{
public:
	MSSpacerShape(void);
	~MSSpacerShape(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSpacerShape)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual void Init();

	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	MSSpacerShape & operator = (const MSSpacerShape & TheCopySpacerShape)
	{
		m_B = TheCopySpacerShape.m_B;
		m_C = TheCopySpacerShape.m_C;
		m_D = TheCopySpacerShape.m_D;
		m_E = TheCopySpacerShape.m_E;
		m_F = TheCopySpacerShape.m_F;
		m_G = TheCopySpacerShape.m_G;
		m_S = TheCopySpacerShape.m_S;

		m_DiaName = TheCopySpacerShape.m_DiaName;
		m_XSpacing = TheCopySpacerShape.m_XSpacing;
		m_YSpacing = TheCopySpacerShape.m_YSpacing;
		return *this;
	}

	CString GetName(){return m_Name;}
	int GetG(){return m_G;}
	int GetS(){return m_S;}
	int GetB(){return m_B;}
	int GetC(){return m_C;}
	int GetD(){return m_D;}
	int GetE(){return m_E;}
	int GetF(){return m_F;}
	CString GetDiaName(){return m_DiaName;}
	int GetXSpacing(){return m_XSpacing;}
	int GetYSpacing(){return m_YSpacing;}

	void SetName(CString theName){m_Name = theName;}
	void SetG(int TheG){m_G = TheG;}
	void SetS(int TheS){m_S = TheS;}
	void SetB(int TheB){m_B = TheB;}
	void SetC(int TheC){m_C = TheC;}
	void SetD(int TheD){m_D = TheD;}
	void SetE(int TheE){m_E = TheE;}
	void SetF(int TheF){m_F = TheF;}
	void SetDiaName(CString diaName){m_DiaName = diaName;}
	void SetXSpacing(int spacing){m_XSpacing = spacing;}
	void SetYSpacing(int spacing){m_YSpacing = spacing;}
	
private:
	CString m_Name;
	CString m_DiaName;
	int m_XSpacing;
	int m_YSpacing;
	int m_G;
	int m_S;
	int m_B;
	int m_C;
	int m_D;
	int m_E;
	int m_F;
};

