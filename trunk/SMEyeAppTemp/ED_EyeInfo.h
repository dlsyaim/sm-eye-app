#pragma once

class ED_EyeInfo
{
public:
	ED_EyeInfo(void);
	~ED_EyeInfo(void);

	
public:
	bool m_bPupilFound;		//pupil�� ã�Ҵ���
	bool m_bTorsionFound;	//torsion�� ã�Ҵ���
		
	

	float m_fCx;			//������ �߽�(pixel)
	float m_fCy;

	float m_fRadius;		//������ ������(pixel)
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

