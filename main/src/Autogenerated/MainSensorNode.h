/** ***********************************************************************************************
* @class        MainSensorNodeParameters
* @brief        Parameter class for MainSensorNode
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
#include "Autogenerated/CSensorNode.h"

#include "Autogenerated/VisuSensorNode.h"

//! AUTO: Parameters for class MainSensorNodeParameters
class MainSensorNodeParameters // AUTO: 
{
public: // AUTO: 
};


/** ***********************************************************************************************
* @class        MainSensorNode
* @brief        A sensor attached to a node. The sensor measures OutputVariables and outputs values into a file, showing per line [time, sensorValue[0], sensorValue[1], ...]. A user function can be attached to modify sensor values accordingly.
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

//! AUTO: MainSensorNode
class MainSensorNode: public MainSensor // AUTO: 
{
protected: // AUTO: 
    CSensorNode* cSensorNode; //pointer to computational object (initialized in object factory) AUTO:
    VisualizationSensorNode* visualizationSensorNode; //pointer to computational object (initialized in object factory) AUTO:

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    MainSensorNode()
    {
        name = "";
    };

    // AUTO: access functions
    //! AUTO: Get pointer to computational class
    CSensorNode* GetCSensorNode() { return cSensorNode; }
    //! AUTO: Get const pointer to computational class
    const CSensorNode* GetCSensorNode() const { return cSensorNode; }
    //! AUTO: Set pointer to computational class (do this only in object factory!!!)
    void SetCSensorNode(CSensorNode* pCSensorNode) { cSensorNode = pCSensorNode; }

    //! AUTO: Get pointer to visualization class
    VisualizationSensorNode* GetVisualizationSensorNode() { return visualizationSensorNode; }
    //! AUTO: Get const pointer to visualization class
    const VisualizationSensorNode* GetVisualizationSensorNode() const { return visualizationSensorNode; }
    //! AUTO: Set pointer to visualization class (do this only in object factory!!!)
    void SetVisualizationSensorNode(VisualizationSensorNode* pVisualizationSensorNode) { visualizationSensorNode = pVisualizationSensorNode; }

    //! AUTO: Get const pointer to computational base class object
    virtual CSensor* GetCSensor() const { return cSensorNode; }
    //! AUTO: Set pointer to computational base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetCSensor(CSensor* pCSensor) { cSensorNode = (CSensorNode*)pCSensor; }

    //! AUTO: Get const pointer to visualization base class object
    virtual VisualizationSensor* GetVisualizationSensor() const { return visualizationSensorNode; }
    //! AUTO: Set pointer to visualization base class object (do this only in object factory; type is NOT CHECKED!!!)
    virtual void SetVisualizationSensor(VisualizationSensor* pVisualizationSensor) { visualizationSensorNode = (VisualizationSensorNode*)pVisualizationSensor; }

    //! AUTO:  Get type name of sensor (without keyword 'Sensor'...!)
    virtual const char* GetTypeName() const override
    {
        return "Node";
    }


    //! AUTO:  dictionary write access
    virtual void SetWithDictionary(const py::dict& d) override
    {
        cSensorNode->GetParameters().nodeNumber = py::cast<Index>(d["nodeNumber"]); /* AUTO:  read out dictionary and cast to C++ type*/
        cSensorNode->GetParameters().writeToFile = py::cast<bool>(d["writeToFile"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "fileName", cSensorNode->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/
        cSensorNode->GetParameters().outputVariableType = (OutputVariableType)py::cast<Index>(d["outputVariableType"]); /* AUTO:  read out dictionary and cast to C++ type*/
        EPyUtils::SetStringSafely(d, "name", name); /*! AUTO:  safely cast to C++ type*/
        if (EPyUtils::DictItemExists(d, "Vshow")) { visualizationSensorNode->GetShow() = py::cast<bool>(d["Vshow"]); /* AUTO:  read out dictionary and cast to C++ type*/} 
    }

    //! AUTO:  dictionary read access
    virtual py::dict GetDictionary() const override
    {
        auto d = py::dict();
        d["sensorType"] = (std::string)GetTypeName();
        d["nodeNumber"] = (Index)cSensorNode->GetParameters().nodeNumber; //! AUTO: cast variables into python (not needed for standard types) 
        d["writeToFile"] = (bool)cSensorNode->GetParameters().writeToFile; //! AUTO: cast variables into python (not needed for standard types) 
        d["fileName"] = (std::string)cSensorNode->GetParameters().fileName; //! AUTO: cast variables into python (not needed for standard types) 
        d["outputVariableType"] = (OutputVariableType)cSensorNode->GetParameters().outputVariableType; //! AUTO: cast variables into python (not needed for standard types) 
        d["name"] = (std::string)name; //! AUTO: cast variables into python (not needed for standard types) 
        d["Vshow"] = (bool)visualizationSensorNode->GetShow(); //! AUTO: cast variables into python (not needed for standard types) 
        return d; 
    }

    //! AUTO:  parameter read access
    virtual py::object GetParameter(const STDstring& parameterName) const override 
    {
        if (parameterName.compare("name") == 0) { return py::cast((std::string)name);} //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { return py::cast((Index)cSensorNode->GetParameters().nodeNumber);} //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { return py::cast((bool)cSensorNode->GetParameters().writeToFile);} //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { return py::cast((std::string)cSensorNode->GetParameters().fileName);} //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { return py::cast((OutputVariableType)cSensorNode->GetParameters().outputVariableType);} //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { return py::cast((bool)visualizationSensorNode->GetShow());} //! AUTO: get parameter
        else  {PyError(STDstring("SensorNode::GetParameter(...): illegal parameter name ")+parameterName+" cannot be read");} // AUTO: add warning for user
        return py::object();
    }


    //! AUTO:  parameter write access
    virtual void SetParameter(const STDstring& parameterName, const py::object& value) override 
    {
        if (parameterName.compare("name") == 0) { EPyUtils::SetStringSafely(value, name); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("nodeNumber") == 0) { cSensorNode->GetParameters().nodeNumber = py::cast<Index>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("writeToFile") == 0) { cSensorNode->GetParameters().writeToFile = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("fileName") == 0) { EPyUtils::SetStringSafely(value, cSensorNode->GetParameters().fileName); /*! AUTO:  safely cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("outputVariableType") == 0) { cSensorNode->GetParameters().outputVariableType = py::cast<OutputVariableType>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else if (parameterName.compare("Vshow") == 0) { visualizationSensorNode->GetShow() = py::cast<bool>(value); /* AUTO:  read out dictionary and cast to C++ type*/; } //! AUTO: get parameter
        else  {PyError(STDstring("SensorNode::SetParameter(...): illegal parameter name ")+parameterName+" cannot be modified");} // AUTO: add warning for user
    }

};


