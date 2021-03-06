#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  The solver module provides solver functionality
#           Most of the solvers are implemented inside the C++ core
#
# Author:   Johannes Gerstmayr 
# Date:     2020-12-02
# Notes:    This module is still under construction and for testing purposes only!
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++import sys

#import is necessary, otherwise the solvers cannot be called
import exudyn

#**function: solves the static mbs problem using simulationSettings; check theDoc.pdf for MainSolverStatic for further details of the static solver
#**input:
#   mbs: the MainSystem containing the assembled system; note that mbs may be changed upon several runs of this function
#   simulationSettings: specific simulation settings used for computation of jacobian (e.g., sparse mode in static solver enables sparse computation)
#   updateInitialValues: if True, the results are written to initial values, such at a consecutive simulation uses the results of this simulation as the initial values of the next simulation
#   storeSolver: if True, the staticSolver object is stored in the mbs.sys dictionary as mbs.sys['staticSolver'] 
#**output: returns True, if successful, False if fails; if storeSolver = True, mbs.sys contains staticSolver, which allows to investigate solver problems (check theDoc.pdf section \refSection{sec:solverSubstructures} and the items described in \refSection{sec:MainSolverStatic})
#**example:
# import exudyn as exu
# from exudyn.itemInterface import *
# SC = exu.SystemContainer()
# mbs = SC.AddSystem()
# #create simple system:
# ground = mbs.AddObject(ObjectGround())
# mbs.AddNode(NodePoint())
# body = mbs.AddObject(MassPoint(physicsMass=1, nodeNumber=0))
# m0 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=ground))
# m1 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=body))
# mbs.AddObject(CartesianSpringDamper(markerNumbers=[m0,m1], stiffness=[100,100,100]))
# mbs.AddLoad(LoadForceVector(markerNumber=m1, loadVector=[10,10,10]))
# mbs.Assemble()
# sims = exu.SimulationSettings()
# sims.timeIntegration.endTime = 10
# success = exu.SolveStatic(mbs, sims, storeSolver = True)
# print("success =", success)
# print("iterations = ", mbs.sys['staticSolver'].it)
# print("pos=", mbs.GetObjectOutputBody(body,localPosition=[0,0,0], 
#       variableType=exu.OutputVariableType.Position))
def SolveStatic(mbs, simulationSettings = exudyn.SimulationSettings(), 
                updateInitialValues = False,
                storeSolver = True):
    staticSolver = exudyn.MainSolverStatic()
    
    success = staticSolver.SolveSystem(mbs, simulationSettings)
    
    if updateInitialValues:
        currentState = mbs.systemData.GetSystemState() #get current values
        mbs.systemData.SetSystemState(sysStateList=currentState, configuration = exudyn.ConfigurationType.Initial)

    if storeSolver:
        mbs.sys['staticSolver'] = staticSolver #copy solver structure to sys variable

    return success

#**function: solves the dynamic mbs problem using simulationSettings and solver type; check theDoc.pdf for MainSolverImplicitSecondOrder for further details of the dynamic solver
#**input:
#   mbs: the MainSystem containing the assembled system; note that mbs may be changed upon several runs of this function
#   simulationSettings: specific simulation settings
#   solverType: use exudyn.DynamicSolverType to set specific solver (default=generalized alpha)
#   updateInitialValues: if True, the results are written to initial values, such at a consecutive simulation uses the results of this simulation as the initial values of the next simulation
#   storeSolver: if True, the staticSolver object is stored in the mbs.sys dictionary as mbs.sys['staticSolver'] 
#**output: returns True, if successful, False if fails; if storeSolver = True, mbs.sys contains staticSolver, which allows to investigate solver problems (check theDoc.pdf section \refSection{sec:solverSubstructures} and the items described in \refSection{sec:MainSolverStatic})
#**example:
# import exudyn as exu
# from exudyn.itemInterface import *
# SC = exu.SystemContainer()
# mbs = SC.AddSystem()
# #create simple system:
# ground = mbs.AddObject(ObjectGround())
# mbs.AddNode(NodePoint())
# body = mbs.AddObject(MassPoint(physicsMass=1, nodeNumber=0))
# m0 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=ground))
# m1 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=body))
# mbs.AddObject(CartesianSpringDamper(markerNumbers=[m0,m1], stiffness=[100,100,100]))
# mbs.AddLoad(LoadForceVector(markerNumber=m1, loadVector=[10,10,10]))
# mbs.Assemble()
# sims = exu.SimulationSettings()
# sims.timeIntegration.endTime = 10
# success = exu.SolveDynamic(mbs, sims, storeSolver = True)
# print("success =", success)
# print("iterations = ", mbs.sys['dynamicSolver'].it)
# print("pos=", mbs.GetObjectOutputBody(body,localPosition=[0,0,0], 
#       variableType=exu.OutputVariableType.Position))
def SolveDynamic(mbs,
                simulationSettings = exudyn.SimulationSettings(), 
                solverType = exudyn.DynamicSolverType.GeneralizedAlpha,
                updateInitialValues = False,
                storeSolver = True):

    if (solverType != exudyn.DynamicSolverType.TrapezoidalIndex2 and 
        solverType != exudyn.DynamicSolverType.GeneralizedAlpha):
        raise ValueError("SolveDynamic: solver type not implemented: ", solverType)
    
    dynamicSolver = exudyn.MainSolverImplicitSecondOrder()
    
    #store old settings:
    newmarkOld = simulationSettings.timeIntegration.generalizedAlpha.useNewmark
    index2Old = simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints

    if solverType == exudyn.DynamicSolverType.TrapezoidalIndex2:
        #manually override settings for integrator
        simulationSettings.timeIntegration.generalizedAlpha.useNewmark = True
        simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = True
    
    success = dynamicSolver.SolveSystem(mbs, simulationSettings)

    #restore old settings:
    simulationSettings.timeIntegration.generalizedAlpha.useNewmark = newmarkOld
    simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = index2Old
    
    if updateInitialValues:
        currentState = mbs.systemData.GetSystemState() #get current values
        mbs.systemData.SetSystemState(sysStateList=currentState, configuration = exudyn.ConfigurationType.Initial)

    if storeSolver:
        mbs.sys['dynamicSolver'] = dynamicSolver #copy solver structure to sys variable

    return success

