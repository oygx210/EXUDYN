/** ***********************************************************************************************
* @class        VisualizationMarkerNodeRigid
* @brief        A rigid-body (position+orientation) node-marker attached to a rigid-body node.
*
* @author       Gerstmayr Johannes
* @date         2019-07-01 (generated)
* @date         2020-07-20  12:33:24 (last modfied)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: https://github.com/jgerstmayr/EXUDYN
                
************************************************************************************************ */

#ifndef VISUALIZATIONMARKERNODERIGID__H
#define VISUALIZATIONMARKERNODERIGID__H

#include <ostream>

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

class VisualizationMarkerNodeRigid: public VisualizationMarker // AUTO: 
{
protected: // AUTO: 

public: // AUTO: 
    //! AUTO: default constructor with parameter initialization
    VisualizationMarkerNodeRigid()
    {
        show = true;
    };

    // AUTO: access functions
    //! AUTO:  Update visualizationSystem -> graphicsData for item; index shows item Number in CData
    virtual void UpdateGraphics(const VisualizationSettings& visualizationSettings, VisualizationSystem* vSystem, Index itemNumber) override;

};



#endif //#ifdef include once...
