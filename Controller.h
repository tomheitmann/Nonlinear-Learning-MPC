#include <iostream>
#include <string>
#include "SysModel.h"
#include "Track.h"
#include "Classes.h"

#include <math.h>
#include <algorithm>    // std::max
#include <random>

#ifndef CONTROLLER
#define CONTROLLER

std::default_random_engine generator;
std::normal_distribution<double> distribution(0, 1.0);


class Controller {
public:
	class PID{
	public :
		// target velocity
		double v_target;

		// control parameters
		double p_vx = 0; // proportional factor on longitudinal velocity
		double d_psi = 0; // derivational factor on yaw rate
		double p_ey = 0; // proportional factor on lateral error

		// initialize predicted inputs
		double u_pred[1][2]{};

		// feasibility flag 
		int feasible = 1;

		// constructor 
		PID(double set_v_target, double set_p_vx, double set_d_psi, double set_p_ey) { 
			v_target = set_v_target; 
			p_vx = set_p_vx;
			d_psi = set_d_psi;
			p_ey = set_p_ey;
		};


		// solve for the control action at time
		void solve_control_input(double x0_curv[6]) {
			// x0_curv current state (in curvilinear reference frame)
			// x0_curv[0]: vxt vehicle longitudinal velocity 
			// x0_curv[1]: vyt lateral velocity
			// x0_curv[2]: dpsit yaw rate
			// x0_curv[3]: epsit heading angle error
			// x0_curv[4]: eyt lateral distance error between vehicle and path
			// x0_curv[5]: st distance travelled along centerline of road


			//std::default_random_engine generator;
			//std::normal_distribution<double> distribution(0, 1.0);
			//double randn_1 = distribution(generator);
			//double randn_2 = distribution(generator);


			// calculation of inputs includes noise as well
			u_pred[0][0] = -p_ey * x0_curv[5] - d_psi * x0_curv[3] + 0*std::max(-0.9, std::min(distribution(generator) * 0.25, 0.9));
			u_pred[0][1] = p_vx * (v_target - x0_curv[0]) + 0*std::max(-0.2, std::min(distribution(generator) * 0.10, 0.2));

		}; // end solve_control_input

	};// end class PID



};
#endif // !CONTROLLER
