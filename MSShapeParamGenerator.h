#pragma once
#include "MSObject.h"
class MSShapeParams;
class StructureModel_CLASS MSShapeParamGenerator :public MSObject
{
public:
	MSShapeParamGenerator();
	~MSShapeParamGenerator();

	static MSShapeParams* GeneratorShapeParms(int GCode, int SCode);

private:
	static MSShapeParams* G1S9();
};

