// Project EvoForm
// Licensed under GPLv3
// Author: Wenjie Fang (a.k.a fwjmath)

// cnvt.h
// Functions on various conversions related to colors

struct color_RGB{
	int r;
	int g;
	int b;
};

color_RGB hsv2rgb(float, float, float);

color_RGB float2rgb(float);