#**function: compute eigenvalues for unconstrained ODE2 part of mbs, not considering the effects of algebraic constraints; the computation is done for the initial values of the mbs, independently of previous computations. If you would like to use the current state for the eigenvalue computation, you need to copy the current state to the initial state (using GetSystemState,SetSystemState, see \refSection{sec:mbs:systemData}).
#**input:    
#   mbs: the MainSystem containing the assembled system
#   simulationSettings: specific simulation settings used for computation of jacobian (e.g., sparse mode in static solver enables sparse computation)
#   useSparseSolver: if False (only for small systems), all eigenvalues are computed in dense mode (slow for large systems!); if True, only the numberOfEigenvalues are computed (numberOfEigenvalues must be set!); Currently, the matrices are exported only in DENSE MODE from mbs! NOTE that the sparsesolver accuracy is much less than the dense solver
#   numberOfEigenvalues: number of eigenvalues and eivenvectors to be computed
#   convert2Frequencies: if True, the eigen values are converted into frequencies (Hz) and the output is [eigenFrequencies, eigenVectors]
#**output: [eigenValues, eigenVectors]; eigenValues being a numpy array of eigen values ($\omega_i^2$, being the squared eigen frequencies in ($\omega_i$ in rad/s)!), eigenVectors a numpy array containing the eigenvectors in every column
#**example:
#  #take any example from the Examples or TestModels folder, e.g., 'cartesianSpringDamper.py' and run it
#  #then execute the following commands in the console (or add it to the file):
#  [values, vectors] = exu.ComputeODE2Eigenvalues(mbs)
#  print("eigenvalues=", values)
#  #==>values contains the eigenvalues of the ODE2 part of the system in the current configuration
def ComputeODE2Eigenvalues(mbs, 
                           simulationSettings = exudyn.SimulationSettings(),
                           useSparseSolver = False, 
                           numberOfEigenvalues = -1,
                           setInitialValues = True,
                           convert2Frequencies = False):
    import numpy as np
    #use static solver, as it does not include factors from time integration (and no velocity derivatives) in the jacobian
    staticSolver = exudyn.MainSolverStatic()

    #initialize solver with initial values
    staticSolver.InitializeSolver(mbs, simulationSettings)

    staticSolver.ComputeMassMatrix(mbs)
    M = staticSolver.GetSystemMassMatrix()

    staticSolver.ComputeJacobianODE2RHS(mbs)    #compute ODE2 part of jacobian ==> stored internally in solver
    staticSolver.ComputeJacobianAE(mbs)         #compute algebraic part of jacobian (not needed here...)
    jacobian = staticSolver.GetSystemJacobian() #read out stored jacobian

    nODE2 = staticSolver.GetODE2size()

    #obtain ODE2 part from jacobian == stiffness matrix
    K = jacobian[0:nODE2,0:nODE2]

    if not useSparseSolver:
        from scipy.linalg import eigh  #eigh for symmetric matrices, positive definite; eig for standard eigen value problems
        [eigenValues, eigenVectors] = eigh(K, M) #this gives omega^2 ... squared eigen frequencies (rad/s)
    else:
        if numberOfEigenvalues < 1:
            raise ValueError("ComputeODE2Eigenvalues: numberOfEigenvalues must be > 0")

        from scipy.sparse.linalg import eigsh, csr_matrix #eigh for symmetric matrices, positive definite

        Kcsr = csr_matrix(K)
        Mcsr = csr_matrix(M)

        #use "LM" (largest magnitude), but shift-inverted mode with sigma=0, to find the zero-eigenvalues:
        #see https://docs.scipy.org/doc/scipy/reference/tutorial/arpack.html
        [eigenValues, eigenVectors] = eigsh(A=Kcsr, k=numberOfEigenvalues, M=Mcsr, 
                                   which='LM', sigma=0, mode='normal') 

    #sort eigenvalues
    eigenValues = np.sort(a=abs(eigenValues))

    if convert2Frequencies:
        eigenFrequencies = np.sqrt(eigenValues)/(2*np.pi)
        return [eigenFrequencies, eigenVectors]
    else:
        return [eigenValues, eigenVectors]
    

