#pragma once
#include "MSRcFaceMemberM.h"

class StructureModel_CLASS MSRcFWSlabM :public MSRcFaceMemberM
{
public:
	MSRcFWSlabM();
	~MSRcFWSlabM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSRcFWSlabM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eFWSlabType { General = 0, BottomSlope, TopSlope};

	static CString ToShapeName(eFWSlabType eType);
	static vector<CString> GetFormWorkSlabShapeNameList();

	void MakeProfile();
	void MakeProfile(GM2DPolyline* pTargetProflie, double dLength);
	GM2DPolyline* GetProfile() { return m_pProfile; };
	eFWSlabType GetFWSlabType() { return me_FWSlabType; };
	virtual double GetThick() { return m_T; };
	virtual double GetT() { return m_T; };
	virtual double GetT1() { return m_T1; };

	//FromWork용 함수 - start
	virtual CString GetShapeName();
	//FromWork용 함수 - end

	void SetFWSlabType(eFWSlabType type) { me_FWSlabType = type; };
	void SetThick(double param) { m_T = param; };
	void SetT(double param) { m_T = param; };
	void SetT1(double param) { m_T1 = param; };

protected:
	void makeProfile_General(GM2DPolyline* pTargetProflie, double dLength);
	void makeProfile_TopSlope(GM2DPolyline* pTargetProflie, double dLength);
	void makeProfile_BottomSlope(GM2DPolyline* pTargetProflie, double dLength);

protected:
	eFWSlabType me_FWSlabType;
	double m_T;
	double m_T1;
	GM2DPolyline* m_pProfile;
};