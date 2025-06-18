#pragma once
#include "afx.h"

#define CURVE_TYPE		1
#define OUTLINE_TYPE	2
#define STRPLINE_TYPE	3
#define TIELINE_TYPE		4
#define MAIN_CIRCLE_TYPE	5
#define SUB_CIRCLE_TYPE		6
class MTReader;
class GMText;
class SRSectionBox;
class SRDimension;
class GM2DRectangle;

#include "MTReaderLayerHelper.h"
class StructureModel_CLASS SRSectionTable :
	public CObject
{
public:
	SRSectionTable(MTReader* pReader);
	~SRSectionTable(void);

	MTReader*			mp_Reader;
	GM2DRectangle*		mp_Rect;
	GMText*					mp_Name;
	CTypedPtrArray<CObArray, GMText*> ma_Name;
	MTReaderLayerHelper m_LayerHelper;

	int m_VdId;
	long m_nFromStory, m_nToStory;
	double m_Width, m_Depth;
	CString m_sWidth, m_sDepth;
	CString m_sDimensionWidth, m_sDimensionDepth;
	long m_nMaterialID;

	bool m_bStandardSection;
	//	enum MSLineMemberM :: SECT_SHAPE {RECT=1, CIRCLE, POLYGON, POLYGONINPLAN}
	long m_Shape;
	double m_dFactor;
	CString m_SteelSectionName;
	CString m_StudBolt;

	CTypedPtrArray<CObArray, SRSectionBox*> ma_Box;
	CTypedPtrArray<CObArray, SRDimension*> ma_SRDim;
	CTypedPtrArray<CObArray, GMText*>   ma_Text;
	CTypedPtrArray<CObArray, GMCircle*> ma_Circle;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_Curve;
	CTypedPtrArray<CObArray, GMCircle*> ma_SubMainBarCircle;

protected:
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_OutLine;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_StrpLine;
	CTypedPtrArray<CObArray, GM2DGeometry*> ma_TieLine;
	CTypedPtrArray<CObArray, GM2DPolyline*> ma_Polyline;
public:
	int AddLine(GM2DLineVector* pNewLine, int lineType);
	int AddPolyline(GM2DPolyline* polyline);

	bool Contains(GM2DVector* pVec1, GM2DVector*  pVec2);
	GMCircle* FindGMCircle(GM2DVector CenPnt, double dRadius);

	void MakeData();
	void MakeSectionBox();
	
	void MakeSectionBox_RcBeam();
	void MakeSectionBox_SteelBeam();
	void MakeSectionBox_SrcBeam();

	void MakeSectionBox_RcColumn();
	void MakeSectionBox_SteelColumn();
	void MakeSectionBox_SrcColumn();

	void MergeLineVector();
	void MergeLineVector(CTypedPtrArray<CObArray, GM2DGeometry*>* pGeomDataArr);

	void CopySRDimOfBeam();

	int GetBeamSectionCount();
	long GetTopBarNum();
	void CopyData(SRSectionBox* pBox, bool bCalcLoc);
	void CopyData(SRSectionBox* pBox, CTypedPtrArray<CObArray, GM2DGeometry*>& DataArr, bool bCalcLoc, long nDataType);
	void CopyCircleData(SRSectionBox* pBox, CTypedPtrArray<CObArray, GMCircle*>& CircleArr, bool bCalcLoc, long nDataType);

	long GetSectionName(CStringArray& strNames);
	long GetName(CString name, CStringArray& strNames);
	long AddDimension(SRDimension* pNewDim);
	void SetSizeInfo();
	void ParsingSize(CString strData);


	void SetErrorCode(long errorCode){ m_nErrorCode |= errorCode;};
protected:
	long m_nErrorCode;
	long m_nWarningCode;
public:
	long GetErrorCode(){return m_nErrorCode;};
	long GetWarningCode(){return m_nWarningCode;};
	long MakeErrorObject(long nNO);
	long MakeErrorObject_Column(long nNO);
	long MakeErrorObject_Wall(long nNO);
	long MakeErrorObject_Beam(long nNO);
	long MakeDimensionFromLine();
	CString GetBeamSectionName(long BoxNumber);

	enum { ERROR_NO_BOX = 1,
		ERROR_NO_NAME = 2,
		ERROR_STORY = 4,
		ERROR_SIZE = 8,
		ERROR_MATERIAL = 16,
		ERROR_MAINBAR = 32,
		ERROR_STRPBAR1 = 64,
		ERROR_STRPBAR2 = 128,
	};
	enum { WARNING_MAINBAR_NUM = 1,
		WARNING_STRPBAR1_NUM = 2,
		WARNING_STRPBAR2_NUM = 4,
	};

	void MakeData_Excel();
	int AddBoxData(int type, int row, CString data);

	void AddTextByLayerName(CString layerName, GMText* pText);
	void AddDimensionByLayerName(CString layerName, SRDimension* pDimension);

	vector<GMText*> GetTextsToLayerName(int cellType);
	GMText* GetFirstTextToLayerName(int cellType);
	vector<SRDimension*> GetDimensionsToLayerName(int cellType);
	SRDimension* SRSectionTable::GetFirstDimensionsToLayerName(int cellType);

	void AnalyizeWallM_byLH();
protected:
	void DeleteBoxArr();
	void DeleteSRDimArr();
	void DeleteTextArr();
	void DeleteCurveArr();
	void DeleteOutLineArr();
	void DeleteStrpLineArr();
	void DeleteTieLineArr();
	void DeleteCircleArr();
	void DeleteSubMainBarCircleArr();
	void RemoveCurve(GM2DGeometry* pCurve);
	CString RemoveTextInBracket(CString text);
public:
	void AddCellTypeToGeometry(long cellType, GMObject* pGeom);
private:
	map<long, vector<GMObject*>> mm_CellTypeToGeoms;
	void CopyCellTypeToGeometry(SRSectionBox* pBox);

	void SetSectionBoxSection(SRSectionBox* pBox, vector<GMObject*> geoms);
	void SetSectionBoxMainBar(SRSectionBox* pBox, vector<GMObject*> geoms);
	void SetSectionBoxTieBar(SRSectionBox* pBox, vector<GMObject*> geoms);
	void SetSectionBoxSubMainBar(SRSectionBox* pBox, vector<GMObject*> geoms);

	bool IsContained(SRSectionBox* pBox, GM2DPolyline* polyline);
	bool IsContained(SRSectionBox* pBox, GMCircle* circle);
	bool IsContained(SRSectionBox* pBox, GM2DLineVector* line);
};

