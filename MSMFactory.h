#pragma once
#include "MSCompoG.h"
class MSCompoM;
class MSRebarBuilding;
class StructureModel_CLASS MSMFactory
{
public:
	
	static MSMFactory*  GetInstance();
	static void SetFactory(MSMFactory* );
	virtual MSCompoM* GetMInstance(MSElement::Type_MSCompoG type)=0;
	virtual MSRebarBuilding* GetBuildingInstance()=0;
protected:
	static MSMFactory * m_pFactory;
	MSMFactory(void);
	~MSMFactory(void);
};

