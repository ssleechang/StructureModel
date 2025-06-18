#pragma once
#include "MSAddRebarBase.h"

class MSLineRebar;
class StructureModel_CLASS MSAddRebarAreaLine :public MSAddRebarBase
{
public:

	MSAddRebarAreaLine();
	~MSAddRebarAreaLine();

	virtual Type_MSCompoG GetType() { return msAddRebarAreaLine; };

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAddRebarAreaLine)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);


	MSLineRebar* GetTopMainBar() { return m_pTopMainBar; };
	MSLineRebar* GetTopSubBar() { return m_pTopSubBar; };
	MSLineRebar* GetBotMainBar() { return m_pBotMainBar; };
	MSLineRebar* GetBotSubBar() { return m_pBotSubBar; };

	static MSAddRebarAreaLine* ms_AddAreaLine;
	static MSAddRebarAreaLine* GetStaticAddAreaLine();
protected:
	MSLineRebar* m_pTopMainBar;
	MSLineRebar* m_pTopSubBar;
	MSLineRebar* m_pBotMainBar;
	MSLineRebar* m_pBotSubBar;
};

