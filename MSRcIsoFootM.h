#pragma once
#include "MSRcFaceMemberM.h"

#include "GMLib/GMPolygon.h"


class StructureModel_CLASS MSRcIsoFootM :
	public MSRcFaceMemberM
{
public:
	MSRcIsoFootM(void);
	~MSRcIsoFootM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcIsoFootM)
	virtual void CopyFromMe(MSCompoM* pSource);

	GMPolygon m_SecPGon;
	long m_nShape;

	long m_Haunch; //long m_Haunch;

	enum ISOFOOT_TYPE { ISOFOOT_ISOFOOT=0, ISOFOOT_DROPPANEL };
	long m_nIsoFootType;
	bool IsDropPanel() { return (m_nIsoFootType == ISOFOOT_DROPPANEL); };

	virtual void InitValue();
	virtual double GetWidth() {return m_dWidth;};//by LSS
	virtual double GetDepth(){return m_dDepth;};//by LSS
	virtual void GetSecPolyline( GM2DPolyline& TheGon );//by LSS
	virtual long GetHaunchType(){return m_Haunch;}//by LSS for IsoFoot
	virtual void SetWidth(double dWidth){m_dWidth = dWidth;};	// by gracias
	virtual void SetDepth(double dDepth){m_dDepth = dDepth;};	// by gracias

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

private:
	double m_dDepth, m_dWidth; 

};
