#include "StdAfx.h"
#include "DTDowelDevelopment.h"


DTDowelDevelopment::DTDowelDevelopment()
{
}


DTDowelDevelopment::~DTDowelDevelopment(void)
{
}

long DTDowelDevelopment::MakeTestDefaultSample( vector<DTDowelDevelopment*>& DTDowelDevelopmentArr )
{
	//최하단피복까지 Hook정착
	DTDowelDevelopment* pDTDowelDevelopment = new DTDowelDevelopment();
	pDTDowelDevelopment->SetDevType(DevType_Hook_Cover);
	pDTDowelDevelopment->SetSpliceType(SpliceType_All_B);
	DTDowelDevelopmentArr.push_back(pDTDowelDevelopment);


	//자동계산 정착
	pDTDowelDevelopment = new DTDowelDevelopment();
	pDTDowelDevelopment->SetDevType(DevType_Stright_Tension);
	pDTDowelDevelopment->SetSpliceType(SpliceType_All_B);
	DTDowelDevelopmentArr.push_back(pDTDowelDevelopment);

	//정착길이 사용자 입력
	pDTDowelDevelopment = new DTDowelDevelopment();
	pDTDowelDevelopment->SetDevType(DevType_UserLenHook_UserDevLen);
	pDTDowelDevelopment->SetUserDevLen(1000);
	pDTDowelDevelopment->SetUserHookLen(500);
	pDTDowelDevelopment->SetSpliceType(SpliceType_All_B);
	DTDowelDevelopmentArr.push_back(pDTDowelDevelopment);

	//코너만 최하단 정착

	return DTDowelDevelopmentArr.size();

}
