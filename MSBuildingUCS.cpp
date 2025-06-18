#include "stdafx.h"
#include "MSBuildingUCS.h"

IMPLEMENT_SERIAL(MSBuildingUCS, MSObject, VERSIONABLE_SCHEMA | 1)
MSBuildingUCS::MSBuildingUCS()
{
	m_Origin.SetXY(0, 0);
	m_xDir.SetXY(1, 0);
	m_yDir.SetXY(0, 1);
	m_UCSz = 0.0;
}


MSBuildingUCS::~MSBuildingUCS()
{
}

void MSBuildingUCS::Serialize(CArchive& ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		m_Origin.Serialize(ar);
		m_xDir.Serialize(ar);
		m_yDir.Serialize(ar);
	}
	else
	{
		m_Origin.Serialize(ar);
		m_xDir.Serialize(ar);
		m_yDir.Serialize(ar);
	}
}

void MSBuildingUCS::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	m_Origin.Dump(buffer);
	m_xDir.Dump(buffer);
	m_yDir.Dump(buffer);
}

void MSBuildingUCS::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);
	m_Origin.Recovery(buffer);
	m_xDir.Recovery(buffer);
	m_yDir.Recovery(buffer);
}

void MSBuildingUCS::SetUCS(GM2DVector originPoint, GM2DVector xDir, GM2DVector yDir)
{
	m_Origin = originPoint;
	m_xDir = xDir;
	m_yDir = yDir;
}

void MSBuildingUCS::SetUCSz(double z)
{
	m_UCSz = z;
}
