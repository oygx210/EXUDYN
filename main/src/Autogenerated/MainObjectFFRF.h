/** ***********************************************************************************************
* @class        MainObjectFFRFParameters
* @brief        Parameter class for MainObjectFFRF
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-06-01  20:10:12 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#include <pybind11/pybind11.h>      //! AUTO: include pybind for dictionary access
#include <pybind11/stl.h>           //! AUTO: needed for stl-casts; otherwise py::cast with std::vector<Real> crashes!!!
namespace py = pybind11;            //! AUTO: "py" used throughout in code
#include <pybind11/functional.h> //! AUTO: for function handling ... otherwise gives a python error (no compilation error in C++ !)
#include "Autogenerated/CObjectFFRF.h"

#include "Autogenerated/VisuObjectFFRF.h"

//! AUTO: Parameters for class MainObjectFFRFParameters
class MainObjectFFRFParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainObjectFFRF
* @brief        This object is used to represent equations modelled by the floating frame of reference formulation (FFRF). It contains a RigidBodyNode (always node 0) and a list of other nodes representing the finite element nodes used in the FFRF. Note that temporary matrices and vectors are subject of change in future.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */
#pragma once

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

//! AUTO: MainObjectFFRF
class MainObjectFFRF: public MainObjectBody // AUTO: 
{
protected: // AUTO: 
    CObjectFFRF* cObjectFFRF; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationObjectFFRF* visualizationObjectFFRF; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainObjectFFRF()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CObjectFFRF* GetCObjectFFRF() { return cObjectFFRF; }
    //! AUTO: Get const pointer to computational class
    const CObjectFFRF* GetCObjectFFRF() const { return cObjectFFRF; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCObjectFFRF(CObjectFFRF* pCObjectFFRF) { cObjectFFRF = pCObjectFFRF; }

    //! AUTO: Get pointer to visualization class
    VisualizationObjectFFRF* GetVisualizationObjectFFRF() { return visualizationObjectFFRF; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationObjectFFRF* GetVisualizationObjectFFRF() const { return visualizationObjectFFRF; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationObjectFFRF(VisualizationObjectFFRF* pVisualizationObjectFFRF) { visualizationObjectFFRF = pVisualizationObjectFFRF; }

    //! AUTO: Get const pointer to computational base class object
    virtual CObject* GetCObject() const { return cObjectFFRF; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCObject(CObject* pCObject) { cObjectFFRF = (CObjectFFRF*)pCObject; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationObject* GetVisualizationObject() const { return visualizationObjectFFRF; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationObject(VisualizationObject* pVisualizationObject) { visualizationObjectFFRF = (VisualizationObjectFFRF*)pVisualizationObject; }

    //! AUTO:  Get type name of object; could also be realized via a string -> type conversion?
    virtual const char* GetTypeName() const override
    {
        return "GenericODE2";
    }

    //! AUTO:  provide requested nodeType for objects; used for automatic checks in CheckSystemIntegrity()
    virtual Node::Type GetRequestedNodeType() const override
    {
        return Node::_None;
    }

    //! AUTO:  Check consistency prior to CSystem::Assemble(); needs to find all possible violations such that Assemble() would fail
    virtual bool CheckPreAssembleConsistency(const MainSystem& mainSystem, STDstring& errorString) const override;


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cObjectFFRF->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(d["nodeNumbers"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "massMatrixFF", cObjectFFRF->GetParameters().massMatrixFF); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "stiffnessMatrixFF", cObjectFFRF->GetParameters().stiffnessMatrixFF); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetPyMatrixContainerSafely(d, "dampingMatrixFF", cObjectFFRF->GetParameters().dampingMatrixFF); /*! AUTO:  safely cast to C++ type*/
        EPyUtils::SetNumpyVectorSafely(d, "forceVector", cObjectFFRF->GetParameters().forceVector); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "forceUserFunction")) { if (EPyUtils::CheckForValidFunction(d["forceUserFunction"])) { cObjectFFRF->GetParameters().forceUserFunction = py::cast<std::function<StdVector(Real, StdVector,StdVector)>>((py::function)d["forceUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        if (EPyUtils::DictItemExists(d, "massMatrixUserFunction")) { if (EPyUtils::CheckForValidFunction(d["massMatrixUserFunction"])) { cObjectFFRF->GetParameters().massMatrixUserFunction = py::cast<std::function<NumpyMatrix(Real, StdVector,StdVector)>>((py::function)d["massMatrixUserFunction"]); /* AUTO:  read out dictionary and cast to C++ type*/}} 
        if (EPyUtils::DictItemExists(d, "computeFFRFterms")) { cObjectFFRF->GetParameters().computeFFRFterms = py::cast<bool>(d["computeFFRFterms"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "objectIsInitialized")) { cObjectFFRF->GetObjectIsInitialized() = py::cast<bool>(d["objectIsInitialized"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationObjectFFRF->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "Vcolor")) { visualizationObjectFFRF->GetColor() = py::cast<std::vector<float>>(d["Vcolor"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VtriangleMesh")) { EPyUtils::SetNumpyMatrixISafely(d, "VtriangleMesh", visualizationObjectFFRF->GetTriangleMesh()); /*! AUTO:  safely cast to C++ type*/} 
        if (EPyUtils::DictItemExists(d, "VshowNodes")) { visualizationObjectFFRF->GetShowNodes() = py::cast<bool>(d["VshowNodes"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
        GetCObject()->ParametersHaveChanged();
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["objectType"] = (std::string)GetTypeName();
        d["nodeNumbers"] = (std::vector<Index>)cObjectFFRF->GetParameters().nodeNumbers; //! AUTO: cast variables into python (not needed for standard types) 
        d["massMatrixFF"] = (PyMatrixContainer)cObjectFFRF->GetParameters().massMatrixFF; //! AUTO: cast variables into python (not needed for standard types) 
        d["stiffnessMatrixFF"] = (PyMatrixContainer)cObjectFFRF->GetParameters().stiffnessMatrixFF; //! AUTO: cast variables into python (not needed for standard types) 
        d["dampingMatrixFF"] = (PyMatrixContainer)cObjectFFRF->GetParameters().dampingMatrixFF; //! AUTO: cast variables into python (not needed for standard types) 
        d["forceVector"] = EPyUtils::Vector2NumPy(cObjectFFRF->GetParameters().forceVector); //! AUTO: cast variables into python (not needed for standard types) 
        d["forceUserFunction"] = (std::function<StdVector(Real, StdVector,StdVector)>)cObjectFFRF->GetParameters().forceUserFunction; //! AUTO: cast variables into python (not needed for standard types) 
        d["massMatrixUserFunction"] = (std::function<NumpyMatrix(Real, StdVector,StdVector)>)cObjectFFRF->GetParameters().massMatrixUserFunction; //! AUTO: cast variables into python (not needed for standard types) 
        d["computeFFRFterms"] = (bool)cObjectFFRF->GetParameters().computeFFRFterms; //! AUTO: cast variables into python (not needed for standard types) 
        d["coordinateIndexPerNode"] = (std::vector<Index>)cObjectFFRF->GetCoordinateIndexPerNode(); //! AUTO: cast variables into python (not needed for standard types) 
        d["objectIsInitialized"] = (bool)cObjectFFRF->GetObjectIsInitialized(); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsMass"] = (Real)cObjectFFRF->GetPhysicsMass(); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsInertia"] = EXUmath::Matrix3DToStdArray33(cObjectFFRF->GetPhysicsInertia()); //! AUTO: cast variables into python (not needed for standard types) 
        d["physicsCenterOfMass"] = (std::vector<Real>)cObjectFFRF->GetPhysicsCenterOfMass(); //! AUTO: cast variables into python (not needed for standard types) 
        d["PHItTM"] = EPyUtils::Matrix2NumPy(cObjectFFRF->GetPHItTM()); //! AUTO: cast variables into python (not needed for standard types) 
        d["referencePositions"] = EPyUtils::Vector2NumPy(cObjectFFRF->GetReferencePositions()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempVector"] = EPyUtils::Vector2NumPy(cObjectFFRF->GetTempVector()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempCoordinates"] = EPyUtils::Vector2NumPy(cObjectFFRF->GetTempCoordinates()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempCoordinates_t"] = EPyUtils::Vector2NumPy(cObjectFFRF->GetTempCoordinates_t()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempRefPosSkew"] = EPyUtils::Matrix2NumPy(cObjectFFRF->GetTempRefPosSkew()); //! AUTO: cast variables into python (not needed for standard types) 
        d["tempVelSkew"] = EPyUtils::Matrix2NumPy(cObjectFFRF->GetTempVelSkew()); //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationObjectFFRF->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        d["Vcolor"] = (std::vector<float>)visualizationObjectFFRF->GetColor(); //! AUTO: cast variables into python (not needed for standard types) 
        d["VtriangleMesh"] = EPyUtils::MatrixI2NumPy(visualizationObjectFFRF->GetTriangleMesh()); //! AUTO: cast variables into python (not needed for standard types) 
        d["VshowNodes"] = (bool)visualizationObjectFFRF->GetShowNodes(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { return py::cast((std::vector<Index>)cObjectFFRF->GetParameters().nodeNumbers);} //! AUTO: get parameter
        else if (parameterName.compare("massMatrixFF") == 0) { return py::cast((PyMatrixContainer)cObjectFFRF->GetParameters().massMatrixFF);} //! AUTO: get parameter
        else if (parameterName.compare("stiffnessMatrixFF") == 0) { return py::cast((PyMatrixContainer)cObjectFFRF->GetParameters().stiffnessMatrixFF);} //! AUTO: get parameter
        else if (parameterName.compare("dampingMatrixFF") == 0) { return py::cast((PyMatrixContainer)cObjectFFRF->GetParameters().dampingMatrixFF);} //! AUTO: get parameter
        else if (parameterName.compare("forceVector") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRF->GetParameters().forceVector);} //! AUTO: get parameter
        else if (parameterName.compare("forceUserFunction") == 0) { return py::cast((std::function<StdVector(Real, StdVector,StdVector)>)cObjectFFRF->GetParameters().forceUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("massMatrixUserFunction") == 0) { return py::cast((std::function<NumpyMatrix(Real, StdVector,StdVector)>)cObjectFFRF->GetParameters().massMatrixUserFunction);} //! AUTO: get parameter
        else if (parameterName.compare("computeFFRFterms") == 0) { return py::cast((bool)cObjectFFRF->GetParameters().computeFFRFterms);} //! AUTO: get parameter
        else if (parameterName.compare("coordinateIndexPerNode") == 0) { return py::cast((std::vector<Index>)cObjectFFRF->GetCoordinateIndexPerNode());} //! AUTO: get parameter
        else if (parameterName.compare("objectIsInitialized") == 0) { return py::cast((bool)cObjectFFRF->GetObjectIsInitialized());} //! AUTO: get parameter
        else if (parameterName.compare("physicsMass") == 0) { return py::cast((Real)cObjectFFRF->GetPhysicsMass());} //! AUTO: get parameter
        else if (parameterName.compare("physicsInertia") == 0) { return py::cast(EXUmath::Matrix3DToStdArray33(cObjectFFRF->GetPhysicsInertia()));} //! AUTO: get parameter
        else if (parameterName.compare("physicsCenterOfMass") == 0) { return py::cast((std::vector<Real>)cObjectFFRF->GetPhysicsCenterOfMass());} //! AUTO: get parameter
        else if (parameterName.compare("PHItTM") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRF->GetPHItTM());} //! AUTO: get parameter
        else if (parameterName.compare("referencePositions") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRF->GetReferencePositions());} //! AUTO: get parameter
        else if (parameterName.compare("tempVector") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRF->GetTempVector());} //! AUTO: get parameter
        else if (parameterName.compare("tempCoordinates") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRF->GetTempCoordinates());} //! AUTO: get parameter
        else if (parameterName.compare("tempCoordinates_t") == 0) { return EPyUtils::Vector2NumPy(cObjectFFRF->GetTempCoordinates_t());} //! AUTO: get parameter
        else if (parameterName.compare("tempRefPosSkew") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRF->GetTempRefPosSkew());} //! AUTO: get parameter
        else if (parameterName.compare("tempVelSkew") == 0) { return EPyUtils::Matrix2NumPy(cObjectFFRF->GetTempVelSkew());} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationObjectFFRF->GetShow());} //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { return py::cast((std::vector<float>)visualizationObjectFFRF->GetColor());} //! AUTO: get parameter
        else if (parameterName.compare("VtriangleMesh") == 0) { return EPyUtils::MatrixI2NumPy(visualizationObjectFFRF->GetTriangleMesh());} //! AUTO: get parameter
        else if (parameterName.compare("VshowNodes") == 0) { return py::cast((bool)visualizationObjectFFRF->GetShowNodes());} //! AUTO: get parameter
        else  {PyError(STDstring("ObjectFFRF::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumbers") == 0) { cObjectFFRF->GetParameters().nodeNumbers = py::cast<std::vector<Index>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("massMatrixFF") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRF->GetParameters().massMatrixFF); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("stiffnessMatrixFF") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRF->GetParameters().stiffnessMatrixFF); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("dampingMatrixFF") == 0) { EPyUtils::SetPyMatrixContainerSafely(value, cObjectFFRF->GetParameters().dampingMatrixFF); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("forceVector") == 0) { EPyUtils::SetNumpyVectorSafely(value, cObjectFFRF->GetParameters().forceVector); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("forceUserFunction") == 0) { cObjectFFRF->GetParameters().forceUserFunction = py::cast<std::function<StdVector(Real, StdVector,StdVector)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("massMatrixUserFunction") == 0) { cObjectFFRF->GetParameters().massMatrixUserFunction = py::cast<std::function<NumpyMatrix(Real, StdVector,StdVector)>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("computeFFRFterms") == 0) { cObjectFFRF->GetParameters().computeFFRFterms = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("objectIsInitialized") == 0) { cObjectFFRF->GetObjectIsInitialized() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationObjectFFRF->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vcolor") == 0) { visualizationObjectFFRF->GetColor() = py::cast<std::vector<float>>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VtriangleMesh") == 0) { EPyUtils::SetNumpyMatrixISafely(value, visualizationObjectFFRF->GetTriangleMesh()); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("VshowNodes") == 0) { visualizationObjectFFRF->GetShowNodes() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("ObjectFFRF::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
        GetCObject()->ParametersHaveChanged();
    }

};


