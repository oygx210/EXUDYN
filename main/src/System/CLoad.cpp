/** ***********************************************************************************************
* @brief		Implementation for computational loads;
*               This file covers implementation parts of loads, specifically python bindings
*
* @author		Gerstmayr Johannes
* @date			2019-12-01 (generated)
* @pre			...
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See 'LICENSE.txt' for more details.
* @note			Bug reports, support and further information:
* 				- email: johannes.gerstmayr@uibk.ac.at
* 				- weblink: missing
* 				
*
* *** Example code ***
*
************************************************************************************************ */
#pragma once

#include "Main/CSystemData.h"

#include "Autogenerated/CLoadCoordinate.h"
#include "Autogenerated/CLoadForceVector.h"
#include "Autogenerated/CLoadTorqueVector.h"
#include "Autogenerated/CLoadMassProportional.h"

//! AUTO:  read access for load value (IsVector=false)
Real CLoadCoordinate::GetLoadValue(Real t) const
{
	if (!parameters.loadUserFunction)
	{
		return parameters.load;
	}
	else
	{
		//user function args:(Real t, Real load)
		return parameters.loadUserFunction(t, parameters.load);
	}
}

Vector3D CLoadForceVector::GetLoadVector(Real t) const
{
	if (!parameters.loadVectorUserFunction)
	{
		return parameters.loadVector;
	}
	else
	{
		//user function args:(Real t, Real load)
		return (Vector3D)parameters.loadVectorUserFunction(t, parameters.loadVector);
	}
}

Vector3D CLoadTorqueVector::GetLoadVector(Real t) const
{
	if (!parameters.loadVectorUserFunction)
	{
		return parameters.loadVector;
	}
	else
	{
		//user function args:(Real t, Real load)
		return (Vector3D)parameters.loadVectorUserFunction(t, parameters.loadVector);
	}
}

Vector3D CLoadMassProportional::GetLoadVector(Real t) const
{
	if (!parameters.loadVectorUserFunction)
	{
		return parameters.loadVector;
	}
	else
	{
		//user function args:(Real t, Real load)
		return (Vector3D)parameters.loadVectorUserFunction(t, parameters.loadVector);
	}
}

