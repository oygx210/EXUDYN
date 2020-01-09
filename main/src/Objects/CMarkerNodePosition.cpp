/** ***********************************************************************************************
* @brief        Implementation for MarkerNodePosition
*
* @author       Gerstmayr Johannes
* @date         2019-06-13 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */

#include "Main/CSystemData.h"
#include "Autogenerated/CMarkerNodePosition.h"


void CMarkerNodePosition::GetPosition(const CSystemData& cSystemData, Vector3D& position, ConfigurationType configuration) const
{
	position = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPosition(configuration);
}

void CMarkerNodePosition::GetVelocity(const CSystemData& cSystemData, Vector3D& velocity, ConfigurationType configuration) const
{
	velocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetVelocity(configuration);
}

void CMarkerNodePosition::ComputeMarkerData(const CSystemData& cSystemData, bool computeJacobian, MarkerData& markerData) const
{
	//A marker 'NodePosition' must be attached to the ODE2 part of a node
	markerData.position = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPosition(ConfigurationType::Current);
	markerData.velocity = ((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetVelocity(ConfigurationType::Current);
	markerData.velocityAvailable = true;

	if (computeJacobian)
	{
		((CNodeODE2*)(cSystemData.GetCNodes()[parameters.nodeNumber]))->GetPositionJacobian(markerData.positionJacobian);
	}
}

