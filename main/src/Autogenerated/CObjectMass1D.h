/** ***********************************************************************************************
* @class        CObjectMass1DParameters
* @brief        Parameter class for CObjectMass1D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-05-24  21:43:39 (last modfied)
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


//! AUTO: Parameters for class CObjectMass1DParameters
class CObjectMass1DParameters // AUTO: 
{
public: // AUTO: 
    Real physicsMass;                             //!< AUTO: mass [SI:kg] of mass
    Index nodeNumber;                             //!< AUTO: node number of Node1D providing translation coordinate \f$c_0\f$
    Vector3D referencePosition;                   //!< AUTO: a reference position, used to transform the 1D coordinate to a position
    Matrix3D referenceRotation;                   //!< AUTO: the constant body rotation matrix, which transforms body-fixed (b) to global (0) coordinates
    //! AUTO: default constructor with parameter initialization
    CObjectMass1DParameters()
    {
        physicsMass = 0.;
        nodeNumber = EXUstd::InvalidIndex;
        referencePosition = Vector3D({0.,0.,0.});
        referenceRotation = EXUmath::unitMatrix3D;
    };
};


/** ***********************************************************************************************
* @class        CObjectMass1D
* @brief        A 1D (translational) mass which is attached to Node1D. Note, that the mass does not need to have the interpretation as a translational mass.
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

//! AUTO: CObjectMass1D
class CObjectMass1D: public CObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectMass1DParameters parameters; //! AUTO: contains all parameters for CObjectMass1D

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CObjectMass1DParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CObjectMass1DParameters& GetParameters() const { return parameters; }

    //! AUTO:  Computational function: compute mass matrix
    virtual void ComputeMassMatrix(Matrix& massMatrix) const override;

    //! AUTO:  Computational function: compute right-hand-side (RHS) of second order ordinary differential equations (ODE) to 'ode2rhs'
    virtual void ComputeODE2RHS(Vector& ode2Rhs) const override;

    //! AUTO:  return the available jacobian dependencies and the jacobians which are available as a function; if jacobian dependencies exist but are not available as a function, it is computed numerically; can be combined with 2^i enum flags
    virtual JacobianType::Type GetAvailableJacobians() const override
    {
        return JacobianType::_None;
    }

    //! AUTO:  Flags to determine, which access (forces, moments, connectors, ...) to object are possible
    virtual AccessFunctionType GetAccessFunctionTypes() const override;

    //! AUTO:  provide Jacobian at localPosition in 'value' according to object access
    virtual void GetAccessFunctionBody(AccessFunctionType accessType, const Vector3D& localPosition, Matrix& value) const override;

    //! AUTO:  provide according output variable in 'value'
    virtual void GetOutputVariableBody(OutputVariableType variableType, const Vector3D& localPosition, ConfigurationType configuration, Vector& value) const override;

    //! AUTO:  return the (global) position of 'localPosition' according to configuration type
    virtual Vector3D GetPosition(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return the (global) position of 'localPosition' according to configuration type
    virtual Vector3D GetDisplacement(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return the (global) velocity of 'localPosition' according to configuration type
    virtual Vector3D GetVelocity(const Vector3D& localPosition, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return the local position of the center of mass, needed for equations of motion and for massProportionalLoad
    virtual Vector3D GetLocalCenterOfMass() const override
    {
        return Vector3D({0.,0.,0.});
    }

    //! AUTO:  Get global node number (with local node index); needed for every object ==> does local mapping
    virtual Index GetNodeNumber(Index localIndex) const override
    {
        release_assert(localIndex == 0);
        return parameters.nodeNumber;
    }

    //! AUTO:  number of nodes; needed for every object
    virtual Index GetNumberOfNodes() const override
    {
        return 1;
    }

    //! AUTO:  number of ODE2 coordinates; needed for object?
    virtual Index GetODE2Size() const override
    {
        return 1;
    }

    //! AUTO:  Get type of object, e.g. to categorize and distinguish during assembly and computation
    virtual CObjectType GetType() const override
    {
        return (CObjectType)((Index)CObjectType::Body + (Index)CObjectType::SingleNoded);
    }

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::RotationMatrix +
            (Index)OutputVariableType::Rotation +
            (Index)OutputVariableType::AngularVelocity +
            (Index)OutputVariableType::AngularVelocityLocal );
    }

};


