#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Rigid pendulum with tip mass
#           The functionality 'timeIntegration.preStepPyExecute' is used to deactivate a constraint after a given time
#           Shows how to load solution to animate an existing solution
#
# Author:   Johannes Gerstmayr
# Date:     2019-11-22
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
import sys
sys.path.append('../../bin/WorkingRelease') #for exudyn, itemInterface and exudynUtilities
#sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test

import exudyn as exu
from exudynUtilities import *
from itemInterface import *

SC = exu.SystemContainer()
mbs = SC.AddSystem()

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#rigid pendulum with initial velocities
#constraints and joints are deactivated during simulation

rect = [-2.5,-1.5,0.5,1.5] #xmin,ymin,xmax,ymax
background = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[rect[0],rect[1],0, rect[2],rect[1],0, rect[2],rect[3],0, rect[0],rect[3],0, rect[0],rect[1],0]} #background
oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0], visualization=VObjectGround(graphicsData= [background])))
#nGround=mbs.AddNode(NodePointGround(referencePosition= [0,0,0]))
a = 0.5     #x-dim of pendulum
b = 0.05    #y-dim of pendulum
massRigid = 12
mass = 2 #of additional mass
inertiaRigid = massRigid/12*(2*a)**2

omega0 = 4

graphics2 = {'type':'Line', 'color':[0.1,0.1,0.8,1], 'data':[-a,-b,0, a,-b,0, a,b,0, -a,b,0, -a,-b,0]} #background
nRigid = mbs.AddNode(Rigid2D(referenceCoordinates=[-0.5,0,0], initialVelocities=[0,omega0*a,omega0]));
oRigid = mbs.AddObject(RigidBody2D(physicsMass=massRigid, physicsInertia=inertiaRigid,nodeNumber=nRigid,visualization=VObjectRigidBody2D(graphicsData= [graphics2])))

mR1 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oRigid, localPosition=[-0.5,0.,0.])) #support point
mG0 = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oGround, localPosition=[-0.5-a,0.,0.]))
oRJoint = mbs.AddObject(RevoluteJoint2D(markerNumbers=[mG0,mR1],activeConnector=True))

#mass point is attached with coordinate constraints:
mCoordR0 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber=nRigid, coordinate=0)) 
mCoordR1 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber=nRigid, coordinate=1)) 

#additional mass point attached to COM of rigid body:
nMass = mbs.AddNode(Point2D(referenceCoordinates=[-0.5,0], initialVelocities=[0,omega0*a]));
oMass = mbs.AddObject(MassPoint2D(physicsMass=mass, nodeNumber=nMass) )

mCoordM0 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber=nMass, coordinate=0)) 
mCoordM1 = mbs.AddMarker(MarkerNodeCoordinate(nodeNumber=nMass, coordinate=1)) 

oConstraint0 = mbs.AddObject(CoordinateConstraint(markerNumbers=[mCoordM0, mCoordR0],activeConnector=True))
oConstraint1 = mbs.AddObject(CoordinateConstraint(markerNumbers=[mCoordM1, mCoordR1],activeConnector=True))

mbs.Assemble()

simulationSettings = exu.SimulationSettings() #takes currently set values or default values
simulationSettings.timeIntegration.numberOfSteps = 4000
simulationSettings.timeIntegration.endTime = 2
simulationSettings.timeIntegration.newton.relativeTolerance = 1e-8
simulationSettings.timeIntegration.newton.absoluteTolerance = 1e-6
simulationSettings.timeIntegration.verboseMode = 1

#execute this python code in the same scope as this file at beginning of every time step:
s = "if mbs.systemData.GetCurrentTime() > 0.3: mbs.SetObjectParameter(" + str(oRJoint) + ", 'activeConnector', False)\n" + "if mbs.systemData.GetCurrentTime() > 0.1: mbs.SetObjectParameter(" + str(oConstraint0) + ", 'activeConnector', False);mbs.SetObjectParameter(" + str(oConstraint1) + ", 'activeConnector', False)\n"
simulationSettings.timeIntegration.preStepPyExecute = s

simulationSettings.timeIntegration.newton.useModifiedNewton = False
simulationSettings.timeIntegration.newton.numericalDifferentiation.minimumCoordinateSize = 1
simulationSettings.timeIntegration.newton.useNumericalDifferentiation = False
#simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.6
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = True
simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = True
simulationSettings.solutionSettings.solutionInformation = "Rigid pendulum with switching constraints"
simulationSettings.displayStatistics = True

SC.visualizationSettings.openGL.multiSampling = 1

exu.StartRenderer()

SC.TimeIntegrationSolve(mbs, 'GeneralizedAlpha', simulationSettings)
print('end time =',mbs.systemData.GetCurrentTime()) #time after time integration ...
#print('solution =',mbs.systemData.GetODE2Coordinates()) #solution coordinates after time integration ...

mbs.WaitForUserToContinue()

animate = True
if animate:
    fileName = 'coordinatesSolution.txt'
    solution = LoadSolutionFile('coordinatesSolution.txt')
    AnimateSolution(exu, SC, mbs, solution, 1, 0.05)


#SC.WaitForRenderEngineStopFlag()
exu.StopRenderer() #safely close rendering window!

