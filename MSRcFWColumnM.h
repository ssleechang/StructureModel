#pragma once
#include "MSRcLineMemberM.h"
class StructureModel_CLASS MSRcFWColumnM :public MSRcLineMemberM
{
public:
	MSRcFWColumnM();
	~MSRcFWColumnM();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSRcFWColumnM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eFWColumnType {	Rect = 0, Circle, Pentagon_Inner, Pentagon_Outter, 
												Hexagon_Inner, Hexagon_Outter,
												Octagon_Inner, Octagon_Outter,
												Custom
	};

	static CString ToShapeName(eFWColumnType eType);
	static vector<CString> GetFormWorkColumnShapeNameList();
	static eFWColumnType ToFWColumnType(SECT_SHAPE eShapeType);

	void MakeProfile();
	GM2DPolyline* GetProfile() { return m_pProfile; };
	void SetProfile(GM2DPolyline pProfile);
	eFWColumnType GetFWColumnType() { return me_FWColumnType; };
	
	virtual bool IsPolygonSection();
	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual double GetWidth() { return m_W; };
	virtual double GetDepth() { return m_D; };
	virtual double GetW() { return m_W; };
	virtual double GetD() { return m_D; };

	//FromWork용 함수 - start
	virtual CString GetShapeName();
	//FromWork용 함수 - end

	void SetFWColumnType(eFWColumnType type);
	void SetWidth(double param);
	void SetDepth(double param);
	void SetW(double param);
	void SetD(double param);

protected:
	void makeProfile_Rect();
	void makeProfile_Circle();
	void makeProfile_PentagonInner();
	void makeProfile_PentagonOutter();
	void makeProfile_HexagonInner();
	void makeProfile_HexagonOutter();
	void makeProfile_OctagonInner();
	void makeProfile_OctagonOutter();

	void makeProfile_InsideCircle(GM2DCircle* pCircle, double startAngle, int numEdge);
protected:
	GM2DPolyline* m_pProfile;
	eFWColumnType me_FWColumnType;
	double m_W;
	double m_D;
};

