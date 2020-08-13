/** ***********************************************************************************************
* @brief        CObjectConnectorCoordinateVector implementation
*
* @author       Gerstmayr Johannes
* @date         2018-06-14 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Utilities/ExceptionsTemplates.h"
#include "Main/CSystemData.h"
#include "Autogenerated/CObjectConnectorCoordinateVector.h"

//for consistency checks:
#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"
#include "Autogenerated/MainObjectConnectorCoordinateVector.h"

bool MainObjectConnectorCoordinateVector::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CObjectConnectorCoordinateVector* cObject = (CObjectConnectorCoordinateVector*)GetCObject();
	
	Index m0Columns = cObject->GetParameters().scalingMarker0.NumberOfColumns();
	Index m0Rows = cObject->GetParameters().scalingMarker0.NumberOfRows();
	//Index m1Columns = cObject->GetParameters().scalingMarker1.NumberOfColumns();
	Index m1Rows = cObject->GetParameters().scalingMarker1.NumberOfRows();
	Index offsetLength = cObject->GetParameters().offset.NumberOfItems();

	if ((m0Rows != m1Rows && m0Rows != 0 && m1Rows != 0) || (m0Rows == 0 && m1Rows == 0))
	{
		errorString = "ObjectConnectorCoordinateVector: number of rows in scaling matrices must be identical or one scaling matrix can have zero rows";
		return false;
	}

	if (offsetLength != 0 && offsetLength != cObject->GetAlgebraicEquationsSize())
	{
		errorString = "ObjectConnectorCoordinateVector: length of offset vector must be either 0 or identical to number of rows of scaling matrices";
		return false;
	}

	//marker numbers have already been checked in CSystem::CheckSystemIntegrity
	const ArrayIndex& nMarkers = cObject->GetMarkerNumbers();
	const CMarker& marker0 = mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[0]);
	const CMarker& marker1 = mainSystem.GetCSystem()->GetSystemData().GetCMarker(nMarkers[1]);

	if (!(marker0.GetType() & Marker::Coordinate))
	{
		errorString = STDstring("ObjectConnectorCoordinateVector: Marker 0 must be of type = 'Coordinate'");
		return false;
	}

	if (!(marker1.GetType() & Marker::Coordinate))
	{
		errorString = STDstring("ObjectConnectorCoordinateVector: Marker 1 must be of type = 'Coordinate'");
		return false;
	}

	if (marker0.GetDimension(mainSystem.GetCSystem()->GetSystemData()) != m0Columns && m0Columns != 0)
	{
		errorString = STDstring("ObjectConnectorCoordinateVector: marker 0 must have number of (marker) coordinates same as the number of columns in scalingMarker0");
		return false;
	}

	if (marker1.GetDimension(mainSystem.GetCSystem()->GetSystemData()) != m0Columns && m0Columns != 0)
	{
		errorString = STDstring("ObjectConnectorCoordinateVector: marker 1 must have number of (marker) coordinates same as the number of columns in scalingMarker1");
		return false;
	}

	return true;
}

//! number of algebraic equations; independent of node/body coordinates
Index CObjectConnectorCoordinateVector::GetAlgebraicEquationsSize() const
{
	//one scaling matrix could have zero rows, but not both! checked in CheckPreAssembleConsistency
	return EXUstd::Maximum(parameters.scalingMarker0.NumberOfRows(), parameters.scalingMarker1.NumberOfRows()); //number of algebraic equations
}

//! Computational function: compute algebraic equations and write residual into "algebraicEquations"
void CObjectConnectorCoordinateVector::ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, Real t, bool velocityLevel) const
{
	if (parameters.activeConnector)
	{
		Index nAE = GetAlgebraicEquationsSize();
		algebraicEquations.SetNumberOfItems(nAE);
		if (!velocityLevel && !parameters.velocityLevel)
		{
			//algebraicEquations.SetAll(0.);

			//C(q,q_t,t) = scalingMarker1*qMarker1 - scalingMarker0*qMarker0 - offset = 0
			if (markerData.GetMarkerData(0).vectorValue.NumberOfItems() != 0 && parameters.scalingMarker0.NumberOfColumns() != 0)
			{
				EXUmath::MultMatrixVector(parameters.scalingMarker0, markerData.GetMarkerData(0).vectorValue, algebraicEquations);
				algebraicEquations *= -1.;
			}
			else
			{
				algebraicEquations.SetAll(0.);
			}
			if (markerData.GetMarkerData(1).vectorValue.NumberOfItems() != 0 && parameters.scalingMarker1.NumberOfColumns() != 0)
			{
				EXUmath::MultMatrixVectorAdd(parameters.scalingMarker1, markerData.GetMarkerData(1).vectorValue, algebraicEquations);
			}

			//if (parameters.offsetUserFunction)
			//{
			//	Vector offset;
			//	//user function args:(deltaL, deltaL_t, Real stiffness, Real damping, Real offset, Real dryFriction, Real dryFrictionProportionalZone)
			//	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
			//	{
			//		offset = parameters.offsetUserFunction(t, parameters.offset);
			//	}, "ObjectConnectorCoordinateVector::offsetUserFunction");

			//	CHECKandTHROW(offset.NumberOfItems() == algebraicEquations.NumberOfItems(), "CObjectConnectorCoordinateVector::offsetUserFunction: return value has incorrect size");
			//	algebraicEquations -= offset;

			//}
			//else
			{
				if (parameters.offset.NumberOfItems() != 0)
				{
					algebraicEquations -= parameters.offset;
				}
			}
		}
		else
		{
			CHECKandTHROW(markerData.GetMarkerData(1).velocityAvailable && markerData.GetMarkerData(0).velocityAvailable,
				"CObjectConnectorCoordinateVector::ComputeAlgebraicEquations: marker do not provide velocityLevel information");

			//C(q,q_t,t) = scalingMarker1*qMarker1_t - scalingMarker0*qMarker0_t = 0
			if (markerData.GetMarkerData(0).vectorValue_t.NumberOfItems() != 0 && parameters.scalingMarker0.NumberOfColumns() != 0)
			{
				EXUmath::MultMatrixVector(parameters.scalingMarker0, markerData.GetMarkerData(0).vectorValue_t, algebraicEquations);
				algebraicEquations *= -1.;
			}
			else
			{
				algebraicEquations.SetAll(0.);
			}
			if (markerData.GetMarkerData(1).vectorValue_t.NumberOfItems() != 0 && parameters.scalingMarker1.NumberOfColumns() != 0)
			{
				EXUmath::MultMatrixVectorAdd(parameters.scalingMarker1, markerData.GetMarkerData(1).vectorValue_t, algebraicEquations);
			}



			
			//if (parameters.offsetUserFunction_t)
			//{
			//	Vector offset;
			//	UserFunctionExceptionHandling([&] //lambda function to add consistent try{..} catch(...) block
			//	{
			//		offset = parameters.offsetUserFunction_t(t, parameters.offset);
			//	}, "ObjectConnectorCoordinate::offsetUserFunction_t");
			//	CHECKandTHROW(offset.NumberOfItems() == algebraicEquations.NumberOfItems(), "CObjectConnectorCoordinateVector::offsetUserFunction_t: return value has incorrect size");
			//	algebraicEquations -= offset;
			//}
			//else
			{
				if (parameters.velocityLevel) 
				{ 
					if (parameters.offset.NumberOfItems() != 0)
					{
						algebraicEquations -= parameters.offset;
					}
				}
			}
		}
	}
	else
	{
		algebraicEquations.CopyFrom(markerData.GetLagrangeMultipliers()); //equation lambda = 0, means that the current values need to be copied
	}

}

void CObjectConnectorCoordinateVector::ComputeJacobianAE(ResizableMatrix& jacobian, ResizableMatrix& jacobian_t, ResizableMatrix& jacobian_AE, 
	const MarkerDataStructure& markerData, Real t) const
{
	Index nAE = GetAlgebraicEquationsSize();
	if (parameters.activeConnector)
	{
		ResizableMatrix* usedJac;
		if (parameters.velocityLevel) //in this case, always the jacobian_t must be used
		{
			usedJac = &jacobian_t;
			jacobian.SetNumberOfRowsAndColumns(0, 0); 
		}
		else
		{
			usedJac = &jacobian;
			jacobian_t.SetNumberOfRowsAndColumns(0, 0); 
		}

		usedJac->SetNumberOfRowsAndColumns(nAE, markerData.GetMarkerData(0).jacobian.NumberOfColumns()
			+ markerData.GetMarkerData(1).jacobian.NumberOfColumns());

		//needs memory allocation:
		ResizableMatrix temp;
		if (markerData.GetMarkerData(0).jacobian.NumberOfColumns() != 0 && parameters.scalingMarker0.NumberOfColumns() != 0)
		{
			EXUmath::MultMatrixMatrixTemplate<Matrix, ResizableMatrix,ResizableMatrix>(parameters.scalingMarker0, markerData.GetMarkerData(0).jacobian, temp);
			usedJac->SetSubmatrix(temp, 0, 0, -1.);
		}
		if (markerData.GetMarkerData(1).jacobian.NumberOfColumns() != 0 && parameters.scalingMarker1.NumberOfColumns() != 0)
		{
			EXUmath::MultMatrixMatrixTemplate<Matrix, ResizableMatrix, ResizableMatrix>(parameters.scalingMarker1, markerData.GetMarkerData(1).jacobian, temp);
			usedJac->SetSubmatrix(temp, 0, markerData.GetMarkerData(0).jacobian.NumberOfColumns());
		}
	}
	else
	{
		jacobian_AE.SetScalarMatrix(nAE, 1.); //represents derivative of algebraic equation 'lambda = 0'
		//jacobian_t.SetNumberOfRowsAndColumns(0, 0); //not necessary, because of CObjectConnectorCoordinateVector::GetAvailableJacobians()
		//jacobian.SetNumberOfRowsAndColumns(0, 0);	  ////not necessary, because of CObjectConnectorCoordinateVector::GetAvailableJacobians()
	}

}

JacobianType::Type CObjectConnectorCoordinateVector::GetAvailableJacobians() const
{
	if (parameters.activeConnector)
	{
		if (parameters.velocityLevel)
		{
			return (JacobianType::Type)(JacobianType::AE_ODE2_t + JacobianType::AE_ODE2_t_function);
		}
		else
		{
			return (JacobianType::Type)(JacobianType::AE_ODE2 + JacobianType::AE_ODE2_function);
		}
	}
	else //deactivated constraint means that only the lagrange multiplier needs to be set to zero
	{
		return (JacobianType::Type)(JacobianType::AE_AE + JacobianType::AE_AE_function);
	}
}


//! provide according output variable in "value"
void CObjectConnectorCoordinateVector::GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const
{
	switch (variableType)
	{
	case OutputVariableType::Displacement: 
	{
		value.CopyFrom(markerData.GetMarkerData(1).vectorValue);
		value -= markerData.GetMarkerData(0).vectorValue;
		break;
	}
	case OutputVariableType::Velocity: 
	{
		value.CopyFrom(markerData.GetMarkerData(1).vectorValue_t);
		value -= markerData.GetMarkerData(0).vectorValue_t;
		break;
	}
	case OutputVariableType::ConstraintEquation: 
	{
		Real t = GetCSystemData()->GetCData().GetCurrent().GetTime();
		bool velocityLevel = false;
		Vector algebraicEquations;
		ComputeAlgebraicEquations(algebraicEquations, markerData, t, velocityLevel);
		value.CopyFrom(algebraicEquations); 
		break;
	}
	case OutputVariableType::Force: value.CopyFrom(markerData.GetLagrangeMultipliers()); break;
	default:
		SysError("CObjectConnectorCoordinateVector::GetOutputVariableConnector failed"); //error should not occur, because types are checked!
	}

}



