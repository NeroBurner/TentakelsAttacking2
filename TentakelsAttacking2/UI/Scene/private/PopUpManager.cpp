//
// Purpur Tentakel
// 05.09.2022
//

#include "PopUpManager.h"

PopUpManager::PopUpManager(Vector2 resolution)
	: m_appContext(&(AppContext::GetInstance())), m_resolution(resolution) {
	m_appContext->eventManager.AddListener(this);
}

bool PopUpManager::IsActivePopUp() const {
	return m_popUps.size() > 0;
}

void PopUpManager::OnEvent(Event const& event) {

	// Message Pop Up
	if (auto const PopUpEvent = dynamic_cast<ShowMessagePopUpEvent const*>(&event)) {
		NewMessagePopUp(PopUpEvent);
		return;
	}

	// Delete Player Pop Up
	if (auto const PopUpEvent = dynamic_cast<ShowDeletePlayerEvent const*>(&event)) {
		NewDeletePlayerPopUp(PopUpEvent);
		return;
	}

	// Table Pop Up
	if (auto const PopUpEvent = dynamic_cast<ShowStringCellPopUpEvent const*>(&event)) {
		NewTableCellPopUp<StringCellPopUp, ShowStringCellPopUpEvent>(PopUpEvent);
		return;
	}
	if (auto const PopUpEvent = dynamic_cast<ShowIntCellPopUpEvent const*>(&event)) {
		NewTableCellPopUp<IntCellPopUp, ShowIntCellPopUpEvent>(PopUpEvent);
		return;
	}
	if (auto const PopUpEvent = dynamic_cast<ShowFloatCellPopUpEvent const*>(&event)) {
		NewTableCellPopUp<FloatCellPopUp, ShowFloatCellPopUpEvent>(PopUpEvent);
		return;
	}
	if (auto const PopUpEvent = dynamic_cast<ShowDoubleCellPopUpEvent const*>(&event)) {
		NewTableCellPopUp<DoubleCellPopUp, ShowDoubleCellPopUpEvent>(PopUpEvent);
		return;
	}
	if (auto const PopUpEvent = dynamic_cast<ShowColorCellPopUpEvent const*>(&event)) {
		NewTableCellPopUp<ColorCellPopUp, ShowColorCellPopUpEvent>(PopUpEvent);
		return;
	}

	// Close Pop Up
	if (auto const PopUpEvent = dynamic_cast<ClosePopUpEvent const*>(&event)) {
		DeleteLastPopUp();
		return;
	}

}

void PopUpManager::NewMessagePopUp(ShowMessagePopUpEvent const* event) {
	auto focusEvent = NewFocusPopUpLayerEvent();
	m_appContext->eventManager.InvokeEvent(focusEvent);

	m_popUps.push_back(std::make_unique<MessagePopUp>(
		Vector2(0.5f, 0.5f),
		Vector2(0.5f, 0.5f),
		Alignment::MID_MID,
		m_resolution,
		event->GetTitle(),
		const_cast<std::string&>(event->GetSubTitle()),
		AssetType::EXCLAMATION_MARK
		)
	);
}
void PopUpManager::NewDeletePlayerPopUp(ShowDeletePlayerEvent const* event) {
	auto focusEvent = NewFocusPopUpLayerEvent();
	m_appContext->eventManager.InvokeEvent(focusEvent);

	m_popUps.push_back(std::make_unique<DeletePlayerPopUp>(
		Vector2(0.5f, 0.5f),
		Vector2(0.5f, 0.5f),
		Alignment::MID_MID,
		m_resolution,
		event->GetTitle(),
		AssetType::QUESTION_MARK,
		event->GetOnClick()
		));
}

void PopUpManager::DeleteLastPopUp() {
	if (m_popUps.size() > 0) {
		auto event = DeleteFocusPopUpLayerEvent();
		m_appContext->eventManager.InvokeEvent(event);

		m_popUps.pop_back();
	}
}

void PopUpManager::CheckAndUpdate(Vector2 const& mousePosition,
	AppContext const& appContext) {
	if (!IsActivePopUp()) {
		return;
	}

	m_popUps.at(m_popUps.size() - 1)->CheckAndUpdate(mousePosition, appContext);
}
void PopUpManager::Render(AppContext const& appContext) {
	for (auto& p : m_popUps) {
		p->Render(appContext);
	}
}
void PopUpManager::Resize(Vector2 resolution, AppContext const& appContext) {
	m_resolution = resolution;
	for (auto& e : m_popUps) {
		e->Resize(resolution, appContext);
	}
}