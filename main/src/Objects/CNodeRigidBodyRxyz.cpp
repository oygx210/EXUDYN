/** ***********************************************************************************************
* @brief		Implementation for NodeRigidBodyRxyz
*
* @author		Gerstmayr Johannes
* @date			2019-10-19 (generated)
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
#include "Autogenerated/CNodeRigidBodyRxyz.h"
#include "Utilities/RigidBodyMath.h"

//for CallFunction(...)
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainNodeRigidBodyRxyz.h"

// Compute vector to of 3 Euler angles from reference and configuration coordinates
ConstSizeVector<CNodeRigidBody::maxRotationCoordinates> CNodeRigidBodyRxyz::GetRotationParameters(ConfigurationType configuration) const
{
	LinkedDataVector refCoordinates = GetReferenceCoordinateVector();
	
	ConstSizeVector<maxRotationCoordinates> rot({ refCoordinates[nDisplacementCoordinates + 0], //these are the reference values
		refCoordinates[nDisplacementCoordinates + 1],
		refCoordinates[nDisplacementCoordinates + 2]}); //reference values for Euler angles

	if (configuration != ConfigurationType::Reference)
	{
		LinkedDataVector coordinates = GetCoordinateVector(configuration);
		rot[0] += coordinates[nDisplacementCoordinates + 0];
		rot[1] += coordinates[nDisplacementCoordinates + 1];
		rot[2] += coordinates[nDisplacementCoordinates + 2];
	}

	return rot;
}

// Link vector to time derivative of 3 Euler angles in given configuration
LinkedDataVector CNodeRigidBodyRxyz::GetRotationParameters_t(ConfigurationType configuration) const
{
	return LinkedDataVector(GetCoordinateVector_t(configuration), nDisplacementCoordinates, nRotationCoordinates);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
void CNodeRigidBodyRxyz::GetG(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::RotXYZ2GTemplate<CSVector4D>(GetRotationParameters(configuration));
}

//! Compute local G matrix for given configuration
void CNodeRigidBodyRxyz::GetGlocal(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::RotXYZ2GlocalTemplate<CSVector4D>(GetRotationParameters(configuration));
}

//! Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
void CNodeRigidBodyRxyz::GetG_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::RotXYZ2G_tTemplate<CSVector4D, LinkedDataVector>(GetRotationParameters(configuration), GetRotationParameters_t(configuration));
}

//! Compute local G matrix for given configuration
void CNodeRigidBodyRxyz::GetGlocal_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration) const
{
	matrix = RigidBodyMath::RotXYZ2Glocal_tTemplate<CSVector4D, LinkedDataVector>(GetRotationParameters(configuration), GetRotationParameters_t(configuration));
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Vector3D CNodeRigidBodyRxyz::GetPosition(ConfigurationType configuration) const
{
	//compute reference coordinate vector (this is the position/orientation of the node without displacements)
	LinkedDataVector pRef3D = GetReferenceCoordinateVector();
	Vector3D pRef({ pRef3D[0], pRef3D[1], pRef3D[2] });
	if (configuration == ConfigurationType::Reference) { return pRef; }

	//compute displacement vector (this is the displacement/rotation superimposed to reference configuration)
	LinkedDataVector u3D = GetCoordinateVector(configuration);
	Vector3D displacement({ u3D[0], u3D[1], u3D[2] });
	
	return pRef + displacement; //valid, because position and displacement is still additive!
}

Vector3D CNodeRigidBodyRxyz::GetVelocity(ConfigurationType configuration) const
{
	LinkedDataVector u3D_t = GetCoordinateVector_t(configuration);
	return Vector3D({ u3D_t[0], u3D_t[1], u3D_t[2] });
}

Matrix3D CNodeRigidBodyRxyz::GetRotationMatrix(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> rot(GetRotationParameters(configuration));
	return RigidBodyMath::RotXYZ2RotationMatrix(rot);
}

//! return configuration dependent angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyRxyz::GetAngularVelocity(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> rot(GetRotationParameters(configuration));
	LinkedDataVector rot_t(GetRotationParameters_t(configuration));

	Vector3D omega;
	EXUmath::MultMatrixVector(RigidBodyMath::RotXYZ2GTemplate<CSVector4D>(rot), rot_t, omega);

	return omega;
}

//! return configuration dependent local (=body fixed) angular velocity of node; returns always a 3D Vector
Vector3D CNodeRigidBodyRxyz::GetAngularVelocityLocal(ConfigurationType configuration) const
{
	ConstSizeVector<maxRotationCoordinates> rot(GetRotationParameters(configuration));
	LinkedDataVector rot_t(GetRotationParameters_t(configuration));

	Vector3D omegaLocal;
	EXUmath::MultMatrixVector(RigidBodyMath::RotXYZ2GlocalTemplate<CSVector4D>(rot), rot_t, omegaLocal);

	return omegaLocal;
}

//! AUTO:  provide position jacobian of node; derivative of 3D Position with respect to 3 displacement coordinates \f$[q_0,\,q_1,\,q_2]\tp\f$ and 3 rotation coordinates \f$[\psi_0,\,\psi_1,\,\psi_2]\tp\f$
void CNodeRigidBodyRxyz::GetPositionJacobian(Matrix& value) const
{
	value.SetNumberOfRowsAndColumns(3, nDisplacementCoordinates + nRotationCoordinates);
	value.SetAll(0.);
	value(0, 0) = 1.;
	value(1, 1) = 1.;
	value(2, 2) = 1.;
}

//! AUTO:  provide 'rotation' jacobian \f$\Jm_R\f$ of node; derivative of 3D angular velocity vector with respect to all velocity coordinates ('G-matrix'); action of torque \f$\mv\f$: \f$\Qm_m = \Jm_R^T \mv\f$
void CNodeRigidBodyRxyz::GetRotationJacobian(Matrix& value) const
{
	value.SetNumberOfRowsAndColumns(3, nDisplacementCoordinates + nRotationCoordinates);
	value.SetAll(0.);

	ConstSizeMatrix<3 * maxRotationCoordinates> G;
	GetG(G);

	for (Index i = 0; i < 3; i++) //dimensionality
	{
		for (Index j = 0; j < nRotationCoordinates; j++)
		{
			value(i, j + nDisplacementCoordinates) = G(i, j);
		}
	}
}


//! provide according output variable in "value"
void CNodeRigidBodyRxyz::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Position: value.CopyFrom(GetPosition(configuration)); break;
	case OutputVariableType::Displacement: value.CopyFrom(GetPosition(configuration) - GetPosition(ConfigurationType::Reference)); break;
	case OutputVariableType::Velocity: value.CopyFrom(GetVelocity(configuration)); break;
	case OutputVariableType::AngularVelocity: value.CopyFrom(GetAngularVelocity(configuration)); break;
	case OutputVariableType::AngularVelocityLocal: value.CopyFrom(GetAngularVelocityLocal(configuration)); break;
	case OutputVariableType::RotationMatrix: {
		Matrix3D rot = GetRotationMatrix(configuration);
		value.SetVector(9, rot.GetDataPointer());
		break;
	}
	case OutputVariableType::Rotation: {
		value.CopyFrom(GetRotationParameters(configuration));
		break;
	}
	case OutputVariableType::Coordinates:
	{
		if (IsConfigurationInitialCurrentReferenceVisualization(configuration)) //((Index)configuration & ((Index)ConfigurationType::Current + (Index)ConfigurationType::Initial + (Index)ConfigurationType::Reference + (Index)ConfigurationType::Visualization))
		{
			value = GetCoordinateVector(configuration);
		}
		else
		{
			PyError("CNodeRigidBodyRxyz::GetOutputVariable: invalid configuration");
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
			PyError("CNodeRigidBodyRxyz::GetOutputVariable: invalid configuration");
		}
		break;
	}
	default:
		SysError("CNodeRigidBodyRxyz::GetOutputVariable failed"); //error should not occur, because types are checked!
	}
}

//! call a certain function of object (autogenerated in future!)
py::object MainNodeRigidBodyRxyz::CallFunction(STDstring functionName, py::dict args) const
{

	PyError(STDstring("MainNodeRigidBodyRxyz::CallFunction called with invalid functionName '" + functionName + "'"));
	return py::int_(EXUstd::InvalidIndex);
}
