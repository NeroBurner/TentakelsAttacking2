//
// Purpur Tentakel
// 25.09.2022
//

#include "HTextProcessing.h"
#include "AppContext.h"
#include "Allignment.h"
#include <raylib.h>
#include <string_view>

[[nodiscard]] std::vector<std::string> GetSerializedText(
	std::string const& text) {
	size_t lhs = 0;
	size_t rhs = 0;
	std::vector<std::string> toReturn;

	while (true) {
		rhs = text.find_first_of('\n', rhs + 1);
		if (rhs == std::string::npos) {
			toReturn.push_back(text.substr(lhs));
			break;
		}

		toReturn.push_back(text.substr(lhs, rhs - lhs));
		lhs = rhs + 1;
	}

	return toReturn;
}
[[nodiscard]] std::string GetStringFromVector(
	std::vector<std::string> const& slicedText) {

	std::string toReturn;

	for (auto const& s : slicedText) {
		toReturn += s;
		toReturn += '\n';
	}
	toReturn.pop_back();

	return toReturn;
}


void BreakText(std::string& toBreak, float fontSize, float length,
	AppContext const& appContext) {
	size_t lhs = 0;
	size_t rhs = 0;

	while (true) {
		rhs = toBreak.find_first_of(' ', rhs + 1);
		if (rhs == std::string::npos) {
			break;
		}

		std::string line(toBreak.c_str() + lhs, rhs - lhs);
		Vector2 textSize = MeasureTextEx(
			*(appContext.assetManager.GetFont()),
			line.data(),
			fontSize,
			0.0f
		);

		if (textSize.x > length) {
			rhs = toBreak.find_last_of(' ', rhs - 1);
			if (rhs == std::string::npos) {
				rhs = toBreak.find_first_of(' ');
				if (rhs == std::string::npos) {
					break;
				}
			}
			toBreak.at(rhs) = '\n';
			lhs = rhs + 1;
		}
	}
}

float GetElementTextHight(Vector2 const& size, float height) {
	return size.y * height;
}

std::string GetPritableTextInColider(std::string const& text,
	float fontSize, Rectangle colider, AppContext const& appContext) {
	auto constants = appContext.constants.textProcecing;
	Vector2 textSize = MeasureTextEx(
		*(appContext.assetManager.GetFont()),
		(text + constants.enter).c_str(),
		fontSize,
		0.0f);
	if ((textSize.x + constants.cursorOffset) < colider.width) {
		return text;
	}

	std::string toReturn = text;
	std::string toCheck = constants.prefix + text + constants.enter;

	do {
		toReturn = toReturn.substr(1, toReturn.size());
		toCheck = constants.prefix + toReturn + constants.enter;
		textSize = MeasureTextEx(
			*(appContext.assetManager.GetFont()),
			toCheck.c_str(),
			fontSize,
			0.0f);
	} while (textSize.x + constants.cursorOffset >= colider.width);

	return constants.prefix + toReturn;
}
std::string GetPritablePlaceholderTextInColider(std::string const& text,
	float fontSize, Rectangle colider, AppContext const& appContext) {
	auto constants = appContext.constants.textProcecing;
	Vector2 textSize = MeasureTextEx(
		*(appContext.assetManager.GetFont()),
		text.c_str(),
		fontSize,
		0.0f);
	if ((textSize.x + constants.cursorOffset) < colider.width) {
		return text;
	}

	std::string toReturn = text;
	std::string toCheck = constants.prefix + text;

	do {
		toReturn = toReturn.substr(0, toReturn.size() - 1);
		toCheck = constants.prefix + toReturn;
		textSize = MeasureTextEx(
			*(appContext.assetManager.GetFont()),
			toCheck.c_str(),
			fontSize,
			0.0f);
	} while (textSize.x + constants.cursorOffset >= colider.width);

	return  toReturn + constants.prefix;
}

void StripString(std::string& toStrip) {
	size_t pos1 = toStrip.find_first_not_of(' ');

	if (pos1 == std::string::npos) {
		toStrip = std::string();
		return;
	}

	size_t pos2 = toStrip.find_last_not_of(' ');
	toStrip = toStrip.substr(pos1, pos2 - pos1 + 1);
}

Vector2 GetVerticalAlignedTextPosition(std::string const& text,
	float fontSize, Rectangle colider, Alignment alignment) {
	
	TextAlignment textAlignment = GetVerticalTextAlignment(alignment);

	if (textAlignment == TextAlignment::TOP) {
		return { colider.x, colider.y };
	}

	AppContext& appContext = AppContext::GetInstance();
	Vector2 textSize = MeasureTextEx(
		*(appContext.assetManager.GetFont()),
		text.c_str(),
		fontSize,
		0.0f
	);

	if (colider.height < textSize.y) {
		return { colider.x, colider.y };
	}

	float difference = colider.height - textSize.y;
	difference = textAlignment == TextAlignment::MID
		? difference / 2 : difference;

	return { colider.x, colider.y + difference };
}

std::string GetHorisontalAlignedText(std::string const& text,
	Rectangle colider, float fontSize,
	Alignment alignment) {

	TextAlignment textAlignment =
		GetHorisontalTextAlignment(alignment);

	if (textAlignment == TextAlignment::LEFT) {
		return text;
	}

	AppContext& appContext = AppContext::GetInstance();

	Vector2 spaceSize = MeasureTextEx(
		*(appContext.assetManager.GetFont()),
		" ",
		fontSize,
		0.0f
	);

	std::vector<std::string> slicedText = GetSerializedText(text);
	std::vector<std::string> alignedSlicedText;

	for (auto const& t : slicedText) {
		Vector2 textSize = MeasureTextEx(
			*(appContext.assetManager.GetFont()),
			t.c_str(),
			fontSize,
			0.0f
		);

		if (colider.width < textSize.x) {
			alignedSlicedText.push_back(t);
			continue;
		}

		float difference = colider.width - textSize.x;
		difference = textAlignment == TextAlignment::MID ?
			difference / 2 : difference;

		size_t spaceCount =
			static_cast<size_t>(difference / spaceSize.x);

		std::string entry(spaceCount, ' ');
		entry += t;
		alignedSlicedText.push_back(entry);
	}


	std::string toReturn = GetStringFromVector(alignedSlicedText);

	return toReturn;
}