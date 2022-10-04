//
// Purpur Tentakel
// 01.10.2022
// 

#include "Table.h"
#include "Allignment.h"
#include "UIEvents.h"
#include "AppContext.h"
#include "StringCellPopUp.h"
#include <stdexcept>

size_t Table::GetIndex(size_t row, size_t column) const {
	return row * m_columns + column;
}
Vector2 Table::GetElementPosition(size_t row, size_t column) const {
	Vector2 elementSize = GetElementSize();
	return Vector2 { 
		m_pos.x + (elementSize.x * column),
		m_pos.y + (elementSize.y * row)
	};
}
Vector2 Table::GetElementSize() const {
	return Vector2{
		m_size.x / m_columns,
		m_size.y / m_rows
	};
}
[[nodiscard]] void Table::CheckValidRowColumn(size_t row, size_t column) const {
	if (row < 0 or m_rows <= row) {
		throw std::out_of_range("row out of range" + row);
	}
	if (column < 0 or m_columns <= column) {
		throw std::out_of_range("column out of range" + column);
	}
}

Table::Table(Vector2 pos, Vector2 size, Alignment alignment, unsigned int ID,
	size_t rows, size_t columns, Vector2 resolution)
	: UIElement(pos, size, alignment), Focusable(ID),
	m_rows(rows), m_columns(columns), m_resolution(resolution) {
	m_colider = GetAlignedCollider(m_pos, m_size, alignment, resolution);
	m_cells.reserve(m_columns * m_rows);
	for (int row = 0; row < m_rows; ++row) {
		for (int column = 0; column < m_columns; ++column) {
			m_cells.push_back(std::make_unique<EmptyCell>(
				GetElementPosition(row, column),
				GetElementSize(),
				Alignment::DEFAULT,
				static_cast<unsigned int>(GetIndex(row, column)),
				resolution
				));
		}
	}
	AppContext::GetInstance().eventManager.AddListener(this);
}
Table::~Table() {
	AppContext::GetInstance().eventManager.RemoveListener(this);
}

void Table::OnEvent(Event const& event) {
	if (auto const PopUpEvent = dynamic_cast<ShowStringCellPopUpEvent const*>(&event)) {
		GeneratePremitiveCellPopUp<StringCellPopUp, ShowStringCellPopUpEvent>(PopUpEvent);
		return;
	}

	if (auto const PopUpEvent = dynamic_cast<ShowIntCellPopUpEvent const*>(&event)) {
		GeneratePremitiveCellPopUp<IntCellPopUp, ShowIntCellPopUpEvent>(PopUpEvent);
		return;
	}

	if (auto const PopUpEvent = dynamic_cast<ClosePopUpEvent const*>(&event)) {
		if (PopUpEvent->GetPop() == m_popUp.get()) {
			m_popUp.reset(nullptr);
		}
	}
}

void Table::CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) {
	if (m_popUp) {
		m_popUp->CheckAndUpdate(mousePosition, appContext);
		return;
	}

	if (!m_cellFocus) {
		if (!IsFocused()) {
			if (CheckCollisionPointRec(mousePosition, m_colider)) {
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					auto event = SelectFocusElementEvent(this);
					appContext.eventManager.InvokeEvent(event);
				}
			}
		}

		if (IsFocused()) {
			bool focusCell = IsKeyPressed(KEY_ENTER)
				or IsKeyPressed(KEY_SPACE)
				or (CheckCollisionPointRec(mousePosition, m_colider)
					and IsMouseButtonPressed(MOUSE_BUTTON_LEFT));

			if (focusCell) {
				auto event = NewFocusLayerEvent();
				appContext.eventManager.InvokeEvent(event);
				for (auto& c : m_cells) {
					auto event2 = NewFocusElementEvent(c.get());
					appContext.eventManager.InvokeEvent(event2);
				}
				m_cellFocus = true;
			}
		}
	}

	if (m_cellFocus) {
		for (auto& c : m_cells) {
			c->CheckAndUpdate(mousePosition, appContext);
		}

		if (IsKeyPressed(KEY_ESCAPE)) {
			auto event = DeleteFocusLayerEvent();
			appContext.eventManager.InvokeEvent(event);
			m_cellFocus = false;
		}
	}
}
void Table::Render(AppContext const& appContext){
	for (auto& c : m_cells) {
		c->Render(appContext);
	}

	if (m_popUp) {
		m_popUp->Render(appContext);
	}
}
void Table::Resize(Vector2 resolution, AppContext const& appContext){
	m_resolution = resolution;
	m_colider = {
		resolution.x * m_pos.x,
		resolution.y * m_pos.y,
		resolution.x * m_size.x,
		resolution.y * m_size.y
	};

	for (auto& c : m_cells) {
		c->Resize(resolution, appContext);
	}

	if (m_popUp) {
		m_popUp->Resize(resolution, appContext);
	}

}

bool Table::IsEnabled() const {
	return true;
}
Rectangle Table::GetCollider() const {
	return m_colider;
}

Vector2 Table::GetResolution() const {
	return m_resolution;
}

void Table::SetEmptyCell(size_t row, size_t column) {
	CheckValidRowColumn(row, column);
	SetCell<EmptyCell>(row, column);
}
