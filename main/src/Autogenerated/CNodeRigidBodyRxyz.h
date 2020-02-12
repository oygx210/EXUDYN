/** ***********************************************************************************************
* @class        CNodeRigidBodyRxyzParameters
* @brief        Parameter class for CNodeRigidBodyRxyz
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-02-05  00:03:34 (last modfied)
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


//! AUTO: Parameters for class CNodeRigidBodyRxyzParameters
class CNodeRigidBodyRxyzParameters // AUTO: 
{
public: // AUTO: 
    Vector6D referenceCoordinates;                //!< AUTO: reference coordinates (x-pos,y-pos,z-pos and 3 xyz Euler angles) of node ==> e.g. ref. coordinates for finite elements or reference position of rigid body (e.g. for definition of joints)
    //! AUTO: default constructor with parameter initialization
    CNodeRigidBodyRxyzParameters()
    {
        referenceCoordinates = Vector6D({0.,0.,0., 0.,0.,0.});
    };
};


/** ***********************************************************************************************
* @class        CNodeRigidBodyRxyz
* @brief        A 3D rigid body node based on Euler / Tait-Bryan angles for rigid bodies or beams; the node has 3 displacement coordinates (displacements of center of mass - COM: \f$[u_x,u_y,u_z]\f$) and three rotation coordinates (angles \f$[\varphi_x,\varphi_y,\varphi_z]\f$ for rotations around x,y, and z-axis); all coordinates lead to second order differential equations; The rotation matrix \f$\Am=\Rm_x \Rm_y \Rm_z\f$ transforms local (body-fixed) 3D positions \f$\pv_{loc} = [p^x_{loc}\;\;p^y_{loc}\;\;p^z_{loc}]^T\f$ to global 3D positions \f$\pv_{glob} = [p^x_{glob}\;\;p^y_{glob}\;\;p^z_{glob}]^T\f$, \f[ \pv_{glob} = \Am \pv_{loc} \f]; the transformation matrix \f$\mathbf{G}\f$ between time derivatives of Euler angles and angular velocities is defined as according to the book of Nikravesh. NOTE that this node has a singularity if the second rotation parameter reaches \f$k \times \pi/2\f$, with \f$k \in \{...,-3,-1,1,3,... \}\f$.
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

//! AUTO: CNodeRigidBodyRxyz
class CNodeRigidBodyRxyz: public CNodeRigidBody // AUTO: 
{
protected: // AUTO: 
    static const Index nRotationCoordinates = 3;
    static const Index nDisplacementCoordinates = 3;
    CNodeRigidBodyRxyzParameters parameters; //! AUTO: contains all parameters for CNodeRigidBodyRxyz

public: // AUTO: 

    // AUTO: access functions
    //! AUTO: Write (Reference) access to parameters
    virtual CNodeRigidBodyRxyzParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const CNodeRigidBodyRxyzParameters& GetParameters() const { return parameters; }

    //! AUTO:  return number of second order diff. eq. coordinates
    virtual Index GetNumberOfODE2Coordinates() const override
    {
        return 6;
    }

    //! AUTO:  return number of displacement coordinates
    virtual Index GetNumberOfDisplacementCoordinates() const override
    {
        return nDisplacementCoordinates;
    }

    //! AUTO:  return number of rotation coordinates
    virtual Index GetNumberOfRotationCoordinates() const override
    {
        return nRotationCoordinates;
    }

    //! AUTO:  return node type (for node treatment in computation)
    virtual Node::Type GetType() const override
    {
        return (Node::Type)(Node::Position + Node::Orientation + Node::RigidBody + Node::RotationRxyz);
    }

    //! AUTO:  return node group, which is special because of algebraic equations
    virtual CNodeGroup GetNodeGroup() const override
    {
        return CNodeGroup::ODE2variables;
    }

    //! AUTO:  return configuration dependent position of node; returns always a 3D Vector
    virtual Vector3D GetPosition(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent velocity of node; returns always a 3D Vector
    virtual Vector3D GetVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent rotation matrix of node; returns always a 3D Vector
    virtual Matrix3D GetRotationMatrix(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocity(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return configuration dependent local (=body-fixed) angular velocity of node; returns always a 3D Vector
    virtual Vector3D GetAngularVelocityLocal(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  return internally stored reference coordinates of node
    virtual LinkedDataVector GetReferenceCoordinateVector() const override
    {
        return parameters.referenceCoordinates;
    }

    //! AUTO:  provide according output variable in "value"; used e.g. for postprocessing and sensors
    virtual void GetOutputVariable(OutputVariableType variableType, ConfigurationType configuration, Vector& value) const override;

    //! AUTO:  Compute vector to of 4 Euler Parameters from reference and configuration coordinates
    virtual ConstSizeVector<maxRotationCoordinates> GetRotationParameters(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute vector to time derivative of 4 Euler Parameters in given configuration
    virtual LinkedDataVector GetRotationParameters_t(ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
    virtual void GetG(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute local G matrix for given configuration
    virtual void GetGlocal(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute G matrix (=diff(angularVelocity, velocityParameters)) for given configuration
    virtual void GetG_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    //! AUTO:  Compute local G matrix for given configuration
    virtual void GetGlocal_t(ConstSizeMatrix<maxRotationCoordinates * nDim3D>& matrix, ConfigurationType configuration = ConfigurationType::Current) const override;

    virtual OutputVariableType GetOutputVariableTypes() const override
    {
        return (OutputVariableType)(
            (Index)OutputVariableType::Position +
            (Index)OutputVariableType::Displacement +
            (Index)OutputVariableType::RotationMatrix +
            (Index)OutputVariableType::Rotation +
            (Index)OutputVariableType::Velocity +
            (Index)OutputVariableType::AngularVelocity +
            (Index)OutputVariableType::AngularVelocityLocal +
            (Index)OutputVariableType::Coordinates +
            (Index)OutputVariableType::Coordinates_t );
    }

};

