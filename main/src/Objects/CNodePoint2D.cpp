/** ***********************************************************************************************
* @brief		Implementation for NodePoint2D
*
* @author		Gerstmayr Johannes
* @date			2019-06-15 (generated)
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

#include "Main/CSystemData.h"
#include "Autogenerated/CNodePoint2D.h"

//for CallFunction(...)
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainNodePoint2D.h"


Vector3D CNodePoint2D::GetPosition(ConfigurationType configuration) const
{
	LinkedDataVector pRef2D = GetReferenceCoordinateVector();
	Vector3D pRef({ pRef2D[0], pRef2D[1], 0. });
	if (configuration == ConfigurationType::Reference) {return pRef; }

	LinkedDataVector u2D = GetCoordinateVector(configuration);
	Vector3D displacement({ u2D[0], u2D[1], 0. });
	return pRef + displacement;
}

Vector3D CNodePoint2D::GetVelocity(ConfigurationType configuration) const
{
	LinkedDataVector u2D_t = GetCoordinateVector_t(configuration);
	return Vector3D({ u2D_t[0], u2D_t[1], 0. });
}

Vector3D CNodePoint2D::GetAcceleration(ConfigurationType configuration) const
{
	LinkedDataVector u2D_tt = GetCoordinateVector_tt(configuration);
	return Vector3D({ u2D_tt[0], u2D_tt[1], 0. });
}

//! provide according output variable in "value"
void CNodePoint2D::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(configuration)); break;
	case OutputVariableType::Displacement: value.CopyFrom(GetPosition(configuration) - GetPosition(ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(configuration)); break;
	case OutputVariableType::Acceleration: value.CopyFrom(GetAcceleration(configuration)); break;
	case OutputVariableType::Coordinates:
	{
		if (IsConfigurationInitialCurrentReferenceVisualization(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Reference + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodePoint2D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_t:
	{
		if (IsConfigurationInitialCurrentVisualization(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector_t(configuration);
		}
		else
		{
			PyError("CNodePoint2D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	case OutputVariableType::Coordinates_tt:
	{
		if (IsConfigurationInitialCurrentVisualization(configuration)) 
		{
			value = GetCoordinateVector_tt(configuration);
		}
		else
		{
			PyError("CNodePoint2D::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodePoint2D::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}


//! call a certain function of object (autogenerated in future!)
py::object MainNodePoint2D::CallFunction(STDstring functionName, py::dict args) const
{
	//these calls should be automated by python script ...
	if (functionName == "GetTypeName")
	{
		return py::str(GetTypeName());
	}
	else if (functionName == "GetNodeType")
	{
		return py::cast(GetCNodePoint2D()->GetType());
	}
	else if (functionName == "GetNodeGroup")
	{
		return py::cast(GetCNodePoint2D()->GetNodeGroup());
	}
	else if (functionName == "GetNumberOfODE2Coordinates")
	{
		return py::int_(GetCNodePoint2D()->GetNumberOfODE2Coordinates());
	}
	else if (functionName == "GetGlobalODE2CoordinateIndex")
	{
		return py::int_(GetCNodePoint2D()->GetGlobalODE2CoordinateIndex());
	}
	else if (functionName == "GetCurrentCoordinateVector")
	{
		LinkedDataVector v(GetCNodePoint2D()->GetCurrentCoordinateVector());
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}
	else if (functionName == "GetInitialCoordinateVector")
	{
		LinkedDataVector v(GetCNodePoint2D()->GetInitialCoordinateVector());
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}
	else if (functionName == "GetCurrentCoordinateVector_t")
	{
		LinkedDataVector v(GetCNodePoint2D()->GetCurrentCoordinateVector_t());
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}
	else if (functionName == "GetInitialCoordinateVector_t")
	{
		LinkedDataVector v(GetCNodePoint2D()->GetInitialCoordinateVector_t());
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}
	else if (functionName == "GetCurrentPosition")
	{
		Vector3D v = GetCNodePoint2D()->GetPosition(ConfigurationType::Current);
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}

	PyError(STDstring("MainNodePoint2D::CallFunction called with invalid functionName '" + functionName + "'"));
	return py::int_(EXUstd::InvalidIndex);
}
