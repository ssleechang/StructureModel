#pragma once
#include "afx.h"

class SRCell;
class GMText;
class MTReader;
class GM2DRectangle;

class StructureModel_CLASS SRTableLine : public CObject
{
public:
	SRTableLine(GM2DLineVector* pLVec);
	~SRTableLine(){};

	GM2DLineVector* mp_LVec;

	CTypedPtrArray<CObArray, SRTableLine*> ma_TopLeftLine;

	bool CheckCrossWithVerticalLine( SRTableLine* pVLine, double MinHeight, double MinWidth );
};

class StructureModel_CLASS SRTable :
	public CObject
{
public:
	SRTable(void);
	~SRTable(void);

	long m_Row1Type, m_Row2Type;
	long m_Col1Type, m_Col2Type;
	long m_RemainText;
	long m_NumBoxRow;

	double m_TOL_MinCellHeight;
	double m_TOL_MinCellWidth;
	SRCell* mp_Cells;
	int m_nRows;
	int m_nColumns;
	double m_MaxX, m_MaxY, m_MinX, m_MinY;

	double m_BoxHeight;
	double m_CellHeight;

	MTReader* mp_Reader;
	CTypedPtrArray<CObArray, GM2DLineVector*> ma_Line;
	CTypedPtrArray<CObArray, GMText*> ma_Text;
	CTypedPtrArray<CObArray, GMText*> ma_EtcText;

	long MakeSectionTable();
	long MakeSectionBox();
	CTypedPtrArray<CObArray, SRTableLine*> ma_HLine;
	CTypedPtrArray<CObArray, SRTableLine*> ma_VLine;
	CTypedPtrArray<CObArray, GM2DRectangle*> ma_Box;
	// 
	CArray<double, double> ma_XLoc;
	CArray<double, double> ma_YLoc;

	void DeleteLineArr();
	void DeleteTextArr();
	void DeleteHLineArr();
	void DeleteVLineArr();
	void DeleteBoxArr();
	// 	CArray<double, double> ma_ColGroupLoc;
	// 	CArray<double, double> ma_RowGroupLoc;
	// 	CArray<long, long> ma_ColGroupIdx;
	// 	CArray<long, long> ma_RowGroupIdx;
	// 
	virtual void Init();
	
	bool AddLine(double sX1, double sY1, double eX2, double eY2);
	bool AddText(double cenX, double cenY, CString text, double degree, double height);

	// 
	// 	char AddLine(GM2DLineVector* pLine);
	virtual BOOL SetupFrame( CTypedPtrArray<CObArray, GM2DLineVector*>& GMLineArr);
	// 	BOOL GetCellAddress(GMText* pText, long* RowNum, long* ColNum);
	// 	BOOL GetCellAddress( GMCircle* pMark, long* RowNum, long* ColNum );
	// 	virtual BOOL ReadCells( CTypedPtrArray<CObArray, GMText*>& GMTextArr);
	// 	virtual BOOL ReadRebarMarks( CTypedPtrArray<CObArray, GMCircle*>& GMCircleArr);
	// 
	// 	CTypedPtrArray<CObArray, T_BoxInfo*> ma_BoxInfo;
	// 	void DeleteBoxInfo();
	// 	BOOL SetupFrame4Box();
	// 	void SetTypeCells4Box();
	// 	virtual BOOL ReadCells4Box( CTypedPtrArray<CObArray, GMText*>& GMTextArr);
	// 	virtual void BuildMembers4Box(){};
	// 	BOOL ReadRebarMarks4Box(CTypedPtrArray<CObArray, GMCircle*>& GMCircleArr);
	// 	void SetBoxInfo();
	long SortValueArr(CArray<double, double>& ValueArr, BOOL bMinOrder=TRUE);
	// 	T_BoxInfo* GetBoxInfo(double dXc, double dYc);
	// 	long GetBoxYcArr(CArray<double, double>& YcArr);
	// 	long GetBoxXcArr(double dYc, CArray<double, double>& XcArr);
	// 	virtual long AnalyzeText4Box(GMText* pText){return 0;};
	// 	virtual long AnalyzeText4BoxHeader(GMText* pText);
	// 	T_BoxInfo* GetCellAddress4Box( GMVector TheVec, long* RowNum, long* ColNum );
	// 	BOOL GetBoxArea_X(GMVector TheVec, long& nRow, long& nRow2, long& nCol, long& nCol2);
	// 	BOOL GetBoxArea_Y(GMVector TheVec, long& nRow, long& nRow2, long& nCol, long& nCol2);
	// 	T_BoxInfo* GetCellAddress4Box(long nRowNum, long nColNum);
	// 	long GetCellType(GMVector TheVec);
	// 
	// 	virtual long GetWallNum(long RowNum, long ColNum) {return -1;};
	// 	virtual long AnalyzeText(GMText* pText) {return 0;};
	// 	virtual void BuildMembers(){};
	// 
	virtual BOOL IsValidLine(GM2DLineVector* pLine);
	// 
	virtual BOOL ClassifyVLines();
	virtual BOOL ClassifyHLines();
	void SetMinimumTolerence(CTypedPtrArray<CObArray, GMText*>& GMTextArr);
	// 	virtual BOOL AssignColumn();
	// 	virtual BOOL AssignRow();
	// 	virtual BOOL IsWallBayName(long RowNum, long ColNum) {return FALSE;};
	// 	virtual MTCell* AddOrMergeCell(long RowNum, long ColNum, GMText* pText);
	// 	virtual MTCell* AddOrMergeCell(long RowNum, long ColNum, long nCellType, GMText* pText);
	// 	long GetFloorNumFromText(CString sText);
	// 
	// 	virtual long AnalyzeLabel(GMText *pText, long RowNum, long ColNum) {return 0;};
	// 	virtual MTMember* MakeNewMember(GMText* pText)  {return NULL;};
	// 	BOOL GetCellLocYFromCellColNum(long CellColNum, double& FromY, double& ToY);
	// 
	// 	virtual long WriteMembers(){return 0;};
	// 	void SetTypeCells();

	int GetBoxCount();

};

