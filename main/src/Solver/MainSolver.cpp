/** ***********************************************************************************************
* @brief		Implementation for MainSolver: python interface for solver structure classes
* @details		Details:
* 				- base classes for computational solver (as compared to main solver, which interacts with Python, etc.)
*
* @author		Gerstmayr Johannes
* @date			2020-01-05 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: missing
* 				
*
************************************************************************************************ */


#include <pybind11/pybind11.h> //for integrated python connectivity (==>put functionality into separate file ...!!!)
#include <pybind11/eval.h>
#include <pybind11/numpy.h>       //interface to numpy
//
#include "Linalg/BasicLinalg.h"
#include "Linalg/LinearSolver.h"

#include "Linalg/Geometry.h"

#include "Main/MainSystem.h"

#include "Pymodules/PybindUtilities.h"
//#include "Autogenerated/MainObjectMassPoint.h"
//#include "Autogenerated/MainNodePoint.h"
//
#include "Solver/TimeIntegrationSolver.h"
#include "Autogenerated/SystemContainer.h"

#include "Main/MainSystemContainer.h"

#include "Solver/CSolverImplicitSecondOrder.h"
#include "Solver/CSolverStatic.h"

#include "Solver/MainSolverBase.h" //pybind access to solvers
#include "Autogenerated/MainSolver.h" //pybind access to solvers
//#include "Autogenerated/SimulationSettings.h" 

namespace py = pybind11;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//MainSolverImplicitSecondOrder specific functions
//! constructor, in order to set valid state (settings not initialized at beginning); set userFunctions to zero
MainSolverImplicitSecondOrder::MainSolverImplicitSecondOrder() 
{
	isInitialized = false;

}

//! get locally stored / last computed system residual
py::array_t<Real> MainSolverImplicitSecondOrder::GetAAlgorithmic()
{
	return HPyUtils::PyVector(cSolver.data.aAlgorithmic);
}

//! get locally stored / last computed algorithmic accelerations at start of step
py::array_t<Real> MainSolverImplicitSecondOrder::GetStartOfStepStateAAlgorithmic()
{
	return HPyUtils::PyVector(cSolver.data.startOfStepStateAAlgorithmic);
}

//OLD:
////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////overriding with USERFUNCTIONs
////! finish static step / time step; write output of results to file
//void MainSolverImplicitSecondOrder::FinishStep(MainSystem& mainSystem, const SimulationSettings& simulationSettings)
//{
//	if (userFunctionFinishStep == 0) { MainSolverBase::FinishStep(mainSystem, simulationSettings); }
//	else { userFunctionFinishStep(*this, mainSystem, simulationSettings); }
//}
