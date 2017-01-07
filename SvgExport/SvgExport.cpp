// SvgExport.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbEntity.h"
#include "DbLine.h"
#include "DbPolyline.h"
#include "DbBlockReference.h"
#include "Ge\Ge.h"
#include "Ge/GeScale3d.h"

#include "SvgExport.h"
#include "SvgWrappers.h"
#include "MemoryStream.h"

using namespace std;
using namespace TD_SVG_EXPORT;
using namespace xml;
int Attribute::_doublePrecision = 6;



void addLine(OdDbLinePtr pLine, xml::Node* pParent, OdGeMatrix3d &blockMatrix);
void addPolyline(OdDbPolylinePtr pPl, xml::Node* pParent, OdGeMatrix3d &blockMatrix);
void addBlockRef(OdDbBlockReferencePtr pBlockRef, xml::Node* pParent);

OdGeMatrix3d world2SvgMatrix;


int exportSvg(OdDbDatabasePtr pDb, const char* fileName)
{
	OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForRead);
	OdGeExtents3d msExt;
	pMs->getGeomExtents(msExt);
	OdGeLine3d mirrorLine;
	mirrorLine.set(OdGePoint3d(msExt.minPoint().x,msExt.center().y,0.), 
		OdGePoint3d(msExt.maxPoint().x, msExt.center().y, 0.));

	world2SvgMatrix.setToMirroring(mirrorLine);


	xml::Node* _svgRoot;
	OdGsDCRect outputRect;
	outputRect.m_min = OdGsDCPoint(msExt.minPoint().x, msExt.minPoint().y);
	outputRect.m_max = OdGsDCPoint(msExt.maxPoint().x, msExt.maxPoint().y);
	_svgRoot = new svg::RootNode(outputRect, RGB(255, 255, 255)/*_properties->get_BackGround()*/);


	OdDbObjectIteratorPtr pObjIter = pMs->newIterator();
	while (!pObjIter->done())
	{
		OdDbEntityPtr pEnt = pObjIter->entity(OdDb::kForRead);
		if (pEnt->isKindOf(OdDbLine::desc()))
		{
			addLine(pEnt, _svgRoot, OdGeMatrix3d());
		}
		else if (pEnt->isKindOf(OdDbPolyline::desc()))
		{
			addPolyline(pEnt, _svgRoot, OdGeMatrix3d());
		}
		else if (pEnt->isKindOf(OdDbBlockReference::desc()))
		{
			addBlockRef(pEnt, _svgRoot);
		}
		pObjIter->step();
	}

	// "Output" property must be set before exporting
	OdMemoryStreamPtr stream = OdMemoryStream::createNew();/* = _properties->get_Output()*/;
	(*stream) << OdString(OD_T("<?xml version=\"1.0\" standalone=\"no\"?>\r\n"));
	(*stream) << *_svgRoot;

	//const char * filename = "d:\\a.svg";
	fstream svgFile(fileName, ios::in | ios::out | ios::trunc);
	OdUInt64 length = stream->length();
	char *buffer = new char[length];
	stream->seek(0, OdDb::FilerSeekType::kSeekFromStart);
	stream->getBytes(buffer, length);
	svgFile.write(buffer, length);
	svgFile.close();

	delete[] buffer;
	delete _svgRoot;
	return 0;
}

void addLine(OdDbLinePtr pLine, xml::Node * pParent, OdGeMatrix3d &blockMatrix)
{
	OdDb::LineWeight lw = pLine->lineWeight();
	OdCmColor color = pLine->color();
	OdGePoint3d ptS = pLine->startPoint();
	OdGePoint3d ptE = pLine->endPoint();
	ptS.transformBy(blockMatrix);
	ptE.transformBy(blockMatrix);
	ptS.transformBy(world2SvgMatrix);
	ptE.transformBy(world2SvgMatrix);

	OdGePoint3d pts[2];
	pts[0] = ptS;
	pts[1] = ptE;

	xml::Node* _currentNode = pParent->addChild(OD_T("g"));
	xml::Node* pPolylineNode = _currentNode->addChild(new xml::Node(OD_T("polyline")));
	pPolylineNode->addAttribute(svg::Points(2, pts));
	pPolylineNode->addAttribute(L"stroke", L"rgb(0, 0, 0)");
	pPolylineNode->addAttribute(L"stroke-width", L"0.5");
}

void addPolyline(OdDbPolylinePtr pPl, xml::Node * pParent, OdGeMatrix3d &blockMatrix)
{
	OdDb::LineWeight lw = pPl->lineWeight();
	OdCmColor color = pPl->color();
	unsigned int iNumVerts = pPl->numVerts();

	OdGePoint2d* pPts = new OdGePoint2d[iNumVerts];
	for (size_t i = 0; i < iNumVerts; i++)
	{
		OdGePoint3d pt3d;
		pPl->getPointAt(i, pt3d);
		pt3d.transformBy(blockMatrix);
		pt3d.transformBy(world2SvgMatrix);
		pPts[i] = pt3d.convert2d();
	}

	xml::Node* _currentNode = pParent->addChild(OD_T("g"));
	xml::Node* pPolylineNode = _currentNode->addChild(new xml::Node(OD_T("polyline")));
	pPolylineNode->addAttribute(svg::Points(iNumVerts, pPts));
	pPolylineNode->addAttribute(L"stroke", L"rgb(0, 0, 0)");
	pPolylineNode->addAttribute(L"stroke-width", L"0.5");

	delete[] pPts;
}

void addBlockRef(OdDbBlockReferencePtr pBlockRef, xml::Node * pParent)
{
	pBlockRef->position();
	pBlockRef->rotation();
	pBlockRef->scaleFactors();
	OdDbObjectId blockId = pBlockRef->blockTableRecord();
	OdGeMatrix3d matrix3d = pBlockRef->blockTransform();

	xml::Node* _currentNode = pParent->addChild(OD_T("g"));

	OdDbBlockTableRecordPtr pBR = blockId.safeOpenObject(OdDb::kForRead);
	OdDbObjectIteratorPtr pIter = pBR->newIterator();
	while (!pIter->done())
	{
		OdDbEntityPtr pEnt = pIter->entity(OdDb::kForRead);
		if (pEnt->isKindOf(OdDbLine::desc()))
		{
			addLine(pEnt, _currentNode, matrix3d);
		}
		else if (pEnt->isKindOf(OdDbPolyline::desc()))
		{
			addPolyline(pEnt, _currentNode, matrix3d);
		}
		pIter->step();
	}
}
