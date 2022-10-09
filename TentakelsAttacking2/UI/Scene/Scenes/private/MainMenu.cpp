//
// Purpur Tentakel
// 19.09.2022
//

#include "MainMenu.h"
#include "AppContext.h"
#include "UIManager.h"
#include "ClassicButton.h"
#include "Title.h"
#include "Text.h"
#include <memory>

void MainMenu::Initialize(Vector2 resolution, AppContext& appContext) {
	float btnPosX = 0.25f;
	float btnPosY = 0.3f;
	float btnSizX = 0.2f;
	float btnSizY = 0.1f;
	int focusID;

	auto newGameBtn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(btnPosX, btnPosY),
		GetElementSize(btnSizX, btnSizY),
		Alignment::MID_RIGHT,
		resolution,
		"New Game",
		SoundType::ACCEPTED
		);
	newGameBtn->SetOnClick([]() {
		AppContext::GetInstance().eventManager.InvokeEvent(
			SwitchSceneEvent(SceneType::TEST)
			);
		});
	m_elements.push_back(newGameBtn);

	btnPosY += 0.15f;
	++focusID;

	auto loadGameBtn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(btnPosX, btnPosY),
		GetElementSize(btnSizX, btnSizY),
		Alignment::MID_RIGHT,
		resolution,
		"Load Game",
		SoundType::ACCEPTED
		);
	loadGameBtn->SetEnabled(false);
	m_elements.push_back(loadGameBtn);

	btnPosY += 0.15f;
	++focusID;

	auto settingsBtn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(btnPosX, btnPosY),
		GetElementSize(btnSizX, btnSizY),
		Alignment::MID_RIGHT,
		resolution,
		"Settings",
		SoundType::CLICKED_RELEASE_STD
		);
	settingsBtn->SetEnabled(false);
	m_elements.push_back(settingsBtn);

	btnPosY += 0.15f;
	++focusID;

	auto creditsBtn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(btnPosX, btnPosY),
		GetElementSize(btnSizX, btnSizY),
		Alignment::MID_RIGHT,
		resolution,
		"Credits",
		SoundType::CLICKED_RELEASE_STD
		);
	creditsBtn->SetEnabled(false);
	m_elements.push_back(creditsBtn);

	btnPosY += 0.15f;
	++focusID;

	auto quitBtn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(btnPosX, btnPosY),
		GetElementSize(btnSizX, btnSizY),
		Alignment::MID_RIGHT,
		resolution,
		"Quit",
		SoundType::ACCEPTED
		);
	quitBtn->SetOnClick([]() {AppContext::GetInstance().eventManager.InvokeEvent(CloseWindowEvent());});
	m_elements.push_back(quitBtn);

	auto title = std::make_shared<Title>(
		GetElementPosition(0.625f, 0.025f),
		GetElementSize(0.7f, 0.2f),
		Alignment::TOP_MID,
		false,
		resolution,
		appContext
		);
	m_elements.push_back(title);

	auto version = std::make_shared<Text>(
		GetElementPosition(1.0f, 0.97f),
		GetElementSize(0.04f, 0.04f),
		Alignment::BOTTOM_RIGHT,
		0.02f,
		appContext.Version(),
		resolution
		);
	m_elements.push_back(version);

	auto copyRight = std::make_shared<Text>(
		GetElementPosition(1.0f, 1.0f),
		GetElementSize(0.1f, 0.04f),
		Alignment::BOTTOM_RIGHT,
		0.02f,
		appContext.CopyRight(),
		resolution
		);
	m_elements.push_back(copyRight);
}

MainMenu::MainMenu(Vector2 pos, Vector2 size, Alignment alignment,
	Vector2 resolution)
	: Scene(pos, size, alignment) {

	AppContext& appContext = AppContext::GetInstance();
	Initialize(resolution, appContext);
}

void MainMenu::SetActive(bool active, AppContext const& appContext) {
	Scene::SetActive(active, appContext);
}
