#pragma once
#include "MSBaseBuilding.h"
#include "MSFormWorkMaterial.h"

class MSFormWorkDesignUnit;
class StructureModel_CLASS MSFormWorkBuilding : public MSBaseBuilding
{
public:
	MSFormWorkBuilding(void);
	~MSFormWorkBuilding(void);
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSFormWorkBuilding)

#pragma region override
	virtual bool WriteFileBinary() override;
	virtual bool WriteFileBinary(CString strFilePath) override;
	virtual bool ReadFileBinary(CString strFilePath) override;

	virtual MSCompoM* CreateNewCompoM(long nType) override;
	virtual MSCompoG* GetCompoG(long CompoGID) override;
	virtual MSCompoG* GetCompoGByFloors(long CompoGID) override;
	
	virtual void ChangFloorQT(double dMinX, double dMinY, double dMaxX, double dMaxY) override;
	bool IsVerticalCompoM(long nType);
protected:
	void RecoverTypeUIDFactoryByMSObjectLastID() override;
#pragma endregion

#pragma region MSFormWorkDesignUnit
public:
	MSFormWorkDesignUnit* CreateDesignUnit(double dHeight = 2800);
	void DeleteDesignUnits();
	void Add(MSFormWorkDesignUnit* pDesignUnit);
	long Remove(MSFormWorkDesignUnit* pDesignUnit);
	MSFormWorkDesignUnit* GetDesignUnit(long designUnitID);
	vector<MSFormWorkDesignUnit*> GetDesignUnits();
#pragma endregion

#pragma region MSMaterial
	void DeleteMaterials();
	virtual MSMaterial* CreateNewMaterial(long nType) override;
	virtual MSMaterial* GetMaterial(long MaterialID) override;
	virtual MSMaterial* GetMaterial(CString strMateName) override;
	virtual MSMaterial* GetDefMaterial() override;
	virtual MSMaterial* GetDefMaterial(MSMaterial::Type_MSMaterial materialType) override;
	virtual MSMaterial* GetMaterialbyName(MSMaterial::Type_MSMaterial materialType, CString name) override;
	virtual vector<MSMaterial*> GetMaterialsByType(MSMaterial::Type_MSMaterial type);
	virtual MSMaterial* GetDefMaterialByFWMaterialType(MSFormWorkMaterial::eFWMaterialType eType);

	virtual map<long, MSMaterial*>* GetMaterialMap();
	virtual bool Add(MSMaterial* pMaterial) override;
	virtual long Remove(MSMaterial* pMaterial) override;
	virtual long Delete(MSMaterial* pMaterial) override;

	vector<MSMaterial*> GetMaterias();

#pragma endregion

#pragma region ShapeTemplate
	void DeleteShapeTemplates();
	void AddShapeTemplate(MSCompoM* shapeTemplate);
	virtual MSCompoM* GetShapeTemplate(long shapeTemplateID);
	vector<MSCompoM*> GetShapeTemplates();
	virtual MSCompoM* GetShapeTemplate(CString ShapeName);

	virtual map<long, MSCompoM*> GetCompoMMap() override { return mm_ShapeTemplate; };

	virtual bool Add(MSCompoM* pCompoM) override;
	virtual MSCompoM* GetCompoM(long CompoMID) override;
	virtual long Remove(MSCompoM* pCompoM) override;
#pragma endregion

#pragma region MSFloor
	virtual MSFloor* GetFloorByID(long FloorID) override;
	virtual double GetFloorLevel(long FloorNum) override;
	virtual double GetSlabLevel(long FloorNum) override;
	virtual vector<MSFloor*> GetAllFloors() override;
	virtual void ClearAllCashFloorLevel() override;
#pragma endregion

#pragma region MSCompoGs
	virtual vector<MSCompoG*> GetAllCompoGs();

#pragma endregion
public:
	map<long, MSMaterial*> mm_Material;

protected:
	map<long, MSFormWorkDesignUnit*> mm_DesignUnit;
	map<long, MSCompoM*> mm_ShapeTemplate;
};
