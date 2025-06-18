#include "StdAfx.h"
#include "MSCheckModelOption.h"
#include "MSElement.h"
#include "MSCompoG.h"

MSCheckModelOption* MSCheckModelOption::m_instance = NULL;
MSCheckModelOption* MSCheckModelOption::GetInstance()
{
	if(m_instance == NULL)
		m_instance = new MSCheckModelOption();		

	return m_instance;
}


MSCheckModelOption::MSCheckModelOption(void)
{
	m_dColinearDegreeTol = 5;
	InitOpt();
}


MSCheckModelOption::~MSCheckModelOption(void)
{
	DeleteOptItems();
}

bool MSCheckModelOption::WriteFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if(file.Open(strFilePath, CFile::modeCreate | CFile::modeWrite, &e)==FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	return true;
}

int MSCheckModelOption::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if(file.Open(strFilePath, CFile::modeRead, &e)==FALSE)
	{
		if(e.m_cause == CFileException::fileNotFound)
			return e.m_cause;
		else
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	if(file.GetLength() <= 0)	return e.m_cause;

	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	return e.m_cause;
}

void MSCheckModelOption::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		m_nCheckModelOpt.Serialize(ar);

		int NumItem = ma_CheckModelItem.size();
		ar << NumItem;
		for(int i = 0 ; i < NumItem; i++)
		{
			MSCheckModelItem* pItem = ma_CheckModelItem[i];
			pItem->Serialize(ar);
		}
	}
	else
	{
		DeleteOptItems();
		m_nCheckModelOpt.Serialize(ar);
		
		int NumItem = 0;
		ar >> NumItem;
		for(long i = 0 ; i < NumItem ; i++)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem();
			pItem->Serialize(ar);
			ma_CheckModelItem.push_back(pItem);
		}

		UpdateOptItems();
	}
}

long MSCheckModelOption::GetErrorLevel(eCheckModel eBaseOpt, long eSecondOpt)
{
	long nERROR = 1;
	long nWarning = -1;

	MSCheckModelItem* pItem = GetOptItem(eBaseOpt, eSecondOpt);
	if(pItem != NULL)
		return pItem->m_nErrorLevel;

	return MESSAGE_LEVEL_ERROR;
}

int MSCheckModelOption::GetCheckVal(eCheckModel eBaseOpt, long eSecondOpt)
{
	MSCheckModelItem* pItem = GetOptItem(eBaseOpt, eSecondOpt);
	if(pItem != NULL)
		return pItem->m_nCheckVal;

	return -1;
}

bool MSCheckModelOption::isUseCheckVal(eCheckModel eBaseOpt, long eSecondOpt)
{
	MSCheckModelItem* pItem = GetOptItem(eBaseOpt, eSecondOpt);
	if(pItem != NULL)
		return pItem->bUseCheckVal();
	
	return false;
}

void MSCheckModelOption::InitOpt()
{
	DeleteOptItems();
	m_nCheckModelOpt.Init();

	CString strKey;
	MSCheckModelItem* pItem = NULL;

	// CMO_GEN
	strKey = GetKey(CMO_GEN, CMO_GEN_SLAB_PROFILE);
	pItem = new MSCheckModelItem(CMO_GEN, CMO_GEN_SLAB_PROFILE, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_GEN, CMO_GEN_SLAB_DIST);
	pItem = new MSCheckModelItem(CMO_GEN, CMO_GEN_SLAB_DIST, 5, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);
	
	strKey = GetKey(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN);
	pItem = new MSCheckModelItem(CMO_GEN, CMO_GEN_LINEMEMBEROUTLEN, 10, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_GEN, CMO_GEN_SLAB_AREA);
	pItem = new MSCheckModelItem(CMO_GEN, CMO_GEN_SLAB_AREA, 100, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	// CMO_MODEL
	strKey = GetKey(CMO_MODEL, CMO_MODEL_NONAME);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_NONAME, -1, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);
	
	strKey = GetKey(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_VER_ZLEVELOVERLAP, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_HOR_ZLEVELOVERLAP, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_VER_CANTIL);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_VER_CANTIL, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_HOR_CANTIL);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_HOR_CANTIL, -1, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_NOT_CONNECTION);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_NOT_CONNECTION, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_VER_CONTAIN);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_VER_CONTAIN, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP);
	pItem = new MSCheckModelItem(CMO_MODEL, CMO_MODEL_SAMEMEMBER_OVERLAP, -1, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	//CMO_VER_SEC
	strKey = GetKey(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW);
	pItem = new MSCheckModelItem(CMO_VER_SEC, CMO_VER_SEC_SIZE_BELOW, 50, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER);
	pItem = new MSCheckModelItem(CMO_VER_SEC, CMO_VER_SEC_SIZE_OVER, 50, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	//CMO_VER_OUTECC
	strKey = GetKey(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW);
	pItem = new MSCheckModelItem(CMO_VER_OUTECC, CMO_VER_OUTECC_BELOW, 5, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER);
	pItem = new MSCheckModelItem(CMO_VER_OUTECC, CMO_VER_OUTECC_OVER, 5, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	//CMO_VER_SUPPORT
	strKey = GetKey(CMO_VER_SUPPORT, CMO_VER_SUPPORT_OVER);
	pItem = new MSCheckModelItem(CMO_VER_SUPPORT, CMO_VER_SUPPORT_OVER, 99, MESSAGE_LEVEL_WARNING, false);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_VER_SUPPORT, CMO_VER_SUPPORT_BELOW);
	pItem = new MSCheckModelItem(CMO_VER_SUPPORT, CMO_VER_SUPPORT_BELOW, 99, MESSAGE_LEVEL_ERROR, true);
	ma_CheckModelItem.push_back(pItem);

	//CMO_HOR_SEC
	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_BELOW);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_BELOW, 50, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_OVER);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_WIDTH_OVER, 50, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_BELOW);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_BELOW, 50, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_OVER);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_BEAM_HEIGHT_OVER, 50, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_SLAB_THICK_BELOW);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_SLAB_THICK_BELOW, 50, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_SEC, CMO_HOR_SEC_SLAB_THICK_OVER);
	pItem = new MSCheckModelItem(CMO_HOR_SEC, CMO_HOR_SEC_SLAB_THICK_OVER, 50, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	//CMO_HOR_LEVELDIF
	strKey = GetKey(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW);
	pItem = new MSCheckModelItem(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_BELOW, 50, MESSAGE_LEVEL_WARNING, true);
	ma_CheckModelItem.push_back(pItem);

	strKey = GetKey(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER);
	pItem = new MSCheckModelItem(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_OVER, 50, MESSAGE_LEVEL_ERROR, false);
	ma_CheckModelItem.push_back(pItem);

	SetCheckedBaseOption(CMO_GEN, true);
	SetCheckedBaseOption(CMO_MODEL, true);
	SetCheckedBaseOption(CMO_VER_SEC, true);
	SetCheckedBaseOption(CMO_VER_OUTECC, true);
	SetCheckedBaseOption(CMO_VER_SUPPORT, true);
	SetCheckedBaseOption(CMO_HOR_SEC, true);
	SetCheckedBaseOption(CMO_HOR_LEVELDIF, true);

	SetCheckedSecoundOptionAll(CMO_GEN, true);
	SetCheckedSecoundOptionAll(CMO_MODEL, true);
	SetCheckedSecoundOptionAll(CMO_VER_SEC, true);
	SetCheckedSecoundOptionAll(CMO_VER_OUTECC, true);
	SetCheckedSecoundOptionAll(CMO_VER_SUPPORT, true);
	SetCheckedSecoundOptionAll(CMO_HOR_SEC, true);
	SetCheckedSecoundOptionAll(CMO_HOR_LEVELDIF, true);
}

