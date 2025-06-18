#include "StdAfx.h"
#include "MSMFactory.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSObject *************************************************************************   more things in MSObject.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSObject.h"

//void MSObject::InnerImportIFC(int pIfcId)
//{
//}
//
//int MSObject::InnerExportIFC()
//{
//	return 0;
//}

//int MSObject::GlobalIXStatus = 1;
//ifcEnumIX MSObject::GetIXStatus()
//{
//	return this->IXStatus;
//}
//
//void MSObject::SetIXStatus(ifcEnumIX pIXStatus)
//{
//	this->IXStatus = pIXStatus;
//}
//
//void MSObject::ImportIFC(int pIfcId)
//{
//	if (pIfcId == 0) return; // invalid ifc id
//	if (this->IXStatus == MSObject::GlobalIXStatus) return; // already imported
//
//	this->InnerImportIFC(pIfcId);
//
//	this->IXStatus = MSObject::GlobalIXStatus;
//	return;
//}
//
//int MSObject::ExportIFC()
//{
//	if (this->IXStatus == MSObject::GlobalIXStatus) return 0; // already exported
//
//	int mIfcId = this->InnerExportIFC();
//
//	// once MSObject::ExportIFC starts, set status to exported
//	// the sequence of this before this->InnerExportIFC() is very important!!!
//	// if the sequence is not appropriate, infinite export within element between element type and property set occurs
//	this->IXStatus = MSObject::GlobalIXStatus;
//
//	return mIfcId;
//}
//







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSCompoM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSCompoM.h"
#include "MSConcrete.h"
#include "MSSteel.h"
#include "MSBuilding.h"

void MSCompoM::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, Desc(m_ID), ...
	char* mName = 0x0; char* mDesc = 0x0; char* mTag = 0x0; char* mEleType = 0x0; char* mPreType = 0x0;
	importElementTypeInstance(pIfcId, &(this->GlobalId), &mName, &mDesc, &mTag, &mEleType, &mPreType);
	this->m_Name = CString(mName);
	if (mTag == NULL) this->GetID();
	else this->m_ID = _atol_l(mTag, 0);

	// import the related material
	char* mMatName = 0x0; double mMassDensity, mE, mG, mNu, mAlpha, mfck, mFy, mFu;
	int mMatType = importMaterialInstance(pIfcId, &mMatName, mMassDensity, mE, mG, mNu, mAlpha, mfck, mFy, mFu);

	// get property for from story and to story; not import; this is temporary
	// important!!! **************************************************************************************
	// for element type, Pset is directly added by attribute "HasPropertySet"; this is different for element
	// important!!! **************************************************************************************
	int* mPsetIds; sdaiGetAttrBN(pIfcId, "HasPropertySets", sdaiAGGR, &mPsetIds);
	int mPsetCount = sdaiGetMemberCount(mPsetIds);
	for (int mIdx = 0; mIdx < mPsetCount; mIdx++)
	{
		int mPsetId; engiGetAggrElement(mPsetIds, mIdx, sdaiINSTANCE, &mPsetId);
		ifcPset* mPset = new ifcPset(mPsetId);
		importPropertySet(mPset);
		if (mPset->Name == "Pset_FromToStory")
		{
			this->m_nFromStory = mPset->GetProperty("FromStory")->iValue;
			this->m_nToStory = mPset->GetProperty("ToStory")->iValue;
			break;
		}
	}

	// generate MSMaterial
	if (mMatType > 0)
	{
		MSConcrete* mConc = new MSConcrete();
		mConc->GetID();
		mConc->SetNameA((LPCSTR)mMatName);
		mConc->m_Desc = CString(mMatName);
		mConc->m_Type = CString("SURcMaterial");
		mConc->m_ElasModulus = mE;
		mConc->m_ElasModulusAnal = mE;
		mConc->m_PoisonRat = mNu;
		mConc->m_Density = mMassDensity;
		mConc->m_ConcStrength = mfck;
		mConc->m_ConcKind = CString(mMatName);
		mConc->m_ConcStandard = CString("KS(KBC05)");
		this->mp_Material = mConc;
	}
	else
	{
		MSSteel* mStl = new MSSteel();
		mStl->GetID();
		mStl->SetNameA((LPCSTR)mMatName);
		mStl->m_Desc = CString(mMatName);
		//mStl->m_Type = CString("SURcMaterial");
		mStl->m_ElasModulus = mE;
		mStl->m_ElasModulusAnal = mE;
		mStl->m_PoisonRat = mNu;
		mStl->m_Density = mMassDensity;
		mStl->m_Fy = mFy;
		this->mp_Material = mStl;
	}

	// get the owner (building)
	MSBuilding* mBldg = NULL;
	GetCurrentBuilding((CObject*&)mBldg);
	mBldg->Add(this->mp_Material); // just add
	//this->mp_Material->SetOwner(mBldg);
}

void MSCompoM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
{
	return;
}


int MSCompoM::InnerExportIFC()
{
	// if member type is not assigned to any member entity, do not export
	if (this->AggrBldgEleInstance == NULL) return 0;

	// prapare entity name, member group name, predefined type
	char* mEntityName; char* mPreType;
	char mTag[10];
	_ltoa(this->m_ID, mTag, 10);
	switch (this->m_nType)
	{
		case msBeamM: mEntityName = "IFCBEAMTYPE"; mPreType = "BEAM"; break;
		case msBraceM: mEntityName = "IFCMEMBERTYPE"; mPreType = "BRACE"; break;
		case msBWallM:
		case msRWallM: mEntityName = "IFCWALLTYPE"; mPreType = "STANDARD"; break;
		case msColumnM: mEntityName = "IFCCOLUMNTYPE"; mPreType = "COLUMN"; break;
		case msSlabM: mEntityName = "IFCSLABTYPE"; mPreType = "FLOOR"; break;
		case msIsoFootM:
		case msWallFootM: mEntityName = "IFCMEMBERTYPE"; mPreType = "NOTDEFINED"; break; // for IfcFooting, there is no type, therefore IfcMemberType is used; this is temporary
		case msHandRailM: mEntityName = "IFCRAILINGTYPE"; mPreType = "HANDRAIL"; break;
		case msLevelDifM: mEntityName = "IFCBUILDINGELEMENTPROXYTYPE"; mPreType = "NOTDEFINED"; break;
		//case msStairM: return 0;
		default: return 0;
	}
	//msBeamM=1, msBraceM, msBWallM, msColumnM, msIsoFootM, msRampM, msRWallM, msSlabM, msStairM, msWallFootM, msPitM, msLevelDifM

	// export Pset and relation for from story and to story
	// important!!! **************************************************************************************
	// for element type, Pset is directly added by attribute "HasPropertySet"; this is different for element
	// important!!! **************************************************************************************
	ifcProperty* mFromPrpt = new ifcProperty("FromStory", this->m_nFromStory);
	ifcProperty* mToPrpt = new ifcProperty("ToStory", this->m_nToStory);
	ifcPset* mPset = new ifcPset("Pset_FromToStory", _T(""));
	mPset->AddProperty(mFromPrpt);
	mPset->AddProperty(mToPrpt);
	//mPset->AddObjectIfcId(mBuildingElementTypeInstance); // these are not required for element type
	int mPsetId = buildPropertySet(mPset); // export pset first!!

	// export element type instance
	int mBuildingElementTypeInstance = buildElementTypeInstance(mEntityName, (LPSTR)(LPCSTR)(this->GetNameA()), NULL, mTag, NULL, mPreType, mPsetId);

	// export relation for type
	if (this->AggrBldgEleInstance != NULL) buildRelDefinesByType(mTag, NULL, mBuildingElementTypeInstance, this->AggrBldgEleInstance, (_msize(this->AggrBldgEleInstance) / sizeof(int)));


	// export material
	int mMatId = 0;
	MSConcrete* mConc = (MSConcrete*)this->mp_Material;
	MSSteel* mStl = (MSSteel*)this->mp_Material;
	switch (this->mp_Material->GetType())
	{
	case MSMaterial::msConcrete:
		mMatId = buildMechanicalConcreteMaterialPropertiesTypeInstance((LPSTR)(LPCSTR)mConc->m_Desc, mConc->m_Density, mConc->m_ElasModulus, 0.0, mConc->m_PoisonRat, 0.0, mConc->m_ConcStrength);
		break;
	case MSMaterial::msSteel:
		mMatId = buildMechanicalSteelMaterialPropertiesInstance((LPSTR)(LPCSTR)mStl->m_Desc, mStl->m_Density, mStl->m_ElasModulus, 0.0, mStl->m_PoisonRat, 0.0, mStl->m_Fy, 0.0);
		break;
	}
	buildRelMaterialInstance((LPSTR)(LPCSTR)this->mp_Material->m_Desc, NULL, mMatId, &mBuildingElementTypeInstance, 1);

	// return ifcId
	return mBuildingElementTypeInstance;
}

//int* AggrBldgEleInstance;
//int ProfileInstance;

int MSCompoM::ExportProfile(double pWidth) // called by MSCompoG.cpp not by this->InnerExportIFC()
{
	return 0;
}

void MSCompoM::AddBuildingElementInstance(int pBldgEleInstance)
{
	if (this->AggrBldgEleInstance == NULL)
	{
		this->AggrBldgEleInstance = (int*)malloc(sizeof(int));
		this->AggrBldgEleInstance[0] = pBldgEleInstance;
	}
	else
	{
		int mSize = _msize(this->AggrBldgEleInstance) / sizeof(int); // number of members
		this->AggrBldgEleInstance = (int*)realloc(this->AggrBldgEleInstance, (mSize + 1) * sizeof(int));
		this->AggrBldgEleInstance[mSize] = pBldgEleInstance;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSLineMemberM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSLineMemberM.h"

void MSLineMemberM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
{
	char* mName = NULL; double mWidth = 0, mDepth = 0, mRadius = 0, mH = 0, mB = 0, mtw = 0, mtf = 0, mr = 0;
	if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), pIfcId) == true) // import rectangle profile
	{
		importRectangleProfileDefInstance(pIfcId, &mName, mWidth, mDepth);
		this->me_Shape = MSLineMemberM::RECT;
	}
	else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), pIfcId) == true) // import circle profile
	{
		importCircleProfileDefInstance(pIfcId, &mName, mRadius);
		this->me_Shape = MSLineMemberM::CIRCLE;
	}
	else  // import H-section profile
	{
		importIShapeProfileDefInstance(pIfcId, &mName, mH, mB, mtw, mtf, mr);
		this->me_Shape = MSLineMemberM::POLYGON;
	}

	// for beam, column, brace, rail, line level diff
	this->m_dWidth = mWidth;
	this->m_dDepth = mDepth;
	this->SetLevel(mDepth); // for line level diff
}

