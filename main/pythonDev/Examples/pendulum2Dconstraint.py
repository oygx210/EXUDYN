#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# This is an EXUDYN example
#
# Details:  Mathematical pendulum with constraint;
#           Remark: update from pendulum.py example
#
# Author:   Johannes Gerstmayr
# Date:     2019-12-26
#
# Copyright:This file is part of Exudyn. Exudyn is free software. You can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
#
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
import sys
sys.path.append('../../bin/WorkingRelease') #for exudyn, itemInterface and exudynUtilities
#sys.path.append('../TestModels')            #for modelUnitTest as this example may be used also as a unit test

#import time #for sleep()
import exudyn as exu
from exudynUtilities import*

SC = exu.SystemContainer()
mbs = SC.AddSystem()

L = 0.8 #distance
mass = 2.5
g = 9.81

r = 0.05 #just for graphics
graphicsBackground = GraphicsDataRectangle(-1.2*L,-1.2*L, 1.2*L, 0.2*L, [1,1,1,1]) #for appropriate zoom
graphicsSphere = GraphicsDataSphere(point=[0,0,0], radius=r, color=[1.,0.2,0.2,1], nTiles = 16)

#add ground object and mass point:
oGround = mbs.AddObject(ObjectGround(referencePosition = [0,0,0], 
                           visualization = VObjectGround(graphicsData = [graphicsBackground])))
nMass = mbs.AddNode(NodePoint2D(referenceCoordinates=[L,0], 
                                initialDisplacements=[0,0],
                                initialVelocities=[0,0]))
oMass = mbs.AddObject(MassPoint2D(physicsMass = mass, nodeNumber = nMass, 
                                  visualization = VObjectMassPoint2D(graphicsData = [graphicsSphere])))

mMass = mbs.AddMarker(MarkerNodePosition(nodeNumber=nMass))
mGround = mbs.AddMarker(MarkerBodyPosition(bodyNumber=oGround, localPosition = [0,0,0]))
oDistance = mbs.AddObject(DistanceConstraint(markerNumbers = [mGround, mMass], distance = L))

#add loads:
mbs.AddLoad(Force(markerNumber = mMass, loadVector = [0, -mass*g, 0])) 

print(mbs)

mbs.Assemble()

simulationSettings = exu.SimulationSettings()

f = 1000000
simulationSettings.timeIntegration.numberOfSteps = int(1*f)
simulationSettings.timeIntegration.endTime = 0.001*f
simulationSettings.solutionSettings.solutionWritePeriod = simulationSettings.timeIntegration.endTime/5000
#simulationSettings.displayComputationTime = True
simulationSettings.timeIntegration.verboseMode = 1
simulationSettings.timeIntegration.verboseModeFile = 0

#simulationSettings.timeIntegration.newton.useModifiedNewton = False
simulationSettings.timeIntegration.generalizedAlpha.useNewmark = False
simulationSettings.timeIntegration.generalizedAlpha.useIndex2Constraints = simulationSettings.timeIntegration.generalizedAlpha.useNewmark
simulationSettings.timeIntegration.generalizedAlpha.spectralRadius = 0.60 #0.62 is approx. the limit
simulationSettings.timeIntegration.adaptiveStep = False

simulationSettings.timeIntegration.generalizedAlpha.computeInitialAccelerations = True
simulationSettings.solutionSettings.coordinatesSolutionFileName= "coordinatesSolution.txt"

simulationSettings.displayStatistics = True
#simulationSettings.solutionSettings.recordImagesInterval = 0.04

SC.visualizationSettings.nodes.defaultSize = 0.05
exu.StartRenderer()

#mbs.WaitForUserToContinue()
#exu.InfoStat()
SC.TimeIntegrationSolve(mbs, 'GeneralizedAlpha', simulationSettings)
#exu.InfoStat()

SC.WaitForRenderEngineStopFlag()
exu.StopRenderer() #safely close rendering window!

nODE2 = len(mbs.systemData.GetODE2Coordinates())
print("ODE2=",nODE2)

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

data = np.loadtxt('coordinatesSolution.txt', comments='#', delimiter=',')
plt.plot(data[:,0], data[:,1+2*nODE2+1], 'b-')

ax=plt.gca() # get current axes
ax.grid(True, 'major', 'both')
ax.xaxis.set_major_locator(ticker.MaxNLocator(10)) 
ax.yaxis.set_major_locator(ticker.MaxNLocator(10)) 
plt.tight_layout()
plt.show() 


