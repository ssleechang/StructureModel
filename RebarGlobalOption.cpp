
#include "StdAfx.h"
#include "RebarGlobalOption.h"
#include "RebarGlobalOptionManager.h"
#include "OrderLengthSet.h"
#include "CoverLengthSet.h"
#include "CorrectiveLengthSet.h"
#include "DevSpliceLengthSet.h"

//using namespace RebarOption;


RebarGlobalOption::RebarGlobalOption(CString Name, RebarGlobalOptionManager* pManager)
{
	m_sName = Name;
	m_pManager = pManager;
}

RebarGlobalOption::RebarGlobalOption( const RebarGlobalOption& option )
{
	m_sName = option.m_sName;
	m_pManager = option.m_pManager;

	m_pOrderLengthOption= option.m_pOrderLengthOption;
	m_pCorrectiveLengthOption= option.m_pCorrectiveLengthOption;
	m_pCoverLengthOption = option.m_pCoverLengthOption;
	m_pDevelopmentSpliceLengthOption = option.m_pDevelopmentSpliceLengthOption;

}

RebarGlobalOption::~RebarGlobalOption()
{

}

void RebarGlobalOption::Serialize( CArchive& ar )
{
	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << m_pOrderLengthOption->m_sName;
		ar << m_pCorrectiveLengthOption->m_sName;
		ar << m_pCoverLengthOption->m_sName;
		ar << m_pDevelopmentSpliceLengthOption->m_sName ;

	}
	else
	{
		ar >> m_sName;
		CString tmpString;
		ar >> tmpString;
		m_pOrderLengthOption = m_pManager->GetOrderLengthOption(tmpString);
		if(m_pOrderLengthOption == NULL)
			m_pOrderLengthOption = m_pManager->GetOrderLengthOption(L"Default");

		ar >> tmpString;
		m_pCorrectiveLengthOption = m_pManager->GetCorrectiveLengthOption(tmpString);

		ar >> tmpString;
		m_pCoverLengthOption = m_pManager->GetCoverLengthOption(tmpString);

		ar >> tmpString;
		m_pDevelopmentSpliceLengthOption = m_pManager->GetDevSpliceLengthOption(tmpString);


	}

}

bool RebarGlobalOption::SetCorrectiveLengthOption( const CString& name )
{
	m_pCorrectiveLengthOption = m_pManager->GetCorrectiveLengthOption(name);
	if(m_pCorrectiveLengthOption == NULL)
		return false;

	return true;
}

bool RebarGlobalOption::SetOrderLengthOption( const CString& name )
{
	m_pOrderLengthOption = m_pManager->GetOrderLengthOption(name);

	if(m_pOrderLengthOption == NULL)
		return false;

	return true;
}
bool RebarGlobalOption::SetDevelopmentSpliceLengthSettingOption( const CString& name )
{
	m_pDevelopmentSpliceLengthOption = m_pManager->GetDevSpliceLengthOption(name);

	if(m_pDevelopmentSpliceLengthOption == NULL)
		return false;

	return true;
}

const shared_ptr<CorrectiveLengthSet> RebarGlobalOption::GetCorrectiveLengthOption( )
{
	return m_pCorrectiveLengthOption;
}

const shared_ptr<OrderLengthSet> RebarGlobalOption::GetOrderLengthOption( )
{
	return m_pOrderLengthOption;
}

const shared_ptr<CoverLengthSet> RebarGlobalOption::GetCoverLengthOption( )
{
	return m_pCoverLengthOption;
}

bool RebarGlobalOption::SetCoverLengthOption( const CString& name )
{
	m_pCoverLengthOption = m_pManager->GetCoverLengthOption(name);

	if(m_pCoverLengthOption == NULL)
		return false;

	return true;
}

const shared_ptr<DevSpliceLengthSet> RebarGlobalOption::GetDevelopmentSpliceLengthSettingOption( )
{
	return m_pDevelopmentSpliceLengthOption;
}

const shared_ptr<OrderLengthSet> RebarGlobalOption::GetOrderLengthSet( const CString& OrderLengthSetName )
{
	return m_pManager->GetOrderLengthOption(OrderLengthSetName);
}

const CorrectiveLengthItem* RebarGlobalOption::GetCorrectiveLengthItem( int eCorLenType )
{
	return m_pCorrectiveLengthOption->GetCorrectiveLengthOptByType((RebarOption::CorLen_Type)eCorLenType);

}

void RebarGlobalOption::SetGlobalOptionName( CString m_sName )
{
	if(this->m_sName == m_pManager->GetCurrentOptionName())
	{
		m_pManager->SetCurrentOptionName(m_sName);
	}

	this->m_sName = m_sName;
}

void RebarGlobalOption::Copy(shared_ptr<RebarGlobalOption> desRebarGlobalOption)
{
	m_sName = desRebarGlobalOption->m_sName;
	
	m_pOrderLengthOption = m_pManager->GetOrderLengthOption(desRebarGlobalOption->m_pOrderLengthOption->m_sName);
	m_pCorrectiveLengthOption = m_pManager->GetCorrectiveLengthOption(desRebarGlobalOption->m_pCorrectiveLengthOption->m_sName);
	m_pCoverLengthOption = m_pManager->GetCoverLengthOption(desRebarGlobalOption->m_pCoverLengthOption->m_sName);
	m_pDevelopmentSpliceLengthOption = m_pManager->GetDevSpliceLengthOption(desRebarGlobalOption->m_pDevelopmentSpliceLengthOption->m_sName);
}

void RebarGlobalOption::SetCorrectiveLengthOption(shared_ptr<CorrectiveLengthSet> correctiveLengthOption)
{
	m_pCorrectiveLengthOption = correctiveLengthOption;
}

void RebarGlobalOption::SetOrderLengthOption(shared_ptr<OrderLengthSet> orderLengthOption)
{
	m_pOrderLengthOption = orderLengthOption;
}

void RebarGlobalOption::SetCoverLengthOption(shared_ptr<CoverLengthSet> coverLengthOption)
{
	m_pCoverLengthOption = coverLengthOption;
}

void RebarGlobalOption::SetDevelopmentSpliceLengthSettingOption(shared_ptr<DevSpliceLengthSet> developmentSpliceLengthOption)
{
	m_pDevelopmentSpliceLengthOption = developmentSpliceLengthOption;
}