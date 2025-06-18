#include "StdAfx.h"
#include "MSRebarShape.h"

#include <math.h>

IMPLEMENT_SERIAL(MSRebarShape, MSObject, VERSIONABLE_SCHEMA | 1)
long MSRebarShape::ms_LastID = 0;
MSRebarShape::MSRebarShape(long TheID)
{
 	m_ID = TheID;
	m_Vs = NULL;
	m_nGCode = 0;
	m_nSCode = 0;

	m_TotalLength = 0.0;
	m_Name = _T("");
	m_dDia = 0.0;		// 철근 지름		
	m_UpDown = FALSE;
	m_dDir = 0.0;
	m_bReverse = FALSE;
	m_nBarSetTableNum = 0;
	m_IsTieStrp = FALSE;
}

MSRebarShape::MSRebarShape(CString Name, double dDia, long IsTieStrp)
{
	m_TotalLength = 0.0;
// 	m_ID = 0;
	m_Name = Name;
	m_dDia = dDia;
	m_UpDown = 1;
	m_bReverse = FALSE;
	m_IsTieStrp = IsTieStrp;

	m_Pos.SetXYZ(0., 0.);
	m_dDir = 0.;
	m_nBarSetTableNum = 0;
}

MSRebarShape::~MSRebarShape()
{
	if(m_Vs)
		delete m_Vs;
}

MSObject* MSRebarShape::Copy(bool bNewID)
{
	MSRebarShape* pNewObj;

	pNewObj = new MSRebarShape();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSRebarShape::CopyFromMe(MSRebarShape* pSource, bool bNewID)
{
	MSObject::CopyFromMe(pSource);

	m_nGCode = pSource->m_nGCode;
	m_nSCode = pSource->m_nSCode;

	m_TotalLength = pSource->m_TotalLength;
	m_Name = pSource->m_Name;
	m_dDia = pSource->m_dDia;
	m_UpDown = pSource->m_UpDown;
	m_dDir = pSource->m_dDir;
	m_bReverse = pSource->m_bReverse;
	m_nBarSetTableNum = pSource->m_nBarSetTableNum;
	m_IsTieStrp = pSource->m_IsTieStrp;

	m_Pos.CopyFromMe(&pSource->m_Pos);

	if (m_Vs != nullptr)	delete m_Vs;

	int NumVs = s_MSNumVs[m_nGCode - 1][m_nSCode];
	m_Vs = new double[NumVs];
	for (long iV = 0; iV < NumVs; iV++)
	{
		*(m_Vs + iV) = *(pSource->m_Vs + iV);
	}
}

void MSRebarShape::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	buffer << m_nGCode << m_nSCode << m_TotalLength;
	buffer << m_Name << m_dDia << m_UpDown;
	buffer << m_dDir << m_bReverse << m_nBarSetTableNum << m_IsTieStrp;

	m_Pos.Dump(buffer);

	int NumVs = 0;
	if(m_Vs != nullptr)
		NumVs = s_MSNumVs[m_nGCode - 1][m_nSCode];

	buffer << NumVs;
	for (long iV = 0; iV < NumVs; iV++)
	{
		buffer << *(m_Vs + iV);
	}
}

void MSRebarShape::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	buffer >> m_nGCode >> m_nSCode >> m_TotalLength;
	buffer >> m_Name >> m_dDia >> m_UpDown;
	buffer >> m_dDir >> m_bReverse >> m_nBarSetTableNum >> m_IsTieStrp;

	m_Pos.Recovery(buffer);

	int NumVs = 0;
	buffer >> NumVs;
	m_Vs = new double[NumVs];

	for (long iV = 0; iV < NumVs; iV++)
	{
		buffer >> *(m_Vs + iV);
	}
}

// m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
long MSRebarShape::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		ms_LastID = m_ID;
		return m_ID;
	}

	return m_ID;
}

void MSRebarShape::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}


