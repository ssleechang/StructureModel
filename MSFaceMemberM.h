#pragma once
#include "mscompom.h"

class StructureModel_CLASS MSFaceMemberM :
	public MSCompoM
{
// end1309
public:
	MSFaceMemberM(void);
	~MSFaceMemberM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSFaceMemberM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual CString ToString();

	virtual double GetWidth();
	virtual double GetDepth();//by LSS for Slab
	virtual double GetThick();
	virtual double GetThickXMLExport();
	virtual void SetThick(double Thick);

	
	virtual bool IsMatSlab(){return false;};//by LSS for Slab
	virtual bool IsSOGSlab(){ return false; };//by LSS for Slab
	
	virtual long GetHaunchType(){return 0;}//by LSS for IsoFoot
	virtual void SetWidth(double dWidth){m_dThick = dWidth;};	// by gracias for IsoFootM
	virtual void SetDepth(double dDepth){};	// by gracias for IsoFootM;
	virtual long GetSlabMType(){return -1;};
	
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
protected:
	double m_dThick;


//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	// start of addition by ironbell
//public:
//	virtual void ImportProfile(int pIfcId); // called by MSCompoG.cpp not by this->InnerImportIFC()
//	virtual int ExportProfile(double pWidth); // called by MSCompoG.cpp not by this->InnerExportIFC()
//	// end of addition by ironbell
//	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};
