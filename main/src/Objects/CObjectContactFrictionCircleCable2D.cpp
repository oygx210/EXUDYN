/** ***********************************************************************************************
* @brief        Implementation of CObjectContactFrictionCircleCable2D
*
* @author       Gerstmayr Johannes
* @date         2018-05-06 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */


#include "Main/CSystemData.h"
#include "Linalg/Geometry.h"
#include "Autogenerated/CNodeGenericData.h"
//#include "Autogenerated/CObjectContactCoordinate.h"

//for consistency checks:
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectContactFrictionCircleCable2D.h"

const Index maxObject0Coordinates = 12; //this is a temporary hack; used for a constsizevector in the computation of the action on the body of marker0
bool MainObjectContactFrictionCircleCable2D::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectContactFrictionCircleCable2D* cObject = (CObjectContactFrictionCircleCable2D*)GetCObject();
	Index node = cObject->GetNodeNumber(0);

	if (cObject->GetParameters().numberOfContactSegments > cObject->maxNumberOfSegments)
	{
		errorString = "ObjectContactFrictionCircleCable2D: numberOfContactSegments must be <= " + EXUstd::ToString(cObject->maxNumberOfSegments);
		return false;
	}

	//check for valid node number already done prior to this function
	if (std::strcmp(mainSystem.GetMainSystemData().GetMainNode(node).GetTypeName(), "GenericData") != 0) 
	{
		errorString = "ObjectContactFrictionCircleCable2D: node must be of type 'GenericData'";
		return false;
	}

	Index nc = ((const CNodeGenericData&)(cObject->GetCSystemData()->GetCNode(node))).GetNumberOfDataCoordinates();
	Index nGenericCoordinates = 3*cObject->GetParameters().numberOfContactSegments; //per segment: contact gap, stick/slip (stick=1), last friction position (UNUSED)
	if (nc != nGenericCoordinates)
	{
		errorString = STDstring("ObjectContactFrictionCircleCable2D: NodeGenericData (Node ") + EXUstd::ToString(node) + ") must have " + EXUstd::ToString(nGenericCoordinates)
			+ " coordinates (found: " + EXUstd::ToString(nc) + ")";
		return false;
	}

	const ArrayIndex& nMarkers = cObject->GetMarkerNumbers();
	Marker::Type mType = mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[0]).GetType();
	if (!((mType & Marker::Position) && (mType & Marker::Orientation)))
	{
		errorString = STDstring("ObjectContactFrictionCircleCable2D: Marker 0 must be of type = 'Rigid' (Position + Orientation)");
		return false;
	}

	Index object0Index = mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[0]).GetObjectNumber();
	if (mainSystem.GetCSystem()->GetSystemData().GetCObjects()[object0Index]->GetNumberOfCoordinates() > maxObject0Coordinates)
	{
		errorString = STDstring("ObjectContactFrictionCircleCable2D: Marker 0 must refer to an object which has no more than ") + EXUstd::ToString(maxObject0Coordinates) + " coordinates";
		return false;
	}


	if (STDstring(mainSystem.GetMainSystemData().GetMainMarkers()[nMarkers[1]]->GetTypeName()) != STDstring("BodyCable2DShape"))
	{
		errorString = STDstring("ObjectContactFrictionCircleCable2D: Marker 1 must be of type = 'BodyCable2DShape'");
		return false;
	}

	return true;
}


