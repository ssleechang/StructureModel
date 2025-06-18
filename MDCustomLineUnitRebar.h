#pragma once
#include "MSRcObject.h"
#include "MSStirrupBar.h"
#include "MSMainBarSet.h"
#include "MSCustomLineBarUnit.h"

class MDMember;
class StructureModel_CLASS MDCustomLineUnitRebar :public MSRcObject
{
public:
	MDCustomLineUnitRebar();
	~MDCustomLineUnitRebar();

	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomLineUnitRebar)
	virtual void CopyFromMe(MSRcObject* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void SetUnitID(int nID) { m_LineUnitID = nID; };
	void SetNumBars(int nBars) { m_NumBars = nBars; };
	void SetStirrupBar(MSStirrupBar stirrupBar) { m_StirrupBar.CopyFromMe(&stirrupBar); };
	void SetStirrupBar(CString rebarText);

	int GetUnitID() { return m_LineUnitID; };
	int GetNumBars() { return m_NumBars; };
	MSStirrupBar& GetStirrupBar() { return m_StirrupBar; };
	CString GetStirrupBarText();

	MDMember* GetMDMemberM() { return mp_RebarM; }
	void SetMDMemberM(MDMember* pOwner) { mp_RebarM = pOwner; };

	vector<GM2DPolyline*> CalculatorLineBarLocation(MSCustomLineBarUnit* pUnit);
	double GetMaxDeformedStirrupBarDiameter();

protected:
	vector<GM2DPolyline*> CalculatorLineBarLocationByHoop(MSCustomLineBarUnit* pUnit);
	vector<GM2DPolyline*> CalculatorLineBarLocationByTie(MSCustomLineBarUnit* pUnit);
	vector<GM2DPolyline*> CalculatorTieLineBarLocationByEqual(MSCustomLineBarUnit* pUnit);
	vector<GM2DPolyline*> CalculatorTieLineBarLocationByEffectiveSpacing(MSCustomLineBarUnit* pUnit);

	GM2DPolyline* ConvertLineToPolyline(GM2DLineVector* pLineVec);
protected:
	int m_LineUnitID;
	int m_NumBars;
	MSStirrupBar m_StirrupBar;

	MDMember* mp_RebarM;

};
