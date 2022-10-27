//
// Purpur Tentakel
// 27.10.2022
//

#include "SliderAndInputLine.h"
#include "ClassicButton.h"
#include "Slider.h"
#include <iostream>

void SliderAndInputLine::Initialize(unsigned int focusID, Vector2 resolution) {
	m_slider = std::make_shared<Slider>(
		GetElementPosition(0.0f, 0.1f),
		GetElementSize(0.75f, 0.8f),
		Alignment::TOP_LEFT,
		true,
		10.0f,
		resolution
		);
	m_slider->SetOnSlide([this](float position) {
		Slide(position);
		});
	m_elements.push_back(m_slider);

	m_inputLine = std::make_shared<InputLine<int>>(
		focusID,
		GetElementPosition(0.77f, 0.0f),
		GetElementSize(0.13f, 1.0f),
		Alignment::TOP_LEFT,
		5,
		resolution
		);
	m_inputLine->SetOnEnter([this]() {
		this->BtnPressed();
		});
	m_inputLine->SetPlaceholderText("%");
	m_elements.push_back(m_inputLine);

	++focusID;

	m_btn = std::make_shared<ClassicButton>(
		focusID,
		GetElementPosition(0.9f, 0.0f),
		GetElementSize(0.1f, 1.0f),
		Alignment::TOP_LEFT,
		resolution,
		"Set",
		SoundType::CLICKED_RELEASE_STD
		);
	m_btn->SetOnClick([this]() {
		this->BtnPressed();
	});
	m_btn->SetEnabled(false);
	m_elements.push_back(m_btn);
}

void SliderAndInputLine::BtnPressed() {
	SaveValue();
	m_btn->SetEnabled(false);
	m_slided = false;
	SetSliderValue();
}
void SliderAndInputLine::SaveValue() const {
	if (!m_btn->IsEnabled()) {
		return;
	}
	std::cout << "New Value: " << m_inputLine->GetValue() << '\n';

}
void SliderAndInputLine::Slide(float position) {
	m_inputLine->SetValue(static_cast<int>(position));
	SaveValue();
	m_slided = true;
}
void SliderAndInputLine::ValidateCurrentValue() {
	m_currentValue = m_inputLine->GetValue();
	if (m_currentValue < m_minValue) {
		m_currentValue = m_minValue;
		m_inputLine->SetValue(m_currentValue);
		return;
	}
	
	if (m_currentValue > m_maxValue) {
		m_currentValue = m_maxValue;
		m_inputLine->SetValue(m_maxValue);
		return;
	}
	
	if (m_currentValue == 0) {
		m_inputLine->SetValue(0);
		return;
	}
}
void SliderAndInputLine::SetSliderValue() const {
	m_slider->SetButtonPosition(static_cast<float>(m_currentValue));
}

SliderAndInputLine::SliderAndInputLine(unsigned int focusID, Vector2 pos,
	Vector2 size, Alignment alignment, int minValue, int maxValue,
	Vector2 resolution)
	: Scene(pos, size, alignment), m_minValue(minValue), m_maxValue(maxValue) {
	GetAlignedCollider(m_pos, m_size, alignment, resolution);
	Initialize(focusID, resolution);
}

void SliderAndInputLine::CheckAndUpdate(Vector2 const& mousePosition,
	AppContext const& appContext) {
	Scene::CheckAndUpdate(mousePosition, appContext);

	if (m_inputLine->HasValueChanced()) {
		if (!m_slided) {
			m_btn->SetEnabled(true);
			ValidateCurrentValue();
		} else {
			m_slided = false;
		}
	}
}
