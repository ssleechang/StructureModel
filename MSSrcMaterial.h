#pragma once
#include "msmaterial.h"

class MSConcrete;
class MSSteel;
class StructureModel_CLASS MSSrcMaterial : public MSMaterial
{
public:
	MSSrcMaterial(void);
	~MSSrcMaterial(void);
	void Serialize( CArchive& archive );
	virtual void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSSrcMaterial)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	MSConcrete* mp_RcMaterial;
	MSSteel*         mp_SteelMaterial;

	double GetFy();
	void SetFy(double fy);
	void SetBuildingMaterialBySerializeID();
	void SetGlobalOptionMaterialBySerializeID();

	virtual double GetFck();
	virtual CString GetFckName();
	virtual double GetConcStrength();
	virtual MSReinforceBarSetting* GetReinforceBarSetting();
	virtual bool IsPlainConcreteReinforceBar() override;

	long GetSerializeRcID();
	long GetSerializeSteelID();
	void SetSerializeRcID(long RcID);
	void SetSerializeSteelID(long SteelID);
private:
	long m_nSerializeRcID;
	long m_nSerializeSteelID;
};

