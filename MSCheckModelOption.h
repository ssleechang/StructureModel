#pragma once
#include "msobject.h"
#include "StructureModelEnum.h"
#include "resource.h"

typedef union _uCheckModelOpt
{
	long m_nBaseOpt;
	long m_nSecondOpt;
	_uCheckModelOpt(long a = 0, long b = 0)
	{
		m_nBaseOpt = a;
		m_nSecondOpt = b;
	}
}uCheckModelOpt;

typedef struct _CheckModelOpt
{
	_CheckModelOpt()
	{
		Init();
	};
	_CheckModelOpt& operator= (const _CheckModelOpt& aData)
	{
		m_nBaseOption = aData.m_nBaseOption;

		m_nGenOpt = aData.m_nGenOpt;
		m_nModelOpt = aData.m_nModelOpt;
		m_nVerSecOpt = aData.m_nVerSecOpt;
		m_nVerOutEccOpt = aData.m_nVerOutEccOpt;
		m_nVerSupportOpt = aData.m_nVerSupportOpt;
		m_nHorSecOpt = aData.m_nHorLevelDifOpt;
		m_nHorLevelDifOpt = aData.m_nHorLevelDifOpt;

		return *this;
	}
	void Init()
	{
		m_nBaseOption = 0;

		m_nGenOpt = 0;
		m_nModelOpt = 0;
		m_nVerSecOpt = 0;
		m_nVerOutEccOpt = 0;
		m_nVerSupportOpt = 0;
		m_nHorSecOpt = 0;
		m_nHorLevelDifOpt = 0;
	}
	void Serialize( CArchive& ar )
	{
		if(ar.IsStoring())
		{
			ar << m_nBaseOption;

			ar << m_nGenOpt;
			ar << m_nModelOpt;
			ar << m_nVerSecOpt;
			ar << m_nVerOutEccOpt;
			ar << m_nVerSupportOpt;
			ar << m_nHorSecOpt;
			ar << m_nHorLevelDifOpt;
		}
		else
		{
			ar >> m_nBaseOption;

			ar >> m_nGenOpt;
			ar >> m_nModelOpt;
			ar >> m_nVerSecOpt;
			ar >> m_nVerOutEccOpt;
			ar >> m_nVerSupportOpt;
			ar >> m_nHorSecOpt;
			ar >> m_nHorLevelDifOpt;
		}
	}

	bool isCheck()
	{
		if(m_nGenOpt + m_nModelOpt + m_nVerSecOpt + m_nVerOutEccOpt + m_nVerSupportOpt + m_nHorSecOpt + m_nHorLevelDifOpt == 0)
			return false;

		return true;
	}

	long GetCheckCnt()
	{
		int nErrorCnt = 0;

		nErrorCnt += GetCheckCntInSecondOpt(CMO_GEN, CMO_GEN_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_MODEL, CMO_MODEL_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_VER_SEC, CMO_VER_SEC_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_VER_OUTECC, CMO_VER_OUTECC_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_VER_SUPPORT, CMO_VER_SUPPORT_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_HOR_SEC, CMO_HOR_SEC_CNT);
		nErrorCnt += GetCheckCntInSecondOpt(CMO_HOR_LEVELDIF, CMO_HOR_LEVELDIF_CNT);
		
		return nErrorCnt;
	}

	long GetCheckCntInSecondOpt(long nBaseOpt, long nCnt)
	{
		long nCheckCnt = 0;
		if(IsCheckedBaseOption(nBaseOpt) == false)
			return nCheckCnt;

		long nSecondOpt = 2;
		for(long i = 0 ; i < nCnt ; i++)
		{
			if(IsCheckedSecoundOption(nBaseOpt, nSecondOpt))
				nCheckCnt++;
			nSecondOpt = nSecondOpt << 1;
		}
		return nCheckCnt;
	}


	bool IsCheckedBaseOption(long nBaseOpt)
	{
		if(m_nBaseOption & nBaseOpt)
			return true;
		return false;
	};

	static long GetErrorCode(long nBaseOpt, long nSecondOpt)
	{
		if(nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_PROFILE)
			return MESSAGE_CMO_GEN_SLAB_PROFILE;
		else if(nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_DIST)
			return MESSAGE_CMO_GEN_SLAB_DIST;
		else if(nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_LINEMEMBEROUTLEN)
			return MESSAGE_CMO_GEN_LINEMEMBEROUTLEN;
		else if (nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_AREA)
			return MESSAGE_CMO_GEN_SLAB_AREA;
		
		if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_NONAME)
			return MESSAGE_CMO_MODEL_NONAME;
		else if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_ZLEVELOVERLAP)
			return MESSAGE_CMO_MODEL_VER_ZLEVELOVERLAP;
		else if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_HOR_ZLEVELOVERLAP)
			return MESSAGE_CMO_MODEL_HOR_ZLEVELOVERLAP;
		else if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_CANTIL)
			return MESSAGE_CMO_MODEL_VER_CANTIL;
		else if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_HOR_CANTIL)
			return MESSAGE_CMO_MODEL_HOR_CANTIL;
		else if(nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_NOT_CONNECTION)
			return MESSAGE_CMO_MODEL_NOT_CONNECTION;
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_CONTAIN)
			return MESSAGE_CMO_MODEL_VER_CONTAIN;
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_SAMEMEMBER_OVERLAP)
			return MESSAGE_CMO_MODEL_SAMEMEMBER_OVERLAP;
		
		if(nBaseOpt == CMO_VER_SEC && nSecondOpt == CMO_VER_SEC_SIZE_BELOW)
			return MESSAGE_CMO_VER_SEC_SIZE_BELOW;
		else if(nBaseOpt == CMO_VER_SEC && nSecondOpt == CMO_VER_SEC_SIZE_OVER)
			return MESSAGE_CMO_VER_SEC_SIZE_OVER;

		if(nBaseOpt == CMO_VER_OUTECC && nSecondOpt == CMO_VER_OUTECC_BELOW)
			return MESSAGE_CMO_VER_OUTECC_BELOW;
		else if(nBaseOpt == CMO_VER_OUTECC && nSecondOpt == CMO_VER_OUTECC_OVER)
			return MESSAGE_CMO_VER_OUTECC_OVER;

		if(nBaseOpt == CMO_VER_SUPPORT && nSecondOpt == CMO_VER_SUPPORT_BELOW)
			return MESSAGE_CMO_VER_SUPPORT_BELOW;
		else if(nBaseOpt == CMO_VER_SUPPORT && nSecondOpt == CMO_VER_SUPPORT_OVER)
			return MESSAGE_CMO_VER_SUPPORT_OVER;

		if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_WIDTH_BELOW)
			return MESSAGE_CMO_HOR_SEC_BEAM_WIDTH_BELOW;
		else if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_WIDTH_OVER)
			return MESSAGE_CMO_HOR_SEC_BEAM_WIDTH_OVER;
		else if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_HEIGHT_BELOW)
			return MESSAGE_CMO_HOR_SEC_BEAM_HEIGHT_BELOW;
		else if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_HEIGHT_OVER)
			return MESSAGE_CMO_HOR_SEC_BEAM_HEIGHT_OVER;
		else if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_SLAB_THICK_BELOW)
			return MESSAGE_CMO_HOR_SEC_SLAB_THICK_BELOW;
		else if(nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_SLAB_THICK_OVER)
			return MESSAGE_CMO_HOR_SEC_SLAB_THICK_OVER;
		
		if(nBaseOpt == CMO_HOR_LEVELDIF && nSecondOpt == CMO_HOR_LEVELDIF_BELOW)
			return MESSAGE_CMO_HOR_LEVELDIF_BELOW;
		else if(nBaseOpt == CMO_HOR_LEVELDIF && nSecondOpt == CMO_HOR_LEVELDIF_OVER)
			return MESSAGE_CMO_HOR_LEVELDIF_OVER;

		ASSERT(0);

		return MESSAGE_CMO_UNKNOWN;
	}

	static CString GetErrorCodeStr(long nBaseOpt, long nSecondOpt)
	{
		if (nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_PROFILE)
			return L"MESSAGE_CMO_GEN_SLAB_PROFILE";
		else if (nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_DIST)
			return L"MESSAGE_CMO_GEN_SLAB_DIST";
		else if (nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_LINEMEMBEROUTLEN)
			return L"MESSAGE_CMO_GEN_LINEMEMBEROUTLEN";
		else if (nBaseOpt == CMO_GEN && nSecondOpt == CMO_GEN_SLAB_AREA)
			return L"MESSAGE_CMO_GEN_SLAB_AREA";

		if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_NONAME)
			return L"MESSAGE_CMO_MODEL_NONAME";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_ZLEVELOVERLAP)
		return L"MESSAGE_CMO_MODEL_VER_ZLEVELOVERLAP";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_HOR_ZLEVELOVERLAP)
		return L"MESSAGE_CMO_MODEL_HOR_ZLEVELOVERLAP";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_CANTIL)
		return L"MESSAGE_CMO_MODEL_VER_CANTIL";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_HOR_CANTIL)
		return L"MESSAGE_CMO_MODEL_HOR_CANTIL";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_NOT_CONNECTION)
		return L"MESSAGE_CMO_MODEL_NOT_CONNECTION";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_VER_CONTAIN)
			return L"MESSAGE_CMO_MODEL_VER_CONTAIN";
		else if (nBaseOpt == CMO_MODEL && nSecondOpt == CMO_MODEL_SAMEMEMBER_OVERLAP)
			return L"MESSAGE_CMO_MODEL_SAMEMEMBER_OVERLAP";

		if (nBaseOpt == CMO_VER_SEC && nSecondOpt == CMO_VER_SEC_SIZE_BELOW)
			return L"MESSAGE_CMO_VER_SEC_SIZE_BELOW";
		else if (nBaseOpt == CMO_VER_SEC && nSecondOpt == CMO_VER_SEC_SIZE_OVER)
		return L"MESSAGE_CMO_VER_SEC_SIZE_OVER";

		if (nBaseOpt == CMO_VER_OUTECC && nSecondOpt == CMO_VER_OUTECC_BELOW)
			return L"MESSAGE_CMO_VER_OUTECC_BELOW";
		else if (nBaseOpt == CMO_VER_OUTECC && nSecondOpt == CMO_VER_OUTECC_OVER)
		return L"MESSAGE_CMO_VER_OUTECC_OVER";

		if (nBaseOpt == CMO_VER_SUPPORT && nSecondOpt == CMO_VER_SUPPORT_BELOW)
			return L"MESSAGE_CMO_VER_SUPPORT_BELOW";
		else if (nBaseOpt == CMO_VER_SUPPORT && nSecondOpt == CMO_VER_SUPPORT_OVER)
		return L"MESSAGE_CMO_VER_SUPPORT_OVER";

		if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_WIDTH_BELOW)
			return L"MESSAGE_CMO_HOR_SEC_BEAM_WIDTH_BELOW";
		else if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_WIDTH_OVER)
		return L"MESSAGE_CMO_HOR_SEC_BEAM_WIDTH_OVER";
		else if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_HEIGHT_BELOW)
		return L"MESSAGE_CMO_HOR_SEC_BEAM_HEIGHT_BELOW";
		else if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_BEAM_HEIGHT_OVER)
		return L"MESSAGE_CMO_HOR_SEC_BEAM_HEIGHT_OVER";
		else if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_SLAB_THICK_BELOW)
		return L"MESSAGE_CMO_HOR_SEC_SLAB_THICK_BELOW";
		else if (nBaseOpt == CMO_HOR_SEC && nSecondOpt == CMO_HOR_SEC_SLAB_THICK_OVER)
		return L"MESSAGE_CMO_HOR_SEC_SLAB_THICK_OVER";

		if (nBaseOpt == CMO_HOR_LEVELDIF && nSecondOpt == CMO_HOR_LEVELDIF_BELOW)
			return L"MESSAGE_CMO_HOR_LEVELDIF_BELOW";
		else if (nBaseOpt == CMO_HOR_LEVELDIF && nSecondOpt == CMO_HOR_LEVELDIF_OVER)
		return L"MESSAGE_CMO_HOR_LEVELDIF_OVER";

		ASSERT(0);

		return L"MESSAGE_CMO_UNKNOWN";
	}
	bool IsCheckedSecoundOption(long nBaseOpt, long nSecondOpt)
	{
		if(nBaseOpt == CMO_GEN)
		{
			if(m_nGenOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_MODEL)
		{
			if(m_nModelOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_VER_SEC)
		{
			if(m_nVerSecOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_VER_OUTECC)
		{
			if(m_nVerOutEccOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_VER_SUPPORT)
		{
			if(m_nVerSupportOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_HOR_SEC)
		{
			if(m_nHorSecOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else if(nBaseOpt == CMO_HOR_LEVELDIF)
		{
			if(m_nHorLevelDifOpt & nSecondOpt)
				return true;
			else
				return false;
		}
		else
			ASSERT(0);

		return false;
	};

	void SetCheckedBaseOption(long nBaseOpt, bool bVal)
	{
		if(bVal == true)
			m_nBaseOption |= (long)nBaseOpt;
		else
			m_nBaseOption &= ~(long)nBaseOpt;
	};

	void SetCheckedSecoundOption(long nBaseOpt, long nSecondOpt, bool bVal)
	{
		long* nOpt = NULL;
		if(nBaseOpt == CMO_GEN)
			nOpt = &m_nGenOpt;
		else if(nBaseOpt == CMO_MODEL)
			nOpt = &m_nModelOpt;
		else if(nBaseOpt == CMO_VER_SEC)
			nOpt = &m_nVerSecOpt;
		else if(nBaseOpt == CMO_VER_OUTECC)
			nOpt = &m_nVerOutEccOpt;
		else if(nBaseOpt == CMO_VER_SUPPORT)
			nOpt = &m_nVerSupportOpt;
		else if(nBaseOpt == CMO_HOR_SEC)
			nOpt = &m_nHorSecOpt;
		else if(nBaseOpt == CMO_HOR_LEVELDIF)
			nOpt = &m_nHorLevelDifOpt;

		if(nOpt != NULL)
		{
			if(bVal == true)
			{
				(long)(*nOpt) |= (long)nSecondOpt;
				SetCheckedBaseOption(nBaseOpt, bVal);
			}
			else
				(long)(*nOpt) &= ~(long)nSecondOpt;
		}
	};

	void SetCheckedSecoundOptionAll(long nBaseOpt, bool bVal)
	{
		long nSecondOptCnt = 0;
		long* nOpt = NULL;
		if(nBaseOpt == CMO_GEN)
		{
			nOpt = &m_nGenOpt;
			nSecondOptCnt = CMO_GEN_CNT;
		}
		else if(nBaseOpt == CMO_MODEL)
		{
			nOpt = &m_nModelOpt;
			nSecondOptCnt = CMO_MODEL_CNT;
		}
		else if(nBaseOpt == CMO_VER_SEC)
		{
			nOpt = &m_nVerSecOpt;
			nSecondOptCnt = CMO_VER_SEC_CNT;
		}
		else if(nBaseOpt == CMO_VER_OUTECC)
		{
			nOpt = &m_nVerOutEccOpt;
			nSecondOptCnt = CMO_VER_OUTECC_CNT;
		}
		else if(nBaseOpt == CMO_VER_SUPPORT)
		{
			nOpt = &m_nVerSupportOpt;
			nSecondOptCnt = CMO_VER_SUPPORT_CNT;
		}
		else if(nBaseOpt == CMO_HOR_SEC)
		{
			nOpt = &m_nHorSecOpt;
			nSecondOptCnt = CMO_HOR_SEC_CNT;
		}
		else if(nBaseOpt == CMO_HOR_LEVELDIF)
		{
			nOpt = &m_nHorLevelDifOpt;
			nSecondOptCnt = CMO_HOR_LEVELDIF_CNT;
		}

		if(nOpt != NULL)
		{
			long nSecondOpt = 2;
			if(bVal == true)
			{
				for(long i = 0 ; i < nSecondOptCnt ; i++)
				{
					(long)(*nOpt) |= (long)nSecondOpt;
					nSecondOpt = nSecondOpt << 1;
				}


			}
			else
			{
				for(long i = 0 ; i < nSecondOptCnt ; i++)
				{
					(long)(*nOpt) &= ~(long)nSecondOpt;
					nSecondOpt = nSecondOpt << 1;
				}
			}

			SetCheckedBaseOption(nBaseOpt, bVal);
		}
	};

	long m_nBaseOption;

	long m_nGenOpt;
	long m_nModelOpt;
	long m_nVerSecOpt;
	long m_nVerOutEccOpt;
	long m_nVerSupportOpt;
	long m_nHorSecOpt;
	long m_nHorLevelDifOpt;
}CheckModelOpt;


class StructureModel_CLASS MSCheckModelItem : public MSObject
{
public:
	MSCheckModelItem()
	{
	};

	MSCheckModelItem(long nBaseOpt, int nSecondOpt, int nCheckVal = 0, int nErrorLevel = -1, bool bCheckDownVal = true)
	{
		m_nBaseOpt = nBaseOpt;
		m_nSecondOpt = nSecondOpt;
		m_nCheckVal = nCheckVal;
		m_nErrorLevel = nErrorLevel;
		m_bCheckDownVal = bCheckDownVal;
	};

	void MSCheckModelItem::Serialize(CArchive &ar)
	{
		if(ar.IsStoring())
		{
			ar << m_nBaseOpt;
			ar << m_nSecondOpt;
			ar << m_nCheckVal;
			ar << m_nErrorLevel;
			ar << m_bCheckDownVal;
		}
		else
		{
			ar >> m_nBaseOpt;
			ar >> m_nSecondOpt;
			ar >> m_nCheckVal;
			ar >> m_nErrorLevel;
			ar >> m_bCheckDownVal;
		}
	};

	bool bUseCheckVal()
	{
		if(m_nCheckVal <= -1)
			return false;
		return true;
	}

	long m_nBaseOpt;
	long m_nSecondOpt;
	int m_nCheckVal; // -1 은 사용하지 않음
	int m_nErrorLevel;
	bool m_bCheckDownVal; // true:이하 , false:초과

};

class MSCompoG;
class StructureModel_CLASS MSCheckModelOption : public MSObject
{
public:

	long GetErrorLevel(eCheckModel eBaseOpt, long eSecondOpt);
	int GetCheckVal(eCheckModel eBaseOpt, long eSecondOpt);
	bool isUseCheckVal(eCheckModel eBaseOpt, long eSecondOpt);


	// Default Name
	CString m_strPreFixName;

	CheckModelOpt m_nCheckModelOpt;
	double m_dColinearDegreeTol;

	static MSCheckModelOption* GetInstance();
	~MSCheckModelOption(void);

	void Serialize( CArchive& archive );
	bool WriteFileBinary(CString strFilePath);
	int ReadFileBinary(CString strFilePath);

	CString GetKey(eCheckModel eBaseOpt, long nSecondOpt);

	//map<CString, MSCheckModelItem*> mm_CheckModelItem;
	vector<MSCheckModelItem*> ma_CheckModelItem;

	void InitOpt();
	void DeleteOptItems();
	void UpdateOptItems();
	MSCheckModelItem* GetOptItem(eCheckModel eBaseOpt, long nSecondOpt);

	bool IsCheckedBaseOption(long nBaseOpt);
	bool IsCheckedSecoundOption(long nBaseOpt, long nSecondOpt);

	void SetCheckedBaseOption(long nBaseOpt, bool bVal);
	void SetCheckedSecoundOption(long nBaseOpt, long nSecondOpt, bool bVal);
	void SetCheckedSecoundOptionAll(long nBaseOpt, bool bVal);

protected:
	static MSCheckModelOption* m_instance;
	MSCheckModelOption(void);
	
};


