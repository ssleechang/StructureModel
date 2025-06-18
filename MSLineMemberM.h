#pragma once
#include "mscompom.h"

class StructureModel_CLASS MSLineMemberM :
	public MSCompoM
{
public:

	MSLineMemberM(void);
	~MSLineMemberM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSLineMemberM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual CString ToString();
	virtual void GetSecPolyline(GM2DPolyline& APGon){};
	virtual long GetHaunchType(){return 0;}// by LSS for WallFoot

	virtual double GetLevel(){return 0.0;};// by gracias for LevelDifM
	virtual void SetLevel(double dLevel){};// by gracias for LevelDifM


//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//public:
//	virtual void ImportProfile(int pIfcId); // called by MSCompoG.cpp not by this->InnerImportIFC()
//	virtual int ExportProfile(double pWidth); // called by MSCompoG.cpp not by this->InnerExportIFC()
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};
