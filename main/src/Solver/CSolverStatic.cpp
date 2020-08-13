/** ***********************************************************************************************
* @brief		Implentation for CSolverStatic
*
* @author		Gerstmayr Johannes
* @date			2019-12-16 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: missing
* 				
*
************************************************************************************************ */

#define useCSolverStatic
#ifdef useCSolverStatic


#include <pybind11/pybind11.h> //for integrated python connectivity (==>put functionality into separate file ...!!!)
#include <pybind11/eval.h>
#include <fstream>

#include "Linalg/BasicLinalg.h" //for Resizable Vector
#include "Main/CSystem.h"
#include "Autogenerated/CMarkerBodyPosition.h"
//#include "Solver/CSolverBase.h" 
#include "Solver/CSolverStatic.h" 


namespace py = pybind11;	//for py::object


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++   STATIC SOLVER   ++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//! reduce step size (1..normal, 2..severe problems); return true, if reduction was successful
bool CSolverStatic::ReduceStepSize(CSystem& computationalSystem, const SimulationSettings& simulationSettings, Index severity)
{
	//it.currentTime is the only important value to be updated in order to reset the step time:
	it.currentTime = computationalSystem.GetSystemData().GetCData().currentState.time;

	if (it.currentStepSize > it.minStepSize)
	{
		it.currentStepSize *= 0.25;

		it.currentStepSize = EXUstd::Maximum(it.minStepSize, it.currentStepSize);
		return true;
	}

	return false;
}
//! initialize static step / time step: do some outputs, checks, etc.
void CSolverStatic::UpdateCurrentTime(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	const StaticSolverSettings& staticSettings = simulationSettings.staticSolver;

	const Real loadFactorEps = 1e-10;				//small number to avoid effects of round off errors in it.currentTime
	Real loadTime = (it.currentTime - it.startTime);	//usually runs from 0 to 1, but may be scaled for quasi-static computations
	if (!staticSettings.loadStepGeometric) //arithmetric
	{
		//loadTime += 1. / it.numberOfSteps;
		loadTime += it.currentStepSize;
	}
	else //geometric
	{
		if (loadTime - loadFactorEps * staticSettings.loadStepDuration <= 0)
		{ 
			//initial value for geometric load step factor
			//it.currentTime = loadStepGeometricFactor / staticSettings.loadStepGeometricRange; //old solver;
			loadTime = staticSettings.loadStepDuration / staticSettings.loadStepGeometricRange; //new range; starts correctly
		}
		else
		{
			loadTime *= loadStepGeometricFactor;
		}
		it.currentStepSize = 0; //for safety
	}
	if (loadTime + loadFactorEps * staticSettings.loadStepDuration >= staticSettings.loadStepDuration) { loadTime = staticSettings.loadStepDuration; } //it.currentStepSize not updated!

	it.currentTime = loadTime + it.startTime;
	computationalSystem.GetSystemData().GetCData().currentState.time = it.currentTime; //update time in state --> also available in Python for user-defined functions

	if (simulationSettings.staticSolver.useLoadFactor)
	{
		computationalSystem.GetSolverData().loadFactor = ComputeLoadFactor(simulationSettings); //loadTime / loadStepDuration; //scale ALL loads e.g. in ODERHS(...) with loadFactor
	}
	else
	{
		computationalSystem.GetSolverData().loadFactor = 1.;
	}

	//CSolverBase::InitializeStep(computationalSystem, simulationSettings);
}

////! initialize things at the very beginning of initialize
//void CSolverStatic::PreInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
//{
//
//}

//! initialize things at the very beginning of initialize
void CSolverStatic::PostInitializeSolverSpecific(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	const StaticSolverSettings& staticSettings = simulationSettings.staticSolver;

	//now as numberOfSteps and loadFactor has been reset, define the initial loadFactor
	if (staticSettings.loadStepGeometric && it.numberOfSteps <= 1) 
	{ 
		it.numberOfSteps = 2; 
		PyWarning("loadStepGeometric must have at least 2 load steps; performing 2 load steps", file.solverFile); 
	}
	loadStepGeometricFactor = pow(staticSettings.loadStepGeometricRange, 1. / (it.numberOfSteps - 1)); //multiplicative load step factor

	//loadStepGeometricFactor = pow(staticSettings.loadStepGeometricRange, 1. / it.numberOfSteps); //old solver multiplicative load step factor
	//set already in InitializeSolver(...); it.currentTime = 0;  //1. / it.numberOfSteps; //this is the initial load step factor for writing the initial values; range [0,1], which is increased at end of step; used to scale loads arithmetically/geometrically

	if (IsVerbose(2))
	{
		STDstring str = "";
		if (staticSettings.loadStepGeometric) { str = " geometric"; }
		Verbose(2, STDstring("*********************\nStatic solver (") + EXUstd::ToString(staticSettings.numberOfLoadSteps) + str + " load steps):\n*********************\n");
	}
}


