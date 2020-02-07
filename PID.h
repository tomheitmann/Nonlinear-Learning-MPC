#include <iostream>
#include <math.h>
#include <algorithm>    // std::max
#include <random>
 


class PID {
public:
	// target velocity
	double v_target;

	// initialize predicted inputs
	double u_pred[1][2]{};

	// feasibility flag 
	int feasible = 1;

	// constructor 
	PID(double set_v_target) { v_target = set_v_target; };


	// solve for the control action at time
	void solve_control_input(double x0_curv[6]) {
		// x0_curv current state (in curvilinear reference frame)


		std::default_random_engine generator;
		std::normal_distribution<double> distribution(0, 1.0);
		double randn_1 = distribution(generator);
		double randn_2 = distribution(generator);


		// calculation of inputs includes noise as well
		u_pred[0][0] = -0.6 * x0_curv[5] - 0.9 * x0_curv[3] +  std::max(-0.9, std::min(randn_1 * 0.25, 0.9));
		u_pred[0][1] = 1.5 * (v_target - x0_curv[0]) +  std::max(-0.2, std::min(randn_2 * 0.10, 0.2));

	}; // end solve_control_input







};// end class PID




