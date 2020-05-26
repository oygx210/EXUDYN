#+++++++++++++++++++++++++++++++++++++++++++
# Mini example for class ObjectMass1D
#+++++++++++++++++++++++++++++++++++++++++++

import sys
sys.path.append('../../bin/WorkingRelease')
sys.path.append('../TestModels')
from modelUnitTests import ExudynTestStructure, exudynTestGlobals
import exudyn as exu
from exudynUtilities import *
from itemInterface import *
import numpy as np

#create an environment for mini example
SC = exu.SystemContainer()
mbs = SC.AddSystem()

oGround=mbs.AddObject(ObjectGround(referencePosition= [0,0,0]))
nGround = mbs.AddNode(NodePointGround(referenceCoordinates=[0,0,0]))

testError=1 #set default error, if failed
exu.Print("start mini example for class ObjectMass1D")
try: #puts example in safe environment
    node = mbs.AddNode(Node1D(referenceCoordinates = [1], 
                              initialCoordinates=[0.5],
                              initialVelocities=[0.5]))
    mass = mbs.AddObject(Mass1D(nodeNumber = node, physicsMass=1))

    #assemble and solve system for default parameters
    mbs.Assemble()
    SC.TimeIntegrationSolve(mbs, 'GeneralizedAlpha', exu.SimulationSettings())

    #check result, get current mass position at local position [0,0,0]
    testError = mbs.GetObjectOutputBody(mass, exu.OutputVariableType.Position, [0,0,0])[0] - 2
    #final x-coordinate of position shall be 2

except BaseException as e:
    exu.Print("An error occured in test example for ObjectMass1D:", e)
else:
    exu.Print("example for ObjectMass1D completed, test error =", testError)

