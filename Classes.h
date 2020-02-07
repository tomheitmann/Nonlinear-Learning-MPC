

#ifndef CLASSES
#define CLASSES
#include <iostream>
#include <array>

using namespace std;

class Closed_Loop_Data {
	//Object collecting closed loop data points
	//	Attributes :
	// updateInitialConditions: 
	//		function which updates initial conditions and clears the memory


public:
	// discretization time
	double dt;

	// number of points in simulation
	const int points = 1000; // equivalent to a simulation time of 100s

	//Initialize the input vector
	double u[1000][2]{};

	// Initialize state vector (In curvilinear abscissas)
	double x_curv[1000 + 1][6]{};

	// Initialize the state vector in absolute reference frame
	double x_glob[1000 + 1][6]{};

	double sim_time = 0.0;

	// v0: velocity initial condition
	double v0;

	// constructor of class
	Closed_Loop_Data(double set_dt, double set_v0) {
		dt = set_dt; v0 = set_v0; x_curv[0][0] = v0; x_glob[0][0] = v0;
	};
	
	
	// Initialize the state vector in absolute reference frame
	//x_curv[0][0] = v0; 
	//x_glob[0][0] = v0;
	
	void update_initial_conditions(double x_curv_init[6], double x_global_init[6]) {
		// clear memory
		//std::fill(x_curv,0)
		//x_curv.fill(0);// = {};
		//x_glob = {};

		// reset initial conditions
		for (int i = 0; i < 6; i++) {
			x_curv[0][i] = x_curv_init[i];
			x_glob[0][i] = x_global_init[i];
		}


	};

};// end definition class Closed_Loop_Data






#endif // !CLASSES
