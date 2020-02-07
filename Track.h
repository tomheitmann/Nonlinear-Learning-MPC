/*double wrap_(double angle);
double unwrap_(double angle1, double angle2);
double compute_angle_(double x_1, double y_1, double x_origin, double y_origin, double x_2, double y_2);
*/

#ifndef TRACK
#define TRACK


const double pi = 3.14159265358979323846;




/*
///////////////////////////////////////////////////////////////////
// Utility functions 

//double wrap(double angle) { // checking that angle is in (-pi,pi)
double wrap_(double angle) { // checking that angle is in (-pi,pi)

	double w_angle;
	if (angle < -pi) {
		w_angle = 2.0 * pi + angle;
	}
	else if (angle > pi) {
		w_angle = angle - 2.0 * pi;
	}
	else {
		w_angle = angle;
	}

	return w_angle;
}

double unwrap_(double angle1, double angle2) {
	// checking that diff between angles is smaller than pi

	double angle = angle2 - angle1;

	while (angle < -pi || angle > pi) {
		std::cout << angle << std::endl;
		if (angle < -pi) {
			angle += 2 * pi;
		}
		else if (angle > pi) {
			angle -= 2 * pi;
		}
		angle2 = angle + angle1;
		angle = angle2 - angle1;
	}

	return angle2;
}


double compute_angle_(double x_1, double y_1, double x_origin, double y_origin, double x_2, double y_2) {
	// calculates angle between two points and the origin of the angle

	// dot product between [x1, y1] and [x2, y2]
	double dot = (x_1 - x_origin) * (x_2 - x_origin) + (y_1 - y_origin) * (y_2 - y_origin);
	// determinant
	double det = (x_1 - x_origin) * (y_2 - y_origin) - (y_1 - y_origin) * (x_2 - x_origin);

	return atan2(det, dot);
} // end fcn definition compute_angle
*/

class Map {
private:

