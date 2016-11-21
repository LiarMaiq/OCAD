#pragma once
class CmdLine : public OdStaticRxObject<OdEdCommand>
{
public:
	CmdLine();
	~CmdLine();
public:
	const OdString groupName() const { return OdString::kEmpty; }
	const OdString globalName() const { return OdString("line"); }
	const OdString localName() const { return globalName(); }
	void execute(OdEdCommandContext* pCmdCtx);
};

