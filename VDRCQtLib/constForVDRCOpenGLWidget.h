#pragma once

#include "Color3f.h"

const int SPHERE_RESOLUTION = 100;

const float VEDGE_THICKNESS = 1.0f;
const float CANVAS_RANGE = 10000;

const int NUM_PICKING_CLASS = 10;
const int CLASS_VVERTEX = 1;

const float SELECTION_BOX_SIZE = 10.0;
const int SELECTION_BUFFER_SIZE = 64;

const float VVERTEX_BALL_RADIUS = 1;

const float STIPPLE_FACTOR = 4;
const GLushort STIPPLE_PATTERN = 0xAAAA;

const float CONE_HEIGHT = 1;
const float CONE_BASE_RADIUS = 0.5;
const float SPEED_RATIO_FOR_DRAWING_CONE = 100;

static string translate_to_window_path(const QString& QfilePath)
{
	string filePath = QfilePath.toLocal8Bit();

	size_t i = filePath.find('/');
	while (i != string::npos)
	{
		string part1 = filePath.substr(0, i);
		string part2 = filePath.substr(i + 1);
		filePath = part1 + R"(\)" + part2; // Use "\\\\" instead of R"(\\)" if your compiler doesn't support C++11's raw string literals
		i = filePath.find('/', i + 1);
	}
	return filePath;
}

const float ZOOM_RATIO = 1.001f;
const float MAX_DEPTH = 1000000.0f;