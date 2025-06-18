#pragma once
#include "MSLineRebar.h"
#include "GMLib/GM2DLineVector.h"
class MSAddRebarAreaBent;
class StructureModel_CLASS MSBentRebar :public MSLineRebar
{
public:
	MSBentRebar();
	~MSBentRebar();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSBentRebar)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	enum eBentType { OneBent = 0, TwoBent, Straight };
	enum eStartBentLocation { Top = 0, Bot };

	void SetOwner(MSAddRebarAreaBent* pOwner);
	MSAddRebarAreaBent* GetOwner();

	eBentType GetBentType() { return m_eBentType; };
	void SetBentType(eBentType bentType) { m_eBentType = bentType; };

	eStartBentLocation GetStartBentLocation() { return m_eStartBentLocation; };
	void SetStartBentLocation(eStartBentLocation startBentLocation);

	vector<double> GetBentLocValue();
	void SetBentLocValue(vector<double> Locs);

	vector<GM2DVector> GetBentPoints();
	GM2DPolyline* GetBentLine() { return &m_BentLine; };
	GM2DLineVector GetRebarCenterLine();

	double GetDegree();
	GM2DVector GetNormal();
	GM2DVector GetDirection();
	GM2DVector GetRebarOrigin();

	void CalculateBentLine();
	bool CheckBentLocation(GM2DPolyline* profile);
protected:
	//---- Serialize Start ----//
	eBentType m_eBentType;
	eStartBentLocation m_eStartBentLocation;

	vector<double> ma_BentLocValue;
	GM2DLineVector m_RebarZone;
	GM2DLineVector m_RebarLine;
	GM2DPolyline m_BentLine;
	//---- Serialize End ----//
	
	MSAddRebarAreaBent* m_pAddRebarAreaBent;
};