//! compute gap for given configuration (current, start of step, ...); gap <= 0 means contact, gap > 0 is no contact
//! gapPerSegment returns the 'area' gap per segment, which is the distance/penetration * segment length (or real penetrating area)
//! referenceCoordinatePerSegment returns the reference coordinate at the segment (range: [0,1]) in case of contact ==> used to apply forces (either this is the nearest point or a vertex of the segment)
//! the x/yDirectionGap show the direction of the gap, in which the contact force should act
void CObjectContactFrictionCircleCable2D::ComputeGap(const MarkerDataStructure& markerData, 
	ConstSizeVector<maxNumberOfSegments>& gapPerSegment, 
	ConstSizeVector<maxNumberOfSegments>& referenceCoordinatePerSegment, 
	ConstSizeVector<maxNumberOfSegments>& xDirectionGap, ConstSizeVector<maxNumberOfSegments>& yDirectionGap) const
{
	//circular segment, which cuts a piece with height h off the circle with radius r:
	//    alpha = 2*arccos(1-h/r);			//angle of the cut
	//    s = 2*r*sin(alpha/2);				//length of the cut
	//    A = r^2/2*(alpha - sin(alpha))	//area of the cut

	//markerData contains:
	//  vectorValue = [x0,y0, x1,y1, x2,y2, ... ] ... the x/y positions of the cable points defining the contact segments [x0,y0,x1,y1], [x1,y1,x2,y2], 

	const MarkerData& markerData0 = markerData.GetMarkerData(0); //position based marker
	const MarkerData& markerData1 = markerData.GetMarkerData(1); //ANCFCable2DShape

	//position and radius of circle:
	Vector2D circleCenter({ markerData0.position[0], markerData0.position[1] }); //center of the circle
	const Real& r = parameters.circleRadius;
	const Real& offset = parameters.offset;
	Vector2D contactVector; //vector from contact point to circle midpoint; the contact force acts in opposite direction

	Index nSeg = parameters.numberOfContactSegments;
	gapPerSegment.SetNumberOfItems(nSeg);
	referenceCoordinatePerSegment.SetNumberOfItems(nSeg);
	xDirectionGap.SetNumberOfItems(nSeg);
	yDirectionGap.SetNumberOfItems(nSeg);

	//iterate over all segments:
	for (Index i = 0; i < nSeg; i++)
	{
		//take 2 points as one segment
		Vector2D p0({ markerData1.vectorValue[i * 2], markerData1.vectorValue[i * 2 + 1] });	 //markerdata.value stores the x/y positions of the contact points
		Vector2D p1({ markerData1.vectorValue[i * 2+2], markerData1.vectorValue[i * 2 + 1+2] }); //markerdata.value stores the x/y positions of the contact points

		//compute shortest distance:
		//referenceCoordinatePerSegment is the relative position [0..1] of the shortest projected point at the line segment
		Real distance = HGeometry::ShortestDistanceEndPointsRelativePosition(p0, p1, circleCenter, referenceCoordinatePerSegment[i], contactVector);
		if (distance != 0.) { contactVector *= 1. / distance; } //computes normal vector

		gapPerSegment[i] = distance - r - offset;
		xDirectionGap[i] = -contactVector[0]; //x-component of direction of force (global coordinates)
		yDirectionGap[i] = -contactVector[1]; //y-component of direction of force (global coordinates)
	}
}

bool CObjectContactFrictionCircleCable2D::IsActive() const
{
	if (!parameters.activeConnector) { return false; }
	for (Index i = 0; i < parameters.numberOfContactSegments; i++)
	{
		if (GetCNode(0)->GetCurrentCoordinate(i) <= 0) { return true; }
	}
	return false;
}

