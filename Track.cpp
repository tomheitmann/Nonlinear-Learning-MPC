#include <iostream>
#include <math.h>
#include "SysModel.h"

const double pi = 3.14159265358979323846;

class Map {
private:


public:
	// track width
	float track_width;

	// constructor of class
	Map(float set_width) { track_width = set_width; };

	// define the geometry of the track: [segment #][length of segment, radius of segment]
	const float track_geometry[10][2] = {
										{60 * 0.03, 0.0} ,
										{80 * 0.03, -80 * 0.03 * 2 / pi},
										{20 * 0.03, 0},
										{80 * 0.03, -80 * 0.03 * 2 / pi},
										{40 * 0.03, +40 * 0.03 * 10 / pi},
										{60 * 0.03, -60 * 0.03 * 5 / pi},
										{40 * 0.03, +40 * 0.03 * 10 / pi},
										{80 * 0.03, -80 * 0.03 * 2 / pi},
										{20 * 0.03, 0},
										{80 * 0.03, -80 * 0.03 * 2 / pi} };

};
