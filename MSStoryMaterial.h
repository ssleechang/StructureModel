#pragma once
#include "MSElement.h"
#include "MSMaterial.h"

class StructureModel_CLASS MSStoryMaterial : public MSElement
{
public:
	MSStoryMaterial(void);
	~MSStoryMaterial(void);

	virtual Type_MSCompoG GetType(){return msStoryMaterial;};
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSStoryMaterial)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	MSBaseBuilding* GetBuilding()					{ return m_pBldg;};
	int GetFloorNum()									{ return m_FloorNum;};
	int GetRcHorizontalMaterialID()		{ return m_RcHMaterialID;};
	int GetRcVerticalMaterialID()				{ return m_RcVMaterialID;};
	int GetSteelHorizontalMaterialID()	{ return m_SteelHMaterialID;};
	int GetSteelVerticalMaterialID()			{ return m_SteelVMaterialID;};
	int GetSrcHorizontalMaterialID()		{ return m_SrcHMaterialID;};
	int GetSrcVerticalMaterialID()				{ return m_SrcVMaterialID;};

	void SetBuilding(MSBaseBuilding* pBldg)				{ m_pBldg = pBldg;};
	void SetFloorNum(int value)								{  m_FloorNum = value;};
	void SetRcHorizontalMaterialID(int value)		{ m_RcHMaterialID = value;};
	void SetRcVerticalMaterialID(int value)			{ m_RcVMaterialID = value;};
	void SetSteelHorizontalMaterialID(int value)	{ m_SteelHMaterialID = value;};
	void SetSteelVerticalMaterialID(int value)		{ m_SteelVMaterialID = value;};
	void SetSrcHorizontalMaterialID(int value)		{ m_SrcHMaterialID = value;};
	void SetSrcVerticalMaterialID(int value)			{ m_SrcVMaterialID = value;};

	MSMaterial* GetMaterial(MSMaterial::Type_MSMaterial matType, bool IsVerticalMember);
	int GetRcMaterialID(bool IsVerticalMember);
	int GetSteelMaterialID(bool IsVerticalMember);
	int GetSrcMaterialID(bool IsVerticalMember);

	bool IsUsedMaterial(int materialID);
protected:
	MSBaseBuilding* m_pBldg;
	int m_FloorNum;

	int m_RcHMaterialID;
	int m_RcVMaterialID;
	int m_SteelHMaterialID;
	int m_SteelVMaterialID;
	int m_SrcHMaterialID;
	int m_SrcVMaterialID;
};