void MSRebarShape::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_nGCode;
		ar << m_nSCode;
		ar << m_Name;
		ar << m_dDia;
		ar << m_UpDown;
		ar << m_dDir;
		ar << m_bReverse;
		ar << m_IsTieStrp;

		long NumVs = s_MSNumVs[m_nGCode-1][m_nSCode];
		ar << NumVs;
		for(long iV = 0 ; iV < NumVs; iV++)
		{
			double dV = *(m_Vs+iV);
			ar << dV;
		}
		m_Pos.Serialize(ar);
	}
	else
	{
		ar >> m_nGCode;
		ar >> m_nSCode;
		ar >> m_Name;
		ar >> m_dDia;
		ar >> m_UpDown;
		ar >> m_dDir;
		ar >> m_bReverse;
		ar >> m_IsTieStrp;

		long NumVs;
		ar >> NumVs;
		m_Vs = new double[NumVs];
		for(long iV = 0 ; iV < NumVs; iV++)
		{
			ar >> *(m_Vs+iV);
		}
		m_Pos.Serialize(ar);
	}
}
long MSRebarShape::MakeBarShapeCode(long nGCode, long nSCode)
{
	switch(nGCode)
	{
	case 1:
		{
			switch(nSCode)
			{
			case  0:	return MST1_00;	case  1:	return MST1_01;	case  2:	return MST1_02;	case  3:	return MST1_03;	case  4:	return MST1_04;
			case  5:	return MST1_05;	case  6:	return MST1_06;	case  7:	return MST1_07;	case  8:	return MST1_08;	case  9:	return MST1_09;
			case 10:	return MST1_10;	case 11:	return MST1_11;	case 12:	return MST1_12;	case 13:	return MST1_13;	case 14:	return MST1_14;
			case 15:	return MST1_15;
			}
		}
	case 2:
		{
			switch(nSCode)
			{
			case  0:	return MST2_00;	case  1:	return MST2_01;	case  2:	return MST2_02;	case  3:	return MST2_03;	case  4:	return MST2_04;
			case  5:	return MST2_05;	case  6:	return MST2_06;	case  7:	return MST2_07;	case  8:	return MST2_08;	case  9:	return MST2_09;
			case 10:	return MST2_10;	case 11:	return MST2_11;	case 12:	return MST2_12;	case 13:	return MST2_13;	case 14:	return MST2_14;
			case 15:	return MST2_15;
			}
		}
	case 3:
		{
			switch(nSCode)
			{
			case  0:	return MST3_00;	case  1:	return MST3_01;	case  2:	return MST3_02;	case  3:	return MST3_03;	case  4:	return MST3_04;
			case  5:	return MST3_05;	case  6:	return MST3_06;	case  7:	return MST3_07;	case  8:	return MST3_08;	case  9:	return MST3_09;
			case 10:	return MST3_10;	case 11:	return MST3_11;	case 12:	return MST3_12;	case 13:	return MST3_13;	case 14:	return MST3_14;
			case 15:	return MST3_15;	case 16:	return MST3_16;	case 17:	return MST3_17;	case 18:	return MST3_18;	case 19:	return MST3_19;	
			case 20:	return MST3_20;	case 21:	return MST3_21;	case 22:	return MST3_22;	
			}
		}
	case 4:
		{
			switch(nSCode)
			{
			case  0:	return MST4_00;	case  1:	return MST4_01;	case  2:	return MST4_02;	case  3:	return MST4_03;	case  4:	return MST4_04;
			case  5:	return MST4_05;	case  6:	return MST4_06;	case  7:	return MST4_07;	case  8:	return MST4_08;	case  9:	return MST4_09;	
			case 10:	return MST4_10;	case 11:	return MST4_11;	case 12:	return MST4_12;	case 13:	return MST4_13;	case 14:	return MST4_14;	
			case 15:	return MST4_15;	case 16:	return MST4_16;	case 17:	return MST4_17;	case 18:	return MST4_18;	case 19:	return MST4_19;	
			case 20:	return MST4_20;	case 21:	return MST4_21;	case 22:	return MST4_22;	case 23:	return MST4_23;	case 24:	return MST4_24;	
			case 25:	return MST4_25;
			}	
		}	
	case 5:
		{
			switch(nSCode)
			{
			case  0:	return MST5_00;	case  1:	return MST5_01;	case  2:	return MST5_02;	case  3:	return MST5_03;	case  4:	return MST5_04;	
			case  5:	return MST5_05;	case  6:	return MST5_06;	case  7:	return MST5_07;	case  8:	return MST5_08;	case  9:	return MST5_09;	
			case 10:	return MST5_10;	case 11:	return MST5_11;	case 12:	return MST5_12;	case 13:	return MST5_13;	case 14:	return MST5_14;	
			case 15:	return MST5_15;	case 16:	return MST5_16;	case 17:	return MST5_17;	case 18:	return MST5_18;	case 19:	return MST5_19;	
			case 20:	return MST5_20;	case 21:	return MST5_21;	case 22:	return MST5_22;	case 23:	return MST5_23;	case 24:	return MST5_24;	
			case 25:	return MST5_25;	case 26:	return MST5_26;	case 27:	return MST5_27;	case 28:	return MST5_28;	case 29:	return MST5_29;	
			case 30:	return MST5_30;	case 31:	return MST5_31;	case 32:	return MST5_32;	case 33:	return MST5_33;	case 34:	return MST5_34;	
			case 35:	return MST5_35;	case 36:	return MST5_36;	case 37:	return MST5_37;	case 38:	return MST5_38;	
			}
		}
	case 6:
		{
			switch(nSCode)
			{
			case  0:	return MST6_00;	case  1:	return MST6_01;	case  2:	return MST6_02;	case  3:	return MST6_03;	case  4:	return MST6_04;
			case  5:	return MST6_05;	case  6:	return MST6_06;	case  7:	return MST6_07;	case  8:	return MST6_08;	case  9:	return MST6_09;
			case 10:	return MST6_10;	case 11:	return MST6_11;	case 12:	return MST6_12;	case 13:	return MST6_13;	case 14:	return MST6_14;
			case 15:	return MST6_15;	case 16:	return MST6_16;	case 17:	return MST6_17;	case 18:	return MST6_18;	case 19:	return MST6_19;
			case 20:	return MST6_20;	case 21:	return MST6_21;	case 22:	return MST6_22;	case 23:	return MST6_23;	case 24:	return MST6_24;
			case 25:	return MST6_25;	case 26:	return MST6_26;	case 27:	return MST6_27;	case 28:	return MST6_28;	
			}
		}	
	case 7://커플러
		{
			switch(nSCode)
			{
			case 0:	return MST7_00;	case 1:	return MST7_01;	case 2:	return MST7_02;	case 3:	return MST7_03;	case 4:	return MST7_04;
			case 5:	return MST7_05;	case 6:	return MST7_06;	case 7:	return MST7_07;	case 8:	return MST7_08;	case 9:	return MST7_09;	
			case 10:	return MST7_10;	case 11:	return MST7_11;
			}
		}
	case 8:
		{
			switch(nSCode)
			{
			case 0:	return MST8_00;	case 1:	return MST8_01;	case 2:	return MST8_02; case 3:	return MST8_03;	case 4:	return MST8_04;
			case 5:	return MST8_05;	case 6:	return MST8_06;	case 7:	return MST8_07;	case 8:	return MST8_08;
			}
		}
	}
	ASSERT(0);
	return MST0_00;
}
double MSRebarShape::GetTotalLenGroup1()
{
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup2()
{
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup3()
{
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup4()
{
	m_TotalLength = 0.;
	switch(m_nSCode)
	{
	case 5:
		{
			double dA = GetA();
			double dG = GetG();
			double dO = GetO();
			m_TotalLength = PI*dO + dA + dG;
			return m_TotalLength;
		}
	case 6:
		{
			double dK = GetK();
			double dO = GetO();
			m_TotalLength = PI*dO + dK;
			return m_TotalLength;
		}
	}
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup5()
{
	m_TotalLength = 0;
	double A = GetA();
	double B = GetB();
	double C = GetC();
	double D = GetD();
	double E = GetE();
	double F = GetF();
	double G = GetG();
	switch(m_nSCode)
	{
	case 4:	return GetTotalLenHaunch();	
	case 28:
		{				
			if(B > DTOL10)	m_TotalLength+=B;
			if(C > DTOL10)	m_TotalLength+=(C*2);
			if(D > DTOL10)	m_TotalLength+=D;
			if(E > DTOL10)	m_TotalLength+=E;			
			return m_TotalLength;
		}
	case 37:
		{
			if(A > DTOL10)	m_TotalLength+=A;
			if(B > DTOL10)	m_TotalLength+=B;
			if(C > DTOL10)	m_TotalLength+=(C*2);
			if(D > DTOL10)	m_TotalLength+=D;
			if(E > DTOL10)	m_TotalLength+=E;			
			return m_TotalLength;
		}
	}
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup6()
{
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup7()
{
	switch(m_nSCode)
	{
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
		return GetTotalLenHaunch();		
	}
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenGroup8()
{
	m_TotalLength = 0;
	double A = GetA();
	double B = GetB();
	double C = GetC();
	double D = GetD();
	double E = GetE();
	double F = GetF();
	double G = GetG();
	switch(m_nSCode)
	{
	case 0:	
	case 2:
		{
			if(A > DTOL10)	m_TotalLength+=(A*2);
			if(B > DTOL10)	m_TotalLength+=(B*2);
			if(C > DTOL10)	m_TotalLength+=C;
			return m_TotalLength;
		}
	case 1:
		{
			if(A > DTOL10)	m_TotalLength+=A;
			if(B > DTOL10)	m_TotalLength+=B;
			if(C > DTOL10)	m_TotalLength+=C;			
			return m_TotalLength;
		}
	}
	return GetTotalLenAllAdd();
}
double MSRebarShape::GetTotalLenAllAdd()
{
	m_TotalLength = 0;
	double A = GetA();
	double B = GetB();
	double C = GetC();
	double D = GetD();
	double E = GetE();
	double F = GetF();
	double G = GetG();

	if(A > DTOL10)	m_TotalLength+=A;
	if(B > DTOL10)	m_TotalLength+=B;
	if(C > DTOL10)	m_TotalLength+=C;
	if(D > DTOL10)	m_TotalLength+=D;
	if(E > DTOL10)	m_TotalLength+=E;
	if(F > DTOL10)	m_TotalLength+=F;
	if(G > DTOL10)	m_TotalLength+=G;
	return m_TotalLength;
}
double MSRebarShape::GetTotalLenHaunch()
{
	m_TotalLength = 0.;
	double A = GetA();
	double B = GetB();	
	double D = GetD();

	if(A > DTOL10)	m_TotalLength+=A;
	if(B > DTOL10)	m_TotalLength+=B;	
	if(D > DTOL10)	m_TotalLength+=D;
	return m_TotalLength;
}
double MSRebarShape::GetTotalLength()
{
	if(m_TotalLength < DTOL_GM)
	{
		m_TotalLength = 0.;

		switch(m_nGCode)
		{
		case 1:		return GetTotalLenGroup1();
		case 2:		return GetTotalLenGroup2();
		case 3:		return GetTotalLenGroup3();
		case 4:		return GetTotalLenGroup4();
		case 5:		return GetTotalLenGroup5();
		case 6:		return GetTotalLenGroup6();
		case 7:		return GetTotalLenGroup7();
		case 8:		return GetTotalLenGroup8();
		}
		/*
		long NumVs;
		NumVs = s_MSNumVs[m_nGCode-1][m_nSCode];
		m_TotalLength = 0.;
		long nShapeCode = MakeBarShapeCode(m_nGCode, m_nSCode);
		if(nShapeCode==MST4_05)
		{
			double dA = GetA();
			double dG = GetG();
			double dO = GetO();

			m_TotalLength = PI*dO + dA + dG;
			return m_TotalLength;
		}
		else if(nShapeCode==MST4_06)
		{
			double dK = GetK();
			double dO = GetO();

			m_TotalLength = PI*dO + dK;
			return m_TotalLength;
		}
		double A = GetA();
		double B = GetB();
		double C = GetC();
		double D = GetD();
		double E = GetE();
		double F = GetF();
		double G = GetG();

		if(A > DTOL10)	m_TotalLength+=A;
		if(B > DTOL10)	m_TotalLength+=B;
		if(C > DTOL10)	m_TotalLength+=C;
		if(D > DTOL10)	m_TotalLength+=D;
		if(E > DTOL10)	m_TotalLength+=E;
		if(F > DTOL10)	m_TotalLength+=F;
		if(G > DTOL10)	m_TotalLength+=G;
		*/
	}

	return m_TotalLength;
}

double MSRebarShape::BendDia()
{
	double ms_DefaultBendDia25 = -1.; 
	double ms_DefaultBendDia35 = -1.; 
	double ms_DefaultBendDia38 = -1.; 
	double BndDia = 0;
	if(m_IsTieStrp == 1)
	{
		if(m_dDia <= 16)
			BndDia = 4.*m_dDia;
		else
			BndDia = 6.*m_dDia;
	}

	else
	{
		if(m_dDia <= 25)
		{
			if(ms_DefaultBendDia25 < 0.)
				BndDia = 6.*m_dDia;
			else
				BndDia = ms_DefaultBendDia25*m_dDia;
		}
		else if(m_dDia <= 35)
		{
			if(ms_DefaultBendDia35 < 0.)
				BndDia = 8.*m_dDia;
			else
				BndDia = ms_DefaultBendDia35*m_dDia;
		}
		else
		{
			if(ms_DefaultBendDia38 < 0.)
				BndDia = 10.*m_dDia;
			else
				BndDia = ms_DefaultBendDia38*m_dDia;
		}
	}

	return BndDia;
}

double MSRebarShape::a2()
{
	//if(m_PrintType == 1)
	//	return 0.;
	return BendDia()/2.+m_dDia;
}

double MSRebarShape::a1(double HookDeg)   // KBC 2005 0505.2.1
{
	double Value = 0.;

	if(fabs(fabs(HookDeg) - 180) < DTOL_GM)  // for Main Bar
	{
		Value = 4.*m_dDia;
		if(Value < 60.)
			Value = 60.;
	}
	else if(fabs(fabs(HookDeg) - 135) < DTOL_GM)  // for Tie or Stirrup
  {
    if(m_dDia >25)
      ASSERT(0);    //기준에 없는듯
		Value = 6.*m_dDia;
  }
	else if(fabs(fabs(HookDeg) - 90) < DTOL_GM)
	{
		if(m_IsTieStrp == 0)   // for Main Bar
			Value = 12.*m_dDia;
		else if(m_dDia <= 16)  // for Tie or Stirrup
			Value = 6.*m_dDia;
		else
			Value = 12.*m_dDia;
	}
	return Value;
}

BOOL MSRebarShape::IsSameShape(MSRebarShape* Src)
{
	if(m_dDia != Src->m_dDia)
		return FALSE;

	if(m_nGCode != Src->m_nGCode)	return FALSE;
	if(m_nSCode != Src->m_nSCode)	return FALSE;

	if(fabs(GetA() - Src->GetA()) > DTOL1)	return FALSE;
	if(fabs(GetB() - Src->GetB()) > DTOL1)	return FALSE;
	if(fabs(GetC() - Src->GetC()) > DTOL1)	return FALSE;
	if(fabs(GetD() - Src->GetD()) > DTOL1)	return FALSE;
	if(fabs(GetE() - Src->GetE()) > DTOL1)	return FALSE;
	if(fabs(GetF() - Src->GetF()) > DTOL1)	return FALSE;
	if(fabs(GetG() - Src->GetG()) > DTOL1)	return FALSE;
	if(fabs(GetH() - Src->GetH()) > DTOL1)	return FALSE;
	if(fabs(GetI() - Src->GetI()) > DTOL1)	return FALSE;
	if(fabs(GetJ() - Src->GetJ()) > DTOL1)	return FALSE;
	if(fabs(GetK() - Src->GetK()) > DTOL1)	return FALSE;
	if(fabs(GetL() - Src->GetL()) > DTOL1)	return FALSE;
	if(fabs(GetO() - Src->GetO()) > DTOL1)	return FALSE;

	return TRUE;
}

double& MSRebarShape::SetA()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][0] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetB()
{
	long Index;
	if(m_nGCode == 0)
		Index = 0;
	else
		Index= s_MSBarValue[m_nGCode-1][m_nSCode][1] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetC()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][2] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetD()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][3] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}
 
double& MSRebarShape::SetE()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][4] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetF()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][5] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetG()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][6] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetH()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][7] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetI()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][8] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetJ()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][9] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetK()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][10] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetL()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][11] - 1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

