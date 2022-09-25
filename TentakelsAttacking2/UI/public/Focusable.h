//
// Purpur Tentakel
// 04.09.2022
//

#pragma once
#include <raylib.h>


class Focusable {
private:
	unsigned int m_focusID;
	bool m_isFocus = false;

public:
	Focusable(unsigned int ID);
	~Focusable();
	
	[[nodiscard]] bool IsFocused() const;
	void SetFocus(bool focus);

	[[nodiscard]] virtual bool IsEnabled() const = 0;

	[[nodiscard]] unsigned int GetFocusID() const;
	[[nodiscard]] virtual Rectangle GetCollider() const = 0;
};