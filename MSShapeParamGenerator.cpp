#include "stdafx.h"
#include "MSShapeParamGenerator.h"

#include "MSRebarShape.h"

#include "MSShapeParams.h"
#include "MSShapeLengthParams.h"
#include "MSShapeAngleParams.h"

MSShapeParamGenerator::MSShapeParamGenerator()
{
}


MSShapeParamGenerator::~MSShapeParamGenerator()
{
}

MSShapeParams* MSShapeParamGenerator::GeneratorShapeParms(int GCode, int SCode)
{
	MSShapeParams* pParams = nullptr;
	MSREBAR_SHAPECODE fullCode = (MSREBAR_SHAPECODE)(GCode * 1000 + SCode);

	switch (fullCode)
	{
		case MSREBAR_SHAPECODE::MST1_09:
		{
			pParams = G1S9();
			break;
		}
		default:
		{
			ASSERT(0);
			break;
		}
	}
	return pParams;
}

MSShapeParams* MSShapeParamGenerator::G1S9()
{
	MSShapeParams* pParams = new MSShapeParams();

	MSShapeLengthParams* AParam = new MSShapeLengthParams();
	AParam->SetParamType(MSShapeLengthParams::Hook);
	AParam->SetUserLength(100);
	pParams->Add(AParam);

	MSShapeLengthParams* BParam = new MSShapeLengthParams();
	BParam->SetParamType(MSShapeLengthParams::Constraint);
	BParam->SetUserLength(200);
	pParams->Add(BParam);

	MSShapeLengthParams* CParam = new MSShapeLengthParams();
	CParam->SetParamType(MSShapeLengthParams::Hook);
	CParam->SetUserLength(100);
	pParams->Add(CParam);

	MSShapeAngleParams* R1 = new MSShapeAngleParams();
	R1->SetAngleType(MSShapeAngleParams::Angle135);
	R1->SetUserAngle(100);
	pParams->Add(R1);

	return pParams;
}