double& MSRebarShape::SetO()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][12] -1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}
double& MSRebarShape::SetHH()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][13] -1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}
double& MSRebarShape::SetKK()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][14] -1;
	ASSERT(Index >= 0);
	return *(m_Vs+Index);
}

// Return -1.;
double MSRebarShape::GetA()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][0] - 1;

	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetB()
{
	long Index;
	if(m_nGCode == 0)
		Index = 0;
	else
		Index= s_MSBarValue[m_nGCode-1][m_nSCode][1] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetC()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][2] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetD()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][3] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetE()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][4] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetF()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][5] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetG()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][6] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetH()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][7] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetI()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][8] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetJ()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][9] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetK()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][10] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetL()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][11] - 1;
	if(Index <  0)	return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetO()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][12] -1;
	if(Index < 0) return -1.;
	return *(m_Vs+Index);
}

double MSRebarShape::GetHH()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][13] -1;
	if(Index < 0) return -1.;
	return *(m_Vs+Index);

}
double MSRebarShape::GetKK()
{
	long Index = s_MSBarValue[m_nGCode-1][m_nSCode][14] -1;
	if(Index < 0) return -1.;
	return *(m_Vs+Index);

}

enum {WK_MAKEACTIONS=1, WK_SPLICE};

CString MSRebarShape::GetStringShapeCode()
{
	switch(m_nGCode)
	{
	case 1:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T1_00");	case  1:	return _T("T1_01");	case  2:	return _T("T1_02");	case  3:	return _T("T1_03");	case  4:	return _T("T1_04");
			case  5:	return _T("T1_05");	case  6:	return _T("T1_06");	case  7:	return _T("T1_07");	case  8:	return _T("T1_08");	case  9:	return _T("T1_09");
			case 10:	return _T("T1_10");	case 11:	return _T("T1_11");	case 12:	return _T("T1_12");	case 13:	return _T("T1_13");	case 14:	return _T("T1_14");
			case 15:	return _T("T1_15");
			}
		}
	case 2:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T2_00");	case  1:	return _T("T2_01");	case  2:	return _T("T2_02");	case  3:	return _T("T2_03");	case  4:	return _T("T2_04");
			case  5:	return _T("T2_05");	case  6:	return _T("T2_06");	case  7:	return _T("T2_07");	case  8:	return _T("T2_08");	case  9:	return _T("T2_09");
			case 10:	return _T("T2_10");	case 11:	return _T("T2_11");	case 12:	return _T("T2_12");	case 13:	return _T("T2_13");	case 14:	return _T("T2_14");
			case 15:	return _T("T2_15");
			}
		}
	case 3:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T3_00");	case  1:	return _T("T3_01");	case  2:	return _T("T3_02");	case  3:	return _T("T3_03");	case  4:	return _T("T3_04");
			case  5:	return _T("T3_05");	case  6:	return _T("T3_06");	case  7:	return _T("T3_07");	case  8:	return _T("T3_08");	case  9:	return _T("T3_09");
			case 10:	return _T("T3_10");	case 11:	return _T("T3_11");	case 12:	return _T("T3_12");	case 13:	return _T("T3_13");	case 14:	return _T("T3_14");
			case 15:	return _T("T3_15");	case 16:	return _T("T3_16");	case 17:	return _T("T3_17");	case 18:	return _T("T3_18");	case 19:	return _T("T3_19");	
			case 20:	return _T("T3_20");	case 21:	return _T("T3_21");	case 22:	return _T("T3_22");	
			}
		}
	case 4:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T4_00");	case  1:	return _T("T4_01");	case  2:	return _T("T4_02");	case  3:	return _T("T4_03");	case  4:	return _T("T4_04");
			case  5:	return _T("T4_05");	case  6:	return _T("T4_06");	case  7:	return _T("T4_07");	case  8:	return _T("T4_08");	case  9:	return _T("T4_09");	
			case 10:	return _T("T4_10");	case 11:	return _T("T4_11");	case 12:	return _T("T4_12");	case 13:	return _T("T4_13");	case 14:	return _T("T4_14");	
			case 15:	return _T("T4_15");	case 16:	return _T("T4_16");	case 17:	return _T("T4_17");	case 18:	return _T("T4_18");	case 19:	return _T("T4_19");	
			case 20:	return _T("T4_20");	case 21:	return _T("T4_21");	case 22:	return _T("T4_22");	case 23:	return _T("T4_23");	case 24:	return _T("T4_24");	
			case 25:	return _T("T4_25");
			}	
		}	
	case 5:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T5_00");	case  1:	return _T("T5_01");	case  2:	return _T("T5_02");	case  3:	return _T("T5_03");	case  4:	return _T("T5_04");	
			case  5:	return _T("T5_05");	case  6:	return _T("T5_06");	case  7:	return _T("T5_07");	case  8:	return _T("T5_08");	case  9:	return _T("T5_09");	
			case 15:	return _T("T5_15");	case 16:	return _T("T5_16");	case 17:	return _T("T5_17");	case 18:	return _T("T5_18");	case 19:	return _T("T5_19");	
			case 10:	return _T("T5_10");	case 11:	return _T("T5_11");	case 12:	return _T("T5_12");	case 13:	return _T("T5_13");	case 14:	return _T("T5_14");	
			case 20:	return _T("T5_20");	case 21:	return _T("T5_21");	case 22:	return _T("T5_22");	case 23:	return _T("T5_23");	case 24:	return _T("T5_24");	
			case 25:	return _T("T5_25");	case 26:	return _T("T5_26");	case 27:	return _T("T5_27");	case 28:	return _T("T5_28");	case 29:	return _T("T5_29");	
			case 30:	return _T("T5_30");	case 31:	return _T("T5_31");	case 32:	return _T("T5_32");	case 33:	return _T("T5_33");	case 34:	return _T("T5_34");	
			case 35:	return _T("T5_35");	case 36:	return _T("T5_36");	case 37:	return _T("T5_37");	case 38:	return _T("T5_38");	
			}
		}
	case 6:
		{
			switch(m_nSCode)
			{
			case  0:	return _T("T6_00");	case  1:	return _T("T6_01");	case  2:	return _T("T6_02");	case  3:	return _T("T6_03");	
			case  4:	return _T("T6_06");	// 나중에 다시 수정
			case  5:	return _T("T6_05");	case  6:	return _T("T6_06");	case  7:	return _T("T6_07");	case  8:	return _T("T6_08");	case  9:	return _T("T6_09");
			case 10:	return _T("T6_10");	case 11:	return _T("T6_11");	case 12:	return _T("T6_12");	case 13:	return _T("T6_13");	case 14:	return _T("T6_14");
			case 15:	return _T("T6_15");	case 16:	return _T("T6_16");	case 17:	return _T("T6_17");	case 18:	return _T("T6_18");	case 19:	return _T("T6_19");
			case 20:	return _T("T6_20");	case 21:	return _T("T6_21");	case 22:	return _T("T6_22");	case 23:	return _T("T6_23");	case 24:	return _T("T6_24");
			case 25:	return _T("T6_25");	case 26:	return _T("T6_26");	case 27:	return _T("T6_27");	case 28:	return _T("T6_28");	
			}
		}	
	case 7:
		{
			switch(m_nSCode)
			{
			case 0:	return _T("T7_00");	case 1:	return _T("T7_01");	case 2:	return _T("T7_02");	case 3:	return _T("T7_03");	case 4:	return _T("T7_04");
			case 5:	return _T("T7_05");	case 6:	return _T("T7_06");	case 7:	return _T("T7_07");	case 8:	return _T("T7_08");	case 9:	return _T("T7_09");
			case 10:	return _T("T7_10");	case 11:	return _T("T7_11");
			}
		}
	case 8:
		{
			switch(m_nSCode)
			{
			case 0:	return _T("T8_00");	case 1:	return _T("T8_01");	case 2:	return _T("T8_02");	case 3:	return _T("T8_03");	case 4:	return _T("T8_04");
			case 5:	return _T("T8_05");	case 6:	return _T("T8_06");	case 7:	return _T("T8_07");	case 8:	return _T("T8_08");	case 9:	return _T("T8_09");
			}
		}
	}
	ASSERT(0);
	return _T("T0_00");

}


