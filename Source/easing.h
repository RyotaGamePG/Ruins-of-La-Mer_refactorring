#pragma once

#include <math.h>

#ifndef PI
#define PI  3.14159265
#endif
class Back {
public:
	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};

class Bounce {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};

class Circ {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Cubic {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};

class Elastic {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};

class Expo {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Linear {
public:

	static float easeNone(float t, float b, float c, float d); // ??
	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Quad {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Quart {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Quint {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);
};


class Sine {
public:

	static float easeIn(float t, float b, float c, float d);
	static float easeOut(float t, float b, float c, float d);
	static float easeInOut(float t, float b, float c, float d);

};
