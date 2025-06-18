#pragma once
#include "MSRcLineMemberM.h"

class StructureModel_CLASS MSRcFWBeamM :public MSRcLineMemberM
{
public:
	MSRcFWBeamM();
	~MSRcFWBeamM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSRcFWBeamM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eFWBeamType {	
		Type1 = 0, //일반 보
		Type2,       //캔틸레버 보
		Type3,       //이중 보
		Type4,       //특수보
		Type5,      //일반 역보
		Type6,      //이중역보
		Type7,      //특수 역보
		Type8,     //경사 역보
		Type9      //모따기 역보
	};

	static CString ToShapeName(eFWBeamType eType);
	static vector<CString> GetFormWorkBeamShapeNameList();

	void MakeProfile();
	bool CheckProfile();

	GM2DPolyline* GetProfile() { return m_pProfile; };
	eFWBeamType GetFWBeamType() { return me_FWBeamType; };
	virtual bool IsPolygonSection();
	virtual void GetSecPolyline(GM2DPolyline& TheGon);
	virtual double GetWidth() { return m_W; };
	virtual double GetDepth() { return m_H; };
	virtual double GetW() { return m_W; };
	virtual double GetW1() { return m_W1; };
	virtual double GetW2() { return m_W2; };
	virtual double GetH() { return m_H; };
	virtual double GetH1() { return m_H1; };
	virtual double GetH2() { return m_H2; };
	double GetMaxH();

	virtual bool IsCustomM();
	//FromWork용 함수 - start
	virtual CString GetShapeName();
	//FromWork용 함수 - end

	void SetFWBeamType(eFWBeamType type);
	void SetWidth(double param) { m_W = param; };
	void SetDepth(double param) { m_H = param; };
	void SetW(double param) { m_W = param; };
	void SetW1(double param) { m_W1 = param; };
	void SetW2(double param) { m_W2 = param; };
	void SetH(double param) { m_H = param; };
	void SetH1(double param) { m_H1 = param; };
	void SetH2(double param) { m_H2 = param; };

	bool IsReversBeamM();
protected:
	void makeProfile_Type1();
	void makeProfile_Type2();
	void makeProfile_Type3();
	void makeProfile_Type4();
	void makeProfile_Type5();
	void makeProfile_Type6();
	void makeProfile_Type7();
	void makeProfile_Type8();
	void makeProfile_Type9();

	bool CheckProfile_Type3();
	bool CheckProfile_Type4();
	bool CheckProfile_Type7();
	bool CheckProfile_Type8();
	bool CheckProfile_Type9();

protected:
	GM2DPolyline* m_pProfile;
	eFWBeamType me_FWBeamType;
	double m_W, m_W1, m_W2;
	double m_H, m_H1, m_H2;
};