CString MSCheckModelOption::GetKey(eCheckModel eBaseOpt, long nSecondOpt)
{
	CString strKey;
	strKey.Format(_T("%d-%d"), eBaseOpt, nSecondOpt);
	return strKey;	
}

MSCheckModelItem* MSCheckModelOption::GetOptItem(eCheckModel eBaseOpt, long nSecondOpt)
{
	CString strKey = GetKey(eBaseOpt, nSecondOpt);

	for(long i = 0 ; i < ma_CheckModelItem.size() ; i++)
	{
		MSCheckModelItem* pItem = ma_CheckModelItem[i];
		if(pItem->m_nBaseOpt == (long)eBaseOpt && pItem->m_nSecondOpt == nSecondOpt)
			return pItem;
	}

	return NULL;
}

void MSCheckModelOption::DeleteOptItems()
{
	for(long i = 0 ; i < ma_CheckModelItem.size() ; i++)
	{
		MSCheckModelItem* pItem = ma_CheckModelItem[i];
		delete pItem;
	}
	ma_CheckModelItem.clear();
}

void MSCheckModelOption::UpdateOptItems()
{
	eCheckModel eBaseOpt = CMO_GEN;
	long eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_GEN_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}

	eBaseOpt = CMO_MODEL;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_MODEL_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
	
	eBaseOpt = CMO_VER_SEC;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_VER_SEC_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
	
	eBaseOpt = CMO_VER_OUTECC;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_VER_OUTECC_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
	
	eBaseOpt = CMO_VER_SUPPORT;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_VER_SUPPORT_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
	
	eBaseOpt = CMO_HOR_SEC;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_HOR_SEC_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
	
	eBaseOpt = CMO_HOR_LEVELDIF;
	eSecondOpt = 2;
	for (long nSecondOpt = 0; nSecondOpt < CMO_HOR_LEVELDIF_CNT; nSecondOpt++)
	{
		MSCheckModelItem* pItem = GetOptItem(eBaseOpt, nSecondOpt);
		if (pItem == NULL)
		{
			MSCheckModelItem* pItem = new MSCheckModelItem(eBaseOpt, eSecondOpt, -1, MESSAGE_LEVEL_ERROR, true);
			ma_CheckModelItem.push_back(pItem);
			SetCheckedSecoundOption(eBaseOpt, eSecondOpt, true);
		}
		eSecondOpt = eSecondOpt << 1;
	}
}

bool MSCheckModelOption::IsCheckedBaseOption(long nBaseOpt)
{
	return m_nCheckModelOpt.IsCheckedBaseOption(nBaseOpt);
}

bool MSCheckModelOption::IsCheckedSecoundOption(long nBaseOpt, long nSecondOpt)
{
	return m_nCheckModelOpt.IsCheckedSecoundOption(nBaseOpt, nSecondOpt);
}

void MSCheckModelOption::SetCheckedBaseOption(long nBaseOpt, bool bVal)
{
	m_nCheckModelOpt.SetCheckedBaseOption(nBaseOpt, bVal);
}

void MSCheckModelOption::SetCheckedSecoundOption(long nBaseOpt, long nSecondOpt, bool bVal)
{
	m_nCheckModelOpt.SetCheckedSecoundOption(nBaseOpt, nSecondOpt, bVal);
}

void MSCheckModelOption::SetCheckedSecoundOptionAll(long nBaseOpt, bool bVal)
{
	m_nCheckModelOpt.SetCheckedSecoundOptionAll(nBaseOpt, bVal);
}
