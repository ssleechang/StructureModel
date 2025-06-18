#pragma once
#include "msobject.h"
#include "MSDeformedBar.h"

typedef struct _SpacerShape
{
public:
	_SpacerShape()
	{
		m_G = 6; m_S = 6; 
		m_B = m_D = m_F = 200;
		m_C = m_E = 150;
	}
	int GetG(){return m_G;}
	int GetS(){return m_S;}
	int GetB(){return m_B;}
	int GetC(){return m_C;}
	int GetD(){return m_D;}
	int GetE(){return m_E;}
	int GetF(){return m_F;}

	void SetG(int TheG){m_G = TheG;}
	void SetS(int TheS){m_S = TheS;}
	void SetB(int TheB){m_B = TheB;}
	void SetC(int TheC){m_C = TheC;}
	void SetD(int TheD){m_D = TheD;}
	void SetE(int TheE){m_E = TheE;}
	void SetF(int TheF){m_F = TheF;}

	void AutoCalc(double dSpacing, double TheDepth, double dTopCover, double dBotCover, double SpacerDia, double TopDia, double BotDia)
	{
		m_B = m_F = (int)CEIL(dSpacing*1.5,1);// + 150;
		double dH = TheDepth-dTopCover-dBotCover-TopDia*3-BotDia*2;
		if(dH<DTOL_GM)
			dH = 10;
		m_C = m_E = ROUND(dH/10)*10;
		
		// TODO(김신웅) : 아래 조건문은 한국규준에만 맞춘것이다. 다른 규준에도 적용할 수 있도록 수정되어야 한다.
		CString sDiaName = MSDeformedBar::FindBarDiameterName(SpacerDia);
		double dDiaName = _ttof(sDiaName);

		if(dDiaName < 14.0)
			m_D = 150;		
		else if(dDiaName < 20.0)
			m_D = 200;		
		else if(dDiaName < 26.0)
			m_D = 250;		
		else
			m_D = 200;		

// 		if(SpacerDia == 10 || SpacerDia == 13)
// 			m_D = 150;		
// 		else if(SpacerDia == 16 || SpacerDia == 19)
// 			m_D = 200;		
// 		else if(SpacerDia == 22 || SpacerDia == 25)
// 			m_D = 250;		
// 		else
// 			m_D = 200;		
	}
	_SpacerShape & operator = (const _SpacerShape & TheCopySpacerShape)
	{
		m_B = TheCopySpacerShape.m_B;
		m_C = TheCopySpacerShape.m_C;
		m_D = TheCopySpacerShape.m_D;
		m_E = TheCopySpacerShape.m_E;
		m_F = TheCopySpacerShape.m_F;
		m_G = TheCopySpacerShape.m_G;
		m_S = TheCopySpacerShape.m_S;

		return *this;
	}

	void CopyFromMe(_SpacerShape* Source)
	{
		m_B = Source->m_B;
		m_C = Source->m_C;
		m_D = Source->m_D;
		m_E = Source->m_E;
		m_F = Source->m_F;
		m_G = Source->m_G;
		m_S = Source->m_S;
	}

	void Serialize( CArchive& ar)
	{
		if(ar.IsStoring())
		{
			ar << m_G;
			ar << m_S;
			ar << m_B;
			ar << m_C;
			ar << m_D;
			ar << m_E;
			ar << m_F;
		}
		else
		{
			ar >> m_G;
			ar >> m_S;
			ar >> m_B;
			ar >> m_C;
			ar >> m_D;
			ar >> m_E;
			ar >> m_F;
		}
	}
private:
	int m_G;
	int m_S;
	int m_B;
	int m_C;
	int m_D;
	int m_E;
	int m_F;
}SpacerShape;

class StructureModel_CLASS MDMember :	public MSObject
{
public:
	MDMember(void);
	virtual ~MDMember(void);
	DECLARE_SERIAL(MDMember)
	void Serialize( CArchive& archive );
	virtual void CopyFromMe(MDMember* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual double GetCover(){return m_dCvrConc;};
	virtual void SetCover(double dCover){m_dCvrConc=dCover;};
	virtual BOOL IsPC(){return FALSE;};
#pragma region 전이 부재
	virtual bool IsTransfer() { return false; };
#pragma endregion
	virtual bool IsDeck() { return false; };

	double m_dCvrConc;

	virtual vector<double> GetMainBarDiaAllLayer();
	virtual vector<double> GetStirrupBarDiaAllLayer();
};

