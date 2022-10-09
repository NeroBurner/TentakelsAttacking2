//
// Purpur Tentakel
// 02.10.2022
//

#include "DoubleCell.h"
#include "AppContext.h"
#include "HTextProcessing.h"

Vector2 const& DoubleCell::GetNeededSize() const {
	Vector2 neededSize = MeasureTextEx(
		*(AppContext::GetInstance().assetManager.GetFont()),
		std::to_string(value).c_str(),
		m_textSize,
		0.0f
	);

	return {
		(m_colider.x / neededSize.x) * m_size.x,
		(m_colider.y / neededSize.y) * m_size.y
	};
}

void DoubleCell::CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) {
	Cell::CheckAndUpdate(mousePosition, appContext);
	if (ShouldEdit(mousePosition)) {
		auto event = ShowDoubleCellPopUpEvent(
			"Edit Number",
			this
		);
		appContext.eventManager.InvokeEvent(event);
	}
}

void DoubleCell::Render(AppContext const& appContext) {
	std::string printableValue = GetPritablePlaceholderTextInColider(
		std::to_string(value),
		m_textSize,
		m_colider,
		appContext
	);
	DrawTextEx(
		*(appContext.assetManager.GetFont()),
		printableValue.c_str(),
		m_textPosition,
		m_textSize,
		0.0f,
		WHITE
	);
	Cell::Render(appContext);
}
