#include "StdAfx.h"
#include "MSDeformedBar.h"
#include "MSUnit.h"

CString MSDeformedBar::m_sDefaultBarType = _T("D");
MSDeformedBar::MSDeformedBar(void)
{
}

MSDeformedBar::~MSDeformedBar(void)
{
}
double MSDeformedBar::KS_DBars[KS_DBar_Last][4] = 
{
	//{0.000249, 6.35,  31.6, 20. }, //D6
	{0.000560, 9.53,  71.3, 30. }, //D10
	{0.000995, 12.7, 126.7, 40. }, //D13
	{0.00156 , 15.9, 198.6, 50. }, //D16
	{0.00225 , 19.1, 286.5, 60. }, //D19
	{0.00304 , 22.2, 387.1, 70. }, //D22
	{0.00398 , 25.4, 506.7, 80. }, //D25
	{0.00504 , 28.6, 642.4, 90. }, //D29
	{0.00623 , 31.8, 794.2, 100.}, //D32
	{0.00751 , 34.9, 956.6, 110.}, //D35
	{0.00895 , 38.1, 1140., 120.}, //D38
	{0.0105  , 41.3, 1340., 130.}, //D41
	{0.0159  , 50.8, 2027., 140.}  //D51
};

double MSDeformedBar::ASTM_DBars[ASTM_DBar_Last][4] = 
{
	{0.376, 0.375, 0.11, 1.178 }, //#3
	{0.668, 0.500, 0.20, 1.157 }, //#4
	{1.043, 0.625, 0.31, 1.963 }, //#5
	{1.502, 0.750, 0.44, 2.356 }, //#6
	{2.044, 0.875, 0.60, 2.749 }, //#7
	{2.670, 1.000, 0.79, 3.142 }, //#8
	{3.400, 1.128, 1.00, 3.544 }, //#9
	{4.303, 1.270, 1.27, 3.990 }, //#10
	{5.313, 1.410, 1.56, 4.430 }, //#11
	{7.650, 1.693, 2.25, 5.332 }, //#14
	{13.60, 2.257, 4.00, 7.090 } //#18
};

double MSDeformedBar::GB_DBars[GB_DBar_Last][4] = 
{
	{ 0.000186, 5.5, 23.758, 17.278 },//d5.5
	{ 0.000222, 6., 28.3, 18.850 }, //d6 
	{ 0.000260, 6.5, 33.2, 20.420 }, //d6.5
	{ 0.000395, 8., 50.27, 25.133 }, //d8
	{ 0.000617, 10., 78.54, 31.416 }, //d10
	{ 0.000888, 12., 113.1, 37.699 }, //d12
	{ 0.00121, 14., 153.94, 43.982 }, //d14
	{ 0.00158, 16., 201.06, 50.265 }, //d16
	{ 0.00200, 18., 254.47, 56.549 }, //d18
	{ 0.00247, 20., 314.159, 62.832 }, //d20
	{ 0.00298, 22., 380.133, 69.115 }, //d22
	{ 0.00385, 25., 490.874, 78.540 }, //d25
	{ 0.00483, 28., 615.752, 87.965 }, //d28
	{ 0.00631, 32., 804.248, 100.531 }, //d32
	{ 0.00799, 36., 1017.876, 113.097 },  //d36
	{ 0.00987, 40., 1256.637, 125.664 },  //d40
	{ 0.01542, 50., 1963.495, 157.079 }  //d50
};

double MSDeformedBar::BS_DBars[BS_DBar_Last][4] = 
{
	{0.000222,  6.,   28.27,  18.849}, //P6
	{0.000395,  8.,   50.27,  25.133}, //P8
	{0.000617, 10.,   78.54,  31.416}, //P10
	{0.000888, 12.,  113.10,  37.699}, //P12
	{0.001209, 14.,  153.94,  37.699}, //P14
	{0.001580, 16.,  201.06,  50.265}, //P16
	{0.002000, 18.,  254.47,  37.699}, //P18
	{0.002470, 20.,  314.16,  62.832}, //P20
	{0.003850, 25.,  490.87,  78.540}, //P25
	{0.006310, 32.,  804.25, 100.531}, //P32
	{0.009860, 40., 1256.64, 125.664}, //P40
	{0.015413, 50., 1963.50, 157.080}  //P50
};

