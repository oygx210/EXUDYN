/** ***********************************************************************************************
* @class        CNodeRigidBody2DParameters
* @brief        Parameter class for CNodeRigidBody2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2019-10-20  21:40:13 (last modfied)
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


//! AUTO: Parameters for class CNodeRigidBody2DParameters
class CNodeRigidBody2DParameters // AUTO: 
{
public: // AUTO: 
    Vector3D referenceCoordinates;                //!< AUTO: reference coordinates (x-pos,y-pos and rotation phi) of node ==> e.g. ref. coordinates for finite elements; global position of node without displacement
    //! AUTO: default constructor with parameter initialization
    CNodeRigidBody2DParameters()
    {
        referenceCoordinates = Vector3D({0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        CNodeRigidBody2D
* @brief        A 2D rigid body node for rigid bodies or beams; the node has 2 displacement degrees of freedom (displacement of center of mass - COM: ux,uy) and one rotation coordinate (rotation around z-axis: uphi); all coordinates lead to second order differential equations; The rotation matrix \f$\Am\f$, transforming local (body-fixed) 3D positions \f$\pv_{loc} = [p^x_{loc}\;\;p^y_{loc}\;\;0]^T\f$ to global 3D positions \f$\pv_{glob} = [p^x_{glob}\;\;p^y_{glob}\;\;p^z_{glob}]^T\f$, \f[ \pv_{glob} = \Am \pv_{loc}, \f] is defined as \f[ \Am = \mp{\cos(\varphi)}{-\sin(\varphi)}{\sin(\varphi)}{\cos(\varphi)}.\f]
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

//! AUTO: CNodeRigidBody2D
class CNodeRigidBody2D: public CNodeODE2 // AUTO: 
{
protected: // AUTO: 
    CNodeRigidBody2DParameters parameters; //! AUTO: contains all parameters for CNodeRigidBody2D

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CNodeRigidBody2DParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CNodeRigidBody2DParameters& GetParameters() const { return parameters; }

    //! AUTO:  return number of second order diff. eq. coordinates
    virtual Index GetNumberOfODE2Coordinates() const override
    {
        return 3;
    }

    //! AUTO:  return node type (for node treatment in computation)
    virtual CNodeType GetType() const override
    {
        return CNodeType::RigidBody;
    }

    //! AUTO:  return configuration dependent position of node; returns always a 3D Vector
    virtual Vector3D GetPosition(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent velocity of node; returns always a 3D Vector
    virtual Vector3D GetVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Vector
    virtual Matrix3D GetRotationMatrix(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent local (=body fixed) angular velocity of node; in 2D case, this is the same as the global angular velocity; returns always a 3D Vector
    virtual Vector3D GetAngularVelocityLocal(ConfigurationType configuration = ConfigurationType::Current) const override
    {
        return GetAngularVelocity(configuration);
    }

    //! AUTO:  provide position jacobian of node; derivative of 3D Position with respect to 3 coordinates ux,uy,uphi
    virtual void GetPositionJacobian(Matrix& value) const override
    {
        value.SetMatrix(3,3,{1.f,0.f,0.f, 0.f,1.f,0.f, 0.f,0.f,0.f});
    }

    //! AUTO:  provide "rotation" jacobian \f$\Jm_R\f$ of node; derivative of 3D angular velocity vector with respect to all velocity coordinates ("G-matrix"); action of torque \f$\mv\f$: \f$\Qm_m = \Jm_R^T \mv\f$
    virtual void GetRotationJacobian(Matrix& value) const override
    {
        value.SetMatrix(3,3,{0.f,0.f,0.f, 0.f,0.f,0.f, 0.f,0.f,1.f});
    }

    //! AUTO:  return internally stored reference coordinates of node
    virtual LinkedDataVector GetReferenceCoordinateVector() const override
    {
        return parameters.referenceCoordinates;
    }

    //! AUTO:  provide according output variable in "value"; used e.g. for postprocessing and sensors
    virtual void GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const override;

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::Coordinates +
            (Index)OutputVariableType::Coordinates_t );
    }

};


