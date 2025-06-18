#pragma once
#include "StructureModelEnum.h"
#include "MSLineMemberM.h"

class T_BarLoc;
class MSMainBar;
class MRRcSection;
class MREdge;
class MSMainBarSet;
class StructureModel_CLASS MSRcLineMemberM :
	public MSLineMemberM
{
public:
	MSRcLineMemberM(void);
	~MSRcLineMemberM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSRcLineMemberM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual double GetCover();
	virtual void SetCover(double dCover);

	enum SECT_SHAPE {RECT=1, CIRCLE, POLYGON, POLYGONINPLAN};
	
	GMPolygon m_SecPGon;

	virtual double GetWidth() {return m_dWidth;}
	virtual double GetDepth();
    virtual double GetThick(){return m_dDepth;};//by LSS // mylee - changed to depth (Kim Ha Na)
	virtual SECT_SHAPE GetShape() { return me_Shape;};
	virtual void SetWidth(double Width) {m_dWidth = Width;}
	virtual void SetDepth(double Depth) {m_dDepth = Depth;};
	virtual void SetThick(double Thick) {m_dDepth = Thick;};
	virtual void SetShape(int Shape) { me_Shape = (SECT_SHAPE)Shape;};
	virtual void GetSecPolyline(GM2DPolyline& APGon);
	virtual void GetHeaderPolyline(GM2DPolyline& APGon);
	virtual vector<GM2DPolyline> GetSubHeaderPolyline();

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual CString ToString();

	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	int GetLayerNumbyBarType(eColumnBarType eType);
	int GetLayerBarCnt(int nLayerIdx, eColumnBarType eType = eColumnBarType::ColumnBarType_AllBar);
	void GetMainBarLoc(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0, eColumnBarType eType = eColumnBarType::ColumnBarType_AllBar);
	void GetMainBarLoc_Circle(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0);
	void GetMainBarLoc_Rect(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0, eColumnBarType eType = eColumnBarType::ColumnBarType_AllBar);
	void GetMainBarLoc_Polygon(vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0);
	
	long GetRootType(eColumnBarType eType);
	CString GetInitial(eColumnBarType eType);
	int GetMainBarLoc_BarLine(GM2DLineVector& barLine, MSMainBarSet& mainBars, bool placingConer, vector<T_BarLoc*>& MainBarLocArr, eColumnBarType eType, double dLayerDist, int nTopNumLayer);
	int GetMainBarLoc_BarLineByDist(GM2DLineVector& barLine, MSMainBarSet& mainBars, bool placingConer, vector<T_BarLoc*>& MainBarLocArr, eColumnBarType eType, double dLayerDist);

	long SetMainBarLoc_Sec(MRRcSection* pSec, vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0);
	long SetMainBarLoc_Edge(MREdge* pEdge, vector<T_BarLoc*>& MainBarLocArr, double dAxisRotation = 0);

	virtual double GetSpecificTieHeight(CString sInitial, GM2DLineVector* TheLine = NULL);

	long GetColumnHeaderMID()				{ return m_ColumnHeaderMID;};
	void SetColumnHeaderMID(long ID)	{ m_ColumnHeaderMID = ID;};
	void SetCornerBarInfo(vector<T_BarLoc*>& MainBarLocArr);

	double GetDepthDefaultValue();

protected:
	double m_dWidth;
	double m_dDepth;
	SECT_SHAPE me_Shape;
	long m_ColumnHeaderMID;
};
