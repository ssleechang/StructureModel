#include "StdAfx.h"
#include "Logger.h"



CLogger* CLogger::m_instance = NULL;
CString CLogger::strLog=_T("");
CLogger* CLogger::GetInstance()
{
	if(m_instance == NULL)
	{
		m_instance = new CLogger();		
	}
	return m_instance;
}


CLogger::CLogger(void)
{
	m_writer = NULL;
}


CLogger::~CLogger(void)
{
	if(m_writer)
		delete m_writer;
}

void CLogger::WriteLog()
{
	WriteLog(CLogger::strLog);
}

void CLogger::WriteLog( CString log )
{
	if(m_writer)
		m_writer->WriteLine(log);
}

// Formatted String을 바로 출력함. ex) WirteLogFormat("%d is %d and do %s", id, 23, "Action");
void CLogger::WriteLogFormat( const char* fmt, ... )
{
	//char Temp[1024];

	//va_list Marker;

	//va_start(Marker, fmt);
	//vsprintf(Temp, fmt, Marker);
	//va_end(Marker);

	//WriteLog(Temp);	
}

void CLogger::SetWriter( CLogWriter* pWriter )
{
	if(m_writer != NULL)
		delete m_writer;

	m_writer = pWriter;
}

CLogWriter::~CLogWriter()
{

}

CLogWriter::CLogWriter()
{

}
