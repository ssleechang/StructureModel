#pragma once
#include "MSLineMemberG.h"
class StructureModel_CLASS MSHaunchG :public MSLineMemberG
{
public:
	MSHaunchG();
	~MSHaunchG();

	virtual Type_MSCompoG GetType() { return msHaunchG; };
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSHaunchG)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual double GetWidth();

	virtual vector<GM2DCurve*> GetLevelDifCurves();
	GM2DPolyline* GetXMLBoundary();
	vector<GM2DPolyline*> GetGetLevelDifXMLBoundarys();


	enum eQuantityType { MatSlab, Pit, IsoFooting, WallFooting, Slab };

	eQuantityType GetQuantityType() { return m_QuantityType; };
	double GetTopOffset() { return m_TopOffset; };
	double GetBotOffset() { return m_BotOffset; };

	void SetQuantityType(eQuantityType type) { m_QuantityType = type; };
	void SetTopOffset(double offset);
	void SetBotOffset(double offset);
private: 
	eQuantityType m_QuantityType;
	double m_TopOffset;
	double m_BotOffset;

public:
	virtual MSCompoM::Type_MSCompoM GetDefCompoMType() { return MSCompoM::Type_MSCompoM::msHaunchM; };
};

