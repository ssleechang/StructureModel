#pragma once
#include "MSRcFaceMemberM.h"

class StructureModel_CLASS MSRcFWBWallM :public MSRcFaceMemberM
{
public:
	MSRcFWBWallM();
	~MSRcFWBWallM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSRcFWBWallM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eFWBWallType { General = 0, Brick};

	static CString ToShapeName(eFWBWallType eType);
	static vector<CString> GetFormWorkWallShapeNameList();

	void MakeProfile();
	GM2DPolyline* GetProfile() { return m_pProfile; };
	eFWBWallType GetFWBWallType() { return me_FWBWallType; };
	virtual double GetThick() { return m_T; };
	virtual double GetT() { return m_T; };

	//FromWork용 함수 - start
	virtual CString GetShapeName();
	//FromWork용 함수 - end

	void SetFWBWallType(eFWBWallType type) { me_FWBWallType = type; };
	void SetThick(double param) { m_T = param; };
	void SetT(double param) { m_T = param; };

protected:
	void makeProfile_General();
	void makeProfile_Brick();
protected:
	eFWBWallType me_FWBWallType;
	double m_T;
	GM2DPolyline* m_pProfile;
};