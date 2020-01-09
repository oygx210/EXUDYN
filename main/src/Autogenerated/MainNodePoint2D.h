/** ***********************************************************************************************
* @class        MainNodePoint2DParameters
* @brief        Parameter class for MainNodePoint2D
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2019-11-12  21:47:28 (last modfied)
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

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include "Autogenerated/CNodePoint2D.h"

#include "Autogenerated/VisuNodePoint2D.h"

//! AUTO: Parameters for class MainNodePoint2DParameters
class MainNodePoint2DParameters // AUTO: 
{
public: // AUTO: 
    Vector2D initialCoordinates;                  //!< AUTO: initial displacement coordinate
    Vector2D initialCoordinates_t;                //!< AUTO: initial velocity coordinate
    //! AUTO: default constructor with parameter initialization
    MainNodePoint2DParameters()
    {
        initialCoordinates = Vector2D({0.,0.});
        initialCoordinates_t = Vector2D({0.,0.});
    };
};


/** ***********************************************************************************************
* @class        MainNodePoint2D
* @brief        A 2D point node for point masses or solid finite elements which has 2 displacement degrees of freedom for second order differential equations.
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

//! AUTO: MainNodePoint2D
class MainNodePoint2D: public MainNode // AUTO: 
{
protected: // AUTO: 
    CNodePoint2D* cNodePoint2D; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationNodePoint2D* visualizationNodePoint2D; //pointer to computational object (initialized in object factory) AUTO:
    MainNodePoint2DParameters parameters; //! AUTO: contains all parameters for MainNodePoint2D

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainNodePoint2D()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CNodePoint2D* GetCNodePoint2D() { return cNodePoint2D; }
    //! AUTO: Get const pointer to computational class
    const CNodePoint2D* GetCNodePoint2D() const { return cNodePoint2D; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCNodePoint2D(CNodePoint2D* pCNodePoint2D) { cNodePoint2D = pCNodePoint2D; }

    //! AUTO: Get pointer to visualization class
    VisualizationNodePoint2D* GetVisualizationNodePoint2D() { return visualizationNodePoint2D; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationNodePoint2D* GetVisualizationNodePoint2D() const { return visualizationNodePoint2D; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationNodePoint2D(VisualizationNodePoint2D* pVisualizationNodePoint2D) { visualizationNodePoint2D = pVisualizationNodePoint2D; }

    //! AUTO: Get const pointer to computational base class object
    virtual CNode* GetCNode() const { return cNodePoint2D; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCNode(CNode* pCNode) { cNodePoint2D = (CNodePoint2D*)pCNode; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationNode* GetVisualizationNode() const { return visualizationNodePoint2D; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationNode(VisualizationNode* pVisualizationNode) { visualizationNodePoint2D = (VisualizationNodePoint2D*)pVisualizationNode; }

    //! AUTO: Write (Reference) access to parameters
    virtual MainNodePoint2DParameters& GetParameters() { return parameters; }
    //! AUTO: Read access to parameters
    virtual const MainNodePoint2DParameters& GetParameters() const { return parameters; }

    //! AUTO:  Get type name of node (without keyword "Node"...!); could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "Point2D";
    }

    //! AUTO:  Call a specific node function ==> automatically generated in future
    virtual py::object CallFunction(STDstring functionName, py::dict args) const override;

    //! AUTO:  return internally stored initial coordinates (displacements) of node
    virtual LinkedDataVector GetInitialVector() const override
    {
        return parameters.initialCoordinates;
    }

    //! AUTO:  return internally stored initial coordinates (velocities) of node
    virtual LinkedDataVector GetInitialVector_t() const override
    {
        return parameters.initialCoordinates_t;
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        HPyUtils::SetVector2DSafely(d, "referenceCoordinates", cNodePoint2D->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/
        if (HPyUtils::DictItemExists(d, "initialDisplacements")) { HPyUtils::SetVector2DSafely(d, "initialDisplacements", GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/} 
        if (HPyUtils::DictItemExists(d, "initialVelocities")) { HPyUtils::SetVector2DSafely(d, "initialVelocities", GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/} 
        HPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (HPyUtils::DictItemExists(d, "Vshow")) { visualizationNodePoint2D->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (HPyUtils::DictItemExists(d, "VdrawSize")) { visualizationNodePoint2D->GetDrawSize() = py::cast<float>(d["VdrawSize"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (HPyUtils::DictItemExists(d, "Vcolor")) { visualizationNodePoint2D->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["nodeType"] = (std::string)GetTypeName();
        d["referenceCoordinates"] = (std::vector<Real>)cNodePoint2D->GetParameters().referenceCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialDisplacements"] = (std::vector<Real>)GetParameters().initialCoordinates; //! AUTO: cast variables into python (not needed for standard types) 
        d["initialVelocities"] = (std::vector<Real>)GetParameters().initialCoordinates_t; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationNodePoint2D->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VdrawSize"] = (float)visualizationNodePoint2D->GetDrawSize(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationNodePoint2D->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { return py::cast((std::vector<Real>)cNodePoint2D->GetParameters().referenceCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialDisplacements") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates);} //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { return py::cast((std::vector<Real>)GetParameters().initialCoordinates_t);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationNodePoint2D->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { return py::cast((float)visualizationNodePoint2D->GetDrawSize());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationNodePoint2D->GetColor());} //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint2D::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { HPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("referenceCoordinates") == 0) { HPyUtils::SetVector2DSafely(value, cNodePoint2D->GetParameters().referenceCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialDisplacements") == 0) { HPyUtils::SetVector2DSafely(value, GetParameters().initialCoordinates); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("initialVelocities") == 0) { HPyUtils::SetVector2DSafely(value, GetParameters().initialCoordinates_t); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationNodePoint2D->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VdrawSize") == 0) { visualizationNodePoint2D->GetDrawSize() = py::cast<float>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationNodePoint2D->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("NodePoint2D::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


