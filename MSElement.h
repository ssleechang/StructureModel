#pragma once
#include "msobject.h"
#include "StructureModelEnum.h"
#include "GMLib/GM2DVector.h"
#include "GMLib/GM2DLineVector.h"

//class ifcLocalPlacement;// ironbell
class MSBaseBuilding;
class MSFloorQT;
class MSPolylineObject;
class GM2DPolyline;
class MSFloor;

class StructureModel_CLASS MSElement : public MSObject
{
public:
	MSElement();
	~MSElement(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSElement)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);
	virtual CString ToString();

	long m_ID;
	static long ms_LastID;
	
	// 주변부재 업데이트 할지 말지 결정할때 사용됨 현재 슬래브의 오프닝 입력시 사용
	// false이면 업데이트 리스트에 넣지 않는다.
	bool m_isCalculateUpdatedCompoGArray = true;
	
	long GetContainingElementId() { return m_nContainingElementId;};
	void SetContainingElementId(long nID) { m_nContainingElementId = nID; };
	MSElement* GetContainingElement();

	// 서브 부재가 있는 경우 서브 부재도 같이 리셋해줘야 한다.
	virtual long ResetGetID();
	virtual long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	CTypedPtrArray<CObArray, MSFloorQT*> ma_CrossQT;
	void AddFloorQT(MSFloorQT* pQT);

	MSPolylineObject* mp_PolylineInfo;
	
	// 폴리 부재의 바운더리 반환
	virtual GM2DPolyline* GetProfile();
	virtual void SetProfile(GM2DPolyline* pPoly);
	void DelDuplicatedPointOfProfile();

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual bool IsElement() { return true; };
	virtual MSFloor* GetFloor(){return NULL;};
	bool IsCompoG();

	MSFloor* GetFloorForUser();
	CString GetFloorNameForUser();
	virtual int GetFloorNumForUser();
	virtual int GetFloorNum();

	virtual void ReMakeLevelDif(bool bMakeSlab = true){return; };
	virtual CString GetKey(bool bUseID = true) { return _T(""); };
	enum Type_MSCompoG {
		msNULL = 0, msBuilding = 1, msFloor, msBeamG, msBraceG, msWallG, msColumnG, msIsoFootG, msRampG, msSlabG, msStairG, msPitG,
		msWallFootG, msProject, msSector, msBeamLine, msSlabLineG, msLineLevelDifG, msPointLevelDifG, msHandRailG, msWallJointG, msLevelZone, msOpening,
		msWindowG, msDoorG, /*msWallOpeningG,*/ msStairPart = 26, msSlabRebar, msWell, msTrench, msOpeningG, msWallOpeningExtraG, msConstructionZone, msSubHaunchG, msSectionView,
		msStoryMaterial, msMarkG,
		msBuiltUpBeam = 40, msWallColumnG,
		msAddRebarAreaLine = 70, msAddRebarAreaBent, msAddRebarAreaShape,
		msHaunchG = 80,
		msArea,
		msPile = 82,
		msFormWorkDesignUnit = 1000,
		msFWSurfaceFoldingG = 3000, msFWShadeG, msFWSwitchBoxG, msFWSoundAbsorptionG,
	};

public:

	virtual Type_MSCompoG GetType(){return msNULL;};
	FindType_MSElement GetFindType();
	bool IsSameFindType(MSElement* pElement);
	bool IsSameFindType(FindType_MSElement eType);
	bool IsLineSupportMember();
	bool IsWallMember();
	bool IsVerticalMember();
	bool IsPointMember();
	bool IsLinkMember();
	bool IsPolyMember();
    bool IsFloorMember();
	bool IsSubMember();
	bool IsLowWall();
	bool IsWallOpeningG();

	static bool IsSameFindType(FindType_MSElement eType1, FindType_MSElement eType2);
	static bool IsLineSupportMember(Type_MSCompoG eType);
	static bool IsWallMember(Type_MSCompoG eType);
	static bool IsVerticalMember(Type_MSCompoG eType);
	static bool IsPointMember(Type_MSCompoG eType);
	static bool IsVerticalLineMember(Type_MSCompoG eType);
	static bool IsLinkMember(Type_MSCompoG eType);
	static bool IsLowWall(Type_MSCompoG eType);
	static bool IsPolyMember(Type_MSCompoG eType);
	static bool IsWallOpeningG(Type_MSCompoG eType);
	static bool IsFloorMember(Type_MSCompoG eType);
	static bool IsSubMember(Type_MSCompoG eType);
	static bool CompareElementPriority(MSElement* pMyElement, MSElement* pUrElement);

	virtual long GetPriorityNum() {return 0;};
	virtual bool GetIsTempCompoG() {return false;};
	virtual bool CheckData();

	static bool IsCompoG(Type_MSCompoG eType);
	static CString GetGroupName(Type_MSCompoG eType);
	virtual CString GetGroupName();

	virtual bool IsAssembly() {return false;};
	virtual bool IsFloor() {return false;};
	virtual bool IsBearingWall() {return false;};
	virtual bool IsRetainingWall() {return false;};
	virtual bool IsMatSlab() {return false;};
	virtual bool IsFlatSlab() {return false;};
	virtual bool IsDeckSlab() {return false;};
	virtual bool IsRebarEmbeddedDeckSlab() { return false; };
	virtual bool IsGeneralDeckSlab() { return false; }
	virtual bool IsSOG() { return false; };
	virtual bool IsCircularRamp() { return false; };
	virtual bool IsButtress(){return false;};
	virtual bool IsPC(){return false;};
	virtual bool IsHalfPCSlab(){return false;};
	virtual bool IsHCSPCSlab() { return false; };
	virtual bool IsRPSPCSlab() { return false; };
	virtual bool IsIMTPCSlab(){return false;};
	virtual bool IsDropPanel(){ return false; };

#pragma region 전이 부재
	virtual bool IsTransfer() { return false; };
#pragma endregion

	virtual bool IsDeck() { return false; };
	virtual bool IsCustomM() { return false; };
	virtual bool IsGeneral();

	virtual bool VisibleInRect( GM2DRectangle* pRect) {return false;};

	virtual MSBaseBuilding* GetBuilding(){return NULL;};

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	virtual MSElement* GetOwnerCompoG(){return NULL;};
	virtual void SetOwnerCompoG(MSElement*) { return; };
	virtual bool isXDirection(){ return true; };
	virtual void ModifyObject(bool isDoit = true);

	virtual void InitializeParameter();

	void RemoveOpeningInfoHasMadeByCompoGID();
	void RemoveOpeningInfoAll();
	void RemoveOpeningInfo(long nCompoGID);

	// Formwork의 서브부재(Slab:Trench, Opening, SoundAbsorption, Wall::Opening, Shade, SwitchBox)만 구해놓았다
	// 1. 서브부재가 추가되면 해당 함수 수정
	// 2. 다른 객체들도 사용하려면 추가 구현해야 한다.
	virtual bool RemoveSubMember(MSElement* element);

	CString GetNote() {	return m_Note;};
	void SetNote(CString note);

	bool GetDummy() { return m_IsDummy; };
	virtual void SetDummy(bool IsDummy) { m_IsDummy = IsDummy; };

	virtual vector<MSElement*> GetSubMembers() { vector<MSElement*> subList; return subList; }

	virtual void FixProfile() {};
	
	// Serialize이후 객체정보를 수정해야할 필요가 있는 경우 사용
	virtual void FixCompoGInfoAfterSerialize() {};
private:
	CString m_Note;
private:
		long m_nContainingElementId;

protected:
		bool m_IsDummy;
};

