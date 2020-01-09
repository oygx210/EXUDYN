/** ***********************************************************************************************
* @brief        CObjectJointALEMoving2D implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-17 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CNodeGenericData.h"
//#include "Autogenerated/CObjectJointALEMoving2D.h"

//for consistency checks:
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectJointALEMoving2D.h"
#include "Autogenerated/CObjectANCFCable2D.h" //for access to static functions of cable element

bool MainObjectJointALEMoving2D::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectJointALEMoving2D* cObject = (CObjectJointALEMoving2D*)GetCObject();
	Index dataNodeNumber = cObject->GetNodeNumber(0);
	Index genericODE2NodeNumber = cObject->GetNodeNumber(1);

	const MainNode& dataNode = mainSystem.GetMainSystemData().GetMainNode(dataNodeNumber);
	SignedIndex initialMarker = (SignedIndex)dataNode.GetInitialVector()[0]; //length of initial vector already checked in Node-consistency checks

	Index slidingMarkerSize = cObject->GetParameters().slidingMarkerNumbers.NumberOfItems();
	Index slidingMarkerOffsetSize = cObject->GetParameters().slidingMarkerOffsets.NumberOfItems();

	if (slidingMarkerSize != slidingMarkerOffsetSize)
	{
		errorString = "ObjectJointALEMoving2D: the slidingMarkerNumbers list (size=" + EXUstd::ToString(slidingMarkerSize) + ") must have same size as the slidingMarkerOffsets list (size=" + EXUstd::ToString(slidingMarkerOffsetSize) + ")";
		return false;
	}

	if (initialMarker < 0 || (Index)initialMarker >= slidingMarkerSize)
	{
		errorString = "ObjectJointALEMoving2D: initial Data variable must be >= 0 and < " + EXUstd::ToString(slidingMarkerSize);
		return false;
	}

	//check for valid node number already done prior to this function
	if (std::strcmp(mainSystem.GetMainSystemData().GetMainNode(dataNodeNumber).GetTypeName(), "GenericData") != 0)
	{
		errorString = "ObjectJointALEMoving2D: node must be of type 'GenericData'";
		return false;
	}
	//check for valid node number already done prior to this function
	if (std::strcmp(mainSystem.GetMainSystemData().GetMainNode(genericODE2NodeNumber).GetTypeName(), "GenericODE2") != 0)
	{
		errorString = "ObjectJointALEMoving2D: node must be of type 'GenericODE2'";
		return false;
	}
	

	Index nc = ((const CNodeGenericData&)(cObject->GetCSystemData()->GetCNode(dataNodeNumber))).GetNumberOfDataCoordinates();
	const Index nodeGenericDataSize = 1; //current cable in markerlist and the global sliding position
	if (nc != nodeGenericDataSize)
	{
		errorString = STDstring("ObjectJointALEMoving2D: NodeGenericData (Node ") + EXUstd::ToString(dataNodeNumber) + ") must have " + 
			EXUstd::ToString(nodeGenericDataSize) + " coordinates (found: " + EXUstd::ToString(nc) + ")";
		return false;
	}

	//Check indidual types:
	const ArrayIndex& nMarkers = cObject->GetMarkerNumbers();
	if (!(mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[0]).GetType() & Marker::Position))
	{
		errorString = STDstring("ObjectJointALEMoving2D: Marker 0 must be of type = 'Position'");
		return false;
	}

	if (mainSystem.GetMainSystemData().GetMainMarkers()[nMarkers[1]]->GetTypeName() != "Cable2DCoordinates")
	{
		errorString = STDstring("ObjectJointALEMoving2D: Marker 1 must be of type = 'Cable2DCoordinates'");
		return false;
	}

	return true;
}

//! compute the (local) sliding coordinate within the current cable element
Real CObjectJointALEMoving2D::ComputeLocalSlidingCoordinate() const
{
	Real slidingPos = GetCNode(1)->GetCurrentCoordinate(0) + parameters.slidingOffset; //this is the global (Eulerian) sliding position

	Index slidingMarkerIndex = (Index)GetCNode(0)->GetCurrentCoordinate(0); //this contains the current index in the cable marker list; slidingMarkerIndex will always be in valid range!

	slidingPos -= parameters.slidingMarkerOffsets[slidingMarkerIndex]; //slidingPos now ranges from 0 to L in current cable element

	return slidingPos;
}

//! compute the (local) sliding coordinate within the current cable element
Real CObjectJointALEMoving2D::ComputeLocalSlidingCoordinate_t() const
{
	return ((CNodeODE2*)GetCNode(1))->GetCurrentCoordinate_t(0); //this is the global (Eulerian) sliding velocity
}

//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectJointALEMoving2D::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, bool velocityLevel) const
{
	//markerData.GetMarkerData(1).vectorValue/_t:cable (refCoordinates+coordinates)/velocities
	//markerData.GetMarkerData(1).value:cable Length (current cable)
	//markerData.GetMarkerData(0).position:   position on other body (e.g. rigid body or mass point)

	//CObjectJointALEMoving2D: contains NodeGenericData: for current Cable marker number in marker list
	//2 equations: Residuum X,Residuum Y = 0
	//2 alg. unknowns: forceX, forceY

	if (parameters.activeConnector)
	{

		const Index forceXindex = 0;
		const Index forceYindex = 1;

		if (!velocityLevel)
		{
			//compute ANCF position:
			const Index ns = 4;
			LinkedDataVector qNode0(markerData.GetMarkerData(1).vectorValue, 0, ns); //link to position coordinates (refCoords+displacements)
			LinkedDataVector qNode1(markerData.GetMarkerData(1).vectorValue, ns, ns); //link to position coordinates (refCoords+displacements)

			Real L = markerData.GetMarkerData(1).value; //kind of hack ...
			Real slidingCoordinate = ComputeLocalSlidingCoordinate();

			Vector4D SV = CObjectANCFCable2D::ComputeShapeFunctions(slidingCoordinate, L);
			//Vector4D SV_x = CObjectANCFCable2D::ComputeShapeFunctions_x(slidingCoordinate, L);

			Vector2D slidingPosition = CObjectANCFCable2D::MapCoordinates(SV, qNode0, qNode1);
			//Vector2D slopeVector = CObjectANCFCable2D::MapCoordinates(SV_x, qNode0, qNode1);


			algebraicEquations.SetNumberOfItems(GetAlgebraicEquationsSize());
			Vector2D vPos;
			vPos[0] = (slidingPosition[0] - markerData.GetMarkerData(0).position[0]); //this is the difference between the sliding position and the position of marker0
			vPos[1] = (slidingPosition[1] - markerData.GetMarkerData(0).position[1]);

			algebraicEquations[0] = vPos[0];
			algebraicEquations[1] = vPos[1];

			//Real forceX = GetCurrentAEcoordinate(forceXindex);
			//Real forceY = GetCurrentAEcoordinate(forceYindex);

		}
		else
		{
			//release_assert(0 && "CObjectJointALEMoving2D::ComputeAlgebraicEquations: velocityLevel not implemented");
			//compute ANCF position:
			const Index ns = 4;
			LinkedDataVector qNode0(markerData.GetMarkerData(1).vectorValue, 0, ns); //link to position coordinates (refCoords+displacements)
			LinkedDataVector qNode1(markerData.GetMarkerData(1).vectorValue, ns, ns); //link to position coordinates (refCoords+displacements)
			LinkedDataVector qNode0_t(markerData.GetMarkerData(1).vectorValue_t, 0, ns); //link to velocity coordinates
			LinkedDataVector qNode1_t(markerData.GetMarkerData(1).vectorValue_t, ns, ns); //link to velocity coordinates

			Real L = markerData.GetMarkerData(1).value; //kind of hack ...
			Real slidingCoordinate = ComputeLocalSlidingCoordinate();
			Real slidingCoordinate_t = ComputeLocalSlidingCoordinate_t();

			Vector4D SV = CObjectANCFCable2D::ComputeShapeFunctions(slidingCoordinate, L);
			Vector4D SV_x = CObjectANCFCable2D::ComputeShapeFunctions_x(slidingCoordinate, L);

			Vector2D cableVelocity = CObjectANCFCable2D::MapCoordinates(SV, qNode0_t, qNode1_t);
			Vector2D slopeVector = CObjectANCFCable2D::MapCoordinates(SV_x, qNode0, qNode1);

			Vector2D slidingVelocity = cableVelocity + slidingCoordinate_t*slopeVector;

			algebraicEquations.SetNumberOfItems(GetAlgebraicEquationsSize());
			Vector2D vVel;
			vVel[0] = (slidingVelocity[0] - markerData.GetMarkerData(0).velocity[0]); 
			vVel[1] = (slidingVelocity[1] - markerData.GetMarkerData(0).velocity[1]);

			algebraicEquations[0] = vVel[0];
			algebraicEquations[1] = vVel[1];

			//Real forceX = GetCurrentAEcoordinate(forceXindex);
			//Real forceY = GetCurrentAEcoordinate(forceYindex);

		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation [forceX,forceY]^T = [0,0]^T, means that the current values need to be copied
	}
}

void CObjectJointALEMoving2D::ComputeJacobianAE(ResizableMatrix& jacobian, ResizableMatrix& jacobian_t, ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData) const
{
	const Index ns = 4;
	Index columnsOffset = markerData.GetMarkerData(0).positionJacobian.NumberOfColumns();
	jacobian.SetNumberOfRowsAndColumns(2, columnsOffset + 2 * ns + 1); //add 1 coordinate for sALE
	jacobian.SetAll(0.);

	//GetAvailableJacobians() always requires jacobian AND jacobian_AE ==> always initialize both matrices!!!

	if (parameters.activeConnector)
	{

		jacobian_AE.SetScalarMatrix(2, 0.); //no dependencies

		//marker0: contains position jacobian
		const Index forceXindex = 0;
		const Index forceYindex = 1;

		//compute ANCF position:
		LinkedDataVector qNode0(markerData.GetMarkerData(1).vectorValue, 0, ns); //link to position coordinates (refCoords+displacements)
		LinkedDataVector qNode1(markerData.GetMarkerData(1).vectorValue, ns, ns); //link to position coordinates (refCoords+displacements)

		Real L = markerData.GetMarkerData(1).value; //kind of hack ...
		Real slidingCoordinate = ComputeLocalSlidingCoordinate();

		Vector4D SV = CObjectANCFCable2DBase::ComputeShapeFunctions(slidingCoordinate, L);
		Vector4D SV_x = CObjectANCFCable2DBase::ComputeShapeFunctions_x(slidingCoordinate, L);
		Vector2D r_x = CObjectANCFCable2DBase::MapCoordinates(SV_x, qNode0, qNode1);

		for (Index i = 0; i < columnsOffset; i++)
		{
			jacobian(0, i) = -markerData.GetMarkerData(0).positionJacobian(0, i);
			jacobian(1, i) = -markerData.GetMarkerData(0).positionJacobian(1, i);
		}
		for (Index i = 0; i < ns; i++)
		{
			jacobian(0, 2 * i + columnsOffset) = SV[i];
			jacobian(1, 2 * i + 1 + columnsOffset) = SV[i];
		}
		//new version, missing component for ALE joint:
		jacobian(0, 2 * ns + columnsOffset) = r_x[0];
		jacobian(1, 2 * ns + columnsOffset) = r_x[1];
	}
	else
	{
		jacobian_AE.SetScalarMatrix(2, 1.); //represents derivative of algebraic equation '[lambda0,lambda1]^T = [0,0]^T'
		//jacobian already set to zero on top
	}
}

//JacobianType::Type CObjectJointALEMoving2D::GetAvailableJacobians() const ==> always compute jacobian and jacobian_AE independently of activeConnector

////! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
//OutputVariableType CObjectJointALEMoving2D::GetOutputVariableTypes() const
//{
//	return (OutputVariableType)(
//		(Index)OutputVariableType::Position +
//		(Index)OutputVariableType::Velocity +
//		(Index)OutputVariableType::Coordinates +
//		(Index)OutputVariableType::Coordinates_t +
//		(Index)OutputVariableType::SlidingCoordinate +
//		(Index)OutputVariableType::Force);
//	//return (OutputVariableType)((Index)OutputVariableType::Displacement); //Displacement represents drift in index2 case
//}

//! provide according output variable in "value"
void CObjectJointALEMoving2D::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	//SysError("CObjectJointALEMoving2D::GetOutputVariableConnector not implemented");
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(markerData.GetMarkerData(0).position); break;
	case OutputVariableType::Velocity: value.CopyFrom(markerData.GetMarkerData(0).velocity); break;
	case OutputVariableType::Coordinates:
	{
		value = Vector({ GetCNode(0)->GetCurrentCoordinate(0), GetCNode(1)->GetCurrentCoordinate(0) }); // slidingMarker and aleCoordinate (global)
		break;
	}
	case OutputVariableType::Coordinates_t:
	{
		value = Vector({ ComputeLocalSlidingCoordinate_t() }); // aleCoordinate velocity
		break;
	}
	case OutputVariableType::Force:
	{
		const Index forceXindex = 0;
		const Index forceYindex = 1;
		Real forceX = GetCurrentAEcoordinate(forceXindex);
		Real forceY = GetCurrentAEcoordinate(forceYindex);

		value = Vector({ forceX, forceY, 0. }); //as all output quantities, they are provided as 3D vectors for 2D objects
		break;
	}
	case OutputVariableType::SlidingCoordinate:
	{
		Real slidingPos = GetCNode(1)->GetCurrentCoordinate(0) + parameters.slidingOffset; //this is the global (Eulerian) sliding position

		value = Vector({ slidingPos });
		break;
	}
	default:
		SysError("CObjectJointSliding2D::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

bool aleMovingJoint2Dwarned = false;
//! function called after Newton method; returns a residual error (force); 
//! done for two different computation states in order to estimate the correct time of contact
Real CObjectJointALEMoving2D::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, PostNewtonFlags::Type& flags)
{
	//return force-type error in case of contact: in case that the assumed contact state has been wrong, 
	//  the contact force (also negative) is returned as measure of the error

	//THIS IS VERY SPECIFIC AND MIGHT BE A POTENTIAL PROBLEM IN CASE OF CHANGES OF THE ASSEMBLE/SOLVER STRUCTURE
	//after change of slidingMarkerIndex, we must also change LTG list, if marker is changed ...; 
	//also use second dataCoordinate for current (+initial) position!!!

	Real discontinuousError = 0;
	flags = PostNewtonFlags::None;

	Real L = markerDataCurrent.GetMarkerData(1).value; //kind of hack ...
	const Index slidingCoordinateIndex = 2;

	LinkedDataVector currentState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current);	//copy, but might change values ...

	Real slidingCoordinate = ComputeLocalSlidingCoordinate();


	if (slidingCoordinate < 0) //reduce cableMarkerIndex
	{
		discontinuousError = fabs(-slidingCoordinate);
		if (currentState[0] > 0) { currentState[0] -= 1; } //move to previous cable element if possible
		else
		{
			if (!aleMovingJoint2Dwarned) { pout << "WARNING: JointALEMoving2D: sliding coordinate < 0; further warnings suppressed!\n"; }
			aleMovingJoint2Dwarned = true;
		}
		parameters.markerNumbers[1] = parameters.slidingMarkerNumbers[(Index)currentState[0]]; //now use the new cableMarker
		flags = PostNewtonFlags::UpdateLTGLists; //this signals the system that the LTG lists need to be updated do to major system change
		//pout << "new cable marker: " << parameters.markerNumbers[1] << "\n";

	}
	if (slidingCoordinate > L) //increase cableMarkerIndex
	{
		discontinuousError = fabs(slidingCoordinate - L);
		if (currentState[0] < parameters.slidingMarkerNumbers.NumberOfItems()-1) 
		{ currentState[0] += 1; } //move to next cable element if possible
		else
		{
			if (!aleMovingJoint2Dwarned) { pout << "WARNING: JointALEMoving2D: sliding coordinate > beam length; further warnings suppressed!\n"; }
			aleMovingJoint2Dwarned = true;
		}
		parameters.markerNumbers[1] = parameters.slidingMarkerNumbers[(Index)currentState[0]]; //now use the new cableMarker
		flags = PostNewtonFlags::UpdateLTGLists; //this signals the system that the LTG lists need to be updated do to major system change
		//pout << "new cable marker: " << parameters.markerNumbers[1] << "\n";
	}

	return discontinuousError;
}

//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
void CObjectJointALEMoving2D::PostDiscontinuousIterationStep()
{
}


