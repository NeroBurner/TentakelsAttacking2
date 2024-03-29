//
// Purpur Tentakel
// 28.10.2022
//

#pragma once
#include <string>

enum class PrintType {
	INFO,
	EXPECTED_ERROR,
	ERROR,
};

void Print(std::string const& message, PrintType printType = PrintType::INFO);