	// define the geometry of the track: [segment #][length of segment, radius of segment]
	const double track_geometry[10][2] = {
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


	


	void Calculate_track_data() {
		// Now given the above segments we compute the (x, y) points
		// of the track and the angle of the tangent vector(psi) at
		// these points. For each segment we compute the(x, y, psi)
		// coordinate at the last point of the segment. Furthermore,
		// we compute also the cumulative s at the starting point of
		// the segment at signed curvature

		// track_data = [x, y, psi, cumulative s, segment length,
		// signed curvature]


		double l, r, angle, x, y, direction, center_x, center_y, spanned_angle, normal_angle;


		for (int i = 0; i < 11; i++) {
			if (track_geometry[i][1] == 0) {                       //If the current segment is a straight line
				l = track_geometry[i][0];		                   // Length of the segment

				if (i == 0) {
					angle = 0;
					x = 0 + l * cos(angle);                        // x coordinate of the last point of the segment
					y = 0 + l * sin(angle);			               // y coordinate of the last point of the segment
				}
				else {
					angle = track_data[i - 1][2];                    // Angle of the tangent vector at the starting point of the segment
					x = track_data[i - 1][0] + l * cos(angle);  // x coordinate of the last point of the segment
					y = track_data[i - 1][1] + l * sin(angle);  // y coordinate of the last point of the segment
					track_data[i][3] = track_data[i - 1][3] + track_data[i - 1][4]; // pos s of track segment
				}

				// assign values
				track_data[i][0] = x;
				track_data[i][1] = y;
				track_data[i][2] = angle;   //Angle of the tangent vector at the last point of the segment
				track_data[i][4] = l;
				track_data[i][5] = 0.0;
			}
			else { // if current segment is not a straight line
				l = track_geometry[i][0];
				r = track_geometry[i][1];

				if (r >= 0) {
					direction = 1;
				}
				else {
					direction = -1;
				}

				if (i == 0) {
					angle = 0; // angle of the tangent vector at the starting point of the segment
					center_x = 0 + abs(r) * cos(angle + direction * pi / 2); //x coordinate center of circle
					center_y = 0 + abs(r) * sin(angle + direction * pi / 2); //y coordinate center of circle
				}
				else {
					angle = track_data[i - 1][2];
					center_x = track_data[i - 1][0] + abs(r) * cos(angle + direction * pi / 2); //x coordinate center of circle
					center_y = track_data[i - 1][1] + abs(r) * sin(angle + direction * pi / 2); //y coordinate center of circle
					track_data[i][3] = track_data[i - 1][3] + track_data[i - 1][4]; // pos s of track segment

				}


				spanned_angle = l / abs(r); //Angle spanned by the circle
				track_data[i][2] = wrap_(angle + spanned_angle * copysign(1, r)); //# Angle of the tangent vector at the last point of the segment

				normal_angle = wrap_(direction * pi / 2 + angle);
				x = center_x + abs(r) * cos(-pi + normal_angle + direction * spanned_angle); // x coordinate of the last point of the segment
				y = center_y + abs(r) * sin(-pi + normal_angle + direction * spanned_angle); // y coordinate of the last point of the segment

				// assign values
				track_data[i][0] = x;
				track_data[i][1] = y;
				//track_data[i][2] = angle;   //Angle of the tangent vector at the last point of the segment
				track_data[i][4] = l;
				track_data[i][5] = 1 / r;

			}

		}
		l = sqrt(track_data[9][0] * track_data[9][0] + track_data[9][1] * track_data[9][1]);
		track_data[10][0] = 0;
		track_data[10][1] = 0;
		track_data[10][2] = 0;
		track_data[10][3] = track_data[9][3] + track_data[9][4];
		track_data[10][4] = l;
		track_data[10][5] = 0;

		// assign track length
		track_length = track_data[10][3] + track_data[10][4]; // cumultative s + segment length of last segment

		
		/*for (int i = 0; i < 11; i++) {
			std::cout << track_data[i][0] << " " << track_data[i][1] << " " << track_data[i][2] << " " << track_data[i][3] << " " << track_data[i][4] << " " << track_data[i][5] << std::endl;
		};*/
	}

	///////////////////////////////////////////////////////////////////
// Utility functions 

	double wrap_(double angle) { // checking that angle is in (-pi,pi)

		double w_angle;
		if (angle < -pi) {
			w_angle = 2.0 * pi + angle;
		}
		else if (angle > pi) {
			w_angle = angle - 2.0 * pi;
		}
		else {
			w_angle = angle;
		}

		return w_angle;
	}

	double unwrap_(double angle1, double angle2) {
		// checking that diff between angles is smaller than pi

		double angle = angle2 - angle1;

		while (angle < -pi || angle > pi) {
			std::cout << angle << std::endl;
			if (angle < -pi) {
				angle += 2 * pi;
			}
			else if (angle > pi) {
				angle -= 2 * pi;
			}
			angle2 = angle + angle1;
			angle = angle2 - angle1;
		}

		return angle2;
	}


	double compute_angle_(double x_1, double y_1, double x_origin, double y_origin, double x_2, double y_2) {
		// calculates angle between two points and the origin of the angle

		// dot product between [x1, y1] and [x2, y2]
		double dot = (x_1 - x_origin) * (x_2 - x_origin) + (y_1 - y_origin) * (y_2 - y_origin);
		// determinant
		double det = (x_1 - x_origin) * (y_2 - y_origin) - (y_1 - y_origin) * (x_2 - x_origin);

		return atan2(det, dot);
	} // end fcn definition compute_angle



	void get_local_position(double* s, double* ey, double* epsi, int* completed_flag, double x, double y, double psi) {
		// coordinate transformation from inertial reference frame (X, Y) to curvilinear reference frame (s, ey)
		// (X, Y) : position in the inertial reference frame

		*completed_flag = 0;

		double xf, yf, xs, ys, psi_unwrap, norm_v, angle, s_local, r, direction;
		double center_x, center_y, arc1, arc2;


		for (int i = 0; i < 11; i++) {

			// check for completed flag
			if (*completed_flag == 1) { break; }

			// if segment if straight
			if (track_data[i][5] == 0.0) {

				// extract fist and final initial point of the segment
				xf = track_data[i][0];
				yf = track_data[i][1];
				xs = track_data[i - 1][0];
				ys = track_data[i - 1][1];

				psi_unwrap = unwrap_(track_data[i - 1][2], psi);
				*epsi = psi_unwrap - track_data[i - 1][2];

				// check if on the segment using angles 
				if (xs - x == 0 && ys - y == 0) {
					*s = track_data[i][3];
					*ey = 0;
					*completed_flag = 1;
				} // at start of segment
				else if (xf - x == 0 && yf - y == 0) {
					*s = track_data[i][3] + track_data[i][4];
					*ey = 0;
					*completed_flag = 1;
				} // at end of segment
				else {
					if (abs(compute_angle_(x, y, xs, ys, xf, yf)) <= pi / 2 || abs(compute_angle_(x, y, xf, yf, xs, ys)) <= pi / 2) {
						norm_v = sqrt((x - xs) * (x - xs) + (y - ys) * (y - ys));
						angle = compute_angle_(x, y, xs, ys, xf, yf);
						s_local = norm_v * cos(angle);
						*s = s_local + track_data[i][3];
						*ey = norm_v * sin(angle);

						if (abs(*ey) <= track_width) {
							*completed_flag = 1;
						} // check within track limits
					}
				} // end check if directly on middle of segmant
			}// end if segment straight line

			else { // not a straight segment

				// extract fist and final initial point of the segment
				xf = track_data[i][0];
				yf = track_data[i][1];
				xs = track_data[i - 1][0];
				ys = track_data[i - 1][1];

				r = 1 / track_data[i][5]; // curvature

				if (r >= 0) {
					direction = 1;
				}
				else {
					direction = -1;
				}

				angle = track_data[i - 1][2];

				// x coordinate center of circle
				center_x = xs + abs(r) * cos(angle + direction * pi / 2);
				// y coordinate center of circle
				center_y = ys + abs(r) * sin(angle + direction * pi / 2);

				// check if on the segment using angles 
				if (xs - x == 0 && ys - y == 0) {
					*ey = 0;
					psi_unwrap = unwrap_(angle, psi);
					*epsi = psi_unwrap - angle;
					*s = track_data[i][3];
					*completed_flag = 1;
				} // at start of segment

				else if (xf - x == 0 && yf - y == 0) {
					*s = track_data[i][3] + track_data[i][4];
					*ey = 0;
					psi_unwrap = unwrap_(track_data[i][2], psi);
					*epsi = psi_unwrap - track_data[i][2];
					*completed_flag = 1;
				} // at end of segment
				else {
					arc1 = track_data[i][4] * track_data[i][5];
					arc2 = compute_angle_(xs, ys, center_x, center_y, x, y);
					if (copysign(1, arc1) == copysign(1, arc2) && copysign(1, arc1) >= copysign(1, arc2)) {
						norm_v = sqrt((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
						s_local = abs(arc2) * abs(r);
						*s = s_local + track_data[i][3];
						*ey = copysign(1, direction) * (norm_v - abs(r));
						psi_unwrap = unwrap_(angle + arc2, psi);
						*epsi = psi_unwrap - angle - arc2;

						if (abs(*ey) <= track_width) {
							*completed_flag = 1;
						} // check within track limits
					}
				}
			}
		} // end of for iterating over all segments


	}; // end fnc get_local_position


public:
	// track width
	double track_width;

	//double angle = wrap_(track_width);
	
	double track_data[11][6]{};

	double track_length;

	// for plotting
	double center_line[200][2]{};
	double inner_line[200][2]{};
	double outer_line[200][2]{};


	// constructor of class
	Map(double set_width) { track_width = set_width; Calculate_track_data();
	double x_inner, x_center, x_outer, y_inner, y_center, y_outer;
	for (int i = 0; i < 200; i++) {

		get_global_position(&x_inner, &y_inner, 0.1 * i, -track_width);
		inner_line[i][0] = x_inner;
		inner_line[i][1] = y_inner;

		get_global_position(&x_center, &y_center, 0.1 * i, 0);
		center_line[i][0] = x_center;
		center_line[i][1] = y_center;

		get_global_position(&x_outer, &y_outer, 0.1 * i, track_width);
		outer_line[i][0] = x_outer;
		outer_line[i][1] = y_outer;
	}	// end for
	}; // end constructor


	double curvature(double s) {
		// curvature computation
		// s: curvilinear abscissa at which the curvature has to be evaluated

		
		// check that s is along the track, if in a lap after the first one
		while (s > track_length) {
			s = s - track_length;
		};

		// compute the segment in which the system is evolving
		int i = 0; // segement
		
		while (!(s >= track_data[i][3] && s < track_data[i][3]+ track_data[i][4])) { i++;};


		return track_data[i][5];

	}; //  curvature fcn



	void get_global_position(double* x, double* y, double s, double ey) {
		// coordinate transformation from curvilinear reference frame (e, ey) to inertial reference frame (X, Y)
		// inputs: (s, ey) : position in the curvilinear reference frame
		// inputs: call by reference: pointers to x, y 


		// check that s is along the track
		while (s > track_length) {
			s = s - track_length;
		};

		// compute the segment in which the system is evolving
		int i = 0; // segement
		while (!(s >= track_data[i][3] && s < track_data[i][3] + track_data[i][4])) { i++; };


		double xf, yf, xs, ys, psi, delta_l, relta_l, r, direction, center_x, center_y, spanned_angle, normal_angle, angle_;

		if (track_data[i][5] == 0.0 ) { // if segment is a straight line
			xf = track_data[i][0];
			yf = track_data[i][1];
			xs = track_data[i - 1][0];
			ys = track_data[i - 1][1];
			if (i == 0) {
				xs = track_data[10][0];
				ys = track_data[10][1];
			}
			psi = track_data[i][2];


			// compute the segment length
			delta_l = track_data[i][4];
			relta_l = s - track_data[i][3];

			*x = (1 - relta_l / delta_l) * xs + relta_l / delta_l * xf + ey * cos(psi + pi / 2);
			*y = (1 - relta_l / delta_l) * ys + relta_l / delta_l * yf + ey * sin(psi + pi / 2);
		
		}
		else {
			
			r = 1 / track_data[i][5];   // Extract curvature
			angle_ = track_data[i - 1][2];   // Extract angle of the tangent at the initial point(i - 1)

			// Compute the center of the arc
			if (r >= 0) {
				direction = 1;
			}
			else {
				direction = -1;
			}

			// x coordinate center of circle
			center_x = track_data[i - 1][0] + abs(r) * cos(angle_ + direction * pi / 2);
			// y coordinate center of circle
			center_y = track_data[i - 1][1] + abs(r) * sin(angle_ + direction * pi / 2);

			spanned_angle = (s - track_data[i][3]) / (pi * abs(r)) * pi;

			normal_angle = wrap_((direction * pi / 2 + angle_));

			// x coordinate of the last point of the segment
			*x = center_x + (abs(r) - direction * ey) * cos(-(pi - normal_angle) + direction * spanned_angle);
			// y coordinate of the last point of the segment
			*y = center_y + (abs(r) - direction * ey) * sin(-(pi - normal_angle) + direction * spanned_angle);
		}
	}; // end fnc get_global_position

}; // end class Map
	
#endif // !TRACK