int MSLineMemberM::ExportProfile(double pWidth) // pWidth is not used for line member
{
	if (!this->ProfileInstance) {
		//char mSecName[10];
		//_ltoa(this->m_ID, mSecName, 10);
		if (this->m_nType == msWallFootM) // for wall footing
			this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), this->GetWidth(), this->GetThick());
		else // for beam, column, brace, line level difference
			this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), this->GetWidth(), this->GetDepth());
	}

	return this->ProfileInstance;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSFaceMemberM
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSFaceMemberM.h"

void MSFaceMemberM::ImportProfile(int pIfcId) // called by MSCompoG.cpp not by this->InnerImportIFC()
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
	//this->m_dWidth = mWidth; //by LSS 
	this->m_dThick = mDepth;
}

int MSFaceMemberM::ExportProfile(double pWidth)
{
	if (!this->ProfileInstance) {
		//char mSecName[10];
		//_ltoa(this->m_ID, mSecName, 10);

		// enum Type_MSCompoM {msBeamM=1, msBraceM, msBWallM, msColumnM, msIsoFootM, msRampM, msRWallM, msSlabM, msStairM, msWallFootM};
		switch (this->m_nType)
		{
			case msBWallM:
			case msRWallM:
				//this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), pWidth, this->m_dThick);
				this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), pWidth, this->GetThick());
				break;
			case msIsoFootM:
				this->ProfileInstance = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), pWidth, this->GetDepth());
				break;
			//case msSlabM:
			//	// get polygon
			//	GMPolygon mGMPolygon;
			//	this->GetSecPolygon(mGMPolygon);
			//	// prepare iteration
			//	int mIdx, mNumPnt = mGMPolygon.ma_Vector.GetSize();
			//	// set 1st point
			//	point2DStruct* mPnt = new point2DStruct; // 1st point
			//	mPnt->x = mGMPolygon.ma_Vector[0]->m_X;
			//	mPnt->y = mGMPolygon.ma_Vector[0]->m_Y;
			//	polygon2DStruct* mPrePnt = new polygon2DStruct; // 1st polygon struct
			//	mPrePnt->pPoint = mPnt;
			//	polygon2DStruct* mPolygon = mPrePnt; // head polygon struct
			//	// set n-th point
			//	for (mIdx = 1; mIdx < mNumPnt; mIdx++)
			//	{
			//		mPnt = new point2DStruct; // n-th point
			//		mPnt->x = mGMPolygon.ma_Vector[mIdx]->m_X;
			//		mPnt->y = mGMPolygon.ma_Vector[mIdx]->m_Y;
			//		polygon2DStruct* mCurPnt = new polygon2DStruct; // n-th polygon struct
			//		mCurPnt->pPoint = mPnt;
			//		mPrePnt->next = mCurPnt; // set cur-point to the next of pre-point
			//		mPrePnt = mCurPnt; // prepare next iteration
			//	}
			//	// build profile def
			//	this->ProfileInstance = buildArbitraryClosedProfileDefInstance(mSecName, mPolygon);
			//	break;
		}
	}

	return this->ProfileInstance;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSCompoG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSCompoG.h"
#include "MSContainer.h"

void MSCompoG::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, ...
	char* mName = 0x0; char* mObjType = 0x0; int mRepId = 0;char* mTag = 0x0; 
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	importElementInstance(pIfcId, &(this->GlobalId), &mName, NULL, &mObjType, *this->LP, mRepId, &mTag);

	// set name and id
	this->m_Name = CString(mName);
	this->m_ID = _atol_l(mTag, 0);

	// import element type (MSCompoM)
	// 1. get id of related element type
	CArray<int> mTypeRelArr;
	importRelationInstance(pIfcId, "IsDefinedBy", mTypeRelArr, "IFCRELDEFINESBYTYPE");
	//ASSERT(mTypeRelArr.GetSize() > 1);
	int mTypeId = 0; sdaiGetAttrBN(mTypeRelArr[0], "RelatingType", sdaiINSTANCE, &mTypeId); // assumption: only one element type

	// 2. get material type of element type
	int mMatType = getMaterialType(mTypeId);

	// 3. if MSCompoM is not imported yet, generate MSCompoM and import
	char* mGUID; sdaiGetAttrBN(mTypeId, "GlobalId", sdaiSTRING, &mGUID);
	MSCompoM* mMemType = NULL;
	if (GetCompoMIFC(CString(mGUID), (CObject*&)mMemType) == false)
	{
		// generate MSCompoM
		if (mMatType > -1) // mMatType = 1(concrete), 0(unknown)
		{
			mMemType = MSMFactory::GetInstance()->GetMInstance(this->m_nType);
			/*switch (this->m_nType)
			{
			case msBeamG: mMemType = new MSRcBeamM(); break;
			case msBraceG: mMemType = new MSRcBraceM(); break;
			case msColumnG: mMemType = new MSRcColumnM(); break;
			case msBWallG:
			case msRWallG: mMemType = new MSRcBWallM(); break;
			case msSlabG: mMemType = new MSRcSlabM(); break;
			case msIsoFootG:
			case msStairG:
			default: return;
			}*/
			//MSCompoM* mMemTypeber = this->mp_Owner->CreateNewCompoM(this->m_nType);
		}
		else // steel
		{
		}

		// get the owner (building)
		MSBuilding* mBldg = NULL;
		GetCurrentBuilding((CObject*&)mBldg);
		mMemType->SetOwner(mBldg);
		AddCompoMIFC(CString(mGUID), mMemType);

		// import
		mMemType->ImportIFC(mTypeId);
		mBldg->Add(mMemType); // just add
	}
	this->SetCompoM(mMemType->GetID());
}

int MSCompoG::ExportLocalPlacement()
{
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	return this->LP->InstanceId;
}

int MSCompoG::ExportGeometryModel()
{
	return 0;
}

int MSCompoG::InnerExportIFC()
{
	// export building element instance
	char* mEntityName;
	switch (this->m_nType)
	{
		case msBeamG: mEntityName = "IFCBEAM"; break;
		case msBraceG: mEntityName = "IFCMEMEBR"; break;
		case msColumnG: mEntityName = "IFCCOLUMN"; break;
		case msBWallG:
		case msRWallG: mEntityName = "IFCWALLSTANDARDCASE"; break;
		case msSlabG: mEntityName = "IFCSLAB"; break;
		case msIsoFootG:
		case msWallFootG: mEntityName = "IFCFOOTING"; break;
		case msHandRailG:  mEntityName = "IFCRAILING"; break;
		case msLineLevelDifG: mEntityName = "IFCBUILDINGELEMENTPROXY"; break;
		//case msPointLevelDifG: mEntityName = "IFCBUILDINGELEMENTPROXY"; break;
		case msStairG: return 0;
		default: mEntityName = NULL;
			return 0;
	}
	char mMemType[10];
	_ltoa(this->m_nType, mMemType, 10);
	char mMemId[10];
	_ltoa(this->m_ID, mMemId, 10);

	// export LP and Geometry
	int mLPInstance = this->ExportLocalPlacement();
	int mGeomInstance = this->ExportGeometryModel();

	// export element itself
	int mBuildingElementInstance = buildElementInstance(mEntityName, (LPSTR)(LPCSTR)this->GetNameA(), NULL, mMemType, mLPInstance, mGeomInstance, mMemId);
	sdaiPutAttrBN(mBuildingElementInstance, "CompositionType", sdaiENUM, "ELEMENT");
	if (this->m_nType == msSlabG) sdaiPutAttrBN(mBuildingElementInstance, "PredefinedType", sdaiENUM, "FLOOR");
	else if (this->m_nType == msIsoFootG) sdaiPutAttrBN(mBuildingElementInstance, "PredefinedType", sdaiENUM, "PAD_FOOTING"); // isolated footing
	else if (this->m_nType == msWallFootG) sdaiPutAttrBN(mBuildingElementInstance, "PredefinedType", sdaiENUM, "STRIP_FOOTING"); // wall footing
	else if (this->m_nType == msHandRailG) sdaiPutAttrBN(mBuildingElementInstance, "PredefinedType", sdaiENUM, "HANDRAIL");
	else if (this->m_nType == msLineLevelDifG) sdaiPutAttrBN(mBuildingElementInstance, "CompositionType", sdaiENUM, "ELEMENT");

	// add instance to aggregation of element type 
	this->GetCompoM()->AddBuildingElementInstance(mBuildingElementInstance);

	//// export element type is done in MSProject
	//this->GetCompoM()->ExportIFC(NULL);

	// return ifcId
	return mBuildingElementInstance;
}


