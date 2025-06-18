#pragma once
#include "MSRcObject.h"
#include "MSMainBarSet.h"
#include "MSCustomPointBarUnit.h"
class MDMember;
class StructureModel_CLASS MDCustomPointUnitRebar :public MSRcObject
{
public:
	MDCustomPointUnitRebar();
	~MDCustomPointUnitRebar();

	void Serialize(CArchive& ar);
	DECLARE_SERIAL(MDCustomPointUnitRebar)
	virtual void CopyFromMe(MSRcObject* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void SetUnitID(int nID) { m_PointUnitID = nID; };
	bool SetMainBarSet(CString fullData);

	int GetUnitID() { return m_PointUnitID; };
	MSMainBarSet& GetMainBarSet() { return m_MainBarSet; };
	CString GetMainBarText();

	MDMember* GetMDMemberM() { return mp_RebarM; }
	void SetMDMemberM(MDMember* pOwner) { mp_RebarM = pOwner; };

	vector<GM2DVector> CalculatorPointBarLocation(GM2DLineVector lineVec, MSCustomPointBarUnit::eSpacingType spacingType);
	double GetMaxDeformedMainBarDiameter();
protected:
	vector<GM2DVector> CalculatorPointBarLocationByEqual(GM2DLineVector lineVec);
	vector<GM2DVector> CalculatorPointBarLocationByEffectiveSpacing(GM2DLineVector lineVec);

protected:
	int m_PointUnitID;
	MSMainBarSet m_MainBarSet;

	MDMember* mp_RebarM;
};
