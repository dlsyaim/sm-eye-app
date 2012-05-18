#pragma once

class ED_EyeInfo
{
public:
	ED_EyeInfo(void);
	~ED_EyeInfo(void);

	
public:
	bool m_bPupilFound;		//pupil을 찾았는지
	bool m_bTorsionFound;	//torsion을 찾았는지
		
	

	float m_fCx;			//동공의 중심(pixel)
	float m_fCy;

	float m_fRadius;		//동공의 반지름(pixel)
	float m_fTorsion;		//torsion angle(degree)

	ED_EyeInfo& operator=(const ED_EyeInfo &rhs)
	{
		if(this != &rhs)
		{
			m_bPupilFound = rhs.m_bPupilFound;
			m_bTorsionFound = rhs.m_bTorsionFound;
			m_fCx = rhs.m_fCx;
			m_fCy = rhs.m_fCy;

			m_fRadius = rhs.m_fRadius;
			m_fTorsion = rhs.m_fTorsion;
		}

		return *this;
	};

};

