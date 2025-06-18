#pragma once
#include "afx.h"

class MSRebarBuilding;
class StructureModel_CLASS XMSRowM :	public CObject
{
public:
	MSRebarBuilding* m_pOwner;
	CString m_strName;
	CArray<long, long> ma_MID;
	long m_nRepresentMID;
	long m_nID;
	long m_nRow;
	long m_nCol;
	void Initialize()
	{
		m_pOwner = NULL;
		m_nID = 0;
		m_strName = _T("");
		ma_MID.RemoveAll();
		m_nRepresentMID = 0;
		m_nRow = 0;
		m_nCol = 0;
	}
	void AddMID(long nMID)
	{
		ma_MID.Add(nMID);
	}
	void SetOwner(MSRebarBuilding* pOwner){m_pOwner = pOwner;};
	void SetRepresentMID(long nRepresentMID){m_nRepresentMID = nRepresentMID;};
	long GetIncludeMIDbyFloorNum(long nFloorNum);
	BOOL GetStory(long& nFrom, long& nTo);
	CString GetName();
	XMSRowM(){Initialize();};
	~XMSRowM(){};
	XMSRowM(XMSRowM& aData)	{*this = aData;}
	XMSRowM& operator = (XMSRowM& aData)
	{
		Initialize();

		m_pOwner = aData.m_pOwner;
		m_nID = aData.m_nID;
		m_strName = aData.m_strName;
		m_nRepresentMID = aData.m_nRepresentMID;
		m_nRow = aData.m_nRow;
		m_nCol = aData.m_nCol;
		ma_MID.RemoveAll();
		ma_MID.Copy(aData.ma_MID);
		return *this;
	}
	void Sort();
	BOOL IsIncludeMID(long nMID);

};
