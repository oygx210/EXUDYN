/** ***********************************************************************************************
* @class        CObjectConnectorDistanceParameters
* @brief        Parameter class for CObjectConnectorDistance
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2019-12-18  22:06:39 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"


//! AUTO: Parameters for class CObjectConnectorDistanceParameters
class CObjectConnectorDistanceParameters // AUTO: 
{
public: // AUTO: 
    ArrayIndex markerNumbers;                     //!< AUTO: list of markers used in connector
    Real distance;                                //!< AUTO: prescribed distance [SI:m] of the used markers
    bool activeConnector;                         //!< AUTO: flag, which determines, if the connector is active; used to deactivate (temorarily) a connector or constraint
    //! AUTO: default constructor with parameter initialization
    CObjectConnectorDistanceParameters()
    {
        markerNumbers = ArrayIndex({ EXUstd::InvalidIndex, EXUstd::InvalidIndex });
        distance = 0.;
        activeConnector = true;
    };
};


/** ***********************************************************************************************
* @class        CObjectConnectorDistance
* @brief        Connector which enforces constant or prescribed distance between two bodies/nodes.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: CObjectConnectorDistance
class CObjectConnectorDistance: public CObjectConstraint // AUTO: 
{
protected: // AUTO: 
    CObjectConnectorDistanceParameters parameters; //! AUTO: contains all parameters for CObjectConnectorDistance

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CObjectConnectorDistanceParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CObjectConnectorDistanceParameters& GetParameters() const { return parameters; }

    //! AUTO:  default function to return Marker numbers
    virtual const ArrayIndex& GetMarkerNumbers() const override
    {
        return parameters.markerNumbers;
    }

    //! AUTO:  constraints uses Lagrance multiplier formulation
    virtual bool IsPenaltyConnector() const override
    {
        return false;
    }

    //! AUTO:  constraint also implements velocity level equations
    virtual bool HasVelocityEquations() const override
    {
        return true;
    }

    //! AUTO:  Computational function: compute algebraic equations and write residual into "algebraicEquations"; velocityLevel: equation provided at velocity level
    virtual void ComputeAlgebraicEquations(Vector& algebraicEquations, const MarkerDataStructure& markerData, bool velocityLevel = false) const override;

    //! AUTO:  compute derivative of algebraic equations w.r.t. ODE2 in jacobian [and w.r.t. ODE2_t coordinates in jacobian_t if flag ODE2_t_AE_function is set] [and w.r.t. AE coordinates if flag AE_AE_function is set in GetAvailableJacobians()]; jacobian[_t] has dimension GetAlgebraicEquationsSize() x (GetODE2Size() + GetODE1Size() [+GetAlgebraicEquationsSize()]); q are the system coordinates; markerData provides according marker information to compute jacobians
    virtual void ComputeJacobianAE(ResizableMatrix& jacobian, ResizableMatrix& jacobian_t, ResizableMatrix& jacobian_AE, const MarkerDataStructure& markerData) const override;

    //! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags; available jacobians is switched depending on velocity level and on activeConnector condition
    virtual JacobianType::Type GetAvailableJacobians() const override;

    //! AUTO:  Flags to determine, which output variables are available (displacment, velocity, stress, ...)
    virtual OutputVariableType GetOutputVariableTypes() const override;

    //! AUTO:  provide according output variable in "value"
    virtual void GetOutputVariableConnector(OutputVariableType variableType, const MarkerDataStructure& markerData, Vector& value) const override;

    //! AUTO:  provide requested markerType for connector
    virtual Marker::Type GetRequestedMarkerType() const override
    {
        return Marker::Position;
    }

    //! AUTO:  return object type (for node treatment in computation)
    virtual CObjectType GetType() const override
    {
        return (CObjectType)((Index)CObjectType::Connector + (Index)CObjectType::Constraint);
    }

    //! AUTO:  number of algebraic equations; independent of node/body coordinates
    virtual Index GetAlgebraicEquationsSize() const override
    {
        return 1;
    }

    //! AUTO:  return if connector is active-->speeds up computation
    virtual bool IsActive() const override
    {
        return parameters.activeConnector;
    }

};


