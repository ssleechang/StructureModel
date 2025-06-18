#pragma once


class StructureModel_CLASS CLogWriter
{
public:
	CLogWriter();
	virtual ~CLogWriter();
	virtual void WriteLine(CString log)=0;
};

class StructureModel_CLASS CLogger
{
public:
		
	static CLogger* GetInstance();
	~CLogger(void);
	void WriteLog(CString log);
	void WriteLogFormat(const char* fmt, ...);// Formatted String을 바로 출력함. ex) WirteLogFormat("%d is %d and do %s", id, 23, "Action");
	void WriteLog();
	void SetWriter(CLogWriter* pWriter);
	static CString strLog;
protected:
	static CLogger* m_instance;
	CLogWriter* m_writer;
	CLogger(void);

};



