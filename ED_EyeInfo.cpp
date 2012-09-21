#include "StdAfx.h"
#include ".\ed_eyeinfo.h"

ED_EyeInfo::ED_EyeInfo(void)
{
	m_bPupilFound = false;
	m_bTorsionFound = false;
	m_fCx = 0; m_fCy = 0;
	m_fRadius = 0;
	m_fTorsion = 0;
}

ED_EyeInfo::~ED_EyeInfo(void)
{
}
