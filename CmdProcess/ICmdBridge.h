
#pragma once

struct StructureModel_CLASS ICmdBridge
{
public:
	virtual void UnDo() =0;
	virtual void ReDo() = 0;
	virtual void Dispose() =0;
};