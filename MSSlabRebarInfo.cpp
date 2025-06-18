#include "StdAfx.h"
#include "MSSlabRebarInfo.h"
#include "MSPolylineObject.h"
#include "GMLib\MSVersionInfo.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSSlabRebarInfo, MSObject, VERSIONABLE_SCHEMA | 1)
MSSlabRebarInfo::MSSlabRebarInfo(RebarType rbType) : MSObject()
{
    SetRebarType(rbType);
	m_SBar.InitData(MSDeformedBar::GetStirrupBarStr(300, 19));	
	m_IsNumPlaceBar = false;
	m_MainBar = MSDeformedBar::GetMainBarStr(4, 19);
	
}

MSSlabRebarInfo::~MSSlabRebarInfo(void)
{
}

MSObject* MSSlabRebarInfo::Copy(bool bNewID)
{
	MSSlabRebarInfo* pNewObj;

	pNewObj = new MSSlabRebarInfo();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSSlabRebarInfo::CopyFromMe(MSObject* pSource, bool bNewID)
{
	if(pSource == nullptr) return;
	MSSlabRebarInfo* pRebarLine = (MSSlabRebarInfo*)pSource;

	if(pSource)
	{
		m_RebarType = pRebarLine->m_RebarType;
		m_SBar.CopyFromMe(&pRebarLine->m_SBar);
		m_IsNumPlaceBar = pRebarLine->m_IsNumPlaceBar;
		m_MainBar.CopyFromMe(&pRebarLine->m_MainBar);
	}
}

void MSSlabRebarInfo::Dump( EFS::EFS_Buffer& ar )
{
	MSObject::Dump(ar);

	int type = (int)m_RebarType;
	ar << type;
	m_SBar.Dump(ar);
	m_MainBar.Dump(ar);
}

void MSSlabRebarInfo::Recovery( EFS::EFS_Buffer& ar )
{
	MSObject::Recovery(ar);

	int type = 0;
	ar >> type;
	m_RebarType = (RebarType)type;
	m_SBar.Recovery(ar);
	m_MainBar.Recovery(ar);
}

void MSSlabRebarInfo::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);

	bool IsCut;
	if(ar.IsStoring())
	{
		int type = (int)m_RebarType;
		ar << type;
		m_SBar.Serialize(ar);

		ar << m_IsNumPlaceBar;
		m_MainBar.Serialize(ar);
	}
	else
	{
		int type = 0;
		ar >> type;
		m_RebarType = (RebarType)type;

		if(MSVersionInfo::GetCurrentVersion() < 20160621)
			ar >> IsCut;

		m_SBar.Serialize(ar);
		if (MSVersionInfo::GetCurrentVersion() < 20160621)
		{
			GM2DPolyline polyline;
			polyline.Serialize(ar);
		}

		if(MSVersionInfo::GetCurrentVersion() >= 20150313)
		{
			ar >> m_IsNumPlaceBar;
			m_MainBar.Serialize(ar);
		}
	}
}

void MSSlabRebarInfo::SetRebarText(CString sBarText)
{
	CString rebarText = MSDeformedBar::GetBundleSpaceRemoveStr(sBarText);

	int indexAt = rebarText.Find('@');
	if (indexAt == -1)
	{
		m_IsNumPlaceBar = true;
		m_MainBar.InitData(sBarText);
	}
	else
	{
		m_IsNumPlaceBar = false;
		if (m_RebarType == MSSlabRebarInfo::Shear)
		{
			SetShearBarInfo(sBarText);
		}
		else
			m_SBar.InitData(sBarText);
	}
}

