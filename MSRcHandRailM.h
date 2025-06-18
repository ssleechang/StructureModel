#pragma once
#include "MSRcLineMemberM.h"

#include "GMLib\GM2DVector.h"

class StructureModel_CLASS MSRcHandRailM :	public MSRcLineMemberM
{
public:
	MSRcHandRailM(void);
	~MSRcHandRailM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcHandRailM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual bool IsGeometryDifferent(MSCompoM* compoM);
	virtual double GetThick(){return m_dWidth;};// by hana
	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual double GetParapetWidth();
	virtual int GetHandRailType(){return m_nHandRailType;};//by LSS only for Handrail

	virtual double GetBotLevel();
	virtual double GetTopLevel();
	virtual void SetBotLevel(double val);
	virtual void SetTopLevel(double val);
	virtual void SetThick(double val);
	virtual double GetDepth();

	//벽 두께 m_dWidth
	long m_nHandRailType;//0:상부벽만 있음, 1:하부벽도 있음. <- 업데이트 안 한 것!
	double m_dTopDepth;//상부벽 높이
	double m_dBotDepth;//하부벽 높이

	long m_nParapetType;//0:없음, 1:Defalt parapet

	double m_dParapetLengthA;
	double m_dParapetLengthB;
	double m_dParapetLengthC;
	double m_dParapetLengthD;

	vector<GM2DVector> GetSectionShape();
	static vector<GM2DVector> MakeSectionShape(MSRcHandRailM* pRcHandRailM, double dTopOffSet, double dBottomOffSet);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
};
