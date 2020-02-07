#include <iostream>
#include <math.h>      
#include "SysModel.h"
#include "Track.h"



 void DynModel(double*x_global_next, double*x_curv_next, double x[6], double x_glob[6], double u[2], double dt, Map * map) {
	 // x_global_next: pointer to first entry of state vector in global reference frame
	 // x_curv_next : pointer to first entry of state vector in local curvilinear reference frame
	 


	// vehicle parameters  
	 double m = 1.98;
	 double lf = 0.125;
	 double lr = 0.125;
	 double Iz = 0.024;
	 double Df = 0.8 * m * 9.81 / 2.0;
	 double Cf = 1.25;
	 double Bf = 1.0;
	 double Dr = 0.8 * m * 9.81 / 2.0;
	 double Cr = 1.25;
	 double Br = 1.0;

	// Discretization Parameters
	 double deltaT = 0.001;
	/*double x_next[6]{};
	double cur_x_next[6]{};*/

	// Extract the values of the states
	 double delta = u[0];
	 double a = u[1];

	 //std::cout << delta <<  " " << a << std::endl;

	 double psi = x_glob[3];
	 double X = x_glob[4];
	 double Y = x_glob[5];

	 double vx = x[0];
	 double vy = x[1];
	 double wz = x[2];
	 double epsi = x[3];
	 double s = x[4];
	 double ey = x[5];

	// initialize counter 
	long long int i = 0;

	while ((i + 1.0) * deltaT <= dt) {
	// Compute tire split angle
		double alpha_f = delta - atan2(vy + lf * wz, vx);
		double alpha_r = -atan2(vy - lf * wz, vx);

	// Compute lateral force at front and rear tire
		double Fyf = 2 * Df * sin(Cf * atan(Bf * alpha_f));
		double Fyr = 2 * Dr * sin(Cr * atan(Br * alpha_r));

	// Propagate the dynamics of deltaT
	*x_global_next = vx + deltaT * (a - 1 / m * Fyf * sin(delta) + wz * vy);
	*(x_global_next + 1) = vy + deltaT * (1 / m * (Fyf * cos(delta) + Fyr) - wz * vx);
	*(x_global_next + 2) = wz + deltaT * (1 / Iz * (lf * Fyf * cos(delta) - lr * Fyr));
	*(x_global_next + 3) = psi + deltaT * (wz);
	*(x_global_next + 4) = X + deltaT * (vx * cos(psi) - vy * sin(psi));
	*(x_global_next + 5) = Y + deltaT * (vx * sin(psi) + vy * cos(psi));


	//cur = Curvature(s, PointAndTangent)
	//double cur = 2; // dummy value
	double cur = map->curvature(s);
	//std::cout << cur << " " << s << std::endl;

	/**x_curv_next = *x_global_next;
	*(x_curv_next + 1) = *(x_global_next + 1);
	*(x_curv_next + 2) = *(x_global_next + 2);
	*(x_curv_next + 3) = epsi + deltaT * (wz - (vx * cos(epsi) - vy * sin(epsi)) / (1 - cur * ey) * cur);
	*(x_curv_next + 4) = s + deltaT * ((vx * cos(epsi) - vy * sin(epsi)) / (1 - cur * ey));
	*(x_curv_next + 5) = ey + deltaT * (vx * sin(epsi) + vy * cos(epsi));*/

	// same but explicit
	
	*x_curv_next = vx + deltaT * (a - 1 / m * Fyf * sin(delta) + wz * vy);
	*(x_curv_next + 1) = vy + deltaT * (1 / m * (Fyf * cos(delta) + Fyr) - wz * vx);
	*(x_curv_next + 2) = wz + deltaT * (1 / Iz * (lf * Fyf * cos(delta) - lr * Fyr));
	*(x_curv_next + 3) = epsi + deltaT * (wz - (vx * cos(epsi) - vy * sin(epsi)) / (1 - cur * ey) * cur);
	*(x_curv_next + 4) = s + deltaT * ((vx * cos(epsi) - vy * sin(epsi)) / (1 - cur * ey));
	*(x_curv_next + 5) = ey + deltaT * (vx * sin(epsi) + vy * cos(epsi)); 

	// Update the value of the states
	psi = *(x_global_next+3);
	X = *(x_global_next + 4);
	Y = *(x_global_next + 5);

	vx = *x_curv_next;
	vy = *(x_curv_next + 1);
	wz = *(x_curv_next + 2);
	epsi = *(x_curv_next + 3);
	s = *(x_curv_next + 4);
	ey = *(x_curv_next + 5);
	
	// increment counter
	i++;
	}


}
