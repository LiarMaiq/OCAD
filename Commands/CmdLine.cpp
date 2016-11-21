#include "stdafx.h"
#include "CmdLine.h"
#include "DbLine.h"
#include "DbBlockTableRecord.h"

CmdLine::CmdLine()
{
}


CmdLine::~CmdLine()
{
}

void CmdLine::execute(OdEdCommandContext * pCmdCtx)
{
	OdDbCommandContextPtr pDbCmdCtx(pCmdCtx); // downcast for database access
	if (pDbCmdCtx.isNull())
	{
		//MessageBox(AfxGetMainWnd()->m_hWnd, _T("This command needs an active drawing"), _T("Command unavailable"), MB_ICONWARNING);
		return;
	}
	OdDbDatabasePtr pDatabase = pDbCmdCtx->database(); // Current database
	OdDbUserIOPtr pUserIO = pDbCmdCtx->userIO();

	OdGePoint3d Start, End;
	OdDbBlockTableRecordPtr pBlockTableRecord = pDatabase->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
	try
	{
		Start = pUserIO->getPoint("From point: ");
		while (1)
		{
			End = pUserIO->getPoint("To point: ", OdEd::kGptRubberBand);

			OdDbLinePtr pLine = OdDbLine::createObject();
			pLine->setDatabaseDefaults(pDatabase);

			pLine->setStartPoint(Start);
			pLine->setEndPoint(End);

			pBlockTableRecord->appendOdDbEntity(pLine);

			Start = End;
		}
	}
	catch (OdEdCancel& /*e*/)
	{
		return;
	}
}
