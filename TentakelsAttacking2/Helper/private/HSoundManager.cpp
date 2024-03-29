//
// Purpur Tentakel
// 24.08.2022
//

#include "HSoundManager.h"
#include "UIEvents.hpp"
#include "HRandom.h"
#include "AppContext.h"
#include <filesystem>

void SoundManager::LoadSounds() {
	for (int i = 0; i < m_files.size(); ++i) {
		Sound sound = LoadSound(("Sounds/" + m_files[i]).c_str());
		m_sounds[static_cast<SoundType>(i)] = sound;
	}

	std::string files = "Sounds/TextSounds";
	for (auto const& entry : std::filesystem::directory_iterator(files)) {
		Sound sound = LoadSound(entry.path().string().c_str());
		m_textSounds.push_back(sound);
	}
}

void SoundManager::PlaySound(SoundType soundType) const {
	if (soundType == SoundType::TEXT) {
		PlayTextSound();
		return;
	}

	::PlaySoundMulti(m_sounds.at(soundType));
}
void SoundManager::PlayTextSound() const {
	Random& random = Random::GetInstance();

	static unsigned long long lastIndex = 0;

	unsigned long long nextIndex;
	do {
		nextIndex = random.random(m_textSounds.size());
	} while (lastIndex == nextIndex);

	::PlaySound(m_textSounds.at(nextIndex));
	lastIndex = nextIndex;
}
void SoundManager::MuteMasterSoundLevel(bool mute) const {
	AppContext& appContext = AppContext::GetInstance();
	appContext.constants.sound.muteVolume = mute;
	SetMasterSoundLevel(appContext.constants.sound.masterVolume);
}
void SoundManager::SetMasterSoundLevel(float level) const {
	AppContext& appCpntext = AppContext::GetInstance();
	if (appCpntext.constants.sound.muteVolume) {
		SetMasterVolume(0.0f);
		return;
	}
	appCpntext.constants.sound.masterVolume = level;
	level /= 100;
	SetMasterVolume(level);
}

SoundManager::SoundManager() {
	InitAudioDevice();
	LoadSounds();
}
SoundManager::~SoundManager() {
	for (auto& [_, sound] : m_sounds) {
		UnloadSound(sound);
	}
	CloseAudioDevice();
}

void SoundManager::OnEvent(Event const& event) {
	if (auto const soundEvent = dynamic_cast<PlaySoundEvent const*>(&event)) {
		PlaySound(soundEvent->GetSoundType());
		return;
	}

	if (auto const LevelEvent = dynamic_cast<SetMasterVolumeEvent const*>(&event)) {
		SetMasterSoundLevel(LevelEvent->GetLevel());
		return;
	}
	if (auto const MuteEvent = dynamic_cast<MuteMasterVolumeEvent const*>(&event)) {
		MuteMasterSoundLevel(MuteEvent->GetMute());
		return;
	}
}
