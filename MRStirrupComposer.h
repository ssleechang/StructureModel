#pragma once
#include "MRObject.h"
class MRHBarByEdge;
class MRStirrupComposer :
	public MRObject
{
public:
	MRStirrupComposer(void);
	~MRStirrupComposer(void);

	static CTypedPtrArray<CObArray, MRStirrupComposer*> sm_StirrupComposer;


	bool m_bFullStirrUp;
	int m_StartAnchorType;
	int m_EndAnchorType;

	CTypedPtrArray<CObArray, MRHBarByEdge*> ma_StirrupBar;

};