//! Computational function: compute left-hand-side (LHS) of second order ordinary differential equations (ODE) to "ode2Lhs"
//  MODEL: f
void CObjectContactFrictionCircleCable2D::ComputeODE2LHS(Vector& ode2Lhs, const MarkerDataStructure& markerData) const
{

	ode2Lhs.SetNumberOfItems(markerData.GetMarkerData(0).positionJacobian.NumberOfColumns() + markerData.GetMarkerData(1).jacobian.NumberOfColumns());
	//positionJacobian and orientationJacobian have same number of columns, therefore only one jacobian is used to set size of ode2Lhs vector
	ode2Lhs.SetAll(0.);

	//bool isContact = false;
	//for (Index i = 0; i < parameters.numberOfContactSegments; i++)
	//{
	//	if (GetCNode(0)->GetCurrentCoordinate(i) <= 0) { isContact = true; }
	//}

	if (parameters.activeConnector/* && isContact*/)
	{
		const MarkerData& markerData0 = markerData.GetMarkerData(0); //position based marker
		const MarkerData& markerData1 = markerData.GetMarkerData(1); //ANCFCable2DShape
		//gap>0: no contact, gap<0: contact
		//Real gap = (markerData.GetMarkerData(1).value - markerData.GetMarkerData(0).value - parameters.offset);

		ConstSizeVector<maxNumberOfSegments> gapPerSegment;
		ConstSizeVector<maxNumberOfSegments> referenceCoordinatePerSegment;
		ConstSizeVector<maxNumberOfSegments> xDirectionGap;
		ConstSizeVector<maxNumberOfSegments> yDirectionGap;
		ComputeGap(markerData, gapPerSegment, referenceCoordinatePerSegment, xDirectionGap, yDirectionGap);
		const Index maxNumberOfPoints = maxNumberOfSegments + 1;

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//compute velocities:
		//ConstSizeVector<maxNumberOfSegments> gapVelocityPerSegment;
		//ConstSizeVector<maxNumberOfSegments> tangentialVelocityPerSegment;
		//gapVelocityPerSegment.SetNumberOfItems(parameters.numberOfContactSegments);
		//tangentialVelocityPerSegment.SetNumberOfItems(parameters.numberOfContactSegments);

		//for (Index i = 0; i < parameters.numberOfContactSegments; i++)
		//{
		//	tangentialVelocityPerSegment[i] = vCircle * t;  //tangential velocity
		//	gapVelocityPerSegment[i] = vCircle * n;			//gap velocity
		//}




		ConstSizeVector<maxNumberOfPoints * 2> forcePerPoint; //force (x/y) per contact point ==> used to apply forces
		forcePerPoint.SetNumberOfItems((parameters.numberOfContactSegments + 1) * 2);
		forcePerPoint.SetAll(0.);
		Vector3D forceSum({ 0.,0.,0. }); //sum of all forces acting on circle
		Vector3D torqueSum({ 0.,0.,0. }); //sum of all torques acting on circle (roll)

		for (Index i = 0; i < parameters.numberOfContactSegments; i++)
		{
			//Real hasContact = 0; //1 for contact, 0 else
			if (GetCNode(0)->GetCurrentCoordinate(i) <= 0)  //this is the contact state: <=0: contact/use contact force, >0: no contact
			{
				//velocity terms:
				Vector2D v0({ markerData1.vectorValue_t[i * 2], markerData1.vectorValue_t[i * 2 + 1] });	 //markerdata.value stores the x/y velocities of the contact points
				Vector2D v1({ markerData1.vectorValue_t[i * 2 + 2], markerData1.vectorValue_t[i * 2 + 1 + 2] }); //markerdata.value stores the x/y velocities of the contact points

				Vector2D cableContactVel = (1. - referenceCoordinatePerSegment[i])*v0 + referenceCoordinatePerSegment[i] * v1; //this is the velocity at the point of shortest distance (interpolated from the contact segment)
				Vector2D n({ xDirectionGap[i], yDirectionGap[i] }); //contact normal vector;  already normalized vector!
				Vector2D t({ -n[1], n[0] }); //contact tangent vector

				Vector2D circleContactVel({ markerData0.velocity[0], markerData0.velocity[1] });
				circleContactVel += parameters.circleRadius * markerData0.angularVelocityLocal[2] * t; //this is now the velocity of the contact point of the Circle/Rigid body

				Real gapVel = (cableContactVel - circleContactVel)*n;
				Real tangentialVel = (cableContactVel - circleContactVel)*t;

				Real fNormal = gapPerSegment[i] * parameters.contactStiffness + gapVel * parameters.contactDamping;		//contact normal force of segment
				Real fTangent = tangentialVel * parameters.frictionVelocityPenalty;  //contact (friction) tangent force of segment

				//std::cout << "gap" << i << " = " << gapPerSegment[i] << ",nVel = " << gapVel << ", tVel = " << tangentialVel << "\n";
				//std::cout << "  fN=" << fNormal << ", fT=" << fTangent << ", n=" << n << ", t=" << t << "\n";

				//WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING:
				//WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING:
				//WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING:
				if (fabs(fTangent) > parameters.frictionCoefficient*fabs(fNormal)) { fTangent = parameters.frictionCoefficient*fabs(fNormal)*EXUstd::Sgn(fTangent); } //this should be changed with an according friction flag

				Vector2D contactForce = fNormal * n + fTangent * t; //now bring into global coordinate system

				//as gap is negative in case of contact, the force needs to act in opposite direction
				//force in global x-direction:
				forcePerPoint[i * 2] += contactForce[0] * (1. - referenceCoordinatePerSegment[i]);
				forcePerPoint[(i + 1) * 2] += contactForce[0] * referenceCoordinatePerSegment[i];
				//force in global y-direction:
				forcePerPoint[i * 2 + 1] += contactForce[1] * (1. - referenceCoordinatePerSegment[i]);
				forcePerPoint[(i + 1) * 2 + 1] += contactForce[1] * referenceCoordinatePerSegment[i];

				forceSum[0] += contactForce[0];
				forceSum[1] += contactForce[1];
				torqueSum[2] += fTangent * parameters.circleRadius;
			}
		}

		//if (!cSystemData->isODE2RHSjacobianComputation && forcePerPoint.GetL2Norm() != 0) {
		//	pout << "forcePerPoint=" << forcePerPoint << "\n";
		//	pout << "gapPerSegment=" << gapPerSegment << "\n";
		//	pout << "referenceCoordinatePerSegment=" << referenceCoordinatePerSegment << "\n";
		//	pout << "xDirectionGap=" << xDirectionGap << "\n";
		//	pout << "yDirectionGap=" << yDirectionGap << "\n";
		//}

		//now link ode2Lhs Vector to partial result using the two jacobians
		if (markerData.GetMarkerData(1).jacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv1(ode2Lhs, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns(), markerData.GetMarkerData(1).jacobian.NumberOfColumns());

			//positive force on marker1
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(1).jacobian, forcePerPoint, ldv1);
		}

		if (markerData.GetMarkerData(0).positionJacobian.NumberOfColumns()) //special case: COGround has (0,0) Jacobian
		{
			LinkedDataVector ldv0(ode2Lhs, 0, markerData.GetMarkerData(0).positionJacobian.NumberOfColumns());

			forceSum *= -1; //negative force on marker0
			torqueSum *= -1; //negative force on marker0
			ConstSizeVector<maxObject0Coordinates> temp(ldv0.NumberOfItems()); //possible crash, if rigid body has more than 12 DOF --> check above
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).positionJacobian, forceSum, ldv0);
			EXUmath::MultMatrixTransposedVector(markerData.GetMarkerData(0).rotationJacobian, torqueSum, temp);
			ldv0 += temp;
		}
	}

}