void MSRebarShape::GetRotateDegLine(double * RotateDeg, BOOL direction, double Y, double X)
{	
	double thaRad = atan2(Y,X);	// 휘어지는 철근과 수직선이 이루는 각도   
	double thaDeg = thaRad * Rad2Deg;     

	if(direction)
		*RotateDeg = -(90 + thaDeg); //오른쪽 하단으로 꺾이므로
	else
		*RotateDeg = -(90 - thaDeg);
}

CString MSRebarShape::GetTotalLenString_RSTeamType()
{
	CString BarText = _T("");
	double TheRotateDeg1,TheRotateDeg2;
	long ShapeCode = MakeBarShapeCode(m_nGCode,m_nSCode);
	if(ShapeCode == MST1_00)
		BarText.Format(_T("%.0f-1"),GetB());
	else if(ShapeCode == MST1_02)
		BarText.Format(_T("61(%.0fX%.0f,45°)"),GetB(),GetG());
	else if(ShapeCode == MST1_03)
		BarText.Format(_T("2(%.0fX%.0f)"),GetB(),GetG());
	else if(ShapeCode == MST1_06)
		BarText.Format(_T("3(%.0fX%.0fX%.0f)"),GetB(),GetA(),GetG());
	else if(ShapeCode == MST3_00)
		BarText.Format(_T("3A(%.0fX%.0fX%.0f)"),GetC(),GetB(),GetD());
	else if(ShapeCode == MST3_03)
		BarText.Format(_T("11(%.0fX%.0fX%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA(),GetD(),GetG());
	else if(ShapeCode == MST3_02)
		BarText.Format(_T("12(%.0fX%.0fX%.0fX%.0fX%.0f,45f°)"),GetC(),GetB(),GetA(),GetD(),GetG());
	else if(ShapeCode == MST4_04)
		BarText.Format(_T("13(%.0fX%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA(),GetG());
	else if(ShapeCode == MST1_09)
		BarText.Format(_T("14B(%.0fX%.0fX%.0f,45f°)"),GetB(),GetA(),GetG());
	else if(ShapeCode == MST4_00)
		BarText.Format(_T("21(%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA());
	else if(ShapeCode == MST4_03)
		BarText.Format(_T("21A(%.0fX%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA(),GetG());
	else if(ShapeCode == MST4_01)
		BarText.Format(_T("21B(%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA());
	else if(ShapeCode == MST2_07)
		BarText.Format(_T("23(%.0fX%.0fX%.0f)"),GetC(),GetB(),GetA());
	else if(ShapeCode == MST2_10)
		BarText.Format(_T("23A(%.0fX%.0fX%.0fX%.0f)"),GetC(),GetB(),GetG(),GetA());
	else if(ShapeCode == MST6_06)
		BarText.Format(_T("34(%.0fX%.0fX%.0f)"),GetD(),GetC(),GetB());
	else if(ShapeCode == MST4_06)
		BarText.Format(_T("41(%.0fX%.0f)"),GetO()/2,GetK());
	else if(ShapeCode == MST4_05)
		BarText.Format(_T("41B(%.0fX%.0fX%.0f)"),GetO()/2,GetA(),GetG());
	else if(ShapeCode == MST2_13)
	{
		GetRotateDegLine(&TheRotateDeg1,TRUE,GetK(),GetH());	
		BarText.Format(_T("61(%.0fX%.0f,%.0f°)"),GetB(),GetC(),TheRotateDeg1);
	}
	else if(ShapeCode == MST5_14)
	{
		GetRotateDegLine(&TheRotateDeg1,TRUE,GetK(),GetH());	
		GetRotateDegLine(&TheRotateDeg2,TRUE,GetKK(),GetHH());	
		BarText.Format(_T("62(%.0fX%.0fX%.0f,%.0f°,%.0f°)"),GetC(),GetB(),GetD(),TheRotateDeg1,TheRotateDeg2);
	}
	else if(ShapeCode == MST5_06)
	{
		GetRotateDegLine(&TheRotateDeg1,TRUE,GetK(),GetH());	
		BarText.Format(_T("62A(%.0fX%.0fX%.0f,%.0f°)"),GetC(),GetB(),GetD(),TheRotateDeg1);
	}
	else if(ShapeCode == MST5_05)
	{
		GetRotateDegLine(&TheRotateDeg1,TRUE,GetK(),GetH());	
		BarText.Format(_T("62B(%.0fX%.0fX%.0f,%.0f°)"),GetC(),GetB(),GetD(),TheRotateDeg1);
	}
	else if(ShapeCode == MST5_04)
	{
		GetRotateDegLine(&TheRotateDeg1,TRUE,GetK(),GetH());	
		BarText.Format(_T("63(%.0fX%.0fX%.0f,%.0f°)"),GetC(),GetB(),GetD(),TheRotateDeg1);
	}
	else if(ShapeCode == MST1_12)
	{
		BarText.Format(_T("64(%.0fX%.0fX%.0f)"),GetB(),GetA(),GetG());
	}
	else if(ShapeCode == MST1_05)
	{
		BarText.Format(_T("62(%.0fX%.0fX%.0f,45,45)"),GetB(),GetA(),GetG());
	}

	else
		ASSERT(0);

	return BarText;
}
