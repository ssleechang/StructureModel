#pragma once

#include "msobject.h"
#include "MSMaterial.h"
#include "MDMember.h"

#include "MSPolySection.h"

class MSBaseBuilding;
class MRCompoM;
class MSSteelSection;

class StructureModel_CLASS MSCompoM :
	public MSObject
{
public:
	MSCompoM(void);
	~MSCompoM(void);

	CString m_Name;
	

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시
    void SetName(CString name);
    CString GetName();
	virtual CString ToString();

	enum Type_MSCompoM {msNONE = 0, msBeamM=100, msBraceM, msBWallM, msColumnM, msIsoFootM, msRampM, msRWallM, msSlabM=107, /*msStairM*/ 
													 msWallFootM = 109, msPitM, msLevelDifM, msHandRailM, msWindowM, msDoorM, msWallOpeningM, 
													 msStairLandingM, msStairStepM, msButtressM, msPcBeamM, msOpeningReinforceM, msOpeningExtraM, 
													 msPcColumnM, msHaunchM,
													 msColumnHeaderM = 150,
													 msSteelBeamM = 200, msSteelColumnM,
													 msSrcBeamM = 300, msSrcColumnM,
													 msCustomColumnM = 1000, msCustomWallColumnM, msCustomBeamM, msCustomButtressM,
													 msFWOpeningM = 3000, msFWShadeM, msFWSwitchBoxM, msFWSoundAbsorptionM, msFWSurfaceFoldingM,
													 msFWTrenchM, msFWBeamM, msFWColumnM, msFWBWallM, msFWSlabM,
												};
	// Modify Together : MSLib_Wrapper.cpp, MSCompoMCli.h
	Type_MSCompoM m_nType;

	MSBaseBuilding* mp_Owner;
	long m_nFromStory;
	long m_nToStory;
	BOOL m_bDummy;	// 20121031추가
	CTime m_Inserted_date;

	MRCompoM* mp_MRM;
	MDMember* mp_RebarM;

	MSMaterial* GetMaterial();
	void SetMaterial(MSMaterial* pMaterial)	{mp_Material = pMaterial;};

	bool GetIsUsedStoryMaterial()												{ return m_IsUsedStoryMaterial;};
	void SetIsUsedStoryMaterial(bool bUsedStoryMaterial)	{ m_IsUsedStoryMaterial = bUsedStoryMaterial;};
	MSMaterial::Type_MSMaterial GetMaterialType();
	long m_nClassificationID;
	CString m_ClassificationName;

	//FromWork용 함수 - start
//	virtual CString GetShapeName() { return GetName(); };
	static bool IsMainCompoM(Type_MSCompoM type);
	virtual CString GetShapeName() { return m_ShapeName; };
	void SetShapeName(CString shapeName) { m_ShapeName = shapeName; };
	//FromWork용 함수 - end

private:
	MSMaterial* mp_Material;
	bool m_IsUsedStoryMaterial;

	//FromWork용 함수
	CString m_ShapeName;
public:
	// 20130903 추가 - by Gracias
	CTime GetInsertedDate();
	void SetInsertedDate(CTime& time);

	void SetOwner(MSBaseBuilding* pOwner){mp_Owner = pOwner;};

	Type_MSCompoM GetType(){return m_nType;};

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSCompoM)

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
// end1309
public:
	static MDMember* CreateRebarM(Type_MSCompoM nType);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual bool IsGeometryDifferent(MSCompoM* compoM);
	virtual bool GetBotSpacerType(){return false;}
	virtual SpacerShape * GetTopSpacerShape(){return NULL;}
	virtual SpacerShape * GetBotSpacerShape(){return NULL;}
	virtual void InitValue(){};
	virtual double GetWidth() {return 0.;};
	virtual double GetDepth() {return 0.;};//by LSS
	virtual double GetThick(){return 0;};//by LSS
    virtual double GetHeight() {return 0;};

	virtual void SetWidth(double dWidth) {};
	virtual void SetDepth(double dDepth) {};
	virtual void SetThick(double dThick) {};
	virtual void SetHeight(double dHeight) {};

	virtual double GetPcWidth()	{ return 0; };
	virtual double GetPcDepth()	{ return 0; };
	virtual double GetPcThick()	{ return 0; };
	virtual int GetHandRailType(){return 0;};//by LSS only for Handrail
	virtual void GetSecPolyline(GM2DPolyline& APGon) {return;};
	virtual void GetHeaderPolyline(GM2DPolyline& APGon) { return; };
	virtual vector<GM2DPolyline> GetSubHeaderPolyline() { vector<GM2DPolyline> subProfile; return subProfile; };
	virtual double GetParapetWidth(){return 0;}//by LSS
	virtual int OnProperty(CWnd * pParent = NULL){return IDCANCEL;};
	virtual double GetCover(){return 0;};
	virtual bool IsPC(){return FALSE;};
	virtual bool IsRC();
	virtual bool IsSteel(){ return FALSE; };
	virtual bool IsSRC(){ return FALSE; };
	virtual bool IsStoryIncluded(long nStory);
	virtual bool IsVerticalMember();
	virtual bool IsCustomM() { return false; };
	virtual bool IsPolygonSection() { return false; };
	virtual MSSteelSection* GetSteelSection() { return NULL; };

	virtual void SetCover(double dCover){};

	virtual MDMember* GetRebarM();
	virtual MSBaseBuilding* GetBuilding() { return mp_Owner; };

// ck130423
	MSPolySection* mp_PSec;

	static bool IsExistedSpacialCharacters(CString name);
	static CString ReplaceSpacialCharactersName(CString name, char replacer);

	bool m_IsDefault;

	virtual CString GetClassificationName();
//	void SetClassificationName(CString value);
	void SetClassificationName(long nId);

	virtual bool CheckData();

#pragma region 전이 부재
public:
	virtual bool IsTransfer();
#pragma endregion
};
 
#pragma make_public(MSCompoM)