//! compute residual for Newton method (e.g. static or time step)
void CSolverStatic::ComputeNewtonResidual(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	const StaticSolverSettings& staticSettings = simulationSettings.staticSolver;

	LinkedDataVector ode2Residual(data.systemResidual, 0, data.nODE2); //link ODE2 coordinates
	LinkedDataVector aeResidual(data.systemResidual, data.startAE, data.nAE); //link ae coordinates
	//LinkedDataVector newtonSolutionODE2(data.newtonSolution, 0, data.nODE2); //temporary subvector for ODE2 components
	//LinkedDataVector newtonSolutionAE(data.newtonSolution, data.startAE, data.nAE); //temporary subvector for ODE2 components

	//link current system vectors for ODE2
	Vector& solutionODE2 = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords;
	Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;

	//now compute the new residual with updated system vectors:
	STARTTIMER(timer.ODE2RHS);
	computationalSystem.ComputeODE2RHS(data.tempCompData, ode2Residual); //tempODE2 contains RHS (linear case: tempODE2 = F_applied - K*u - D*v)
	STOPTIMER(timer.ODE2RHS);

	STARTTIMER(timer.AERHS);
	computationalSystem.ComputeAlgebraicEquations(data.tempCompData, aeResidual, false);
	STOPTIMER(timer.AERHS);

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if (staticSettings.stabilizerODE2term != 0.)
	{
		timer.massMatrix -= EXUstd::GetTimeInSeconds();

		Real currentODE2stabilizer = (1. - pow(ComputeLoadFactor(simulationSettings), 2))*staticSettings.stabilizerODE2term;

		STARTTIMER(timer.massMatrix);
		data.systemMassMatrix->SetNumberOfRowsAndColumns(data.nODE2, data.nODE2);
		data.systemMassMatrix->SetAllZero();
		computationalSystem.ComputeMassMatrix(data.tempCompData, *(data.systemMassMatrix));
		STOPTIMER(timer.massMatrix);

		//this could be optimized, avoiding new during static computation:
		data.tempODE2.CopyFrom(solutionODE2);
		LinkedDataVector x(computationalSystem.GetSystemData().GetCData().startOfStepState.ODE2Coords);

		for (Index i = 0; i < data.nODE2; i++)
		{
			data.tempODE2[i] -= x[i];
		}
		data.tempODE2 *= -currentODE2stabilizer;

		data.systemMassMatrix->MultMatrixVector(data.tempODE2, data.temp2ODE2);
		for (Index i = 0; i < data.nODE2; i++)
		{
			ode2Residual[i] += data.temp2ODE2[i];
		}

		timer.massMatrix += EXUstd::GetTimeInSeconds();
	}

	//compute CqT*lambda:
	timer.reactionForces -= EXUstd::GetTimeInSeconds();
	computationalSystem.ComputeODE2ProjectedReactionForces(data.tempCompData, solutionAE, ode2Residual); //add the forces directly!
	timer.reactionForces += EXUstd::GetTimeInSeconds();

}

void CSolverStatic::ComputeNewtonUpdate(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	Vector& solutionODE2 = computationalSystem.GetSystemData().GetCData().currentState.ODE2Coords;
	Vector& solutionAE = computationalSystem.GetSystemData().GetCData().currentState.AECoords;
	LinkedDataVector newtonSolutionODE2(data.newtonSolution, 0, data.nODE2); //temporary subvector for ODE2 components
	LinkedDataVector newtonSolutionAE(data.newtonSolution, data.startAE, data.nAE); //temporary subvector for ODE2 components

	solutionODE2 -= newtonSolutionODE2;		//compute new displacements; newtonSolution contains the Newton correction
	solutionAE -= newtonSolutionAE;			//compute new Lagrange multipliers; newtonSolution contains the Newton correction
}


//! compute jacobian for newton method of given solver method
void CSolverStatic::ComputeNewtonJacobian(CSystem& computationalSystem, const SimulationSettings& simulationSettings)
{
	STARTTIMER(timer.totalJacobian);
	data.systemJacobian->SetAllZero(); //entries are not set to zero inside jacobian computation!

	//+++++++++++++++++++++++++++++
	//Tangent stiffness
	//compute jacobian (w.r.t. U ==> also add V); jacobianAE used as temporary matrix
	STARTTIMER(timer.jacobianODE2);
	computationalSystem.NumericalJacobianODE2RHS(data.tempCompData, newton.numericalDifferentiation, data.tempODE2F0, data.tempODE2F1, *(data.systemJacobian));
	STOPTIMER(timer.jacobianODE2);

	STARTTIMER(timer.jacobianAE);
	//add jacobian algebraic equations part to system jacobian:
	computationalSystem.JacobianAE(data.tempCompData, newton, *(data.systemJacobian), 1., 1., false, true);
	STOPTIMER(timer.jacobianAE);

	//pout << "stabilizerODE2term=" << simulationSettings.staticSolver.stabilizerODE2term << "\n";
	if (simulationSettings.staticSolver.stabilizerODE2term != 0.)
	{
		Real currentODE2stabilizer = (1. - pow(ComputeLoadFactor(simulationSettings), 2))*simulationSettings.staticSolver.stabilizerODE2term;
		//TemporaryComputationData tempCompData;

		STARTTIMER(timer.massMatrix);
		data.systemMassMatrix->SetNumberOfRowsAndColumns(data.nODE2, data.nODE2);
		data.systemMassMatrix->SetAllZero();
		computationalSystem.ComputeMassMatrix(data.tempCompData, *(data.systemMassMatrix));
		data.systemMassMatrix->MultiplyWithFactor(-currentODE2stabilizer);

		data.systemJacobian->AddSubmatrix(*(data.systemMassMatrix));
		STOPTIMER(timer.massMatrix);
	}

	computationalSystem.GetSolverData().signalJacobianUpdate = false; //as jacobian has been computed, no further update is necessary

	if (IsVerbose(3)) { Verbose(3, "    update Jacobian: Jac    = " + EXUstd::ToString(*(data.systemJacobian)) + "\n"); }
	//done in Newton: else if (IsVerbose(2)) { Verbose(2,"    update Jacobian\n"); }

	STOPTIMER(timer.totalJacobian);

}










#endif
