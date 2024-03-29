/* ==========================================================================
	File :			DrawBox.cpp
	
	Class :			CDrawBox

	Date :			07/27/04

	Purpose :		"CDrawBox" represents a simple drawable box.

	Description :	The class contain attributes for the objects, as well 
					as members to load and draw.

	Usage :			Should be managed by a container class.
   ========================================================================
	Changes :		28/3 2005	Version 2.0

   ========================================================================*/

#include "stdafx.h"
#include "DrawBox.h"
#include "BorderLine.h"
#include "UnitConversion.h"
#include "Tokenizer.h"
#include "StringHelpers.h"

CDrawBox::CDrawBox()
/* ============================================================
	Function :		CDrawBox::CDrawBox
	Description :	Constructor.
	Access :		Public
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetFieldType( FIELD_TYPE_BOX );

}

////////////////////////////////////////////////////////////////////
// Public functions
//
void CDrawBox::Draw( int /*page*/, CDC* dc )
/* ============================================================
	Function :		CDrawBox::Draw
	Description :	Draws this object.
	Access :		Public
					
	Return :		void
	Parameters :	int page	-	Current page (not used)
					CDC* dc		-	CDC to draw to

	Usage :			Called by the generator to draw the object.

   ============================================================*/
{

	const CBorderLine* line = GetBorder();
	CPen pen;
	CBrush brush;

	if( line->GetVisible() )
	{
		int thickness = CUnitConversion::InchesToPixels( line->GetThickness() );
		pen.CreatePen( line->GetStyle(),
					   thickness,
					   line->GetColor() );

		dc->SelectObject( &pen );
	}
	else
		dc->SelectStockObject( NULL_PEN );

	if( GetFill() )
	{
		brush.CreateSolidBrush( GetFillColor() );
		dc->SelectObject( &brush );
	}
	else
		dc->SelectStockObject( NULL_BRUSH );

	CDoubleRect rect = GetPosition();
	CUnitConversion::InchesToPixels( rect );

	CRect r( static_cast< int >( rect.left ), static_cast< int >( rect.top ), static_cast< int >( rect.right ), static_cast< int >( rect.bottom ) );
	CUnitConversion::AdjustPixelsToPaper( dc, r );

	dc->Rectangle( r );

	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDrawObject* CDrawBox::FromString( const CString & str )
/* ============================================================
	Function :		CDrawBox::FromString
	Description :	Creates and returns an object of this type 
					from the string "str".
	Access :		Public
					
	Return :		CDrawObject*		-	Object represented by 
											"str" or "NULL" is "str" 
											is not of this type.
	Parameters :	const CString & str	-	String representation 
											of an object of this 
											type.
	
	Usage :			Called by the generator loading mechanism.

   ============================================================*/
{

	CDrawBox*	result = NULL;

	CTokenizer tok( str, _T( ":" ) );
	int size = tok.GetSize();
	if( size == 2 )
	{
		CString type;
		CString data;
		tok.GetAt( 0, type );
		if( type == _T( "report_box" ) )
		{
			tok.GetAt( 1, data );
			CTokenizer contents( data );
			result = new CDrawBox;
			int count = CDrawObject::FromStringCommon( result, data );

			BOOL		border;
			double		borderthickness;
			int			borderstyle;
			COLORREF	bordercolor;
			BOOL		fill;
			COLORREF	fillcolor;

			contents.GetAt( count++, border );
			contents.GetAt( count++, borderthickness );
			contents.GetAt( count++, borderstyle );
			contents.GetAt( count++, bordercolor );
			contents.GetAt( count++, fill );
			contents.GetAt( count++, fillcolor );

			if( border )
			{
				CBorderLine line;
				line.SetVisible( TRUE );
				line.SetColor( bordercolor );
				line.SetThickness( borderthickness );
				line.SetStyle( borderstyle );
				result->SetBorder( line );
			}

			result->SetFill( fill );
			if( fill )
				result->SetFillColor( fillcolor );

		}
	}

	return result;

}

