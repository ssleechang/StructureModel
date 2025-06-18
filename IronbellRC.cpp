///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// common header
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "IFCLib\IFCLib.h"




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSRcIsoFootM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSRcIsoFootM.h"

void MSRcIsoFootM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
{
	char* mName = NULL; double mWidth = 0, mDepth = 0, mRadius = 0, mH = 0, mB = 0, mtw = 0, mtf = 0, mr = 0;
	if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), pIfcId) == true) // import rectangle profile
	{
		importRectangleProfileDefInstance(pIfcId, &mName, mWidth, mDepth);
	}
	else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), pIfcId) == true) // import circle profile
	{
		importCircleProfileDefInstance(pIfcId, &mName, mRadius);
	}
	else  // import H-section profile
	{
		importIShapeProfileDefInstance(pIfcId, &mName, mH, mB, mtw, mtf, mr);
	}

	// for wall, slab, isolated footing
	this->m_dWidth = mWidth;
	this->m_dDepth = mDepth;
}

int MSRcIsoFootM::ExportProfile(double pWidth) // called by MSCompoG.cpp not by this->InnerExportIFC()
{
	return MSFaceMemberM::ExportProfile(pWidth);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSRcWallFootM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSRcWallFoot.h"

void MSRcWallFootM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
{
	MSLineMemberM::ImportProfile(pIfcId);
	this->m_dThick = this->m_dDepth;
}

int MSRcWallFootM::ExportProfile(double pWidth) // called by MSCompoG.cpp not by this->InnerExportIFC()
{
	return MSLineMemberM::ExportProfile(pWidth);
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSRcHandRailM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSRcHandRailM.h"

void MSRcHandRailM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
{
	//MSLineMemberM::ImportProfile(pIfcId);

	char* mName = NULL; double mWidth = 0, mDepth = 0, mRadius = 0, mH = 0, mB = 0, mtw = 0, mtf = 0, mr = 0; GMPolyline* mProfile;
	if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), pIfcId) == true) // import rectangle profile
	{
		importRectangleProfileDefInstance(pIfcId, &mName, mWidth, mDepth);
		this->me_Shape = MSLineMemberM::RECT;
	}
	//else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), pIfcId) == true) // import circle profile
	//{
	//	importCircleProfileDefInstance(pIfcId, &mName, mRadius);
	//	this->me_Shape = MSLineMemberM::CIRCLE;
	//}
	//else if(compareEntityType(CString("IFCARBITRARYCLOSEDPROFILEDEF"), pIfcId) == true) // import H-section profile
	//{
	//	importIShapeProfileDefInstance(pIfcId, &mName, mH, mB, mtw, mtf, mr);
	//	this->me_Shape = MSLineMemberM::POLYGON;
	//}
	else if (compareEntityType(CString("IFCARBITRARYCLOSEDPROFILEDEF"), pIfcId) == true) // import arbitrary shaped section profile
	{
		importArbitraryClosedProfileDefInstance(pIfcId, &mName, mProfile);
		this->me_Shape = MSLineMemberM::POLYGON;

		// get width, depth, level
		//mWidth
	}

	// for railing
	switch (this->me_Shape)
	{
	case MSLineMemberM::RECT:
		this->m_dWidth = mWidth;
		this->m_dDepth = mDepth;
		this->m_nParapetType = 0; // no parapet
		break;
	case MSLineMemberM::POLYGON:
		this->m_dWidth = mWidth;
		this->m_dDepth = mDepth;
		this->m_nParapetType = 1; // default parapet
	}
}

int MSRcHandRailM::ExportProfile(double pWidth) // called by MSCompoG.cpp not by this->InnerExportIFC()
{
	//MSLineMemberM::ExportProfile(pWidth);

	if (!this->ProfileInstance) {
		//char mSecName[10];
		//_ltoa(this->m_ID, mSecName, 10);
		if (this->m_nHandRailType == 0) // 0: upper part only, 1: upper part + lower part
			this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), this->GetWidth(), this->GetDepth());
		else // 0: upper part only, 1: upper part + lower part
			this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), this->GetWidth(), this->GetDepth()-this->GetLevel());
		//buildArbitraryClosedProfileDefInstance();
	}

	return this->ProfileInstance;
}
