#pragma once
#include "MSObject.h"

class StructureModel_CLASS ViewDisplaySetting : public MSObject
{
public:
	ViewDisplaySetting();
	~ViewDisplaySetting();
	void Serialize(CArchive& archive);
	DECLARE_SERIAL(ViewDisplaySetting)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	bool GetReassignView2D();
	void SetReassignView2D(bool bReassignView2D);

	void GetLineColor2D(int& red, int& green, int& blue);
	void SetLineColor2D(int red, int green, int blue);
	
	CString GetLineTypeName2D();
	void SetLineTypeName2D(CString lineTypeName2D);

	int GetLineWeight2D();
	void SetLineWeight2D(int lineWeight2D);

	CString GetHatchPatternName2D();
	void SetHatchPatternName2D(CString hatchPatternName2D);

	void GetHatchColor2D(int& red, int& green, int& blue);
	void SetHatchColor2D(int red, int green, int blue);

	double GetHatchScale2D();
	void SetHatchScale2D(double hatchScale2D);

	double GetAlphaBlending2D();
	void SetAlphaBlending2D(double alphaBlending2D);

	bool GetReassignView3D();
	void SetReassignView3D(bool bReassignView3D);

	void GetPatternColor3D(int& red, int& green, int& blue);
	void SetPatternColor3D(int red, int green, int blue);

	double GetAlphaBlending3D();
	void SetAlphaBlending3D(double alphaBlending3D);

private :
	bool m_bReassignView2D;
	COLORREF m_LineColor2D;
	CString m_LineTypeName2D;
	int m_LineWeight2D;
	CString m_HatchPatternName2D;
	COLORREF m_HatchColor2D;
	double m_HatchScale2D;
	double m_AlphaBlending2D;

	bool m_bReassignView3D;
	COLORREF m_PatternColor3D;
	double m_AlphaBlending3D;
};
