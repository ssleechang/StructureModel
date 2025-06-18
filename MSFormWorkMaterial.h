#pragma once
#include "MSMaterial.h"
class StructureModel_CLASS MSFormWorkMaterial :public MSMaterial
{
public:
	MSFormWorkMaterial();
	~MSFormWorkMaterial();
	void Serialize(CArchive& archive);
	virtual void GlobalOptionSerialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkMaterial)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	enum eFWMaterialType{ Concrete = 0, PrecastConcrete, Brick, Wood};

	eFWMaterialType GetFWMaterialType() { return m_eType; };
	void SetFWMaterialType(eFWMaterialType eType) { m_eType = eType; };
protected:
	eFWMaterialType m_eType;
};

