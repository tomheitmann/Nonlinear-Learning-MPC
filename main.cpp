// Main file for Nonlinear LMPC

#include <iostream>
#include <string>
#include "SysModel.h"
#include "Track.h"
#include "Classes.h"
#include "Simulator.h"
#include "Controller.h"
#include "Plot.h"




using namespace std;

int main() {

	// dynamic model
	/*
	double x[6]{};
	double x_glob[6]{};
	double u[2]={1,3};
	double dt = 1.0 / 10.0;

	// declare & initialize variables for states
	double x_next[6]{};
	double cur_x_next[6]{};

	DynModel(&x_next[0], &cur_x_next[0], x, x_glob, u, dt);


	 cout << x_next[0] << " " << cur_x_next[0] << endl;
	 cout << x_next[1] << " " << cur_x_next[1] << endl;
	 cout << x_next[2] << " " << cur_x_next[2] << endl;
	 cout << x_next[3] << " " << cur_x_next[3] << endl;
	 cout << x_next[4] << " " << cur_x_next[4] << endl;
	 cout << x_next[5] << " " << cur_x_next[5] << endl;
	*/


	//Map map(0.8); 

	//cout << map.angle << endl;
	//cout << map.track_geometry[9][0] << endl; 

	/*
	// how to call track data
	double track_data[11][6];
	double* ptr_track_data = map.ptr_track_data;

	for (int i=0; i < 11; i++) {
		for (int j=0; j < 6; j++) {
			track_data[i][j] = *(ptr_track_data+6*i+j);
			//cout << 6 * i + j << " " <<track_data[i][j] <<endl;
		}
		cout << track_data[i][0] << " " << track_data[i][1] << " " << track_data[i][2] << " " << track_data[i][3] << " " << track_data[i][4] << " " << track_data[i][5] << endl;
	}
	*/

	// use of get_global_position
	/*
	double X;
	double Y;
	double s = 5;
	double ey = 0.1;

	map.get_global_position(&X, &Y, s, ey);
	cout << X << " " << Y << endl;
   */

   //double bbb = unwrap(7.71, 19.0);
   //cout << bbb << endl;


   /*
   double aa, bb;
   double cc;
   double X_ = 2;
   double Y_ = 0.1;
   double ey = 0.0;
   int completed;

   map.get_local_position(&aa, &bb, &cc, &completed, X_, Y_, ey);
   cout << aa << " " << bb << " " << cc << " " << completed << endl;
  */
  /*
  Closed_Loop_Data closed_loop_data(0.1, 0.5);
  closed_loop_data.x_curv[0][0] += 1;

  cout << closed_loop_data.v0 << " " << closed_loop_data.x_curv[0][0] << endl;
  */

  /*
  Controller::PID controller(0.8);

  double x0_curv[6] = { 7.97045290e-01,  2.26254186e-03,  1.20814254e-02, - 2.08905874e-02, 7.67857785e+01,  2.48953714e-02 };
  controller.solve_control_input(x0_curv) ;
  cout << controller.u_pred[0][0] << " " << controller.u_pred[0][1] << endl;
 */
 //////////////////////////////////////////////////////////////////////////
 // control parameters
 //////////////////////////////////////////////////////////////////////////
 // PID 
	double p_vx = 1.5; // proportional factor on longitudinal velocity
	double d_psi = 0.9; // derivational factor on yaw rate
	double p_ey = 0.6; // proportional factor on lateral error



	double track_width = 0.5;

	int set_lab = 0;
	int set_flag = 0;

	double dt = 0.1;
	double v0 = 0.5;
	double v_target = 2;


	Map map(track_width);

	
	auto start = chrono::steady_clock::now();

	Simulator simulator(set_lab, set_flag);

	auto end = chrono::steady_clock::now();

	// Store the time difference between start and end
	auto diff = end - start;

	cout << "execution time of simulation  "<< chrono::duration <double, milli>(diff).count() << " ms" << endl;
	



	Closed_Loop_Data closed_loop_data(dt, v0);

	Controller::PID controller(v_target, p_vx, d_psi, p_ey);


	
	simulator.sim(&closed_loop_data, &controller, &map);
	
	/*
	////////////////////////////////////////////////////////////
	/// PLOTTING -> adjust matlab path etc -> see Plot.h
	////////////////////////////////////////////////////////////
	cout << "Plotting..." << endl;
	Plot plot;

	plot.matlab(&closed_loop_data, &map);
	
	cout << "Plotting Done" << endl;
	*/
	
	string y;
	getline(cin, y);

	return 0;
}