void CObjectContactFrictionCircleCable2D::ComputeJacobianODE2_ODE2(ResizableMatrix& jacobian, ResizableMatrix& jacobian_ODE2_t, const MarkerDataStructure& markerData) const
{
	CHECKandTHROWstring("ERROR: illegal call to ObjectContactFrictionCircleCable2D::ComputeODE2LHSJacobian");
}

//! Flags to determine, which output variables are available (displacment, velocity, stress, ...)
OutputVariableType CObjectContactFrictionCircleCable2D::GetOutputVariableTypes() const
{
	return OutputVariableType::Distance;
}

//! provide according output variable in "value"
void CObjectContactFrictionCircleCable2D::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	SysError("CObjectContactCoordinate::ObjectContactFrictionCircleCable2D not implemented");
}


//! function called after Newton method; returns a residual error (force); 
//! done for two different computation states in order to estimate the correct time of contact
Real CObjectContactFrictionCircleCable2D::PostNewtonStep(const MarkerDataStructure& markerDataCurrent, PostNewtonFlags::Type& flags)
{
	//return force-type error in case of contact: in case that the assumed contact state has been wrong, 
	//  the contact force (also negative) is returned as measure of the error
	Real discontinuousError = 0;
	flags = PostNewtonFlags::_None;

	if (parameters.activeConnector)
	{
		LinkedDataVector currentState = ((CNodeData*)GetCNode(0))->GetCoordinateVector(ConfigurationType::Current);	//copy, but might change values ...

		ConstSizeVector<maxNumberOfSegments> currentGapPerSegment;
		ConstSizeVector<maxNumberOfSegments> referenceCoordinatePerSegment;
		ConstSizeVector<maxNumberOfSegments> xDirectionGap;
		ConstSizeVector<maxNumberOfSegments> yDirectionGap;
		ComputeGap(markerDataCurrent, currentGapPerSegment, referenceCoordinatePerSegment, xDirectionGap, yDirectionGap);

		for (Index i = 0; i < parameters.numberOfContactSegments; i++)
		{
			//if (currentGapPerSegment[i] > 0 && currentState[i] <= 0 || currentGapPerSegment[i] <= 0 && currentState[i] > 0) //OLD: brackets missing!
			if ((currentGapPerSegment[i] > 0 && currentState[i] <= 0) || (currentGapPerSegment[i] <= 0 && currentState[i] > 0))
			{//action: state1=currentGapState, error = |currentGap*k|
				discontinuousError += fabs((currentGapPerSegment[i] - currentState[i])* parameters.contactStiffness);
				currentState[i] = currentGapPerSegment[i];
			}
		}
	}
	return discontinuousError;
}

//! function called after discontinuous iterations have been completed for one step (e.g. to finalize history variables and set initial values for next step)
void CObjectContactFrictionCircleCable2D::PostDiscontinuousIterationStep()
{

}

