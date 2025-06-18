#include "StdAfx.h"
#include "MSSteelRoundPipeKr.h"
#include "GMLib/GM2DVector.h"
#include "MSSteelUtility.h"

MSSteelRoundPipeKr::MSSteelRoundPipeKr(void)
{
	Init();
}
void MSSteelRoundPipeKr::Init()
{
	m_D = 0.0;
	m_Thk = 0.0;
	m_nSecType = SECTION_ROUND_TYPE;
}
MSSteelRoundPipeKr::~MSSteelRoundPipeKr(void)
{
}

void MSSteelRoundPipeKr::GetSecPolyline(GM2DPolyline& TheGon)
{
	int resolution = 24;
	GM2DVector Pnt;
	GMCircle SecCircle(GetD() / 2., 0., 0.);
	double RadStep = 2.0*PI / resolution;
	for (long PntNum = 0; PntNum<resolution; PntNum++)
	{
		SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
		TheGon.AddCopy(Pnt);
	}

	GM2DPolyline* inner = new GM2DPolyline(TRUE);
	{
		GMCircle SecCircle(GetD() / 2. - GetFThk(), 0., 0.);
		double RadStep = 2.0*PI / resolution;
		for (long PntNum = 0; PntNum<resolution; PntNum++)
		{
			SecCircle.GetGlobalXY(PntNum*RadStep, Pnt.m_X, Pnt.m_Y);
			inner->AddCopy(Pnt);
		}
		inner->MakeClosedPolyline(DTOL_GM);
		inner->MakeCCW();
		TheGon.ma_InnerOpening.push_back(inner);
	}

	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
	// Circle Info
	//TheGon.m_bIsCircle = true;
	//TheGon.m_dCircleDia = GetD();
	//TheGon.m_dCircleDiaInner = GetD() - 2.0 * GetFThk();
}

