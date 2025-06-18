#include "StdAfx.h"
#include "CogReaderHelper.h"

#include "CogReader.h"

CogReaderHelper::CogReaderHelper(MSBaseBuilding* pBldg, CogReader* pReader)
{
	mp_Bldg = pBldg;
	mp_Reader = pReader;
}


CogReaderHelper::~CogReaderHelper(void)
{
}

void CogReaderHelper::AddErrorObject(MSElement* pElement, long nErrorCode, long nLevel, CString strGroup, CString strMsg)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	if(pMessageContainer == NULL)
		return;

	XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nLevel, strMsg, XErrorObject::MSElementIDSelect);
	if(pMsgObject == NULL)
		return;

	if (pMsgObject)
	{
		pMsgObject->SetGroupName(strGroup);
		pMsgObject->SetShowSelectCompoG(TRUE);
		pMessageContainer->AddErrorObject(pMsgObject);
		pMsgObject->SetDocID(mp_Reader->m_nDocID);
		pMsgObject->ma_CompoGID.Add(pElement->m_ID);

		vector<MSElement*> elementArr;
		elementArr.push_back(pElement);
		mp_Reader->mm_ErrorToMSElements.insert(make_pair(pMsgObject, elementArr));
	}
}