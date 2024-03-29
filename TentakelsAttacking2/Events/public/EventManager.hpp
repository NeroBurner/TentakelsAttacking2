//
// Purpur Tentakel
// 15.08.2022
//

#pragma once
#include "EventListener.hpp"
#include <vector>

 class  EventManager final {
public:
	void AddListener(EventListener* eventListener) {
		m_listeners.push_back(eventListener);
	}

	void RemoveListener(EventListener* eventlistener) {
		m_listeners.erase(std::remove(
			m_listeners.begin(), m_listeners.end(), eventlistener),
			m_listeners.end());
	}

	void InvokeEvent(Event const& event) const {
		for (auto const listener : m_listeners) {
			listener->OnEvent(event);
		}
	}

private:
	std::vector<EventListener*> m_listeners;
};