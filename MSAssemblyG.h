#pragma once
#include "mscompog.h"

#include "GMLib/GM2DVector.h"

class MSAssembly;

class StructureModel_CLASS MSAssemblyG :
	public MSCompoG
{
public:
	MSAssemblyG(void);
	~MSAssemblyG(void);

 	void Serialize( CArchive& archive );
 	DECLARE_SERIAL(MSAssemblyG)
 	virtual MSObject* Copy(bool bNewID = true);
 	void CopyFromMe(MSAssemblyG* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

 	MSAssembly* mp_Type;

	GM2DVector m_BasePoint;
	bool m_bMirroring;
	double m_Rotation;
	double m_BotOffsetZ;
	double m_TopOffsetZ;
// 	vector<MSConnectivity*> ma_BoundaryConn;
// 
// public:
// 	virtual MSConnectivity* MakeDefaultJoint( long StartOrEnd, bool IsUpJoint=false ) {return NULL;};
// 	bool ModifyClones();
// 	virtual bool UnlinkFromJoint() {return false;};
// 	virtual bool SetCloneJoint(MSFloor* pCloneFloor) {return false;};
// 	virtual void SetOwner(MSFloor* val){mp_SFloor = val;};
// 	virtual MSFloor* GetOwner(){return mp_SFloor;};
// 	virtual MSCompoM* GetCompoM(){return mp_CompoM;};
// 	virtual bool SetCompoM(long MID);
// 	virtual MSBaseBuilding* GetBuilding();
// 	virtual double GetStartLevel(){return 0;};
// 	virtual double GetEndLevel(){return 0;};
// 
// 
// 	virtual double GetWidth(){return 0;};
// 	virtual double GetDepth(){return 0;};
// 	virtual double GetThick(){return 0;};
// 	virtual double GetHeight(){return 0;};
// 	virtual void GetSecPolygon(GMPolygon& APGon);
// 	virtual double GetFloorLevel();
// 
// 	virtual bool GetZLevels(double& TopLevel, double& BottomLevel ) {return false;};
// 	virtual BOOL VisibleInRect( GM2DRectangle* pRect) {return false;};
};

