/** ***********************************************************************************************
* @brief		Implementation for MainNode
*
* @author		Gerstmayr Johannes
* @date			2019-07-23 (generated)
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

#include "Main/MainSystem.h"
#include "Pymodules/PybindUtilities.h"

//implementation of functions for specific nodes:
#include "Autogenerated/MainNodeGenericData.h"
#include "Autogenerated/MainNodeGenericODE2.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//some specific functions of nodes, which are not put into separate .cpp files:
bool MainNodeGenericData::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CNodeGenericData* cNode = (CNodeGenericData*)GetCNode();
	//Index dataNodeNumber = cNode->GetNodeNumber(0);

	Index len = cNode->GetParameters().numberOfDataCoordinates;
	if (len != GetInitialVector().NumberOfItems())
	{
		errorString = "MainNodeGenericData: numberOfDataCoordinates (size=" + EXUstd::ToString(len) + ") must have same size as initialCoordinates (size=" + 
			EXUstd::ToString(GetInitialVector().NumberOfItems()) + ")";
		return false;
	}

	return true;
}

bool MainNodeGenericODE2::CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const
{
	CNodeGenericODE2* cNode = (CNodeGenericODE2*)GetCNode();
	//Index dataNodeNumber = cNode->GetNodeNumber(0);

	Index len = cNode->GetParameters().numberOfODE2Coordinates;
	if (len != GetInitialVector().NumberOfItems())
	{
		errorString = "MainNodeGenericODE2: numberOfODE2Coordinates (size=" + EXUstd::ToString(len) + ") must have same size as initialCoordinates (size=" + 
			EXUstd::ToString(GetInitialVector().NumberOfItems()) + ")";
		return false;
	}

	if (len != GetInitialVector_t().NumberOfItems())
	{
		errorString = "MainNodeGenericODE2: numberOfODE2Coordinates (size=" + EXUstd::ToString(len) + ") must have same size as initialCoordinates_t (size=" +
			EXUstd::ToString(GetInitialVector_t().NumberOfItems()) + ")";
		return false;
	}

	if (len != cNode->GetReferenceCoordinateVector().NumberOfItems())
	{
		errorString = "MainNodeGenericODE2: numberOfODE2Coordinates (size=" + EXUstd::ToString(len) + ") must have same size as referenceCoordinates (size=" +
			EXUstd::ToString(cNode->GetReferenceCoordinateVector().NumberOfItems()) + ")";
		return false;
	}

	return true;
}


//! GetOutputVariable with type and return value; copies values==>slow!; can be scalar or vector-valued! maps to CNode GetOutputVariable(...)
py::object MainNode::GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration) const
{
	Vector value;
	//check if type is valid:
	if ((Index)GetCNode()->GetOutputVariableTypes() & (Index)variableType)
	{
		GetCNode()->GetOutputVariable(variableType, configuration, value);

		//now check if it is scalar or a vector-valued:
		if (value.NumberOfItems() == 1) { return py::float_(value[0]); }
		else { return py::array_t<Real>(value.NumberOfItems(), value.GetDataPointer()); }
	}
	else
	{
		PyError(STDstring("Invalid OutputVariableType in MainNode::GetOutputVariable: '") + GetOutputVariableTypeString(variableType) + "'");
		return py::int_(EXUstd::InvalidIndex);
		//return py::object();
	}
}