BOOL MSCompoG::IsVerticalMember()
{
	switch(m_nType)
	{
	case msWallG:
	case msColumnG:
		return TRUE;
	default:
		return FALSE;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSLinkMemberG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSLinkMemberG.h"
#include "MSFloor.h"

void MSLinkMemberG::InnerImportIFC(int pIfcId) // for beam, column, brace
{
	MSCompoG::InnerImportIFC(pIfcId);

	// import geometry (body, axis, section)
	int mRepId = 0; sdaiGetAttrBN(pIfcId, "Representation", sdaiINSTANCE, &mRepId);
	CArray<int> mRepIdArr; importProductDefinitionShapeInstance(mRepId, mRepIdArr);
	char* mRepIdfer = 0x0; char* mRepType = 0x0; CArray<int> mRepItemIdArr;
	int mSectId = 0; double mClearSpan = 0, mex = 0, mey = 0, mez = 0, mBetaAngle = 0; GMPolyline* mAxis; //MSLineMemberM::SECT_SHAPE mSectShape = MSLineMemberM::RECT;
	//char* mName = NULL; double mWidth = 0, mDepth = 0, mRadius = 0, mH = 0, mB = 0, mtw = 0, mtf = 0, mr = 0;
	for (int mIdx = 0; mIdx < mRepIdArr.GetSize(); mIdx++)
	{
		mRepItemIdArr.RemoveAll();
		importShapeRepresentationInstance(mRepIdArr[mIdx], &mRepIdfer, &mRepType, mRepItemIdArr); // import body/axis; 'Body/Axis', 'SweptSolid/Curve2D'
		if (CString(mRepIdfer) == CString("Body")) // import body
		{
			if (CString(mRepType) == CString("SweptSolid"))
			{
				// import clear span, eccentricity, beta angle
				importExtrudedAreaSolidInstance(mRepItemIdArr[0], mSectId, mClearSpan, mex, mey, mez, mBetaAngle, false); // false = get beta angle in degree
				this->GetCompoM()->ImportProfile(mSectId);
				//if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), mSectId) == true) // import rectangle profile
				//{
				//	importRectangleProfileDefInstance(mSectId, &mName, mWidth, mDepth);
				//}
				//else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), mSectId) == true) // import circle profile
				//{
				//	importCircleProfileDefInstance(mSectId, &mName, mRadius);
				//	mSectShape = MSLineMemberM::CIRCLE;
				//}
				//else  // import H-section profile
				//{
				//	importIShapeProfileDefInstance(mSectId, &mName, mH, mB, mtw, mtf, mr);
				//	mSectShape = MSLineMemberM::POLYGON;
				//}
			}
		}
		else if (CString(mRepIdfer) == CString("Axis")) // import axis
		{
			if (CString(mRepType) == CString("Curve2D")) // import polyline
				importPolylineInstance(mRepItemIdArr[0], mAxis); // true = radian
		}
	}

	//// set section at MSCompoM
	//MSLineMemberM* mLMemType = (MSLineMemberM*)this->GetCompoM();
	//mLMemType->me_Shape = mSectShape;
	//mLMemType->m_dWidth = mWidth; // width of section
	//switch (this->m_nType)
	//{
	//	case msBeamG:
	//	case msColumnG:
	//	case msBraceG:
	//	case msHandRailG:
	//		mLMemType->m_dDepth = mDepth; break; // depth of section; not height or length of member
	//	case msWallFootG:
	//		mLMemType->m_dThick = mDepth; break;
	//	//default: return;
	//}

	// get end points in absolute coordinates
	int mMomLevel = 1; // 1 level = member(local)->storey(local)->building(world)
	if (this->GetOwner()->GetType() == MSCompoG::msBuilding) mMomLevel = 0; // 0 level = member(local)->building(world)
	GMVector* mSPnt = new GMVector(0, 0, 0);
	GMVector* mEPnt = new GMVector(0, 0, mClearSpan);
	this->LP->GetAbsolutePoint(mSPnt, false, mMomLevel); RemoveGarbage(mSPnt);
	this->LP->GetAbsolutePoint(mEPnt, false, mMomLevel); RemoveGarbage(mEPnt);

	// joints
	// 1. create joints
	MSJoint* mSJoint = new MSJoint(); mSJoint->GetID();
	MSJoint* mEJoint = new MSJoint(); mEJoint->GetID();
	mSJoint->m_dXc = mSPnt->m_X; mSJoint->m_dYc = mSPnt->m_Y; mSJoint->m_dZc = mSPnt->m_Z;
	mEJoint->m_dXc = mEPnt->m_X; mEJoint->m_dYc = mEPnt->m_Y; mEJoint->m_dZc = mEPnt->m_Z;
	// 2. add joints
	this->GetOwner()->Add(mSJoint);
	if (this->GetOwner()->m_nType == MSCompoG::msFloor) // for members contained in floor
	{
		MSFloor* mFloor = (MSFloor*)this->GetOwner();
		mSJoint->m_nFloorNum = mFloor->m_nFloorNum;
		switch (this->m_nType)
		{
		case MSCompoG::msBeamG:
		case MSCompoG::msHandRailG:
		case MSCompoG::msWallFootG:
		case MSCompoG::msLineLevelDifG:
			mFloor->Add(mEJoint);
			mEJoint->m_nFloorNum = mFloor->m_nFloorNum; break;
		case MSCompoG::msColumnG:
		case MSCompoG::msBraceG:
		default:
			MSFloor* mUFloor = mFloor->GetUpperFloor();
			mUFloor->Add(mEJoint);
			mEJoint->m_nFloorNum = mUFloor->m_nFloorNum; break;
		}
	}
	else // for members contained in building
	{
		this->GetOwner()->Add(mEJoint);
	}
	// 3. set joints
	this->SetJoints(mSJoint->GetID(), mEJoint->GetID());

	// set beta angle, eccentricity
	this->m_dAxisRotation = mBetaAngle;
	if (this->m_nType == msBeamG) mey += this->mp_CompoM->GetDepth()/2;
	if (this->m_nType == msLineLevelDifG) mey += this->mp_CompoM->GetDepth()/2;
	if (this->m_nType == msHandRailG) mey -= this->mp_CompoM->GetDepth();
	if (this->m_nType == msWallFootG) mey += this->mp_CompoM->GetThick()/2;
	GMVector mEcc = GMVector(mex, mey, mez);
	this->LP->GetAbsolutePoint(&mEcc, true, 0); RemoveGarbage(mEcc);
	this->m_SEcc.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
	this->m_EEcc.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
}

// must be overrided in wall and slab
int MSLinkMemberG::ExportLocalPlacement()
{
	// get k-vec
	GMVector mKVec; // this is for beam and column
	switch (this->m_nType)
	{
		case msBeamG:
		case msWallFootG:
		case msHandRailG:
		case msLineLevelDifG:
			mKVec.SetXYZ(0.0, 0.0, 1.0); break; // same to msBeamG
		case msBraceG: mKVec.SetXYZ(0.0, 1.0, 0.0); break;
		case msColumnG: mKVec.SetXYZ(0.0, 1.0, 0.0); break;
		//case msBWallG: mKVec.SetXYZ(0.0, 1.0, 0.0); break;
		//default: mEntityName = NULL;
	}

	// get x-vec, y-vec, z-vec
	GMVector mSVec = (GMVector)*(this->mp_SJoint);
	GMVector mEVec = (GMVector)*(this->mp_EJoint);
	GMVector mZVec = mEVec - mSVec; mZVec = mZVec.UnitVector(); // z-vec : extruded direction of member
	GMVector mXVec = mKVec & mZVec; // strong axis
	GMVector mYVec = mZVec & mXVec; // weak axis
	RemoveGarbage(mXVec); RemoveGarbage(mYVec); RemoveGarbage(mZVec);

	// get matrix for coordinate system
	MSCompoG::ExportLocalPlacement();
	this->LP->Set(mXVec.m_X, mXVec.m_Y, mXVec.m_Z, mYVec.m_X, mYVec.m_Y, mYVec.m_Z, mZVec.m_X, mZVec.m_Y, mZVec.m_Z, mSVec.m_X, mSVec.m_Y, 0.0);

	// build LP and return
	return buildLocalPlacementInstance(this->LP);
}

// must be overrided in wall and slab
int MSLinkMemberG::ExportGeometryModel()
{
	// get length
	double mLength = (this->mp_EJoint->m_dZc + this->m_EEcc.m_Z) - (this->mp_SJoint->m_dZc + this->m_SEcc.m_Z); // this is for column
	if (this->m_nType != msColumnG) // for beam, brace, handrail
	{
		GMVector mSVec = (GMVector)*(this->mp_SJoint) + this->m_SEcc;
		GMVector mEVec = (GMVector)*(this->mp_EJoint) + this->m_EEcc;
		GMVector mAxisVec = mEVec - mSVec;
		mLength = mAxisVec.Magnitude(); // eccentricity can not be considered
	}

	// get eccentricity in local by transformation of eccentricity in global
	transformationMatrixStruct mMat;
	generateMatrix(&mMat, *(this->LP->CoordSys));
	transposeMatrix(&mMat);
	GMVector mEcc(this->m_SEcc);
	rotatePoint(&mMat, &mEcc); RemoveGarbage(mEcc);
	double mBmEcc = 0.0;
	MSLineMemberM* mCompoM = (MSLineMemberM*)this->GetCompoM();
	if (this->m_nType == msBeamG) mBmEcc = -mCompoM->GetDepth()/2;
	else if (this->m_nType == msLineLevelDifG) mBmEcc = -mCompoM->GetDepth()/2;
	else if (this->m_nType == msHandRailG) mBmEcc = mCompoM->GetDepth(); // mCompoM->GetDepth() is overrided at MSRcHandRail, the result is not the expected one
	else if (this->m_nType == msWallFootG) mBmEcc = -mCompoM->GetThick()/2;

	// get body representation
	int mSection = this->GetCompoM()->ExportProfile(0.0); // 0.0 is not used in the function

	// build shape and axis representation
	int* mGeometryModel = new int[2];
	int mBody = buildExtrudedAreaSolidInstance(mSection, mLength, mEcc.m_X, mEcc.m_Y + mBmEcc, mEcc.m_Z, this->m_dAxisRotation, false); // this->m_dAxisRotation is in degree
	mGeometryModel[0] = buildShapeRepresentationInstance(mBody, "Body", "SweptSolid");
	int mAxis = buildPolylineInstance(0, 0, mLength);
	mGeometryModel[1] = buildShapeRepresentationInstance(mAxis, "Axis", "Curve2D");

	// get geometry model
	return buildProductDefinitionShapeInstance(mGeometryModel, 2);
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSFaceMemberG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSFaceMemberG.h"

void MSWallMemberG::InnerImportIFC(int pIfcId) // for wall
{
	MSCompoG::InnerImportIFC(pIfcId);

	// import geometry (body, axis, section)
	int mRepId = 0; sdaiGetAttrBN(pIfcId, "Representation", sdaiINSTANCE, &mRepId);
	CArray<int> mRepIdArr; importProductDefinitionShapeInstance(mRepId, mRepIdArr);
	char* mRepIdfer = 0x0; char* mRepType = 0x0; CArray<int> mRepItemIdArr;
	int mSectId = 0; double mClearSpan = 0, mex = 0, mey = 0, mez = 0, mBetaAngle = 0; GMPolyline* mAxis = NULL;
	//char* mName = NULL; double mWidth = 0, mHeight = 0, mRadius = 0, mH = 0, mB = 0, mtw = 0, mtf = 0, mr = 0;
	for (int mIdx = 0; mIdx < mRepIdArr.GetSize(); mIdx++)
	{
		mRepItemIdArr.RemoveAll();
		importShapeRepresentationInstance(mRepIdArr[mIdx], &mRepIdfer, &mRepType, mRepItemIdArr); // import body/axis; 'Body/Axis', 'SweptSolid/Curve2D'
		if (CString(mRepIdfer) == CString("Body")) // import body
		{
			if (CString(mRepType) == CString("SweptSolid"))
			{
				// import clear span, eccentricity, beta angle
				importExtrudedAreaSolidInstance(mRepItemIdArr[0], mSectId, mClearSpan, mex, mey, mez, mBetaAngle, true); // true = radian
				this->GetCompoM()->ImportProfile(mSectId);
				//if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), mSectId) == true) // import rectangle profile
				//{
				//	importRectangleProfileDefInstance(mSectId, &mName, mWidth, mHeight);
				//}
				//else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), mSectId) == true) // import circle profile
				//{
				//	importCircleProfileDefInstance(mSectId, &mName, mRadius);
				//}
				//else  // import H-section profile
				//{
				//	importIShapeProfileDefInstance(mSectId, &mName, mH, mB, mtw, mtf, mr);
				//}
			}
		}
		else if (CString(mRepIdfer) == CString("Axis")) // import axis
		{
			if (CString(mRepType) == CString("Curve2D")) // import polyline
				importPolylineInstance(mRepItemIdArr[0], mAxis); // true = radian
		}
	}

	//// set section at MSCompoM
	//switch (this->m_nType)
	//{
	//	case msBWallG:
	//	case msRWallG:
	//	//case msIsoFootG: // isolated footing is treated in MSIsoFootG
	//		MSFaceMemberM* mFMemType = (MSFaceMemberM*)this->GetCompoM();
	//		mFMemType->m_dThick = mHeight;
	//		break;
	//	//default: return;
	//}

	// get end points in absolute coordinates
	int mMomLevel = 1; // 1 level = member(local)->storey(local)->building(world)
	if (this->GetOwner()->GetType() == MSCompoG::msBuilding) mMomLevel = 0; // 0 level = member(local)->building(world)
	GMVector* mSPnt = new GMVector(0, 0, 0);
	GMVector* mEPnt = new GMVector(this->GetCompoM()->GetWidth(), 0, 0);
	this->LP->GetAbsolutePoint(mSPnt, false, mMomLevel); RemoveGarbage(mSPnt);
	this->LP->GetAbsolutePoint(mEPnt, false, mMomLevel); RemoveGarbage(mEPnt);
	GMVector* mSPntUp = new GMVector(0, 0, mClearSpan);
	GMVector* mEPntUp = new GMVector(this->GetCompoM()->GetWidth(), 0, mClearSpan);
	this->LP->GetAbsolutePoint(mSPntUp, false, mMomLevel); RemoveGarbage(mSPntUp);
	this->LP->GetAbsolutePoint(mEPntUp, false, mMomLevel); RemoveGarbage(mEPntUp);

	// joints
	// 1. create joints
	MSJoint* mSJoint = new MSJoint(); mSJoint->GetID();
	MSJoint* mEJoint = new MSJoint(); mEJoint->GetID();
	MSJoint* mSJointUp = new MSJoint(); mSJointUp->GetID();
	MSJoint* mEJointUp = new MSJoint(); mEJointUp->GetID();
	mSJoint->m_dXc = mSPnt->m_X; mSJoint->m_dYc = mSPnt->m_Y; mSJoint->m_dZc = mSPnt->m_Z;
	mEJoint->m_dXc = mEPnt->m_X; mEJoint->m_dYc = mEPnt->m_Y; mEJoint->m_dZc = mEPnt->m_Z;
	mSJointUp->m_dXc = mSPntUp->m_X; mSJointUp->m_dYc = mSPntUp->m_Y; mSJointUp->m_dZc = mSPntUp->m_Z;
	mEJointUp->m_dXc = mEPntUp->m_X; mEJointUp->m_dYc = mEPntUp->m_Y; mEJointUp->m_dZc = mEPntUp->m_Z;
	// 2. add joints
	this->GetOwner()->Add(mSJoint);
	this->GetOwner()->Add(mEJoint);
	if (this->GetOwner()->m_nType == MSCompoG::msFloor) // for members contained in floor
	{
		MSFloor* mFloor = (MSFloor*)this->GetOwner();
		mSJoint->m_nFloorNum = mFloor->m_nFloorNum;
		mEJoint->m_nFloorNum = mFloor->m_nFloorNum;

		MSFloor* mUFloor = mFloor->GetUpperFloor();
		mUFloor->Add(mSJointUp);
		mUFloor->Add(mEJointUp);
		mSJointUp->m_nFloorNum = mUFloor->m_nFloorNum;
		mEJointUp->m_nFloorNum = mUFloor->m_nFloorNum;
	}
	else // for members contained in building
	{
		this->GetOwner()->Add(mSJointUp);
		this->GetOwner()->Add(mEJointUp);
	}
	// 3. set joints
	this->SetJoints(mSJoint->GetID(), mEJoint->GetID(), mSJointUp->GetID(), mEJointUp->GetID());

	// set eccentricity
	mex -= (*mEPnt - *mSPnt).Magnitude()/2;// this->mp_CompoM->GetWidth()/2;
	GMVector mEcc = GMVector(mex, mey, mez);
	this->LP->GetAbsolutePoint(&mEcc, true, 0); RemoveGarbage(mEcc);
	this->m_SEcc.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
	this->m_EEcc.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
	this->m_SEccUp.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
	this->m_EEccUp.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
}

int MSWallMemberG::ExportLocalPlacement()
{
	// get x-vec, y-vec, z-vec
	GMVector mSVec = (GMVector)*(this->mp_SJoint);
	GMVector mEVec = (GMVector)*(this->mp_EJoint);
	GMVector mXVec = mEVec - mSVec; mXVec = mXVec.UnitVector(); // x-vec : base line of wall
	GMVector mSVecU= (GMVector)*(this->mp_SJointUp);
	GMVector mZVec = mSVecU- mSVec; mZVec = mZVec.UnitVector(); // z-vec : upward direction of wall
	GMVector mYVec = mZVec & mXVec;
	RemoveGarbage(mXVec); RemoveGarbage(mYVec); RemoveGarbage(mZVec);

	// get matrix for coordinate system
	MSCompoG::ExportLocalPlacement();
	this->LP->Set(mXVec.m_X, mXVec.m_Y, mXVec.m_Z, mYVec.m_X, mYVec.m_Y, mYVec.m_Z, mZVec.m_X, mZVec.m_Y, mZVec.m_Z, mSVec.m_X, mSVec.m_Y, 0.0);

	// build LP and return
	return buildLocalPlacementInstance(this->LP);
}

#include "MSFaceMemberM.h"

int MSWallMemberG::ExportGeometryModel()
{
	int* mGeometryModel = new int[2];

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// get body representation - the following commented out codes are not valid because of eccentricity
	//GMVector mSVec = (GMVector)*(this->mp_SJoint);
	//GMVector mEVec = (GMVector)*(this->mp_EJoint);
	//GMVector mAxisVec = mEVec - mSVec; // axis : base line of wall
	//GMVector mSVecU= (GMVector)*(this->mp_SJointUp);
	//GMVector mUpVec = mSVecU - mSVec;
	//double mHeight = mUpVec.Magnitude(); // height of wall
	//double mWidth = mAxisVec.Magnitude();
	double mHeight = this->GetHeight();
	double mWidth = this->GetLength();

	// get body representation
	MSFaceMemberM* mMemM = (MSFaceMemberM*)this->mp_CompoM;

	// get section name
	//char mSecName[10];
	//_ltoa(mMemM->m_ID, mSecName, 10);

	// build profile def
	int mSection = buildRectangleProfileDefInstance((LPSTR)(LPCSTR)mMemM->GetNameA(), mWidth, mMemM->m_dThick);

	// get eccentricity in local by transformation of eccentricity in global
	transformationMatrixStruct mMat;
	generateMatrix(&mMat, *(this->LP->CoordSys));
	transposeMatrix(&mMat);
	GMVector mEcc(this->m_SEcc);
	//point3DStruct mEcc;
	//mEcc.x = this->m_SEcc.m_X; mEcc.y = this->m_SEcc.m_Y; mEcc.z = this->m_SEcc.m_Z;
	rotatePoint(&mMat, &mEcc); RemoveGarbage(mEcc);

	// build shape representation
	int mBody = buildExtrudedAreaSolidInstance(mSection, mHeight, mEcc.m_X + mWidth/2, mEcc.m_Y, mEcc.m_Z, 0, false); // real ecc = this.m_SEcc; mWidth/2 is due to the characteristics of ExtrudedAreaSolid
	mGeometryModel[0] = buildShapeRepresentationInstance(mBody, "Body", "SweptSolid");

	// get axis representation
	int mAxis = buildPolylineInstance(mWidth, 0, 0);
	mGeometryModel[1] = buildShapeRepresentationInstance(mAxis, "Axis", "Curve2D");

	// get geometry model
	return buildProductDefinitionShapeInstance(mGeometryModel, 2);
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSSlabG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSSlabG.h"

void MSSlabG::InnerImportIFC(int pIfcId) // for slab
{
	MSCompoG::InnerImportIFC(pIfcId);

	// import geometry (body, axis, section)
	int mRepId = 0; sdaiGetAttrBN(pIfcId, "Representation", sdaiINSTANCE, &mRepId);
	CArray<int> mRepIdArr; importProductDefinitionShapeInstance(mRepId, mRepIdArr);
	char* mRepIdfer = 0x0; char* mRepType = 0x0; CArray<int> mRepItemIdArr;
	int mSectId = 0; double mClearSpan = 0, mex = 0, mey = 0, mez = 0, mBetaAngle = 0;
	char* mName = NULL; /*double mWidth = 0, mHeight = 0, mRadius = 0; */GMPolyline* mProfile;
	for (int mIdx = 0; mIdx < mRepIdArr.GetSize(); mIdx++)
	{
		mRepItemIdArr.RemoveAll();
		importShapeRepresentationInstance(mRepIdArr[mIdx], &mRepIdfer, &mRepType, mRepItemIdArr); // import body/axis; 'Body/Axis', 'SweptSolid/Curve2D'
		if (CString(mRepIdfer) == CString("Body")) // import body
		{
			if (CString(mRepType) == CString("SweptSolid"))
			{
				// import clear span, eccentricity, beta angle
				importExtrudedAreaSolidInstance(mRepItemIdArr[0], mSectId, mClearSpan, mex, mey, mez, mBetaAngle, true); // true = radian
				this->GetCompoM()->ImportProfile(mSectId);
				if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), mSectId) == true) // import rectangle profile
				{
					//importRectangleProfileDefInstance(mSectId, &mName, mWidth, mHeight);
					double mSpanX = this->GetCompoM()->GetWidth(), mSpanY = this->GetCompoM()->GetDepth(); // for slab, at here, y-span is equal to m_dThick and returned by GetDepth(); this is temporary and later thick is set to m_dThick and returned by GetDepth();

					// upward level
					int mMomLevel = 1; // 1 level = member(local)->storey(local)->building(world)
					if (this->GetOwner()->GetType() == MSCompoG::msBuilding) mMomLevel = 0; // 0 level = member(local)->building(world)

					// add each point in absolute coordinates
					mProfile = new GMPolyline(); GMVector* mVec;
					mVec = new GMVector(0, 0, 0); this->LP->GetAbsolutePoint(mVec, false, mMomLevel); mVec->m_Z = 0; mProfile->Add(mVec);
					mVec = new GMVector(mSpanX, 0, 0); this->LP->GetAbsolutePoint(mVec, false, mMomLevel); mVec->m_Z = 0; mProfile->Add(mVec);
					mVec = new GMVector(mSpanX, mSpanY, 0); this->LP->GetAbsolutePoint(mVec, false, mMomLevel); mVec->m_Z = 0; mProfile->Add(mVec);
					mVec = new GMVector(0, mSpanY, 0); this->LP->GetAbsolutePoint(mVec, false, mMomLevel); mVec->m_Z = 0; mProfile->Add(mVec);
				}
				//else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), mSectId) == true) // import circle profile
				//{
				//	importCircleProfileDefInstance(mSectId, &mName, mRadius);
				//}
				//else  // import arbitrary shaped section profile
				if (compareEntityType(CString("IFCARBITRARYCLOSEDPROFILEDEF"), mSectId) == true) // import arbitrary shaped section profile
				{
					importArbitraryClosedProfileDefInstance(mSectId, &mName, mProfile);
				}
			}
		}
		//else if (CString(mRepIdfer) == CString("Axis")) // import axis
		//{
		//	if (CString(mRepType) == CString("Curve2D")) // import polyline
		//		importPolylineInstance(mRepItemIdArr[0], mAxis); // true = radian
		//}
	}

	// get end points in absolute coordinates
	int mMomLevel = 1; // 1 level = member(local)->storey(local)->building(world)
	if (this->GetOwner()->GetType() == MSCompoG::msBuilding) mMomLevel = 0; // 0 level = member(local)->building(world)
	long mPntCount = mProfile->GetSize();
	for(long mIdx=0; mIdx<mPntCount-1; mIdx++) // in MS model, n points are stored for closed n-polygon, but in IFC model, n+1 points are stored for closed n-polygon
	{
		// get absolute x-, y-coordinates
		GMVector* mPnt = new GMVector(*(mProfile->GetAt(mIdx)));
		MSJoint* mJnt = new MSJoint(); mJnt->GetID();
		mJnt->m_dXc = mPnt->m_X; mJnt->m_dYc = mPnt->m_Y;
		// get absolute z-coordinates
		GMVector* mElevPnt = new GMVector(*mPnt); this->LP->GetAbsolutePoint(mElevPnt, false, mMomLevel); // this to get absolute height(elevation) of the slab
		mJnt->m_dZc = mElevPnt->m_Z;

		// add joint
		this->GetOwner()->Add(mJnt);
		this->Add(mJnt);

		// set floor number of joint
		if (this->GetOwner()->m_nType == MSCompoG::msFloor) // for members contained in floor
		{
			MSFloor* mFloor = (MSFloor*)this->GetOwner();
			mJnt->m_nFloorNum = mFloor->m_nFloorNum;
		}
	}

	// set section at MSCompoM
	switch (this->m_nType)
	{
	case msSlabG:
		MSFaceMemberM* mFMemType = (MSFaceMemberM*)this->GetCompoM();
		mFMemType->m_dThick = mClearSpan; // in MSFaceMemberM::ImportProfile(), m_dThick is y-span of slab; because of this, readl thick is set to m_dThick
		break;
	//default: return;
	}

	// set eccentricity
	this->m_dOffset = mClearSpan + mez;
}

int MSSlabG::ExportLocalPlacement()
{
	// get x-vec, y-vec, z-vec
	vector<MSJoint *>::iterator mpJoint;
	mpJoint = this->ma_Joint.begin();
	GMVector mSVec((*mpJoint)->m_dXc, (*mpJoint)->m_dYc, (*mpJoint)->m_dZc);
	GMVector mZVec(0, 0, 1); // z-vec
	GMVector mKVec(0, 1, 0); // k-vec
	GMVector mXVec = mKVec & mZVec; // x-vec
	GMVector mYVec = mZVec & mXVec; // y-vec
	RemoveGarbage(mXVec); RemoveGarbage(mYVec); RemoveGarbage(mZVec);

	// get matrix for coordinate system
	MSCompoG::ExportLocalPlacement();
	this->LP->Set(mXVec.m_X, mXVec.m_Y, mXVec.m_Z, mYVec.m_X, mYVec.m_Y, mYVec.m_Z, mZVec.m_X, mZVec.m_Y, mZVec.m_Z, mSVec.m_X, mSVec.m_Y, 0.0);

	// build LP and return
	return buildLocalPlacementInstance(this->LP);
}

int MSSlabG::ExportGeometryModel()
{
	int* mGeometryModel = new int[1];

	// get body representation
	MSFaceMemberM* mMemM = (MSFaceMemberM*)this->mp_CompoM;

	// get section name
	//char mSecName[10];
	//_ltoa(mMemM->m_ID, mSecName, 10);

	// get 1st point
	point2DStruct* mPnt = new point2DStruct; // 1st point
	mPnt->x = this->ma_Joint[0]->m_dXc;
	mPnt->y = this->ma_Joint[0]->m_dYc;
	polygon2DStruct* mPrePnt = new polygon2DStruct; // 1st polygon struct
	mPrePnt->pPoint = mPnt;
	polygon2DStruct* mPolygon = mPrePnt; // head polygon struct

	// get n-th point
	int mIdx, mNumPnt = this->ma_Joint.size();
	for (mIdx = 1; mIdx < mNumPnt; mIdx++)
	{
		mPnt = new point2DStruct; // n-th point
		mPnt->x = this->ma_Joint[mIdx]->m_dXc;
		mPnt->y = this->ma_Joint[mIdx]->m_dYc;
		polygon2DStruct* mCurPnt = new polygon2DStruct; // n-th polygon struct
		mCurPnt->next = NULL;
		mCurPnt->pPoint = mPnt;
		mPrePnt->next = mCurPnt; // set cur-point to the next of pre-point
		mPrePnt = mCurPnt; // prepare next iteration
	}

	// get last closing point
	mPnt = new point2DStruct; // n-th point
	mPnt->x = this->ma_Joint[0]->m_dXc;
	mPnt->y = this->ma_Joint[0]->m_dYc;
	polygon2DStruct* mCurPnt = new polygon2DStruct; // n-th polygon struct
	mCurPnt->next = NULL;
	mCurPnt->pPoint = mPnt;
	mPrePnt->next = mCurPnt; // set cur-point to the next of pre-point

	// build profile def
	int mSection = buildArbitraryClosedProfileDefInstance((LPSTR)(LPCSTR)this->GetNameA(), mPolygon);

	// get thickness
	double mThick = mMemM->m_dThick; // thick of slab

	// build shape representation
	vector<MSJoint *>::iterator mpJoint;
	mpJoint = this->ma_Joint.begin(); // origin
	int mBody = buildExtrudedAreaSolidInstance(mSection, mThick, -(*mpJoint)->m_dXc, -(*mpJoint)->m_dYc, -mThick + this->m_dOffset, 0, false); // -m_dXc, -m_dYc, -mThick is for centering; not eccentricity
	mGeometryModel[0] = buildShapeRepresentationInstance(mBody, "Body", "SweptSolid");

	// build geometry model
	return buildProductDefinitionShapeInstance(mGeometryModel, 1);
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSIsoFootG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSIsoFootG.h"

void MSIsoFootG::InnerImportIFC(int pIfcId) // for beam, column, brace
{
	MSCompoG::InnerImportIFC(pIfcId);

	// import geometry (body, section)
	int mRepId = 0; sdaiGetAttrBN(pIfcId, "Representation", sdaiINSTANCE, &mRepId);
	CArray<int> mRepIdArr; importProductDefinitionShapeInstance(mRepId, mRepIdArr);
	char* mRepIdfer = 0x0; char* mRepType = 0x0; CArray<int> mRepItemIdArr;
	int mSectId = 0; double mThick = 0, mex = 0, mey = 0, mez = 0, mBetaAngle = 0;
	//char* mName = NULL; double mWidth = 0, mHeight = 0, mRadius = 0; GMPolyline* mProfile;
	for (int mIdx = 0; mIdx < mRepIdArr.GetSize(); mIdx++)
	{
		mRepItemIdArr.RemoveAll();
		importShapeRepresentationInstance(mRepIdArr[mIdx], &mRepIdfer, &mRepType, mRepItemIdArr); // import body/axis; 'Body/Axis', 'SweptSolid/Curve2D'
		if (CString(mRepIdfer) == CString("Body")) // import body
		{
			if (CString(mRepType) == CString("SweptSolid"))
			{
				// import clear span, eccentricity, beta angle
				importExtrudedAreaSolidInstance(mRepItemIdArr[0], mSectId, mThick, mex, mey, mez, mBetaAngle, false); // false = get beta angle in degree
				this->GetCompoM()->ImportProfile(mSectId);
				//if (compareEntityType(CString("IFCRECTANGLEPROFILEDEF"), mSectId) == true) // import rectangle profile
				//{
				//	importRectangleProfileDefInstance(mSectId, &mName, mWidth, mHeight);
				//}
				//else if (compareEntityType(CString("IFCCIRCLEPROFILEDEF"), mSectId) == true) // import circle profile
				//{
				//	importCircleProfileDefInstance(mSectId, &mName, mRadius);
				//}
				//else  // import arbitrary shaped section profile
				//{
				//	importArbitraryClosedProfileDefInstance(mSectId, &mName, mProfile);
				//}
			}
		}
	}

	// set section at MSCompoM
	switch (this->m_nType) // in fact, this->m_nType is always msIsoFootG
	{
	case msIsoFootG:
		MSFaceMemberM* mFMemType = (MSFaceMemberM*)this->GetCompoM(); // in fact, MSRcIsoFootM
		//mFMemType->SetWidth(mWidth); // done in MSRcIsoFootM::ImportProfile()
		//mFMemType->SetDepth(mHeight); // done in MSRcIsoFootM::ImportProfile()
		mFMemType->m_dThick = mThick; // mFMemType->SetThick() is defined in MSRcIsoFootM.h
		break;
		//default: return;
	}

	// get end points in absolute coordinates
	int mMomLevel = 1; // 1 level = member(local)->storey(local)->building(world)
	if (this->GetOwner()->GetType() == MSCompoG::msBuilding) mMomLevel = 0; // 0 level = member(local)->building(world)
	GMVector* mPnt = new GMVector(0, 0, 0);
	this->LP->GetAbsolutePoint(mPnt, false, mMomLevel); RemoveGarbage(mPnt);

	// joints
	// 1. create joints
	MSJoint* mJoint = new MSJoint(); mJoint->GetID();
	mJoint->m_dXc = mPnt->m_X; mJoint->m_dYc = mPnt->m_Y; mJoint->m_dZc = mPnt->m_Z;
	// 2. add joints
	this->GetOwner()->Add(mJoint);
	if (this->GetOwner()->m_nType == MSCompoG::msFloor) // for members contained in floor
	{
		MSFloor* mFloor = (MSFloor*)this->GetOwner();
		mJoint->m_nFloorNum = mFloor->m_nFloorNum;
	}
	// 3. set joints
	this->mp_Joint = mJoint;

	// set beta angle, eccentricity
	this->m_dAxisRotation = mBetaAngle;
	mez += this->GetCompoM()->GetThick();
	GMVector mEcc = GMVector(mex, mey, mez);
	this->LP->GetAbsolutePoint(&mEcc, true, 0); RemoveGarbage(mEcc);
	this->m_Ecc.SetXYZ(mEcc.m_X, mEcc.m_Y, mEcc.m_Z);
}

int MSIsoFootG::ExportLocalPlacement()
{
	GMVector mx(1.0, 0.0, 0.0);
	GMVector my(0.0, 1.0, 0.0);
	GMVector mz(0.0, 0.0, 1.0);
	GMVector mo = (GMVector)*(this->mp_Joint);

	// get matrix for coordinate system
	MSCompoG::ExportLocalPlacement();
	this->LP->Set(&mx, &my, &mz, &mo);

	// build LP and return
	return buildLocalPlacementInstance(this->LP);
}

int MSIsoFootG::ExportGeometryModel()
{
	double mThick = GetCompoM()->GetThick(); // size-z
	double mWidth = GetCompoM()->GetWidth(); // size-x
	double mDepth = GetCompoM()->GetDepth(); // size-y

	// get eccentricity in local by transformation of eccentricity in global
	transformationMatrixStruct mMat;
	generateMatrix(&mMat, *(this->LP->CoordSys));
	transposeMatrix(&mMat);
	GMVector mEcc(this->m_Ecc);
	rotatePoint(&mMat, &mEcc); RemoveGarbage(mEcc);

	// get body representation; no axis representation
	int mSection = GetCompoM()->ExportProfile(mWidth);
	int mBody = buildExtrudedAreaSolidInstance(mSection, mThick, mEcc.m_X, mEcc.m_Y, mEcc.m_Z - mThick, this->m_dAxisRotation, false); // this->m_dAxisRotation is in degree
	int mGeometryModel = buildShapeRepresentationInstance(mBody, "Body", "SweptSolid");

	// get geometry model
	return buildProductDefinitionShapeInstance(&mGeometryModel, 1);
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSHandRailG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSHandRailG.h" // MSHandRailG.h is not included in MSFloor.h; why??

int MSHandRailG::ExportGeometryModel()
{
	//MSLinkMemberG::ExportGeometryModel();

	// get length
	double mLength = (this->mp_EJoint->m_dZc + this->m_EEcc.m_Z) - (this->mp_SJoint->m_dZc + this->m_SEcc.m_Z); // this is for column
	if (this->m_nType != msColumnG) // for beam, brace, handrail
	{
		GMVector mSVec = (GMVector)*(this->mp_SJoint) + this->m_SEcc;
		GMVector mEVec = (GMVector)*(this->mp_EJoint) + this->m_EEcc;
		GMVector mAxisVec = mEVec - mSVec;
		mLength = mAxisVec.Magnitude(); // eccentricity can not be considered
	}

	// get eccentricity in local by transformation of eccentricity in global
	transformationMatrixStruct mMat;
	generateMatrix(&mMat, *(this->LP->CoordSys));
	transposeMatrix(&mMat);
	GMVector mEcc(this->m_SEcc);
	rotatePoint(&mMat, &mEcc); RemoveGarbage(mEcc);

	MSLineMemberM* mCompoM = (MSLineMemberM*)this->GetCompoM();
	double mBmEcc = this->GetHeight()/2 + mCompoM->GetLevel();
	GMPolygon mPolygon; this->GetSecPolygon(mPolygon);
	if (mPolygon.GetSize() == 4) mBmEcc = this->GetDepth()/2; // 0: upper part only; GetDepth() returns length of upper part(m_dTopDepth)
	else if (mPolygon[0]->m_Z > mPolygon[5]->m_Z) mBmEcc = this->GetDepth()/2; // 0: upper part only; GetDepth() returns length of upper part(m_dTopDepth)

	// get body representation
	int mSection = this->GetCompoM()->ExportProfile(0.0); // 0.0 is not used in the function

	// build shape and axis representation
	int* mGeometryModel = new int[2];
	int mBody = buildExtrudedAreaSolidInstance(mSection, mLength, mEcc.m_X, mEcc.m_Y + mBmEcc, mEcc.m_Z, this->m_dAxisRotation, false); // this->m_dAxisRotation is in degree
	mGeometryModel[0] = buildShapeRepresentationInstance(mBody, "Body", "SweptSolid");
	int mAxis = buildPolylineInstance(0, 0, mLength);
	mGeometryModel[1] = buildShapeRepresentationInstance(mAxis, "Axis", "Curve2D");

	// get geometry model
	return buildProductDefinitionShapeInstance(mGeometryModel, 2);
}








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSFloor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSFloor.h"
#include "MSLineLevelDifG.h"
#include "MSPointLevelDifG.h"
#include "MSBuilding.h"

void MSFloor::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, ...
	char* mName = 0x0; char* mDesc = 0x0; char* mObjType = 0x0; int mRepId = 0;
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	importProductInstance(pIfcId, &(this->GlobalId), &mName, &mDesc, &mObjType, *this->LP, mRepId);
	this->m_Name = CString(mName);
	this->m_nFloorNum = _atol_l(mDesc, 0);

	//double mElevation = this->LP->CoordSys->_43; // Elevation = absolute height
	//sdaiGetAttrBN(pIfcId, "Elevation", sdaiREAL, &mElevation); // here, no need to read Elevation
	//this->m_dHeight is adjusted in MSBuilding later

	// sites, buildings, floors, members
	//int* mRelIds; sdaiGetAttrBN(pIfcId, "ContainsElements", sdaiAGGR, &mRelIds);
	//int mNumRelIds = sdaiGetMemberCount(mRelIds);
	//for (int mIdx = 0; mIdx < mNumRelIds; mIdx++)
	//{
	//	int mRelId; engiGetAggrElement(mRelIds, mIdx, sdaiINSTANCE, &mRelId);
	//	int* mBldgEleIds; sdaiGetAttrBN(mRelId, "RelatedElements", sdaiAGGR, &mBldgEleIds);
	//	int mNumBldgEleIds = sdaiGetMemberCount(mBldgEleIds);
	//	for (int mBldgEleIdx = 0; mBldgEleIdx < mNumBldgEleIds; mBldgEleIdx++)
	//	{
	//		int mBldgEleId; engiGetAggrElement(mBldgEleIds, mBldgEleIdx, sdaiINSTANCE, &mBldgEleId);
	//		char* mType;
	//		sdaiGetAttrBN(mBldgEleId, "ObjectType", sdaiSTRING, &mType);
	//		MSCompoG* mMember;
	//		switch ((MSCompoG::Type_MSCompoG)_atol_l(mType, 0))
	//		{
	//		case msBeamG: mMember = new MSBeamG; break;
	//		case msBraceG: mMember = new MSBraceG; break;
	//		case msColumnG: mMember = new MSColumnG; break;
	//		case msBWallG: mMember = new MSBWallG; break;
	//		case msRWallG: mMember = new MSRWallG; break;
	//		case msSlabG: mMember = new MSSlabG; break;
	//		case msIsoFootG:
	//		case msStairG:
	//		default: return;
	//		}

	//		mMember->SetParentID(this->m_ID);
	//		this->Add(mMember, true);
	//		mMember->ImportIFC(mBldgEleId);
	//	}
	//}
	CArray<int> mRelObjArr;
	importRelationInstance(pIfcId, "ContainsElements", mRelObjArr);
	for (int mRelIdx = 0; mRelIdx < mRelObjArr.GetSize(); mRelIdx++)
	{
		CArray<int> mObjArr;
		importRelationInstance(mRelObjArr[mRelIdx], "RelatedElements", mObjArr);
		for (int mIdx = 0; mIdx < mObjArr.GetSize(); mIdx++)
		{
			int mBldgEleId = mObjArr[mIdx];
			char* mType; sdaiGetAttrBN(mBldgEleId, "ObjectType", sdaiSTRING, &mType);
			MSCompoG* mMember;
			switch ((MSCompoG::Type_MSCompoG)_atol_l(mType, 0))
			{
			case msBeamG: mMember = new MSBeamG; break;
			case msBraceG: mMember = new MSBraceG; break;
			case msColumnG: mMember = new MSColumnG; break;
			case msBWallG: mMember = new MSBWallG; break;
			case msRWallG: mMember = new MSRWallG; break;
			case msSlabG: mMember = new MSSlabG; break;
			case msIsoFootG: mMember = new MSIsoFootG; break;
			case msWallFootG: mMember = new MSWallFootG; break;
			case msHandRailG: mMember = new MSHandRailG; break;
			case msLineLevelDifG: mMember = new MSLineLevelDifG; break;
			//case msPointLevelDifG: mMember = new MSPointLevelDifG; break;
			//case msStairG:
			default: return;
			}
			//MSCompoG* mMember = this->CreateNewCompoG((MSCompoG::Type_MSCompoG)_atol_l(mType, 0));

			//mMember->SetOwner(this); // done in this->Add(mMember, true);
			mMember->GetID();
			mMember->SetParentID(this->m_ID);
			this->Add(mMember, true);
			mMember->ImportIFC(mBldgEleId);
		}
	}
}

int MSFloor::InnerExportIFC()
{
	// get absolute height of base point of this storey
	double mElevation = ((MSBuilding*)this->GetOwner())->GetFloorLevel(this->m_nFloorNum); // Elevation = absolute height

	// export local placement
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	this->LP->CoordSys->_43 = mElevation;
	buildLocalPlacementInstance(this->LP);

	// export building storey instance
	char mFloorId[10];
	_ltoa(this->m_nFloorNum, mFloorId, 10);
	int mBuildingStoreyInstance = buildProductInstance("IFCBUILDINGSTOREY", (LPSTR)(LPCSTR)this->GetNameA(), mFloorId, NULL, this->LP->InstanceId, NULL);
	sdaiPutAttrBN(mBuildingStoreyInstance, "CompositionType", sdaiENUM, "ELEMENT");
	sdaiPutAttrBN(mBuildingStoreyInstance, "Elevation", sdaiREAL, &mElevation);

	// export building element instances
	int i = 0, mMemberCount = this->mm_CompoG.size();
	int* mAggrMemberInstance = new int[mMemberCount];
	map<long, MSCompoG*>::iterator mIterCompoG;
	for(mIterCompoG=mm_CompoG.begin(); mIterCompoG != mm_CompoG.end(); mIterCompoG++)
	{
		MSCompoG* pCompoG = mIterCompoG->second; // first:key, second:value
		mAggrMemberInstance[i] = pCompoG->ExportIFC(); i++;
	}

	//int* mAggrMemberInstance; int mMemberCount = 0;
	//map<long, MSCompoG*>::iterator mIterCompoG;
	//for(mIterCompoG=mm_CompoG.begin(); mIterCompoG != mm_CompoG.end(); mIterCompoG++)
	//{
	//	// prepare memory for relation between building and building storey
	//	mMemberCount++;
	//	if (mMemberCount == 1) mAggrMemberInstance = (int*)malloc(sizeof(int));
	//	else mAggrMemberInstance = (int*)realloc(mAggrMemberInstance, mMemberCount * sizeof(int));

	//	// export each building instance
	//	MSCompoG* pCompoG = mIterCompoG->second;
	//	mAggrMemberInstance[mMemberCount-1] = pCompoG->ExportIFC(mBuildingStoreyLP);
	//}

	//if (mMemberCount > 0) buildRelContainedInSpatialStructureInstance("Container", "Container for members", mBuildingStoreyInstance, mAggrMemberInstance, mMemberCount);
	if (mMemberCount > 0) buildRelAggregatesInstance("Container", "Container for members", mBuildingStoreyInstance, mAggrMemberInstance, mMemberCount, false);
	free(mAggrMemberInstance); // free is required even when mMemberCount == 0

	// return ifcId
	return mBuildingStoreyInstance;
}







///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSBuilding
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include "MSBuilding.h" // ==> done in MSFLoor

void MSBuilding::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, ...
	char* mName = 0x0; char* mDesc = 0x0; char* mObjType = 0x0; int mRepId = 0;
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	importProductInstance(pIfcId, &(this->GlobalId), &mName, &mDesc, &mObjType, *this->LP, mRepId);
	this->m_Name = CString(mName);
	this->m_ID = _atol_l(mDesc, 0);
	this->m_dGroundLevel = this->LP->CoordSys->_43;
	//sdaiGetAttrBN(pIfcId, "ElevationOfRefHeight", sdaiREAL, &this->m_dGroundLevel); // here, no need to read ElevationOfRefHeight

	// import stories
	CArray<int> mRelObjArr;
	importRelationInstance(pIfcId, "IsDecomposedBy", mRelObjArr);
	// 1. create all floors with null value and add to building
	//    - to postpone import of each floor is very important because of joints
	map<long, double> mStoreyLevelMap;
	for (int mRelIdx = 0; mRelIdx < mRelObjArr.GetSize(); mRelIdx++)
	{
		CArray<int> mObjArr;
		importRelationInstance(mRelObjArr[mRelIdx], "RelatedObjects", mObjArr);
		for (int mIdx = 0; mIdx < mObjArr.GetSize(); mIdx++)
		{
			// create and add floor
			int mBldgStryId = mObjArr[mIdx];
			char* mDesc; sdaiGetAttrBN(mBldgStryId, "Description", sdaiSTRING, &mDesc);
			MSFloor* mBldgStry = this->AddNewFloor(_atol_l(mDesc, 0));
			// get and store storey level
			double mStoreyLevel; sdaiGetAttrBN(mBldgStryId, "Elevation", sdaiREAL, &(mStoreyLevel)); // this is temporary
			pair<map<long, double>::iterator, bool> mPair;
			mPair = mStoreyLevelMap.insert(make_pair(mBldgStry->m_nFloorNum, mStoreyLevel));
		}
	}
	// 2. adjust the storey height of floors
	map<long, MSFloor*>::iterator mIterFloor;
	for(mIterFloor=mm_Floor.begin(); mIterFloor != mm_Floor.end(); mIterFloor++)
	{
		// current floor
		MSFloor* mFloor = mIterFloor->second; // first:key, second:value

		// upper floor
		long mUFloorID = mFloor->m_nFloorNum+1;
		if(mUFloorID == 0) mUFloorID = 1;
		MSFloor* mUFloor = this->GetFloor(mUFloorID);

		// storey height of the current floor
		map<long, double>::iterator mIter;
		if (mUFloor != NULL) // for top floor, storey height cannot be calculated <= limitation of IFC
		{
			mIter = mStoreyLevelMap.find(mFloor->m_nFloorNum);
			double mFloorLevel = mIter->second;
			mIter = mStoreyLevelMap.find(mUFloor->m_nFloorNum);
			double mUFloorLevel = mIter->second;

			mFloor->m_dHeight = mUFloorLevel - mFloorLevel;
		}
	}
	// 3. import each floor
	for (int mRelIdx = 0; mRelIdx < mRelObjArr.GetSize(); mRelIdx++)
	{
		CArray<int> mObjArr;
		importRelationInstance(mRelObjArr[mRelIdx], "RelatedObjects", mObjArr);
		for (int mIdx = 0; mIdx < mObjArr.GetSize(); mIdx++)
		{
			int mBldgStryId = mObjArr[mIdx];
			char* mDesc; sdaiGetAttrBN(mBldgStryId, "Description", sdaiSTRING, &mDesc);
			this->GetFloor(_atol_l(mDesc, 0))->ImportIFC(mBldgStryId);
		}
	}

	// import element types
	// done in MSCompoG
}

int MSBuilding::InnerExportIFC()
{
	// export local placement
	//this->LP = new ifcLocalPlacement(3); // these are done in MSSector::InnerExportIFC();
	this->LP->MomLP = this->GetOwner()->LP;
	this->LP->CoordSys->_43 = this->m_dGroundLevel;
	buildLocalPlacementInstance(this->LP);

	// export building instance
	char mBldgId[10];
	_ltoa(this->m_ID, mBldgId, 10);
	int mBuildingInstance = buildProductInstance("IFCBUILDING", (LPSTR)(LPCSTR)this->GetNameA(), mBldgId, NULL, this->LP->InstanceId, NULL);
	sdaiPutAttrBN(mBuildingInstance, "CompositionType", sdaiENUM, "ELEMENT");
	sdaiPutAttrBN(mBuildingInstance, "ElevationOfRefHeight", sdaiREAL, &this->m_dGroundLevel);

	//initiallize data for sections at MSCompoM
	vector<long> mEleTypeVec = this->GetCompoMs();
	vector<long>::iterator mEleTypeIter;
	for(mEleTypeIter = mEleTypeVec.begin(); mEleTypeIter != mEleTypeVec.end(); mEleTypeIter ++)
	{
		MSCompoM* mCompoM = this->GetCompoM(*mEleTypeIter);
		mCompoM->ProfileInstance = 0;
		if(mCompoM->AggrBldgEleInstance != NULL)
		{
			delete mCompoM->AggrBldgEleInstance;
			mCompoM->AggrBldgEleInstance = NULL;
		}
	}

	// export building storey instances
	int i = 0, mBldgStryCount = this->mm_Floor.size();
	int* mAggrBldgStryInstance = new int[mBldgStryCount];
	map<long, MSFloor*>::iterator mIterFloor;
	for(mIterFloor=mm_Floor.begin(); mIterFloor != mm_Floor.end(); mIterFloor++)
	{
		MSFloor* pFloor = mIterFloor->second; // first:key, second:value
		mAggrBldgStryInstance[i] = pFloor->ExportIFC(); i++;
	}
	//int* mAggrBldgStryInstance; int mBldgStryCount = 0;
	//map<long, MSFloor*>::iterator mIterFloor;
	//for(mIterFloor=mm_Floor.begin(); mIterFloor != mm_Floor.end(); mIterFloor++)
	//{
	//	// prepare memory for relation between building and building storey
	//	mBldgStryCount++;
	//	if (mBldgStryCount == 1) mAggrBldgStryInstance = (int*)malloc(sizeof(int));
	//	else mAggrBldgStryInstance = (int*)realloc(mAggrBldgStryInstance, mBldgStryCount * sizeof(int));

	//	// export each building instance
	//	MSFloor* pFloor = mIterFloor->second;
	//	mAggrBldgStryInstance[mBldgStryCount-1] = pFloor->ExportIFC(mBuildingLP);
	//}
	if (mBldgStryCount > 0) buildRelAggregatesInstance("Container", "Container for building storeys", mBuildingInstance, mAggrBldgStryInstance, mBldgStryCount);
	free(mAggrBldgStryInstance);

	// export element types
	for(mEleTypeIter = mEleTypeVec.begin(); mEleTypeIter != mEleTypeVec.end(); mEleTypeIter++)
	{
		this->GetCompoM(*mEleTypeIter)->ExportIFC();
	}

	// return ifcId
	return mBuildingInstance;
}






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSSector
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSSector.h"
#include "BuildingInfo.h"

void MSSector::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, ...
	char* mName = 0x0; char* mDesc = 0x0; char* mObjType = 0x0; int mRepId = 0;
	this->LP = new ifcLocalPlacement(3);
	this->LP->MomLP = this->GetOwner()->LP;
	importProductInstance(pIfcId, &(this->GlobalId), &mName, &mDesc, &mObjType, *this->LP, mRepId);
	this->m_Name = CString(mName);
	this->m_ID = _atol_l(mDesc, 0);

	// sites, buildings, floors, members
	//int* mRelIds; sdaiGetAttrBN(pIfcId, "IsDecomposedBy", sdaiAGGR, &mRelIds);
	//int mNumRelIds = sdaiGetMemberCount(mRelIds);
	//for (int mIdx = 0; mIdx < mNumRelIds; mIdx++)
	//{
	//	int mRelId; engiGetAggrElement(mRelIds, mIdx, sdaiINSTANCE, &mRelId);
	//	int* mBldgIds; sdaiGetAttrBN(mRelId, "RelatedObjects", sdaiAGGR, &mBldgIds);
	//	int mNumBldgIds = sdaiGetMemberCount(mBldgIds);
	//	for (int mBldgIdx = 0; mBldgIdx < mNumBldgIds; mBldgIdx++)
	//	{
	//		int mBldgId; engiGetAggrElement(mBldgIds, mBldgIdx, sdaiINSTANCE, &mBldgId);
	//		char* mName;
	//		sdaiGetAttrBN(mBldgId, "Name", sdaiSTRING, &mName);
	//		MSAMBuilding* mBldg = new MSAMBuilding();
	//		this->AddBldg(mName,  mBldg);
	//		mBldg->ImportIFC(mBldgId);
	//	}
	//}
	CArray<int> mRelObjArr;
	importRelationInstance(pIfcId, "IsDecomposedBy", mRelObjArr);
	for (int mRelIdx = 0; mRelIdx < mRelObjArr.GetSize(); mRelIdx++)
	{
		CArray<int> mObjArr;
		importRelationInstance(mRelObjArr[mRelIdx], "RelatedObjects", mObjArr);
		for (int mIdx = 0; mIdx < mObjArr.GetSize(); mIdx++)
		{
			int mBldgId = mObjArr[mIdx];
			char* mName;
			sdaiGetAttrBN(mBldgId, "Name", sdaiSTRING, &mName);
			MSBuilding* mBldg = MSMFactory::GetInstance()->GetBuildingInstance();
			mBldg->SetOwner(this);
			//this->AddBldg(mName, mBldg);
			SetCurrentBuilding(mBldg); // store building in IFCLib.dll
			mBldg->ImportIFC(mBldgId);

			// building info
			GMVector mOrg = GMVector(mBldg->LP->CoordSys->_41, mBldg->LP->CoordSys->_42, mBldg->LP->CoordSys->_43);
			GMVector mXVec = GMVector(mBldg->LP->CoordSys->_11, mBldg->LP->CoordSys->_21, mBldg->LP->CoordSys->_31);
			double mAngle = mXVec.GetRadian();
			this->AddBldg(mBldg->m_Name, mBldg, mOrg, mAngle);
		}
	}

	//MSFloor* pMSFloor = pBldg->AddNewFloor(iStory);
	//pMSFloor->m_Name = TheFloorL->m_Name;
	//pMSFloor->m_dHeight = TheFloorL->m_dHeight;
}

