#include "StdAfx.h"
#include "DG2DObject.h"

DG2DObject::DG2DObject(void)
{
	m_DGID = -1;
	m_Tag = 0;
	ma_GMs.RemoveAll();
}

DG2DObject::~DG2DObject(void)
{
}
