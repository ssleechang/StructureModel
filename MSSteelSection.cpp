#include "StdAfx.h"
#include "MSSteelSection.h"
#include "GMLib/GM2DRectangle.h"
double MSSteelSection::_STEEL_DENSITY = 7850.0/1000000000.0;//Area가 mm2 -> m2 ,density가 kg 단위이므로 ton으로 바꿔줘야함 
MSSteelSection::MSSteelSection(void)
{
	Init();
}
void MSSteelSection::Init()
{
	m_ID = 0;
	m_Name =_T("");
	m_nSecType = -1;
	m_A = 0.0;
	m_w = 0.0;
	m_DataType = 0;
}
MSSteelSection::~MSSteelSection(void)
{
}

CString MSSteelSection::GetDataPath()
{
	CString DataPath = _T("");
	TCHAR szFilePath[_MAX_PATH];
	GetModuleFileName(NULL, szFilePath, _MAX_PATH);

	CString path(szFilePath);
	int pos = path.ReverseFind('\\');
	if (pos >= 0)
	{
		path = path.Left(pos);    
	}
	CString strDatapath = path;
	pos = strDatapath.ReverseFind('\\');
	if (pos >= 0)
	{
		strDatapath = strDatapath.Left(pos);    
	}

	DataPath.Format(_T("%s\\Data\\"), strDatapath);
	return DataPath;
}

long MSSteelSection::ReadFileData(CString strFile, vector<MSSteelFileInfo*>& DataArr)
{
	FILE* fp;

    //jissi added
    wchar_t* wchar_str = strFile.GetBuffer(strFile.GetLength());
    int _str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
    char* char_str = new char[_str_len];
    WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, _str_len, 0,0);  

	fopen_s(&fp, char_str, "r");
	delete[] char_str;
	if(fp==NULL)	return long(DataArr.size());

	char TempLine[256];
	CString TextLine = _T("");
	long nValue = 0;

	CString strKey;
	fpos_t pos;
	while(TRUE)
	{
		fgetpos(fp, &pos);
		if(!fgets(TempLine, sizeof(TempLine), fp)) break;

		TextLine.Format(_T("%s"), TempLine);
		long nCommandType = GetCommandType(TextLine);
		if(nCommandType >= 0)
			continue;

		MSSteelFileInfo* pFileInfo = new MSSteelFileInfo;
		ParsingData(TextLine, pFileInfo->m_Data);
		DataArr.push_back(pFileInfo);
	}
	fclose(fp);
	return long(DataArr.size());
}
long MSSteelSection::GetCommandType(CString TextLine)
{
 	if(TextLine.Find(_T("//")) >= 0)	return 0;
	return -1;
}
long MSSteelSection::ParsingData(CString TextLine, CStringArray& ValueArr)
{
	ValueArr.RemoveAll();

	TextLine.TrimLeft();
	TextLine.TrimRight();
	if(TextLine==_T("")) return 0;

	CString strValidText = _T("");
	CString strValue = _T("");

	long nPos = TextLine.Find(_T(";"));
	if(nPos >= 0)
	{
		strValidText = TextLine.Mid(0, nPos);
	}
	else strValidText = TextLine;
	if(strValidText=_T("")) return 0;

	long nPos1 = 0, nPos2 = 0;
	while(TRUE)
	{
		TextLine = strValidText;

		nPos = TextLine.Find(_T(","));
		nPos1 = TextLine.Find('"');
		if(nPos1 >= 0 && nPos1 < nPos)
		{
			strValidText = TextLine.Mid(nPos1+1);
			TextLine = strValidText;
			nPos2 = TextLine.Find('"');
			if(nPos2 >= 0)
			{
				strValue = TextLine.Mid(0, nPos2);
				strValue.TrimLeft();
				ValueArr.Add(strValue);
				strValidText = TextLine.Mid(nPos2+1);
				nPos = strValidText.Find(_T(","));
				if(nPos >= 0)
				{
					TextLine = strValidText;
					strValidText = TextLine.Mid(nPos+1);
				}
			}
		}
		else
		{
			if(nPos >= 0)
			{
				strValue = TextLine.Mid(0, nPos);
				strValidText = TextLine.Mid(nPos+1);
				strValue.TrimLeft();
				ValueArr.Add(strValue);
			}
			else
			{
				strValue = TextLine;
				strValue.TrimLeft();
				ValueArr.Add(strValue);
				break;
			}
		}
	}
	return ValueArr.GetSize();
}

