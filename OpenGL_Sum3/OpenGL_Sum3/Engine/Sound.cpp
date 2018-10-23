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

// This Include
#include "Sound.h"

// Global Variable -----------------------------------------------------------------------------
FMOD::System* audioMgr;
FMOD::Sound* bgmTheme;

CSound::CSound()
{
}


CSound::~CSound()
{
}

bool CSound::InitFmod()
{
	FMOD_RESULT result; 
	result = FMOD::System_Create(&audioMgr);

	if (result != FMOD_OK) 
	{ 
		return false; 
	}
	result = audioMgr->init(50, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{ 
		return false; 
	} 
	return true;
}

void CSound::SetSoundAdress(const char* _charAdress)
{
	FMOD::Sound* SoundTemp = nullptr;

	m_pSoundAdress.push_back(_charAdress);
	m_pSoundPointers.push_back(SoundTemp);
}

const bool CSound::LoadAudio()
{
	FMOD_RESULT result;
	// Sound Effects -------------------------------------------------------------------------------
	
		for (unsigned int i = 0; i < m_pSoundPointers.size(); i++)
		{
			result = audioMgr->createSound(m_pSoundAdress[i], FMOD_DEFAULT, 0, &m_pSoundPointers[i]);
		}
		
	

	// Theme music ---------------------------------------------------------------------------------
	result = audioMgr->createSound("Resources/Sound/bensound-epic.mp3", FMOD_DEFAULT, 0, &bgmTheme);

	bgmTheme->setMode(FMOD_LOOP_NORMAL);

	return(true);
}

void CSound::PlaySound()
{
	InitFmod();
	LoadAudio();

	FMOD::Channel* channel;
	
	audioMgr->playSound(bgmTheme, 0, false, &channel);	//main theme loop
	channel->setVolume(1);	//master volume

	for (unsigned int i = 0; i < m_pSoundPointers.size(); i++)
	{
		audioMgr->playSound(m_pSoundPointers[i], 0, false, &channel);	//sound FX
		m_pSoundPointers.clear();
	}
	
}
