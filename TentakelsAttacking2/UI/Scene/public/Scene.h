//
// Purpur Tentakel
// 05.09.2022
//

#pragma once
#include "UIElement.h"
#include <vector>
#include <memory>

class UIManager;

class Scene: public UIElement {
protected:
	std::vector<std::shared_ptr<UIElement>> m_elements;
	bool m_active = false;
	unsigned int m_firstFocusID;

	void SetFocusActive(AppContext const& appContext);

	Vector2 GetElementPosition(float x, float y);
	Vector2 GetElementSize(float x, float y);
	Focusable* GetFocusableByFocusID(unsigned int ID) const;

public:
	Scene(Vector2 pos, Vector2 size);
	Scene(Scene const&) = default;
	Scene(Scene&&) = default;
	Scene& operator= (Scene const&) = default;
	Scene& operator= (Scene&&) = default;
	~Scene() = default;

	[[nodiscard]] bool IsActive() const;
	virtual void SetActive(bool active, AppContext const& appContext) = 0;

	void CheckAndUpdate(Vector2 const& mousePosition, AppContext const& appContext) override;
	void Render(AppContext const& appContext) override;
	void Resize(Vector2 resolution) override;
};