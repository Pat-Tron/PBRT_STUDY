#pragma once

#include <vector>
#include <string>
#include "Color.h"

enum class PIC_FORMAT { PPM, QOI };

void outputPic(
    const std::string &filename,
    const PIC_FORMAT &f,
    const std::vector<std::vector<Color>> &pixels
);