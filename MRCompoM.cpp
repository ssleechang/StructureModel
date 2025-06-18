#include "StdAfx.h"
#include "MRCompoM.h"
#include "MRSection.h"
#include "MSCompoM.h"
#include "MSContainer.h"
IMPLEMENT_SERIAL(MRCompoM, MSObject, VERSIONABLE_SCHEMA | 1)

MRCompoM::MRCompoM(void)
{
	mp_MSM = NULL;
}

MRCompoM::~MRCompoM(void)
{
	DeleteSections();
}

void MRCompoM::DeleteSections()
{
	long nSection = ma_Section.GetSize();
	for(long iSection=0; iSection<nSection; iSection++)
		delete ma_Section[iSection];
	ma_Section.RemoveAll();
}
void MRCompoM::AddSection(MRSection* pSection)
{
	ma_Section.Add(pSection);
}
void MRCompoM::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	//long nSection = 0;
	//if(ar.IsStoring())
	//{
	//	nSection = ma_Section.GetSize();
	//	ar << nSection;
	//	for(long iSection=0; iSection<nSection; iSection++)
	//	{
	//		MRSection* pSection = ma_Section[iSection];
	//		pSection->Serialize(ar);
	//	}
	//}
	//else
	//{
	//	ar >> nSection;
	//	for(long iSection=0; iSection<nSection; iSection++)
	//	{
	//		MRSection* pSection = mp_MSM->mp_Owner->CreateNewMRSection(0);
	//		pSection->Serialize(ar);
	//		ma_Section.Add(pSection);
	//	}
	//}
}



