#pragma once
#include "MRObject.h"
class MRHBarPoint :	public MRObject
{
public:
	MRHBarPoint(void);
	~MRHBarPoint(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MRHBarPoint)

	int m_BarSide; // 1 : 콩을 원쪽으로 돈다.};

	//virtual GM2DVector& GetLocation();
};
