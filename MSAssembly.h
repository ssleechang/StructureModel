#pragma once
#include "MSContainer.h"
#include "GMLib/GM2DRectangle.h"

class MSAssemblyG;
class MSCompoG;
class MSLevelZone;
class MSSectionView;
class MSLinkMemberG;
class MSFloorQT;
class MSPointMemberG;
class MSOpening;

class StructureModel_CLASS MSAssembly : public MSContainer
{
public:
	MSAssembly();
	~MSAssembly(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSAssembly)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSAssembly* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
    vector<MSLevelZone*> ma_LevelZone;
	vector<MSSectionView*> ma_SectionView;
	GM2DRectangle m_SiteRect;

protected:
	MSBaseBuilding* mp_Building;

public:
	virtual void SetOwner(MSBaseBuilding* val){mp_Building = val; };
	virtual MSBaseBuilding* GetOwner(){return mp_Building; };
	virtual MSBaseBuilding* GetBuilding(){return mp_Building;};

	virtual void Add(MSCompoG* pCompoG, bool bChangeOwner = false, bool bUpdateTrimmedBoundary = true);
	virtual bool Remove(MSCompoG* pCompoG);
	virtual bool Delete(MSCompoG* pCompoG);
	// 	virtual long Modify( MSCompoG* pCompoG );

	virtual void Add(MSLevelZone* pZone, bool isDoit = true);
	virtual bool Remove(MSLevelZone* pZone);
	virtual void Add(MSSectionView* pZone, bool isDoit = true);
	virtual bool Remove(MSSectionView* pZone);
//  	virtual bool Delete(MSLevelZone* pZone);
    //virtual void Add(MSOpening* pOpening);
    //virtual bool Remove(MSOpening* pOpening);

	virtual bool IsAssembly() {return true;};

	void DeleteCompoGs();
	void DeleteLevelZones();
	vector<MSLevelZone*> GetLevelZone(double Xc, double Yc);
	vector<MSLevelZone*> GetLevelZone(GM2DCurve* pCurve);
	vector<MSLevelZone*> GetLevelZone(GM2DPolyline* pPolyline);
	vector<MSLevelZone*> GetLevelZonebyCompoG(MSCompoG* pCompoG);

	double GetLevel4LevelZone(double Xc, double Yc, bool isMax = true);
	double GetLevel4LevelZone(GM2DCurve* pCurve, bool isMax = true);
	double GetLevel4LevelZone(MSLinkMemberG* pLinkG, bool isMax = true);
	double GetLevel4LevelZone(MSPointMemberG* pPointG);
	


	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
private:
	
// 	virtual long MergeCrossMembers(MSLinkMemberG* pLink);  // 0 : 성공, 1 : 부재 겹침
    virtual long MergeCrossMembersStep1(MSCompoG* pMyCompoG) {return 0;};
    virtual long MergeCrossMembers_Column(MSPointMemberG* pMemberG) {return 0;};
	static long LevelOverlap(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrLink);
	void AddCrossMemberError(MSLinkMemberG* pMyLink, MSLinkMemberG* pUrMem, XErrorObject* pMsgObject, CString strGroup, CString& strMsg);

	//virtual BOOL FindConnectivity(MSLinkMemberG* pMem, double dNodeTol);
	void DeleteMemPartArr();
	// 	virtual vector<long> GetCompoGs(long nMID);
};

