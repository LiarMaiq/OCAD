#include "stdafx.h"
#include "CommandsModule.h"


ODRX_DEFINE_DYNAMIC_MODULE(CommandsModule);

CommandsModule::CommandsModule()
{
}


CommandsModule::~CommandsModule()
{
}

void CommandsModule::initApp()
{
	OdEdCommandStackPtr pCommands = odedRegCmds();

	pCommands->addCommand(&m_cmdLine);
}

void CommandsModule::uninitApp()
{
	OdEdCommandStackPtr pCommands = odedRegCmds();

	pCommands->removeCmd(OdString::kEmpty, OD_T("line"));
}
