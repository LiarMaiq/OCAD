#pragma once
#include "CmdLine.h"

class CommandsModule : public OdRxModule
{
public:
	CommandsModule();
	~CommandsModule();


public:
	OdStaticRxObject<CmdLine> m_cmdLine;

public:
	void initApp();
	void uninitApp();
};

