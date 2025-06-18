#pragma once
#include "MSElement.h"
#include "GMLib\GM2DRectangle.h"

class MSOpening;
class GM2DVector;
class MSFloor;
class MSCompoG;
class MSWell;
class MSSlabG;

class StructureModel_CLASS MSPolylineObject : public MSElement
{
public:
	MSPolylineObject();
	virtual ~MSPolylineObject(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSPolylineObject)

	virtual MSElement* Copy(bool bNewID = true);
	void CopyFromMe(MSPolylineObject* pSource, bool bNewID=true);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	long Contains(double Xc, double Yc, double tolerance);
	long Contains(GM2DVector* pVec, double tolerance);
	bool Contains(GM2DCurve* pCurve, bool bIncludeOnLine, double tolerance);
	bool Contains(GM2DPolyline* pPoly, bool bIncludeOnLine, double tolerance);
	
	virtual bool OverLap(GM2DCurve* pCurve, double tolerance);
	virtual GM2DPolyline* GetProfile();
	virtual GM2DPolyline* GetTrimmedBoundary(bool isTop = true);
	void SetTrimmedBoundary(GM2DPolyline* pPoly, bool isTop = true);
	void InitTrimmedBoundary(bool isTop);

	void SetProfile(GM2DPolyline* pPoly);
    GM2DPolyline* mp_Profile;
	MSElement* mp_Owner; // MSSlabG, MSLevelZone 이 생성되면서 셋팅해줌
	BOOL m_isAddPoly;

// 	vector<MSPolylineObject*> ma_PolylineObject;
    vector<MSOpening*> ma_Opening;
	vector<GM2DPolyline*> GetMergeOpeningProfiles();

	void SetOwner(MSElement* pOwner);
	MSElement* GetOwner(){return mp_Owner;};
	Type_MSCompoG GetOwnerType();

	bool LevelOverlap(MSPolylineObject* pUrPolylineObject, bool isTop);
	
	bool AddOpening(MSOpening* opeing);
	bool AddOpening(GM2DPolyline* pPoly, MSElement* pElement = NULL, bool isContain = true);
	bool RemoveOpening(MSOpening* pDelOpening);
	bool CheckInValidOpening();

	void TrimOpenings();

	MSFloor* GetFloor();
	static bool AddOpeningInEdgeSlab(MSSlabG* pMySlabG, MSSlabG* pUrSlabG);
	void TrimExistPolyLineObject(bool isTop = true);
	static bool TrimExistPolyLineObjectSetp2(MSPolylineObject* pCurPolylineObject, MSPolylineObject* pPolylineObject, bool isTop, bool bUpdatePartner);

	vector<MSPolylineObject*> ma_UpdateMSPolylineObject;

    virtual GM2DVector* InsertVec(long Index, GM2DVector* pVec);
    virtual void ReplaceVec( long Index, GM2DVector* pVec);
    virtual GM2DVector* GetVec(long Index);
    void MergeOpenings();
    void MergeOpenings(MSOpening* openA, MSOpening* openB);
	void RemoveSameOpening();

	void ReMakeLevelDif(bool bMakeSlab = true);
	void DeleteOpenings();
	void DeleteOpeningsByMadeByCompoG();

	MSOpening* FindOpening_MadeByCompoG(int nID);

	
	inline void CalcBoundRectBeforeAddQuadTree();
	bool VisibleInRect( GM2DRectangle* pRect);

	GM2DRectangle m_RectForQuadTree;
private:
    
};

