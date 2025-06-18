#include "StdAfx.h"
#include "MSMFactory.h"

MSMFactory* MSMFactory::m_pFactory = NULL;
MSMFactory::MSMFactory(void)
{
}


MSMFactory::~MSMFactory(void)
{
}

MSMFactory* MSMFactory::GetInstance()
{
	if(m_pFactory!=NULL)
		return m_pFactory;
	else
		return NULL;

}

void MSMFactory::SetFactory( MSMFactory* factory)
{
	m_pFactory = factory;
}
