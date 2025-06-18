#pragma once
#include "msobject.h"

#include "GMLib/GMPolyline.h"

class StructureModel_CLASS MSPolySection :
	public MSObject
{
public:
	MSPolySection(void);
	~MSPolySection(void);

	GMPolyline m_SecPGon;
	void GetSecPolygon(GMPolyline& APGon);
	void SetSecPolygon(GMPolyline& APGon);
};

