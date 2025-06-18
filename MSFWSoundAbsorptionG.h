#pragma once
#include "MSCompoG.h"
#include "GMLib/GM2DRectangle.h"

class StructureModel_CLASS MSFWSoundAbsorptionG :public MSCompoG
{
public:
	MSFWSoundAbsorptionG();
	~MSFWSoundAbsorptionG();
	virtual Type_MSCompoG GetType() { return msFWSoundAbsorptionG; };
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFWSoundAbsorptionG)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);
	
	virtual bool MakeBoundary();
	virtual inline void CalcBoundRectBeforeAddQuadTree();
	virtual bool VisibleInRect(GM2DRectangle* pRect);

//	virtual MSElement* GetOwnerCompoG() { return m_pOwnerCompoG; };
//	virtual void SetOwnerCompoG(MSElement* pOwnerCompoG);

	bool GetIsTopLocation() { return m_IsTopLocation; };
	void SetIsTopLocation(bool isTopLocation) { m_IsTopLocation = isTopLocation; };

	virtual void Translate(GM2DVector &pDelta) override;
	virtual void MirrorByAxis(GM2DLineVector& pAxis) override;
	virtual void RotateZ(double theta, GM2DVector *pCenter) override;
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal = true, bool bCheckLevelZone = true);
protected:
	//MSElement* m_pOwnerCompoG;
	bool m_IsTopLocation;
	GM2DRectangle m_RectForQuadTree;
};