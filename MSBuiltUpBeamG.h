#pragma once
#include "MSLinkMemberG.h"

#include "MSMainBarSet.h"
#include "MSStirrupBar.h"
#include "MSMaterial.h"
class MSBeamG;
class StructureModel_CLASS MSBuiltUpBeamG : public MSLinkMemberG
{
public:
	MSBuiltUpBeamG(void);
	~MSBuiltUpBeamG(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBuiltUpBeamG)

	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSBuiltUpBeamG* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual Type_MSCompoG GetType(){return msBuiltUpBeam;};
	
	virtual double GetWidth();
	virtual double GetDepth();
	virtual double GetThick();
	virtual double GetLength();
	virtual void SetLines();
	virtual void UpdateBoundaryVisibilities(long nUpdateConnIdx = 0);
	virtual bool GetZLevels(double& TopLevel, double& BottomLevel, bool GetBigVal, bool bCheckLevelZone);
	virtual bool GetZLevelsbyLoc(double& TopLevel, double& BottomLevel, double dLoc);

	void SyncLocationVec2Loc();
	void SyncLocationLoc2Vec();

	enum eMainBarPlace_Type { MainBarPlace_Type_None = 0, MainBarPlace_Type_User };
	enum eStirrupBarPlaceInfo_Type { StirrupBarPlaceInfo_Type_Owner = 0, StirrupBarPlaceInfo_Type_None, StirrupBarPlaceInfo_Type_User};
	enum eStirrupBarPlaceType { StirrupBarPlaceType_Auto = 0, StirrupBarPlaceType_Type1, StirrupBarPlaceType_Type2 };

	bool  GetIsTop()												{ return m_IsTop;};
	virtual double GetHeight()											{ return m_StartHeight; };
	double GetStartLocation()							{ return m_StartLocation;};
	double GetEndLocation()								{ return m_EndLocation;};
	eMainBarPlace_Type GetMainBarPlaceType()		{ return m_MainBarPlaceType;};
	eMainBarPlace_Type GetSideBarPlaceType()		{ return m_SideBarPlaceType; };
	eStirrupBarPlaceInfo_Type GetStirrupBarPlaceInfoType()		{ return m_StirrupBarPlaceInfoType;};
	eStirrupBarPlaceType GetStirrupBarPlaceType() { return m_StirrupBarPlaceType; };

	long CalcStirrupBarPlaceType();

	virtual void SetOwnerCompoG(MSElement* pOwner)		{ mp_OwnerCompoG = (MSBeamG*)pOwner; };
	virtual MSElement* GetOwnerCompoG(){ return (MSElement*)mp_OwnerCompoG; };
	void SetIsTop(bool isTop)								{ m_IsTop = isTop;};
	void SetHeight(int height)							{ m_StartHeight = height;m_EndHeight = height;};
	void SetStartLocation(double loc);
	void SetEndLocation(double loc);
	void SetIsSameStartLocation(bool bValue);
	void SetIsSameEndLocation(bool bValue);
	bool GetIsSameStartLocation() {return m_IsSameStartLocation;};
	bool GetIsSameEndLocation() {return m_IsSameEndLocation;};

	void SetMainBarPlaceType(int type)			{ m_MainBarPlaceType = (eMainBarPlace_Type)type;};
	void SetSideBarPlaceType(int type)			{ m_SideBarPlaceType = (eMainBarPlace_Type)type; };
	void SetStirrupBarPlaceInfoType(int type)		{ m_StirrupBarPlaceInfoType = (eStirrupBarPlaceInfo_Type)type;};
	void SetStirrupBarPlaceType(int type) { m_StirrupBarPlaceType = (eStirrupBarPlaceType)type; };

	bool IsContainLoc(double dLoc, double dTol = DTOL_GM);

	virtual MSMaterial* GetMaterial();
	CString GetMaterialName();
	void SetMaterialName(MSMaterial::Type_MSMaterial materialType, CString sName);
	long GetBuiltUpBeamMaterialID();
	void SetBuiltUpBeamMaterialID(long materialID)		{ m_nBuiltUpBeamMaterialID = materialID; };
	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial; };
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial; };

	void SyncStartLocationVec2Loc();
	void SyncEndLocationVec2Loc();

	void SyncStartLocationLoc2Vec();
	void SyncEndLocationLoc2Vec();

	void SetLength(double dLen);
	void SetOffSetInfo();
	bool IsOnBuiltUpBeamPnt(GM2DVector& Vec);
	MSMainBarSet m_MainBarSet;
	MSStirrupBar m_StirrupBar;

	bool m_bIsNumPlaceSideBar;
	MSMainBar m_SideBars;
	MSStirrupBar m_SideStirrupBar;
private:
	MSBeamG* mp_OwnerCompoG;
	bool m_IsTop;	// loctaion
	int    m_StartHeight;
	int    m_EndHeight;

	// 입력된 BuiltUpBeam의 길이를 저장
	double m_dLength;
	double m_dStartOffSet;
	double m_dEndOffSet;
	double m_StartLocation;
	double m_EndLocation;

	// Beam의 시작 끝 Loc 값이 동일한지..
	bool m_IsSameStartLocation;
	bool m_IsSameEndLocation;

	eMainBarPlace_Type m_MainBarPlaceType;
	eMainBarPlace_Type m_SideBarPlaceType;
	eStirrupBarPlaceInfo_Type m_StirrupBarPlaceInfoType;
	eStirrupBarPlaceType m_StirrupBarPlaceType;

	int m_nBuiltUpBeamMaterialID;
	bool m_IsUsedStoryMaterial;
};

