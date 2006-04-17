// vim:tabstop=4:shiftwidth=4:noexpandtab:textwidth=80
/*
 * =====================================================================================
 *        Filename:  cpage.cc
 *     Description:  Pdf page object implemetation.
 *         Created:  03/20/2006 11:41:43 AM CET
 *          Author:  jmisutka (), 
 * =====================================================================================
 */

// static
#include "static.h"

// Page
#include "cpage.h"

#include "cobjecthelpers.h"

// =====================================================================================
namespace pdfobjects{
// =====================================================================================

using namespace std;
using namespace boost;
using namespace utils;

		
//
// Constructor
//
CPage::CPage (boost::shared_ptr<CDict>& pageDict) : dictionary(pageDict)
{
	printDbg (debug::DBG_DBG, "");
	
	//
	// Get the stream representing content stream, make an xpdf object
	// and instantiate CContentStream
	//
	boost::shared_ptr<CStream> stream = getCStreamFromDict (dictionary, "Contents");
	contentstream = shared_ptr<CContentStream> (new CContentStream (stream, stream->_makeXpdfObject()));
}


//
//
//
Rectangle
CPage::getMediabox () const
{
	printDbg (debug::DBG_DBG, "");
	
	// Get the array representing media box
	shared_ptr<IProperty> mbox = dictionary->getProperty ("MediaBox");
	assert (pArray == mbox->getType ());
	if (pArray != mbox->getType ())
		throw MalformedFormatExeption ("Page::MediaBox is not array.");

	Rectangle rc;

  	rc.xleft  =	getDoubleFromArray (mbox, 0);
	rc.xright = getDoubleFromArray (mbox, 1);
	rc.yleft  =	getDoubleFromArray (mbox, 2);
	rc.yright = getDoubleFromArray (mbox, 3);

	return rc;
}

//
//
//
void
CPage::setMediabox (const Rectangle& rc)
{
	printDbg (debug::DBG_DBG, "");

	// Get the array representing media box
	shared_ptr<IProperty> mbox = dictionary->getProperty ("MediaBox");
	assert (pArray == mbox->getType ());
	if (pArray != mbox->getType ())
		throw MalformedFormatExeption ("Page::MediaBox is not array.");

  	setDoubleInArray (mbox, 0, rc.xleft);
	setDoubleInArray (mbox, 1, rc.xright);
	setDoubleInArray (mbox, 2, rc.yleft);
	setDoubleInArray (mbox, 3, rc.yright);
}

//
// Display a page
//
void
CPage::displayPage (::OutputDev& out, double hDpi, double vDPI, int rotate) const
{
	// Xpdf Global variable TFUJ!!!
	// REMARK: FUCKING xpdf uses global variable globalParams that uses another global
	// variable builtinFonts which causes that globalParams can NOT be nested
	assert (NULL == globalParams);
	boost::scoped_ptr<GlobalParams> aGlobPar (new GlobalParams (NULL));
	GlobalParams* oldGlobPar = globalParams;
	globalParams = aGlobPar.get();
	
	// Get xref
	assert (NULL != dictionary->getPdf ());
	if (NULL == dictionary->getPdf ())
		throw XpdfInvalidObject ();
	XRef* xref = dictionary->getPdf()->getCXref ();
	assert (NULL != xref);
	
	// Create xpdf object representing CPage
	boost::scoped_ptr<Object> xpdfPage (dictionary->_makeXpdfObject());
	// Check page dictionary
	assert (objDict == xpdfPage->getType ());
	if (objDict != xpdfPage->getType ())
		throw XpdfInvalidObject ();
	// Get page dictionary
	Dict* xpdfPageDict = xpdfPage->getDict ();
	assert (NULL != xpdfPageDict);
	
	//
	// Create default page attributes and make page
	// ATTRIBUTES are deleted in Page destructor
	// 
	Page page (xref, 0, xpdfPageDict, new PageAttrs (NULL, xpdfPageDict));
	// Create catalog
	scoped_ptr<Catalog> xpdfCatalog (new Catalog (xref));
	// Page object display (..., useMediaBox, crop, links, catalog)
	page.display (&out, hDpi, vDPI, rotate, gTrue, gFalse, NULL, xpdfCatalog.get());
	
	//
	// Cleanup
	//
	// Free object
	xpdfPage->free ();
	// Set global variable back to null
	globalParams = oldGlobPar;
}

// =====================================================================================
} /* namespace pdfobjects */
// =====================================================================================
