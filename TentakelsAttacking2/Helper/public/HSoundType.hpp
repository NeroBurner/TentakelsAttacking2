//
// Purpur Tentakel
// 24.08.2022
//

#pragma once

// first sounds get casted while loading
// need to keep the order
enum class SoundType {
	ACCEPTED,
	CLICKED_PRESS_STD,
	CLICKED_RELEASE_STD,
	CLICKED_DISABLED_STD,
	HOVER_STD,
	TEXT,
};