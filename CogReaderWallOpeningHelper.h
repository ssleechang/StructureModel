#pragma once
#include "afx.h"
#include "CogReaderHelper.h"

class MSBaseBuilding;
class CogReader;
class MSFloor;
class MSWallMemberG;
class MSWallOpeningG;
class MSCompoM;
class GMText;
class StructureModel_CLASS CogReaderWallOpeningHelper : public CogReaderHelper
{
public:
	CogReaderWallOpeningHelper(MSBaseBuilding* pBldg, CogReader* pReader, MSFloor* pFromFloor, MSFloor* pToFloor);
	~CogReaderWallOpeningHelper(void);

	bool MakeData();

protected:
	void AnalyizeGeometry();
	void GetExistedOpeningMs();

	void MakeWallOpenings(int story);
	vector<MSWallMemberG*> GetWallGs(MSFloor* pFloor);
	void MakeWallOpenings_Line(vector<MSWallMemberG*>& wallGs);
	void MakeWallOpenings_Arc(vector<MSWallMemberG*>& wallGs);

	MSWallOpeningG* CreateWallOpeningG();
	GMText*					 GetGMText_ContainPolyline(GM2DPolyline* pPolyline, vector<GMQuadTree*>& CrossQts);
	vector<GMText*>   GetGMTexts_ContainPolyline(GM2DPolyline* pPolyline, vector<GMQuadTree*>& CrossQts);
	MSCompoM*		FindWallOpeningM(GM2DPolyline* pBoundary, GM2DGeometry* pGeometry);
	MSCompoM*		FindCompoMByCombinationName(vector<GMText*>& TextArr);

	void MakeWallOpenings_Line(MSWallMemberG* pWallG, GM2DLineVector* pLineVec, MSCompoM* pOpeningM);
	void MakeWallOpenings_Arc(MSWallMemberG* pWallG, GM2DArc* pArc, MSCompoM* pOpeningM);
	double GetBottomOffset(MSFloor* pFloor);

protected:
	MSFloor*      mp_FromFloor;
	MSFloor*		mp_ToFloor;

	vector<MSCompoM*> ma_OpeningM;
	vector<GM2DLineVector*> ma_Lines;
	vector<GM2DArc*> ma_Arc;
};

