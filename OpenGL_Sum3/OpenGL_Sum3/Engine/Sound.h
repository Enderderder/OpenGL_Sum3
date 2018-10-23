//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2018 Media Design School
//
// File Name    : Sound.cpp
// Description	: 
// Author       : Richard Wulansari & Jacob Dewse
// Mail         : richard.wul7481@mediadesign.school.nz, jacob.dew7364@mediadesign.school.nz
//

#ifndef CSound_H
#define CSound_H

#include "Utility.h"

class CSound
{
public:
	CSound();
	~CSound();

	bool InitFmod();
	const bool LoadAudio();
	void PlaySound();
	void SetSoundAdress(const char* _charAdress);

private:
	float m_intMasterVolume;
	std::vector<const char*> m_pSoundAdress;
	std::vector<FMOD::Sound*> m_pSoundPointers;
};
#endif // !CSound_H
