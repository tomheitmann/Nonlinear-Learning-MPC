#include <iostream>
#include <string>
#include "SysModel.h"
#include "Track.h"
#include "Classes.h"
#include "Controller.h"

using namespace std;

#ifndef SIMULATOR
#define SIMULATOR

class Simulator {
public:
	// lab counter
	int lab = 0; // not used yet

	
	// indicator if LMPC is used
	int flagLMPC = 0; // not used yet
	

	// constructor of class
	Simulator(int set_lab ,  int set_flag) { lab = set_lab; flagLMPC = set_flag; };

	
	void sim(Closed_Loop_Data* closed_loop_data, Controller::PID *controller, Map *map) {
	
		double simulation_time = 0;

		double x_glob_next[6]{};
		double x_curv_next[6]{};

		double x0_curv[6] = {};
		double x0_glob[6]{};

		double u_pred_[2]{};

		for (int i = 0; i < closed_loop_data->points; i++) {

			// assign current state

			for (int j=0; j < 6; j++) {
				x0_curv[j] = closed_loop_data->x_curv[i][j];
				x0_glob[j] = closed_loop_data->x_glob[i][j];

			}

			// calculate control input
			controller->solve_control_input(x0_curv);

			//controller->u_pred[0];

			for (int k = 0; k < 2; k++) {
				u_pred_[k] = controller->u_pred[0][k];
				closed_loop_data->u[i][k] = u_pred_[k]; // controller->u_pred[0][k];

				
			}
		
			

			// propagate dynamics
			//DynModel(&x_glob_next[0], &x_curv_next[0], x0_curv,  x0_glob, controller->u_pred[1], closed_loop_data->dt);
			DynModel(&x_glob_next[0], &x_curv_next[0], x0_curv, x0_glob, u_pred_, closed_loop_data->dt, map);

			//cout << i << endl;

			for (int j=0; j < 6; j++) {
				closed_loop_data->x_curv[i+1][j] = x_curv_next[j];
				closed_loop_data->x_glob[i+1][j] = x_glob_next[j];

				
				
			}

			if (i < 0) {

				std::cout << i << std::endl;

				std::cout << "x_curv" ;
				for (int j = 0; j < 6; j++) {
					std::cout << closed_loop_data->x_curv[i][j] << " ";
				}
				std::cout << std::endl;

				std::cout << "x_glob";
				for (int j = 0; j < 6; j++) {
					std::cout << closed_loop_data->x_glob[i][j] << " ";
				}
				std::cout << std::endl;

				std::cout << "u";
				for (int j = 0; j < 2; j++) {
					std::cout << closed_loop_data->u[i][j] << " ";
				}
				std::cout << std::endl;

				
			}// if i<5
		
		
		}// end for loop over all (1000) time-steps 
		
	
	};// end fcn sim() 
	

};// end class Simulator

#endif // !SIMULATOR