double MSSteelSection::GetPerimeter()
{
	double peri = 0;
	GM2DPolyline gon(TRUE);
	GetSecPolyline(gon);
	peri += gon.GetLength();

	return peri;
}

bool MSSteelSection::IsGeometryDifferent(MSSteelSection* section)
{
	if (GetSectionType() != section->GetSectionType()) return true;
	if (GetH() != section->GetH()) return true;

	if (GetB() != section->GetB()) return true;
	if (GetB1() != section->GetB1()) return true;
	if (GetB2() != section->GetB2()) return true;
	if (GetWThk() != section->GetWThk()) return true;
	if (GetWThk1() != section->GetWThk1()) return true;
	if (GetWThk2() != section->GetWThk2()) return true;
	if (GetFThk() != section->GetFThk()) return true;
	if (GetFThk1() != section->GetFThk1()) return true;
	if (GetFThk2() != section->GetFThk2()) return true;

	return false;
}

void MSSteelSection::GetSecPolyline(GM2DPolyline& TheGon)
{

	double m_dWidth = GetB();
	double m_dDepth = GetH();
	GM2DRectangle TheRect;
	double dMaxVal = m_dWidth > m_dDepth ? m_dWidth : m_dDepth;

	double dXc1 = -m_dWidth / 2;
	double dYc1 = -m_dDepth / 2;
	double dXc2 = +m_dWidth / 2;
	double dYc2 = +m_dDepth / 2;
	GM2DVector* pVec1 = new GM2DVector(dXc1, dYc1);
	GM2DVector* pVec2 = new GM2DVector(dXc2, dYc1);
	GM2DVector* pVec3 = new GM2DVector(dXc2, dYc2);
	GM2DVector* pVec4 = new GM2DVector(dXc1, dYc2);

	TheGon.AddPointAndBulge(pVec1);
	TheGon.AddPointAndBulge(pVec2);
	TheGon.AddPointAndBulge(pVec3);
	TheGon.AddPointAndBulge(pVec4);
	TheGon.MakeClosedPolyline(DTOL_GM);
	TheGon.MakeCCW();
}

CString MSSteelSection::MakeName(vector<double>& params)
{
	CString makeName=_T("");
	CString temp;
	int paramsize;
	paramsize = params.size();
	for (int i = 0; i < paramsize; i++)
	{
		if (fabs(params[i] - (int)params[i]) < DTOL1)
			temp.Format(_T("%.0f"), params[i]);
		else
			temp.Format(_T("%.1f"), params[i]);

		makeName += temp;
		if (i != paramsize-1)
			makeName += _T("X");
	}
	
	return makeName;
}

bool MSSteelSection::IsSameSectionName(CString oldName, CString compareName)
{
	if (oldName.MakeUpper() == compareName.MakeUpper())
		return true;

	CString oldUpperName = oldName.MakeUpper();
	int oldindexDash = oldUpperName.Find('-');
	CString oldSizeName = oldUpperName.Mid(oldindexDash + 1, oldUpperName.GetLength() - (oldindexDash + 1));
	int oldIndexDash2 = oldSizeName.Find('-');
	if (oldIndexDash2 != -1)
	{
		oldSizeName = oldSizeName.Mid(0, oldIndexDash2);
	}
		
	CString compareUpperName = compareName.MakeUpper();
	int indexDash = compareUpperName.Find('-');
	CString compareSizeName = compareUpperName.Mid(indexDash + 1, compareUpperName.GetLength() - (indexDash + 1));

	if (oldSizeName == compareSizeName)
		return true;

	return false;
}