bool MSSlabRebarInfo::CheckReBarStr()
{
	bool bRval = false;

	if (m_IsNumPlaceBar) // 개수 배근
	{
		long NumBarStr = m_MainBar.ma_strBar.GetSize();
		for (long iBarStr = 0; iBarStr < NumBarStr; iBarStr++)
		{
			CString strBar = m_MainBar.ma_strBar[iBarStr];
			if (strBar.Find('@') != -1)
			{
				m_IsNumPlaceBar = false;
				m_SBar.ma_strBar.RemoveAll();

				for (long i = 0; i < NumBarStr; i++)
					m_SBar.ma_strBar.Add(m_MainBar.ma_strBar[iBarStr]);

				m_MainBar.ma_strBar.RemoveAll();
				bRval = true;
				break;
			}
		}
	}
	else// 간격 배근
	{
		long NumBarStr = m_SBar.ma_strBar.GetSize();
		for (long iBarStr = 0 ; iBarStr < NumBarStr ; iBarStr++)
		{
			CString strBar = m_SBar.ma_strBar[iBarStr];
			if (strBar.Find('@') == -1)
			{
				m_IsNumPlaceBar = true;

				m_MainBar.ma_strBar.RemoveAll();
				for (long i = 0; i < NumBarStr; i++)
					m_MainBar.ma_strBar.Add(m_SBar.ma_strBar[iBarStr]);
				m_SBar.ma_strBar.RemoveAll();
				bRval = true;
				break;
			}
		}
	}

	return bRval;
}

bool MSSlabRebarInfo::IsSameBar(MSSlabRebarInfo* pRebarLine)
{
	if (pRebarLine == NULL)	return false;

	if (GetRebarType() != pRebarLine->GetRebarType())	return false;
	if (GetNumPlaceBar() != pRebarLine->GetNumPlaceBar())	return false;

	if (GetNumPlaceBar())
	{
		if (m_MainBar.IsSameBar(pRebarLine->m_MainBar))	return true;
	}
	else
	{
		if (m_SBar.IsSameBar(pRebarLine->m_SBar))	return true;
	}
	return false;
}

void MSSlabRebarInfo::SetShearBarInfo(CString sBarText)
{
	CString hShearbar = _T("");
	CString vShearbar = _T("");

	int AtCount = 0;
	CString rebarText = MSDeformedBar::GetBundleSpaceRemoveStr(sBarText);
	int length = rebarText.GetLength();
	for (int i = 0; i < length; i++)
	{
		if (rebarText[i] == '@')
			AtCount++;
	}
	if (AtCount == 1)
	{
		hShearbar = sBarText;
		vShearbar = sBarText;
	}
	else if (AtCount == 2)
	{
		int indexAt = sBarText.Find('@');
		int indexH = sBarText.Find('H');
		int indexD = sBarText.Find('D');
		if (indexD == -1 || indexAt <= indexD || indexD <= indexH)
			return;

		long nBundleNum = MSDeformedBar::GetBundleNum(sBarText);
		long nBundleSpace = MSDeformedBar::GetBundleSpace(sBarText);

		CString sDia = sBarText.Mid(indexD + 1, indexAt - (indexD + 1));

		int indexAt2 = rebarText.ReverseFind('@');

		CString temp = sBarText.Mid(0, indexAt);
		hShearbar = sBarText.Mid(0, indexAt2);

		if (nBundleSpace != 0)
			hShearbar.Format(_T("%s[B@=%ld]"), hShearbar, nBundleSpace);

		if(nBundleNum == 0)
			vShearbar.Format(_T("%s%s@%s"), MSDeformedBar::m_sDefaultBarType, sDia, sBarText.Mid(indexAt2 + 1, length - (indexAt2 + 1)));
		else
			vShearbar.Format(_T("%ld%s%s@%s"), nBundleNum, MSDeformedBar::m_sDefaultBarType, sDia, sBarText.Mid(indexAt2 + 1, length - (indexAt2 + 1)));

		if (nBundleSpace != 0)
			vShearbar.Format(_T("%s[B@=%ld]"), vShearbar, nBundleSpace);
	}

	m_SBar.InitData(hShearbar);
	m_SBar.Add(vShearbar);

}