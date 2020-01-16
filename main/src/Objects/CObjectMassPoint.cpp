/** ***********************************************************************************************
* @brief        CObjectMassPoint implementation
*
* @author       Gerstmayr Johannes
* @date         2019-04-29 (generated)
* @date         2019-04-29 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CObjectMassPoint.h"

//for CallFunction(...)
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectMassPoint.h"


//! Computational function: compute mass matrix
void CObjectMassPoint::ComputeMassMatrix(Matrix& massMatrix) const
{
	massMatrix.SetScalarMatrix(nODE2Coordinates, parameters.physicsMass);
}

//! Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to "ode2rhs"
void CObjectMassPoint::ComputeODE2RHS(Vector& ode2Rhs) const
{
	ode2Rhs.SetNumberOfItems(nODE2Coordinates);
	ode2Rhs.SetAll(0.);
}

//! Flags to determine, which access (forces, moments, connectors, ...) to object are possible
AccessFunctionType CObjectMassPoint::GetAccessFunctionTypes() const
{
	return (AccessFunctionType)((Index)AccessFunctionType::TranslationalVelocity_qt + (Index)AccessFunctionType::DisplacementMassIntegral_q);
}

//! provide Jacobian at localPosition in "value" according to object access
void CObjectMassPoint::GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const
{
	switch (accessType)
	{
	case AccessFunctionType::TranslationalVelocity_qt:
		value.SetScalarMatrix(3, 1.); //diagonal matrix
		break;
	case AccessFunctionType::DisplacementMassIntegral_q:
		value.SetScalarMatrix(3, parameters.physicsMass); //diagonal matrix
		break;
	default:
		SysError("CObjectMassPoint:GetAccessFunctionBody illegal accessType");
	}
}

//! provide according output variable in "value"
void CObjectMassPoint::GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(localPosition, configuration)); break;
	case OutputVariableType::Displacement:	value.CopyFrom(GetPosition(localPosition, configuration) - GetPosition(localPosition, ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(localPosition, configuration)); break;
	default:
		SysError("CObjectMassPoint::GetOutputVariableBody failed"); //error should not occur, because types are checked!
	}
}

//! @todo: add ConfigurationType to CObjectMassPoint::GetPosition; 
//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMassPoint::GetPosition(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2*)GetCNode(0))->GetPosition(configuration) + localPosition;

}

//! @todo: add ConfigurationType to CObjectMassPoint::GetPosition; 
//  return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMassPoint::GetVelocity(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2*)GetCNode(0))->GetVelocity(configuration);
}

//! return the (global) position of "localPosition" according to configuration type
Vector3D CObjectMassPoint::GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration) const
{
	return ((CNodeODE2*)GetCNode(0))->GetPosition(configuration) - ((CNodeODE2*)GetCNode(0))->GetPosition(ConfigurationType::Reference); //this also works for NodePointGround
}

//! call a certain function of object (autogenerated in future!)
py::object MainObjectMassPoint::CallFunction(STDstring functionName, py::dict args) const
{
	//these calls should be automated by python script ...
	if (functionName == "GetTypeName")
	{
		return py::str(GetTypeName());
	}
	else if (functionName == "GetType")
	{
		return py::cast(GetCObjectBody()->GetType());
	}
	else if (functionName == "GetOutputVariableTypes")
	{
		return py::cast(GetCObject()->GetOutputVariableTypes());
	}
	else if (functionName == "GetPosition")
	{
		Vector3D locPos;
		STDstring configStr;
		ConfigurationType configType = ConfigurationType::None;
		HPyUtils::SetVector3DSafely(args, "localPosition", locPos);
		HPyUtils::SetStringSafely(args, "configuration", configStr);
		if (configStr == "Current") { configType = ConfigurationType::Current; }
		else if (configStr == "Initial") { configType = ConfigurationType::Initial; }
		else if (configStr == "Reference") { configType = ConfigurationType::Reference; }

		Vector3D v(GetCObjectBody()->GetPosition(locPos, configType));
		return py::array_t<Real>(v.NumberOfItems(), v.GetDataPointer());
	}
	else if (functionName == "GetNodeNumber")
	{
		Index localNode = py::cast<Index>(args["localIndex"]);
		return py::int_(GetCObjectBody()->GetNodeNumber(localNode));
	}
	else if (functionName == "GetNumberOfNodes")
	{
		return py::int_(GetCObjectBody()->GetNumberOfNodes());
	}
	else if (functionName == "GetODE2Size")
	{
		return py::int_(GetCObject()->GetODE2Size());
	}
	else if (functionName == "GetNumberOfCoordinates")
	{
		return py::int_(GetCObject()->GetNumberOfCoordinates());
	}
	PyError(STDstring("MainObjectMassPoint::CallFunction called with invalid functionName '" + functionName + "'"));
	return py::int_(EXUstd::InvalidIndex);
}