int MSSector::InnerExportIFC()
{
	// export default site instance; this is temporary
	int mSiteLP = buildDefaultLocalPlacement3DInstance(this->LP);
	char mSiteId[10];
	_ltoa(this->m_ID, mSiteId, 10);
	int mSiteInstance = buildProductInstance("IFCSITE", (LPSTR)(LPCSTR)this->GetNameA(), mSiteId, NULL, mSiteLP, NULL);
	sdaiPutAttrBN(mSiteInstance, "CompositionType", sdaiENUM, "ELEMENT");

	// export building instances
	int i = 0, mBldgCount = this->ma_BuildingInfo.size();
	int* mAggrBldgInstance = new int[mBldgCount];
	vector<BuildingInfo*>::iterator mIter;
	for(mIter=ma_BuildingInfo.begin(); mIter!= ma_BuildingInfo.end(); mIter++)
	{
		BuildingInfo* mBldgInfo = (*mIter);
		MSBuilding* mBldg = mBldgInfo->pBuilding;

		// get x-vec, y-vec, z-vec
		GMVector mXVec = GMVector(1.0, 0.0, 0.0); mXVec.RotateZ(mBldgInfo->dRotationAngle);
		GMVector mYVec = GMVector(0.0, 1.0, 0.0); mYVec.RotateZ(mBldgInfo->dRotationAngle);
		GMVector mZVec = GMVector(0.0, 0.0, 1.0);
		// get matrix for coordinate system of building
		mBldg->LP = new ifcLocalPlacement(3);
		mBldg->LP->Set(&mXVec, &mYVec, &mZVec, &(mBldgInfo->OffsetCoord));

		//mAggrBldgInstance[i] = (*mIter)->pBuilding->ExportIFC(); i++;
		mAggrBldgInstance[i] = mBldg->ExportIFC(); i++;
	}
	//int* mAggrBldgInstance; int mBldgCount = 0;
	//vector<BuildingInfo*>::iterator mIter;
	//for(mIter=ma_Building.begin(); mIter!= ma_Building.end(); mIter++)
	//{
	//	// prepare memory for relation between site and buildings
	//	mBldgCount++;
	//	if (mBldgCount == 1) mAggrBldgInstance = (int*)malloc(sizeof(int));
	//	else mAggrBldgInstance = (int*)realloc(mAggrBldgInstance, mBldgCount * sizeof(int));

	//	// export each building instance
	//	mAggrBldgInstance[mBldgCount-1] = (*mIter)->pBuilding->ExportIFC(mSiteLP);
	//}
	if (mBldgCount > 0) buildRelAggregatesInstance("Container", "Container for buildings", mSiteInstance, mAggrBldgInstance, mBldgCount);
	free(mAggrBldgInstance);

	// return ifcId
	return mSiteInstance;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// MSProject
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MSProject.h"

void MSProject::InnerImportIFC(int pIfcId)
{
	// import GlobalId, Name, ...
	char* mName = 0x0; char* mDesc = 0x0; char* mObjType = 0x0;
	importObjectInstance(pIfcId, &(this->GlobalId), &mName, &mDesc, &mObjType);
	//this->ms_ProjectName = CString(pName);
	this->m_Name = CString(mName);
	this->m_ID = _atol_l(mDesc, 0);

	// import units

	// sites, buildings, floors, members
	//int* mRelIds; sdaiGetAttrBN(pIfcId, "IsDecomposedBy", sdaiAGGR, &mRelIds);
	//int mNumRelIds = sdaiGetMemberCount(mRelIds);
	//for (int mIdx = 0; mIdx < mNumRelIds; mIdx++)
	//{
	//	int mRelId; engiGetAggrElement(mRelIds, mIdx, sdaiINSTANCE, &mRelId);
	//	int* mSiteIds; sdaiGetAttrBN(mRelId, "RelatedObjects", sdaiAGGR, &mSiteIds);
	//	int mNumSiteIds = sdaiGetMemberCount(mSiteIds);
	//	for (int mSiteIdx = 0; mSiteIdx < mNumSiteIds; mSiteIdx++)
	//	{
	//		int mSiteId; engiGetAggrElement(mSiteIds, mSiteIdx, sdaiINSTANCE, &mSiteId);
	//		char* mName;
	//		sdaiGetAttrBN(mSiteId, "Name", sdaiSTRING, &mName);
	//		MSSector* mSite = this->AddSector(mName);
	//		mSite->ImportIFC(mSiteId);
	//	}
	//}
	CArray<int> mRelObjArr;
	importRelationInstance(pIfcId, "IsDecomposedBy", mRelObjArr/*, bool pRelAggr/* = true*/);
	for (int mRelIdx = 0; mRelIdx < mRelObjArr.GetSize(); mRelIdx++)
	{
		CArray<int> mObjArr;
		importRelationInstance(mRelObjArr[mRelIdx], "RelatedObjects", mObjArr/*, bool pRelAggr/* = true*/);
		for (int mIdx = 0; mIdx < mObjArr.GetSize(); mIdx++)
		{
			int mSiteId = mObjArr[mIdx];
			char* mName;
			sdaiGetAttrBN(mSiteId, "Name", sdaiSTRING, &mName);
			MSSector* mSite = this->AddSector(mName);
			mSite->ImportIFC(mSiteId);
		}
	}

	//// import prpoerty set
	//// get property for from story and to story
	//CArray<int> mRelPrptArr;
	//importRelationInstance(pIfcId, "IsDefinedBy", mRelPrptArr, "IFCRELDEFINESBYPROPERTIES");
	//for (int mRelIdx = 0; mRelIdx < mRelPrptArr.GetSize(); mRelIdx++)
	//{
	//	CArray<int> mPrptIdArr;
	//	importRelationInstance(mRelPrptArr[mRelIdx], "RelatingPropertyDefinition", mPrptIdArr);
	//	for (int mIdx = 0; mIdx < mPrptIdArr.GetSize(); mIdx++)
	//	{
	//		int mPrptId = mPrptIdArr[mIdx];
	//		ifcPset* mPset = new ifcPset(mPrptIdArr[mIdx]);
	//		importPropertySet(mPset);
	//		AddPset(mPset);
	//	}
	//}

	//ifcPset* mPset = new ifcPset(_T("FromToStory"), _T(""));
	//importPropertySet(mPset);
	////this->m_nFromStory = 0L;
	////this->m_nToStory = 0L;
}

int MSProject::InnerExportIFC()
{
	// export project instance
	char mPrjId[10];
	_ltoa(this->m_ID, mPrjId, 10);
	int mProjectInstance  = buildObjectInstance("IFCPROJECT", (LPSTR)(LPCSTR)this->GetNameA(), mPrjId, NULL);
	//CStringA mPrjId; mPrjId.Format("%ld", this->m_ID);
	//if (mPrjId.GetLength() == 0) mPrjId = NULL;
	//int mProjectInstance  = buildObjectInstance("IFCPROJECT", (LPSTR)(LPCSTR)this->GetNameA(), (LPSTR)(LPCSTR)mPrjId, NULL);
	int* mAggrRepresentationContexts = sdaiCreateAggrBN(mProjectInstance, "RepresentationContexts");
	sdaiAppend((int)mAggrRepresentationContexts, sdaiINSTANCE, (void*) buildDefaultGeometricRepresentationContextInstance());
	sdaiPutAttrBN(mProjectInstance, "UnitsInContext", sdaiINSTANCE, (void*)buildDefaultUnitAssignmentInstance());

	// export site instances
	int i = 0, mSiteCount = this->ma_Sector.size();
	int* mAggrSiteInstance = new int[mSiteCount];
	vector<MSSector*>::iterator mIter;
	for(mIter=this->ma_Sector.begin(); mIter != this->ma_Sector.end(); mIter++)
	{
		mAggrSiteInstance[i] = (*mIter)->ExportIFC(); i++; // 0 = no mother local placement = world coordinates
	}

	//int* mAggrSiteInstance; int mSiteCount = 0;
	//vector<MSSector*>::iterator mIter;
	//{
	//	// prepare memory for relation between site and buildings
	//	mSiteCount++;
	//	if (mSiteCount == 1) mAggrSiteInstance = (int*)malloc((sizeof(int)));
	//	else mAggrSiteInstance = (int*)realloc(mAggrSiteInstance, mSiteCount * sizeof(int));

	//	// export each site instance
	//	mAggrSiteInstance[mSiteCount-1] = (*mIter)->ExportIFC(0); // 0 = no mother local placement = world coordinates
	//}
	if (mSiteCount > 0) buildRelAggregatesInstance("Container", "Container for sites", mProjectInstance, mAggrSiteInstance, mSiteCount);
	free(mAggrSiteInstance);

	// return ifcId
	return mProjectInstance;
}

