#pragma once
//#include "SCFMCPEnum.h"

#include "MSElement.h"
#include "MSCompoM.h"

class MSMaterial;
class MSGridLine;
class XMSRowM;
class MSLinkMemberG;
class MSSlabG;
class MSGenStairG;	
class MSIsoFootG;
class MSFloor;
class MRSection;
class MSRebarBuilding;
class MSCompoG;
class MSLevelZone;
class MSConstructionZone;
class MSSectionView;
class MSOpening;
class MSWallOpeningG;
class MSBlock;
class MSSlabRebar;
class MSWell;
class MSTrench;
class MSBuiltUpBeamG;

typedef BOOL (*MSCondFunc)   (MSCompoG* pCompoG, void* ParamList);
typedef void (*MSActionFunc) (MSCompoG* pCompoG, void* ParamList);

class StructureModel_CLASS MSContainer :
	public MSElement
{
public:
	MSContainer();
	~MSContainer(void);
	void Serialize( CArchive& archive );
	bool m_bIsSerializing;
	DECLARE_SERIAL(MSContainer)
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSContainer* pSource, bool bNewID=true);
	virtual void Init(bool bInConstructor=false);

	CString GetName();
    void SetName(CString name);

	map<long, MSCompoG*> mm_CompoG;
	virtual void Add(MSCompoG* pCompoG, bool bChangeOwner = false, bool isDoit = true, bool bUpdateTrimmedBoundary = true);
	virtual bool Remove(MSCompoG* pCompoG);
	virtual bool Delete(MSCompoG* pCompoG);
	virtual bool Modify(MSCompoG* pOldCompoG, MSCompoG* pNewCompoG);
	static MSCompoG* CreateNewCompoG(long nType);
    virtual MSLevelZone* CreateNewLevelZone();
	virtual MSSectionView* CreateNewSectionView();
	virtual MSConstructionZone* CreateNewConstructionZone();
    virtual MSOpening* CreateNewOpening();
    virtual MSWallOpeningG* CreateNewWallOpening();
	virtual MSGridLine*    CreateNewGridLine();
	virtual MSBlock* CreateNewBlock();
    virtual MSSlabRebar* CreateNewSlabRebar();
	virtual MSWell* CreateNewWell();
	virtual MSTrench* CreateNewTrench();
	virtual MSBuiltUpBeamG* CreateNewBuiltUpBeamG();

	virtual void Add(vector<MSCompoG*> CompoGArr, bool bChangeOwner = false, bool isTrim = true) {};
	virtual bool Remove(vector<MSCompoG*> CompoGArr, bool isTrim = true) { return true; }

	virtual void ForEachCompoG(MSActionFunc Action, void* pParams);
	virtual void ChooseCompoGThat( MSCondFunc Test, MSContainer& ChoosedCompoGs, void* pParameters );
	virtual MSCompoG* FirstThat(MSCondFunc Test, void* PParamList);
	virtual MSCompoG* NextThat(MSCompoG* pCurCompoG, MSCondFunc Test, void* PParamList);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	vector<long> GetLinkedCompoGIDArr(long nGID);

protected:
	CString m_Name;
	map<long, vector<long>> mm_GIDToLinkedCompoGIDArr;
};
