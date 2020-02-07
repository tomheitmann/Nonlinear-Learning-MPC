#include <iostream>
//#include <string>

#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

#ifndef PLOT
#define PLOT

class Plot {
public:
	void matlab(Closed_Loop_Data* closed_loop_data, Map* map) {


		// function calls MATLAB engine API
		// pushes cells to MATLAB and calls a function written in MATLAB
		/* how to set up engine API for Visual Studio
		1. Right click on the project name and select PROPERTIES in the solution explorer to open the project properties. Once this window has opened, make the following changes:
		a. Make sure to change the architecture to X64.
		b. Under C/C++ General, add the following directory to the field ADDITIONAL INCLUDE DIRECTORIES:
		C:\Program Files\MATLAB\_<ver>_\extern\include
		c. Under C/C++ Precompiled Headers, select "Not Using Precompiled Headers".
		d. Under Linker General, add the directory to the field ADDITIONAL LIBRARY DIRECTORIES:
		C:\Program Files\MATLAB\_<ver>_\extern\lib\win64\microsoft
		e. Under Configuration Properties ->Debugging
		Add the following Target path to the Environment:
		PATH=C:\Program Files\MATLAB\_<ver>_\extern\bin\win64;
		2. Under Linker Input, add the following names to the field marked ADDITIONAL DEPENDENCIES:
		libMatlabEngine.lib
		libMatlabDataArray.lib
		3. Make sure that the following directory is in the Windows system environment PATH:
		C:\Program Files\MATLAB\_<ver>_\extern\bin\win64;
		To do this, check the Windows system path by clicking on Start -> Right click on Computer -> Properties -> Advanced System Settings -> Environment variables -> System Variables -> Open "Path" for editing.
		Note: If the machine has more than one MATLAB installed, the directory for the target platform must be ahead of any other MATLAB directory (for instance, when compiling a 64-bit application, the directory in the MATLAB 64-bit installation must be the first one on the PATH).
		move it up as much as you can. also all other matlab paths with ending \win64
		*/
		

		// Evaluate functions in MATLAB
		using namespace matlab::engine;

		// Start MATLAB engine synchronously
		std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();

		// Create MATLAB data array factory
		matlab::data::ArrayFactory factory;
	
		// Create a array 
		matlab::data::CellArray x_curv = factory.createArray<matlab::data::Array>({ 1001,6 });
		matlab::data::CellArray x_glob = factory.createArray<matlab::data::Array>({ 1001,6 });
		matlab::data::CellArray u = factory.createArray<matlab::data::Array>({ 1000,2 });
		matlab::data::CellArray track_data = factory.createArray<matlab::data::Array>({ 11,6 });
		matlab::data::CellArray track_lines = factory.createArray<matlab::data::Array>({ 200,6 });
		


		// Assign values to each cell

		for (int i = 0; i < 1001; i++) {
			for (int j = 0; j < 6; j++) {
				x_curv[i][j] = factory.createScalar<double>(closed_loop_data->x_curv[i][j]);
				x_glob[i][j] = factory.createScalar<double>(closed_loop_data->x_glob[i][j]);
			}
		}

		for (int k = 0; k < 1000; k++) {
			for (int l = 0; l < 2; l++) {
				u[k][l] = factory.createScalar<double>(closed_loop_data->u[k][l]);
			}
		}

		for (int k = 0; k < 11; k++) {
			for (int l = 0; l < 6; l++) {
				track_data[k][l] = factory.createScalar<double>(map->track_data[k][l]);
			}
		}

		for (int k = 0; k < 200; k++) {
			for (int l = 0; l < 2; l++) {
								track_lines[k][l] = factory.createScalar<double>(map->inner_line[k][l]);
								track_lines[k][l+2] = factory.createScalar<double>(map->center_line[k][l]);
								track_lines[k][l+4] = factory.createScalar<double>(map->outer_line[k][l]);
			}
		}

		// Put cell array in MATLAB workspace
		matlabPtr->setVariable(u"x_curv", std::move(x_curv));
		matlabPtr->setVariable(u"x_glob", std::move(x_glob));
		matlabPtr->setVariable(u"u", std::move(u));
		matlabPtr->setVariable(u"track_data", std::move(track_data));
		matlabPtr->setVariable(u"track_lines", std::move(track_lines));
		


		// Call function plot1() in MATLAB
		matlabPtr->eval(u"plot1(x_curv, x_glob, u, track_data, track_lines)");

	};


};





#endif // !PLOT