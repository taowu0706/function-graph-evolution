// Project EvoForm
// Licensed under GPLv3
// Author: Wenjie Fang (a.k.a fwjmath)

// cnvt.cpp
// Functions on various conversions related to colors

#include "cnvt.h"
#include <cmath>

// Promise: 0 <= h < 360, 0 <= s,v <= 1
color_RGB hsv2rgb(float h, float s, float v){
	color_RGB c;
	int phase = h/60;
	float p = v * (1 - s);
	float q = v * (1 - s * ((float)h/60.0f - (float)phase));
	float t = q - p;

	switch(phase){
	case 0:
		c.r = (int)(v * 256);
		c.g = (int)(t * 256);
		c.b = (int)(p * 256);
		break;

	case 1:
		c.r = (int)(q * 256);
		c.g = (int)(v * 256);
		c.b = (int)(p * 256);
		break;

	case 2:
		c.r = (int)(p * 256);
		c.g = (int)(v * 256);
		c.b = (int)(t * 256);
		break;

	case 3:
		c.r = (int)(p * 256);
		c.g = (int)(q * 256);
		c.b = (int)(v * 256);
		break;

	case 4:
		c.r = (int)(t * 256);
		c.g = (int)(p * 256);
		c.b = (int)(v * 256);
		break;

	case 5:
		c.r = (int)(v * 256);
		c.g = (int)(p * 256);
		c.b = (int)(q * 256);
		break;

	default:
		c.r = 0;
		c.g = 0;
		c.b = 0;
	}

	return c;
}

// This conversion model can be changed.
color_RGB float2rgb(float d){
	float d1=d;
	unsigned int dd = *((unsigned int *) (&d1));
	unsigned int hh = 0, ss = 0, vv = 0, tt = 1;
	/*
	for(int i = 0; i < 8; i++){
		if(tt & dd) ss += (1 << i);
		tt <<= 1;
		if(tt & dd) hh += (1 << i);
		tt <<= 1;
		if(tt & dd) vv += (1 << i);
		tt <<= 1;
	}
	*/
	vv = ((dd >> 15) & 255);
	d1 = log10(1+abs(d));
	dd = *((unsigned int *) (&d1));
	ss = ((dd >> 15) & 255);
	d1 = atan(d);
	dd = *((unsigned int *) (&d1));
	hh = ((dd >> 15) & 255);
	return hsv2rgb(((float)hh / 256.0) * 360.0, (float)ss / 256.0f, (float)vv / 256.0f);
}