double MSDeformedBar::VIETNAM_DBars[VIETNAM_DBar_Last][4] =
{
	{ 0.000222, 6.0, 28.27, 18.849 },  //6
	{ 0.000395, 8.0, 50.27, 25.133 },  //8
	{ 0.000617, 10.0, 78.54, 31.416 },  //10
	{ 0.000888, 12.0, 113.10, 37.699 },  //12
	{ 0.000994, 13.0, 132.732, 40.841 },  //13
	{ 0.001210, 14.0, 153.94, 43.982 },  //14
	{ 0.001580, 16.0, 201.06, 50.265 },  //16
	{ 0.002000, 18.0, 254.47, 56.549 },  //18
	{ 0.002235, 19.0, 283.529, 59.690 },  //19
	{ 0.002470, 20.0, 314.159, 62.832 },  //20
	{ 0.002980, 22.0, 380.133, 69.115 },  //22
	{ 0.003850, 25.0, 490.874, 78.54 },  //25
	{ 0.004480, 28.0, 615.752, 87.965 },  //28
	{ 0.005060, 29.0, 660.52, 91.106 },  //29
	{ 0.006310, 32.0, 804.248, 100.531 },  //32
	{ 0.007550, 35.0, 962.113, 109.956 },  //35
	{ 0.007990, 36.0, 1017.876, 113.097 },  //36
	{ 0.008900, 38.0, 1134.115, 119.381 },  //38
	{ 0.009860, 40.0, 1256.637, 125.664 },  //40
	{ 0.010360, 41.0, 1320.254, 128.805 },  //41
	{ 0.011400, 43.0, 1452.201, 135.089 },  //43
	{ 0.016040, 51.0, 2042.821, 160.221 }  //51
};

double MSDeformedBar::INDIA_DBars[INDIA_DBar_Last][4] =
{
	{ 0.000395, 8.0, 50.3, 25.133 },  //8
	{ 0.000617, 10.0, 78.6, 31.416 },  //10
	{ 0.000888, 12.0, 113.1, 37.699 },  //12
	{ 0.001580, 16.0, 201.2, 50.265 },  //16
	{ 0.002470, 20.0, 314.3, 62.832 },  //20
	{ 0.003850, 25.0, 491.1, 78.54 },  //25
	{ 0.004830, 28.0, 615.8, 87.965 },  //28
	{ 0.006310, 32.0, 804.6, 100.531 },  //32
	{ 0.007990, 36.0, 1018.3, 113.097 },  //36
	{ 0.009860, 40.0, 1257.2, 125.664 },  //40
};

double MSDeformedBar::SINGAPORE_DBars[SINGAPORE_DBar_Last][4] =
{
	{ 0.000395, 8.0, 50.27, 25.133 },  //8
	{ 0.000617, 10.0, 78.54, 31.416 },  //10
	{ 0.001042, 13.0, 132.732, 40.841 },  //13
	{ 0.001579, 16.0, 201.06, 50.265 },  //16
	{ 0.002466, 20.0, 314.159, 62.832 },  //20
	{ 0.002984, 22.0, 380.133, 69.115 },  //22
	{ 0.003854, 25.0, 490.874, 78.54 },  //25
	{ 0.004834, 28.0, 615.752, 87.965 },  //28
	{ 0.006313, 32.0, 804.248, 100.531 },  //32
	{ 0.009864, 40.0, 1256.637, 125.664 },  //40
	{ 0.015413, 50.0, 1963.50, 157.080 }  //P50
};

double MSDeformedBar::JIS_DBars[JIS_DBar_Last][4] =
{
	{0.000560, 10.00,  71.3, 30. }, //D10
	{0.000995, 13.0, 126.7, 40. }, //D13
	{0.00156 , 16.0, 198.6, 50. }, //D16
	{0.00225 , 19.0, 286.5, 60. }, //D19
	{0.00304 , 22.0, 387.1, 70. }, //D22
	{0.00398 , 25.0, 506.7, 80. }, //D25
	{0.00504 , 29.0, 642.4, 90. }, //D29
	{0.00623 , 32.0, 794.2, 100.}, //D32
	{0.00751 , 35.0, 956.6, 110.}, //D35
	{0.00895 , 38.0, 1140., 120.}, //D38
	{0.0105  , 41.0, 1340., 130.}, //D41
	{0.0159  , 51.0, 2027., 140.}  //D51
};

long MSDeformedBar::sm_nRebarCode = REBAR_MATERIAL_KS;
long MSDeformedBar::sm_nNationCode= NATION_CODE_KOREA;

