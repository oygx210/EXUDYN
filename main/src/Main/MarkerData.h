/** ***********************************************************************************************
* @class        MarkerData
* @brief        A data structure used to transferdata between markers and connectors/loads during computation
*
* @author       Gerstmayr Johannes
* @date         2018-06-13 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include "Utilities/ReleaseAssert.h"
#include <initializer_list>
#include "Utilities/BasicDefinitions.h" //defines Real
#include "Utilities/ResizableArray.h" 
#include "Linalg/ResizableMatrix.h" 


//! a structure, which contains temporary data from markers (position, orientation, velocity, Jacobians, ...)
class MarkerData
{
public:
	Vector3D position;			//position of marker
	Vector3D velocity;			//velocity of marker
	Matrix3D orientation;//rotation matrix describing orientation of marker
	//ResizableMatrix orientation;//rotation matrix describing orientation of marker
	Vector3D angularVelocityLocal;	//local (=body fixed) angular velocity of marker

	ResizableMatrix positionJacobian;	//d(pos)/dq
	ResizableMatrix rotationJacobian;	//d(omega)/dq_t

	Real value;					//general value, e.g. for coordinate marker
	Real value_t;				//general value at velocity level, e.g. for coordinate marker
	Vector vectorValue;			//general vector value, e.g. for ANCF shape marker
	Vector vectorValue_t;		//general vector value at velocity level, e.g. for ANCF shape marker

	bool velocityAvailable;		//used for value/value_t, vectorValue/vectorValue_t, position/velocity, ... to determine, if velocities are available

	ResizableMatrix jacobian;	//general jacobian, e.g. for coordinate marker; ?d(Rotv123)/dq for rigid bodies?

};

//this class contains several MarkerData structures ==> derive from this class for special connectors
class MarkerDataStructure
{
private:
	static const Index numberOfMarkerData = 2;
	MarkerData markerData[numberOfMarkerData];
	LinkedDataVector lagrangeMultipliers; //for constraint equation evaluation; WORKAROUND, in order not to access system coordinates in ComputeAlgebraicEquations
public:
	//! get number of marker Data structures ==> for conventional connectors it is 2, but could be different for complex joints (e.g. sliding joint)
	virtual Index GetNumberOfMarkerData() const { return numberOfMarkerData; }
	//! read access to markerData
	virtual const MarkerData& GetMarkerData(const Index& i) const { return markerData[i]; }
	//! write access to markerData
	virtual MarkerData& GetMarkerData(const Index& i) { return markerData[i]; }

	//! read access to lagrangeMultipliers
	virtual const LinkedDataVector& GetLagrangeMultipliers() const { return lagrangeMultipliers; }
	//! write access to lagrangeMultipliers
	virtual LinkedDataVector& GetLagrangeMultipliers() { return lagrangeMultipliers; }
};

