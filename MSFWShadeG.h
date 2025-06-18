#pragma once
#include "MSCompoG.h"

class StructureModel_CLASS MSFWShadeG :public MSCompoG
{

public:
	MSFWShadeG();
	~MSFWShadeG();
	virtual Type_MSCompoG GetType() { return msFWShadeG; };
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWShadeG)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	virtual bool MakeBoundary();

	virtual MSElement* GetOwnerCompoG() { return m_pOwnerCompoG; };
	//GM2DVector GetBaseOffset() { return m_BaseOffset; };
	bool GetIsLeftWallCurve() { return m_IsLeftWallCurve; };

	virtual void SetOwnerCompoG(MSElement* pOwnerCompoG);
	//void SetBaseOffset(GM2DVector baseVec) { m_BaseOffset = baseVec; };
	void SetIsLeftWallCurve(bool isLeft) { m_IsLeftWallCurve = isLeft; };
		
	double GetVerticalLength(); //수직차양 높이
	double GetHorizontalShadeLength(); //수평차양 길이
	void SetHorizontalShadeLength(double value);
	
	// 수직 차양 전용
	GM2DVector GetStartPnt();
	bool IsVerticalShade();
	GM2DCurve* GetHorShadeGBaseCurve();


	bool GetShadeLoc(double& dSLoc, double& dELoc);
	double GetTopOffSet() { return m_dTopOffset; }
	virtual double GetBotOffSet() { return m_dBotOffset; };
	void SetTopOffSet(double value) { m_dTopOffset = value; }
	void SetBotOffSet(double value) { m_dBotOffset = value; }

	//수직차양 벽체 스타트부터 떨어진 거리
	void SetDistanceX(double value) { m_dDistanceX = value; }
	double GetDistanceX() { return m_dDistanceX; }

	bool CheckData(bool bMakeErrorObject = true);

	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true);

	virtual CString GetKey(bool bUseID = true);
protected:
	MSElement* m_pOwnerCompoG;
	//GM2DVector m_BaseOffset;

	double m_dBotOffset;
	double m_dTopOffset;
	double m_dDistanceX;

	double m_dHorizontalShadeLength;
	bool m_IsLeftWallCurve;

	double GetDegree();
};