MSDeformedBar::MSDeformedBar(CString DiaName)
{
	long DBarType = 0;

	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);

	long BarIndex = FindBarDiameterIndexByName(DiaName);	
	if(BarIndex == -1)
	{
//		ASSERT(0);
		BarIndex = 0;
	}
	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{
		DBarType = (KS_DBar_Kind)BarIndex;

		if(DBarType == KS_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = KS_DBars[DBarType][0];
			m_Dia = KS_DBars[DBarType][1];
			m_Area = KS_DBars[DBarType][2];
			m_Peri = KS_DBars[DBarType][3];
		}
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{
		DBarType = (ASTM_DBar_Kind)BarIndex;

		if(DBarType == ASTM_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			MSUnit CodeUnit;
			CodeUnit.SetUnit(D_UNIT_FORCE_LBF, D_UNIT_LENGTH_IN);
			MSUnit FtUnit;
			FtUnit.SetUnit(D_UNIT_FORCE_LBF, D_UNIT_LENGTH_FT);

			m_Weight = ASTM_DBars[DBarType][0] / FtUnit.m_dUnitForceFactor;
			m_Dia = ASTM_DBars[DBarType][1] / CodeUnit.m_dLengthFactor;
			m_Area = ASTM_DBars[DBarType][2] / CodeUnit.m_dAreaFactor;
			m_Peri = ASTM_DBars[DBarType][3] / CodeUnit.m_dLengthFactor;
		}
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{
		DBarType = (GB_DBar_Kind)BarIndex;

		if(DBarType == GB_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = GB_DBars[DBarType][0];
			m_Dia = GB_DBars[DBarType][1];
			m_Area = GB_DBars[DBarType][2];
			m_Peri = GB_DBars[DBarType][3];
		}
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{
		DBarType = (BS_DBar_Kind)BarIndex;

		if(DBarType == BS_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = BS_DBars[DBarType][0];
			m_Dia = BS_DBars[DBarType][1];
			m_Area = BS_DBars[DBarType][2];
			m_Peri = BS_DBars[DBarType][3];
		}
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		DBarType = (VIETNAM_DBar_Kind)BarIndex;

		if (DBarType == VIETNAM_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = VIETNAM_DBars[DBarType][0];
			m_Dia = VIETNAM_DBars[DBarType][1];
			m_Area = VIETNAM_DBars[DBarType][2];
			m_Peri = VIETNAM_DBars[DBarType][3];
		}
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		DBarType = (INDIA_DBar_Kind)BarIndex;

		if (DBarType == INDIA_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = INDIA_DBars[DBarType][0];
			m_Dia = INDIA_DBars[DBarType][1];
			m_Area = INDIA_DBars[DBarType][2];
			m_Peri = INDIA_DBars[DBarType][3];
		}
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		DBarType = (SINGAPORE_DBar_Kind)BarIndex;

		if (DBarType == SINGAPORE_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = SINGAPORE_DBars[DBarType][0];
			m_Dia = SINGAPORE_DBars[DBarType][1];
			m_Area = SINGAPORE_DBars[DBarType][2];
			m_Peri = SINGAPORE_DBars[DBarType][3];
		}
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		DBarType = (JIS_DBar_Kind)BarIndex;

		if (DBarType == JIS_DBar_Last)
		{
			m_Weight = 0.;
			m_Dia = 0.;
			m_Area = 0.;
			m_Peri = 0.;
		}
		else
		{
			m_Weight = JIS_DBars[DBarType][0];
			m_Dia = JIS_DBars[DBarType][1];
			m_Area = JIS_DBars[DBarType][2];
			m_Peri = JIS_DBars[DBarType][3];
		}
	}
	else ASSERT(0);
}


// 주어진 Width 안에 SpliceType에 의할 때 1열에 배근될 수 있는 최대 철근 갯수 리턴
// SpliceType :: 
long MSDeformedBar::GetNumBarsInOneLayer(double Width, long SpliceType, double MinPitch)
{
	long NumBars;

	switch(SpliceType)
	{
	case NON_SPLICE:
		NumBars = (long) ( (Width + MinPitch) / (m_Dia + MinPitch) ) ;
		break;
	case HALF_SPLICE:
		{
			NumBars = (long) ( (Width + MinPitch) / (1.5 * m_Dia + MinPitch) ) ;
			long OddNum = NumBars/2;
			if(NumBars%2 != 0) OddNum++;
			long EvenNum = NumBars - OddNum;
			double dRqrdWidth = OddNum*(2*m_Dia) + EvenNum*m_Dia + (NumBars-1)*MinPitch;
			if(dRqrdWidth > Width+DTOL10) NumBars--;
		}
		break;
	case FULL_SPLICE:
		NumBars = (long) ( (Width + MinPitch) / (2.0 * m_Dia + MinPitch) ) ;
		break;
	default:
		NumBars = 0;
		break;
	}

	return NumBars;   // num is Maximum Number for Given Width...
}

long MSDeformedBar::GetRebarNameList(CStringArray& arNameList)
{
	arNameList.RemoveAll();

	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{ // 12개
//		arNameList.Add(_T("6"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("13"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("19"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("29"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("35"));
		arNameList.Add(_T("38"));
		arNameList.Add(_T("41"));
		arNameList.Add(_T("51"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{ // 11개
		arNameList.Add(_T("3"));
		arNameList.Add(_T("4"));
		arNameList.Add(_T("5"));
		arNameList.Add(_T("6"));
		arNameList.Add(_T("7"));
		arNameList.Add(_T("8"));
		arNameList.Add(_T("9"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("11"));
		arNameList.Add(_T("14"));
		arNameList.Add(_T("18"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{	// 17개
		arNameList.Add(_T("5.5"));
		arNameList.Add(_T("6"));
		arNameList.Add(_T("6.5"));
		arNameList.Add(_T("8"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("12"));
		arNameList.Add(_T("14"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("18"));
		arNameList.Add(_T("20"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("28"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("36"));
		arNameList.Add(_T("40"));
		arNameList.Add(_T("50"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{ // 12개
		arNameList.Add(_T("6"));
		arNameList.Add(_T("8"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("12"));
		arNameList.Add(_T("14"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("18"));
		arNameList.Add(_T("20"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("40"));
		arNameList.Add(_T("50"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{ // 22개
		arNameList.Add(_T("6"));
		arNameList.Add(_T("8"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("12"));
		arNameList.Add(_T("13"));
		arNameList.Add(_T("14"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("18"));
		arNameList.Add(_T("19"));
		arNameList.Add(_T("20"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("28"));
		arNameList.Add(_T("29"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("35"));
		arNameList.Add(_T("36"));
		arNameList.Add(_T("38"));
		arNameList.Add(_T("40"));
		arNameList.Add(_T("41"));
		arNameList.Add(_T("43"));
		arNameList.Add(_T("51"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{ // 10개
		arNameList.Add(_T("8"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("12"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("20"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("28"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("36"));
		arNameList.Add(_T("40"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{ // 11개
		arNameList.Add(_T("8"));
		arNameList.Add(_T("10"));
		arNameList.Add(_T("13"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("20"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("28"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("40"));
		arNameList.Add(_T("50"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{ // 12개
		arNameList.Add(_T("10"));
		arNameList.Add(_T("13"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("19"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("29"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("35"));
		arNameList.Add(_T("38"));
		arNameList.Add(_T("41"));
		arNameList.Add(_T("51"));
	}
	else
	{
		sm_nRebarCode = REBAR_MATERIAL_KS;
		arNameList.Add(_T("10"));
		arNameList.Add(_T("13"));
		arNameList.Add(_T("16"));
		arNameList.Add(_T("19"));
		arNameList.Add(_T("22"));
		arNameList.Add(_T("25"));
		arNameList.Add(_T("29"));
		arNameList.Add(_T("32"));
		arNameList.Add(_T("35"));
		arNameList.Add(_T("38"));
		arNameList.Add(_T("41"));
		arNameList.Add(_T("51"));
		ASSERT(0);
	}

	return arNameList.GetSize();
}

CString MSDeformedBar::GetDefaultRebarName()
{
	CString sRebarName = _T("");

	if (sm_nRebarCode == REBAR_MATERIAL_KS)
	{
		sRebarName = _T("10");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_ASTM)
	{
		sRebarName = _T("3");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_GB)
	{
		sRebarName = _T("6");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_BS)
	{
		sRebarName = _T("6");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		sRebarName = _T("6");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		sRebarName = _T("8");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{ 
		sRebarName = _T("8");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		sRebarName = _T("10");
	}

	return sRebarName;
}
double MSDeformedBar::GetDefaultDia()
{
	if (sm_nRebarCode == REBAR_MATERIAL_KS)
		return 10;
	else if (sm_nRebarCode == REBAR_MATERIAL_ASTM)
		return 3;
	else if (sm_nRebarCode == REBAR_MATERIAL_GB)
		return 6;
	else if (sm_nRebarCode == REBAR_MATERIAL_BS)
		return 6;
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
		return 6;
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
		return 8;
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
		return 8;
	if (sm_nRebarCode == REBAR_MATERIAL_JIS)
		return 10;
	else
		return 10;
}
long MSDeformedBar::GetRebarColorNameList(CStringArray& arColorList)
{
	arColorList.RemoveAll();

	if (sm_nRebarCode == REBAR_MATERIAL_KS)
	{	// 12개
		//		arColorList.Add(_T("silver"));
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
		arColorList.Add(_T("black"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_ASTM)
	{	// 11개
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_GB)
	{	// 17개
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
		arColorList.Add(_T("geometry = (r=0.22 g=0.55 b=0.55)"));
		arColorList.Add(_T("geometry = (r=0.29 g=0.55 b=0.82)"));
		arColorList.Add(_T("geometry = (r=0.55 g=0.82 b=0.55)"));
		arColorList.Add(_T("geometry = (r=0.55 g=0.22 b=0.82)"));
		arColorList.Add(_T("geometry = (r=0.82 g=0.22 b=0.14)"));
		arColorList.Add(_T("black"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_BS)
	{	// 12개
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
		arColorList.Add(_T("black"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{	// 22개
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
		arColorList.Add(_T("geometry = (r=0.22 g=0.55 b=0.55)"));
		arColorList.Add(_T("geometry = (r=0.29 g=0.55 b=0.82)"));
		arColorList.Add(_T("geometry = (r=0.55 g=0.82 b=0.55)"));
		arColorList.Add(_T("geometry = (r=0.55 g=0.22 b=0.82)"));
		arColorList.Add(_T("geometry = (r=0.82 g=0.22 b=0.14)"));
		arColorList.Add(_T("geometry = (r=0.67 g=0.55 b=0.67)"));
		arColorList.Add(_T("geometry = (r=0.14 g=0.67 b=0.82)"));
		arColorList.Add(_T("geometry = (r=0.82 g=0.82 b=0.82)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.75 b=0.00)"));
		arColorList.Add(_T("geometry = (r=0.55 g=0.55 b=0.82)"));
		arColorList.Add(_T("black"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{	// 12개
		arColorList.Add(_T("yellow"));
		arColorList.Add(_T("cyan"));
		arColorList.Add(_T("green"));
		arColorList.Add(_T("geometry = (r=0.61 g=0.00 b=1.00)"));
		arColorList.Add(_T("red"));
		arColorList.Add(_T("blue"));
		arColorList.Add(_T("geometry = (r=0.39 g=0.49 b=0.00)"));
		arColorList.Add(_T("geometry = (r=1.00 g=0.49 b=0.00)"));
		arColorList.Add(_T("magenta"));
		arColorList.Add(_T("indian red"));
		arColorList.Add(_T("midnight blue"));
		arColorList.Add(_T("black"));
	}
	else ASSERT(0);

	return arColorList.GetSize();
}

CString MSDeformedBar::FindBarTypeFromID(long BarType)
{
	CString Value;
	if(BarType == 0)
		Value.Format(_T("%s"), _T("HD"));
	else
		Value.Format(_T("%s"), _T("D"));

	return Value;
}

long MSDeformedBar::FindBarTypeFromString(CString AString)
{
	long Value = 0;
	if(AString == _T("HD"))
		Value = 0;
	else
		Value = 1;
	return Value;
}

CString MSDeformedBar::GetDefaultRebarNamePrefix(long BarType)
{
	CString strPrefix = _T("");
	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{
		if(BarType==1) // 보통 철근
			strPrefix = _T("D");
		else // 0 : 고강도 철근
			strPrefix = _T("HD");
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{
		strPrefix = _T("#");
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{
		if (ChinaDeformedBarCheck())
		{
			if (BarType == 1) // 보통 철근
				strPrefix = _T("D");
			else // 0 : 고강도 철근
				strPrefix = _T("HD");
		}
		else
		{
			strPrefix = _T("Φ");
		}
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{
		if(sm_nNationCode==NATION_CODE_UAE)
			strPrefix = _T("T");
		else
			strPrefix = _T("Φ");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		if (BarType == 1) // 보통 철근
			strPrefix = _T("D");
		else // 0 : 고강도 철근
			strPrefix = _T("HD");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		if (sm_nNationCode == NATION_CODE_UAE)
			strPrefix = _T("T");
		else
			strPrefix = _T("Φ");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		if (sm_nNationCode == NATION_CODE_UAE)
			strPrefix = _T("T");
		else
			strPrefix = _T("Φ");
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		strPrefix = _T("D");
	}
	else ASSERT(0);

	return strPrefix;
}

bool MSDeformedBar::IsCorrectRebarName( CString strName )
{
	strName.Trim();
	// D10+D13 처럼 교대배근인 경우 분리하여 리바네임 유효성 검사
	int PlusSignPos = strName.Find('+');
	if(PlusSignPos==-1)
	{
		char RebarNamePrefix = GetDefaultRebarNamePrefix()[0];
		int DiaPos = strName.Find(RebarNamePrefix);
		if(DiaPos==-1)
			return false;
// 		CString strRebarDia = strName.Right(strName.ReverseFind(RebarNamePrefix));
		int CurPos = 0;
		CString sDiaName = strName.Right(strName.GetLength()-DiaPos-1);
		int DiaIndex = FindBarDiameterIndexByName(sDiaName);
		if(DiaIndex == -1)
			return false;

		return true;
	}

	if(IsCorrectRebarName(strName.Left(PlusSignPos)) == false)
		return false;

	return IsCorrectRebarName(strName.Right(strName.GetLength()-PlusSignPos-1));
}

void MSDeformedBar::SetRebarCode(long nRebarCode)
{
	sm_nRebarCode = nRebarCode;
}

long MSDeformedBar::GetRebarCode()
{
	return sm_nRebarCode;
}

void MSDeformedBar::SetNationCode(long nNationCode)
{
	sm_nNationCode = nNationCode;
}

double MSDeformedBar::GetMinDia()
{
	CStringArray BarDiaArr;
	long numDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	MSDeformedBar bar(BarDiaArr[0]);
	return bar.m_Dia;
}

double MSDeformedBar::GetMaxDia()
{
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	MSDeformedBar bar(BarDiaArr[NumDia-1]);

	return bar.m_Dia;
}

long MSDeformedBar::GetRebarCodeKindList(CStringArray& rebarCodeKindList)
{
	rebarCodeKindList.RemoveAll();

	rebarCodeKindList.Add(_T("KS"));
	rebarCodeKindList.Add(_T("ASTM"));
	rebarCodeKindList.Add(_T("GB"));
	rebarCodeKindList.Add(_T("BS"));
	rebarCodeKindList.Add(_T("VIETNAM"));
	rebarCodeKindList.Add(_T("INDIA"));
	rebarCodeKindList.Add(_T("SINGAPORE"));
	rebarCodeKindList.Add(_T("JIS"));

	return rebarCodeKindList.GetSize();
}

CString MSDeformedBar::FindBarDiameterName(double dDeformedDia)
{
	CString DiaName = _T("");

	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	long BarIndex;
	for (BarIndex = 0; BarIndex < NumDia; BarIndex++)
	{
		MSDeformedBar bar(BarDiaArr[BarIndex]);
		if(fabs(bar.m_Dia-dDeformedDia)<DTOL1)
		{
			DiaName = BarDiaArr[BarIndex];
			break;
		}
	}
	if(BarIndex == NumDia)
		ASSERT(0);
	return DiaName;
}

CString MSDeformedBar::FindBarDiameterNameByIndex(int index)
{
	CString DiaName;

	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);

	if (index < 0 || index >= NumDia)
		DiaName = _T("");
	else
		DiaName = BarDiaArr.GetAt(index);

	return DiaName;
}

double MSDeformedBar::FindDeformedBarDiameterByIndex(int index)
{
	double dDia = 0;

	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);

	if (index < 0 || index >= NumDia)
		dDia = 0.;
	else
	{
		MSDeformedBar bar(BarDiaArr[index]);
		dDia = bar.m_Dia;
	}

	return dDia;
}
int MSDeformedBar::FindBarDiameterIndexByDia(double dDeformedDia)
{
	CString sDia = MSDeformedBar::FindBarDiameterName(dDeformedDia);
	return MSDeformedBar::FindBarDiameterIndexByName(sDia);
}
int MSDeformedBar::FindBarDiameterIndexByName(CString sDiaName)
{
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (int DiaIndex = 0; DiaIndex < NumDia; DiaIndex++)
	{
		CString sCompareDiaName = BarDiaArr[DiaIndex];
		if(sDiaName == sCompareDiaName || sDiaName == sCompareDiaName + 'S')
			return DiaIndex;
	}

	// 현재 리바코드에 없는 직경인 경우
	return -1;
}

double MSDeformedBar::FindDeformedBarDiameterByName(CString sDiaName)
{
	MSDeformedBar aBar(sDiaName);
	return aBar.m_Dia;
}


bool MSDeformedBar::IsIncludePrefix(CString strBar, vector<CString>& prefixes)
{
	int NumPrefix = prefixes.size();
	for(int iPrefix = 0; iPrefix < NumPrefix; iPrefix++)
	{
		CString sPrefix = prefixes[iPrefix];
		if(strBar.Find(sPrefix) != -1)
			return true;
	}
	return false;
}

int MSDeformedBar::GetStringIndex(CString strBar, vector<CString>& prefixes)
{
	int NumPrefix = prefixes.size();
	for(int iPrefix = 0; iPrefix < NumPrefix; iPrefix++)
	{
		CString sPrefix = prefixes[iPrefix];
		int index = strBar.Find(sPrefix);
		if(index != -1)
			return index;
	}

	// 현재 Profile과 NativeCode에서 재질코드와 상관없이 "HD"로 초기값이 설정되어 있어서 재질코드에 맞는 Prefix를 찾지 못한다. 
	// 모든 초기값을 재질코드에 맞게 Prefix를 설정하고, 각 윈도우 및 코드에서도 "HD"사용부분을 찾아서 수정해야 한다.

	return -1;
}

double MSDeformedBar::GetBarSpacing(CString strBar)
{
	strBar = GetBundleSpaceRemoveStr(strBar);

	vector<CString> prefixSpacings = GetRebarPrefixSpacings();
	if(!IsIncludePrefix(strBar, prefixSpacings))
		return 0.0;

	int IndexSpace = GetStringIndex(strBar, prefixSpacings);
	if(IndexSpace == -1)
		return 0.0;

	double Spacing = _ttof(strBar.Mid(IndexSpace+1, strBar.GetLength()-(IndexSpace+1)));
	return Spacing;
}

int MSDeformedBar::GetNumBars(CString strBar)
{
	strBar = GetBundleSpaceRemoveStr(strBar);

	vector<CString> prefixBarNums = GetRebarPrefixBarNums();
	if(!IsIncludePrefix(strBar, prefixBarNums))
		return 0;

	int IndexBarNum = GetStringIndex(strBar, prefixBarNums);
	if(IndexBarNum == -1)
		return 0;

	int startIndex = 0;
	for (int i = 0; i < IndexBarNum; i++)
	{
		char temp = strBar[i];

		// 입력된 키가 0 ~ 9 사이인가를 체크. 
		if (temp >= '0' && temp <= '9')
		{
			startIndex = i;
			break;
		}

	}
	CString strNum = strBar.Mid(startIndex, IndexBarNum);
	if(strNum.GetLength() > 0)
	{
		int BarNum = _ttol(strNum);
		return BarNum;
	}
	return 0;
}

double MSDeformedBar::GetDeformedBarDiameter(CString strBar)
{
	strBar = GetBundleSpaceRemoveStr(strBar);

	if(IsIncludePrefix(strBar, GetRebarPrefixSpacings()))
		return GetDeformedBarDiameter4StirrupBar(strBar);
	else
		return GetDeformedBarDiameter4MainBar(strBar);
}

CString MSDeformedBar::GetBarDiameterName(CString strBar)
{
	strBar = GetBundleSpaceRemoveStr(strBar);

	if(IsIncludePrefix(strBar, GetRebarPrefixSpacings()))
		return GetBarDiameterName4StirrupBar(strBar);
	else
		return GetBarDiameterName4MainBar(strBar);
}

CString MSDeformedBar::GetBarDiameterName4StirrupBar(CString strBar)
{
	CString sDiaName = _T("0");
	vector<CString> parserNames = GetParserNames();
	vector<CString> prefixSpacings = GetRebarPrefixSpacings();

	int IndexRebar = GetStringIndex(strBar, parserNames);
	int IndexSpace = GetStringIndex(strBar, prefixSpacings);

	if(IndexRebar == -1 || IndexSpace == -1)
		return sDiaName;

	return strBar.Mid(IndexRebar+1, IndexSpace-(IndexRebar+1));
}

CString MSDeformedBar::GetBarDiameterName4MainBar(CString strBar)
{
	CString sDiaName = _T("0");
	vector<CString> parserNames = GetParserNames();

	int IndexRebar = GetStringIndex(strBar, parserNames);

	if(IndexRebar == -1)
		return sDiaName;

	int count = 0;
	for (int i = IndexRebar + 1; i < strBar.GetLength(); i++)
	{
		char ch = strBar.GetAt(i);
		if (isdigit(ch))
		{
			count++;		
			continue;
		}
		if(count > 0 && ch == 'S')
		{
			count++;		
			continue;
		}
		if (ch == '.')
		{
			count++;		
			continue;
		}
		break;
	}

	return strBar.Mid(IndexRebar+1, count);
}

double MSDeformedBar::GetDeformedBarDiameter4StirrupBar(CString strBar)
{
	vector<CString> parserNames = GetParserNames();
	vector<CString> prefixSpacings = GetRebarPrefixSpacings();

	int IndexRebar = GetStringIndex(strBar, parserNames);
	int IndexSpace = GetStringIndex(strBar, prefixSpacings);

	if(IndexRebar == -1 || IndexSpace == -1)
		return 0.0;

	CString sDiaName = strBar.Mid(IndexRebar+1, IndexSpace-(IndexRebar+1));
	MSDeformedBar deformedBar(sDiaName);
	return deformedBar.m_Dia;
}

double MSDeformedBar::GetDeformedBarDiameter4MainBar(CString strBar)
{
	vector<CString> parserNames = GetParserNames();

	int IndexRebar = GetStringIndex(strBar, parserNames);

	if(IndexRebar == -1)
		return 0.0;

	CString sDiaName = strBar.Mid(IndexRebar+1, strBar.GetLength()-(IndexRebar+1));
	MSDeformedBar deformedBar(sDiaName);
	return deformedBar.m_Dia;
}

vector<CString> MSDeformedBar::GetParserNames()
{
	vector<CString> Prefixes;
	Prefixes.push_back(_T("D"));
	return Prefixes;
}

vector<CString> MSDeformedBar::GetRebarPrefixNames()
{
	vector<CString> Prefixes;
	CString strPrefix = _T("");
	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{
		Prefixes.push_back(_T("D"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{
		Prefixes.push_back(_T("#"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{
		if (ChinaDeformedBarCheck())
		{
			Prefixes.push_back(_T("D"));
		}
		else
		{
			Prefixes.push_back(_T("Φ"));
		}
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{
		if(sm_nNationCode==NATION_CODE_UAE)
			Prefixes.push_back(_T("T"));
		else
			Prefixes.push_back(_T("Φ"));
	}
	else if(sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		Prefixes.push_back(_T("D"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		if (sm_nNationCode == NATION_CODE_UAE)
			Prefixes.push_back(_T("T"));
		else
			Prefixes.push_back(_T("Φ"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		if (sm_nNationCode == NATION_CODE_UAE)
			Prefixes.push_back(_T("T"));
		else
			Prefixes.push_back(_T("Φ"));
	}
	if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		Prefixes.push_back(_T("D"));
	}
	else ASSERT(0);

	return Prefixes;
}

vector<CString> MSDeformedBar::GetRebarPrefixSpacings()
{
	vector<CString> Prefixes;
	CString strPrefix = _T("");
	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		Prefixes.push_back(_T("@"));
		Prefixes.push_back(_T("a"));
	}
	else ASSERT(0);

	return Prefixes;
}

vector<CString> MSDeformedBar::GetRebarPrefixBarNums()
{
	vector<CString> Prefixes;
	CString strPrefix = _T("");
	if(sm_nRebarCode==REBAR_MATERIAL_KS)
	{
		Prefixes.push_back(_T("-"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_ASTM)
	{
		Prefixes.push_back(_T("-"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_GB)
	{
		Prefixes.push_back(_T("-"));
	}
	else if(sm_nRebarCode==REBAR_MATERIAL_BS)
	{
		Prefixes.push_back(_T("-"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_VIETNAM)
	{
		Prefixes.push_back(_T("-"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_INDIA)
	{
		Prefixes.push_back(_T("-"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_SINGAPORE)
	{
		Prefixes.push_back(_T("-"));
	}
	else if (sm_nRebarCode == REBAR_MATERIAL_JIS)
	{
		Prefixes.push_back(_T("-"));
	}
	else ASSERT(0);

	return Prefixes;
}

// 중국철근은 Prefix로 "Φ"표시를 사용하는데 현재 해당 표시로 입력받을수 없는 구조로 되어 있다.
// 따라서 우선 "Φ"표시를 사용하는 부분은 한국철근에서 사용하는 Prefix 사용법으로 변경
bool MSDeformedBar::ChinaDeformedBarCheck()
{
	return true;
	//return false;
}

double MSDeformedBar::GetWeightByName(CString sDiaName)
{
	MSDeformedBar aBar(sDiaName);
	return aBar.m_Weight;
}

long MSDeformedBar::GetBundleNum(CString strBar)
{
	long nBundleNum = 0;

	strBar = GetBundleSpaceRemoveStr(strBar);

	CString rebarName = m_sDefaultBarType;
	int indexRebarName = strBar.Find(rebarName);
	if (indexRebarName == -1)
		return nBundleNum;

	if (IsIncludePrefix(strBar, GetRebarPrefixSpacings()))
	{
		if (indexRebarName == 0)
			return nBundleNum;

		nBundleNum = _ttoi(strBar.Left(indexRebarName));
	}
	else
	{
		vector<CString> prefixBarNums = GetRebarPrefixBarNums();
		if (!IsIncludePrefix(strBar, prefixBarNums))
			return nBundleNum;

		int IndexBarNum = GetStringIndex(strBar, prefixBarNums);
		if (IndexBarNum == -1)
			return nBundleNum;

		CString strNum = strBar.Mid(IndexBarNum+1, indexRebarName - IndexBarNum);
		if (strNum.GetLength() > 0)
		{
			nBundleNum = _ttol(strNum);
		}
	}

	return nBundleNum;
}

long MSDeformedBar::GetBundleSpace(CString strBar)
{
	long nBundleSpace = 0;

	int equalIndex = strBar.ReverseFind('=');
	int spaceEndIndex = strBar.ReverseFind(']');
	if (equalIndex == -1 || spaceEndIndex == -1 || equalIndex >= spaceEndIndex)
		return nBundleSpace;

	CString strBundleSpace = strBar.Mid(equalIndex + 1, spaceEndIndex - (equalIndex + 1));
	if (strBundleSpace.GetLength() > 0)
	{
		nBundleSpace = _ttol(strBundleSpace);
	}

	return nBundleSpace;
}

CString MSDeformedBar::GetBundleSpaceRemoveStr(CString strBar)
{
	strBar.Replace(_T(" "), _T(""));
	strBar = strBar.MakeUpper();
	CString editStrBar = strBar;

	int spaceStartndex = strBar.Find(_T("[B@="));
	if (spaceStartndex != -1)
		editStrBar = strBar.Mid(0, spaceStartndex);

	return editStrBar;
}
CString MSDeformedBar::GetDiaStr(double dDia)
{
	CString sDia = _T("");
	if(dDia > (int)dDia)
		sDia.Format(_T("%.1f"), dDia);
	else
		sDia.Format(_T("%d"), (int)dDia);
	return sDia;
}
CString MSDeformedBar::GetMainBarStr(long numBar, double dDia, CString sBarType)
{
	if(dDia == 0)
		dDia = GetDefaultDia();
	CString sBar = _T("");		
	sBar.Format(_T("%ld-%s%s"), numBar, sBarType, GetDiaStr(dDia));
	return sBar;
}
CString MSDeformedBar::GetStirrupBarStr(long nSpacing, double dDia, CString sBarType)
{
	if(dDia == 0)
		dDia = GetDefaultDia();
	CString sBar = _T("");		
	sBar.Format(_T("%s%s@%ld"), sBarType, GetDiaStr(dDia), nSpacing);
	return sBar;
}
bool MSDeformedBar::IsUserSteelGradeS(CString str)
{
	return str.Find('S') != -1;
}
bool MSDeformedBar::EnabledUserSteelGradeS(CString name)
{	
	if(name == STR_SD400 || name == STR_SD500 || name == STR_SD600)
		return true;
	return false;
}