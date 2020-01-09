/** ***********************************************************************************************
* @class        VisualizationSystem
* @brief		
* @details		Details:
 				- a visualization system, containing data and functions for visualization
*
* @author		Gerstmayr Johannes
* @date			2019-05-24 (generated)
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

//#include "autogenerated/SimulationSettings.h" 
//
//#include "Linalg/Vector.h"				//includes ReleaseAssert.h, ... and BasicFunctions.h
//#include "Linalg/SlimVector.h" 
//#include "Linalg/ConstSizeVector.h"		//includes Vector.h
//#include "Linalg/ResizableVector.h"		//includes Vector.h
//#include "Utilities/ResizableArray.h"	//includes SlimArray.h and BasicFunctions.h
//
//#include "System/CSystem.h"  //REMOVE: temporary, because lateron coupling only via visualization system!!! but include Vector
//#include "Autogenerated/VisualizationSettings.h" 
//#include "Graphics/GraphicsData.h"
////#include "Graphics/VisualizationSystemBase.h"
//#include "Graphics/VisualizationSystemData.h"
//


//class CSystem;
class VisualizationSystemContainer; //for UpdateGraphics(...) function


class VisualizationSystem //: public VisualizationSystemBase
{
public: //declared as public for direct access via pybind
	VisualizationSystemData vSystemData;//!< data structure containing the visualization items
    GraphicsData graphicsData;			//!< data to be processed by OpenGL renderer
	//VisualizationSettings settings;		//!< general settings for visualization
	PostProcessData* postProcessData;	//!< link to postProcessData of CSystem ==> communication between the two threads
	CSystemData* systemData;			//!< REMOVE: this is a temporary access, before visualization objects are introduced
	//RendererState rendererState;		//!< Data linked to state variables of the OpenGL engine (e.g. zoom, transformation matrices, ...)
	const float contourPlotFlag = -2.f;	//!< this is the value of transparency used to identify contour plot values in GraphicsData items

public:
	GraphicsData& GetGraphicsData() { return graphicsData; }
	const GraphicsData& GetGraphicsData() const { return graphicsData; }

	VisualizationSystemData& GetVisualizationSystemData() { return vSystemData; }
	const VisualizationSystemData& GetVisualizationSystemData() const { return vSystemData; }

	//VisualizationSettings& GetVisualizationSettings() { return settings; }
    //const VisualizationSettings& GetVisualizationSettings() const { return settings; }

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//SYSTEM FUNCTIONS

	//! links to systemData of cSystem (should be REMOVED)
	void LinkToSystemData(CSystemData* systemDataInit);
	//! link to postProcessData, which is the communication way of graphics to the computational system
	void LinkPostProcessData(PostProcessData* postProcessDataInit);

	////! this function links the VisualizationSystem to a render engine, such that the changes in the graphics structure drawn upon updates, etc.
	//bool LinkToRenderEngine(CSystem& cSystem);

	////! this function releases the VisualizationSystem from the render engine;
	//bool DetachRenderEngine();

	//! reset all visualization functions for new system (but keep render engine linked)
	void Reset();

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//GRAPHICS FUNCTIONS

	//! OpenGL renderer sends message that graphics shall be updated; update is only done, if current state has higher counter than already existing state
	virtual void UpdateGraphicsData(VisualizationSystemContainer& visualizationSystemContainer);

	//! Renderer reports to CSystem that simulation shall be interrupted
	virtual void StopSimulation();		

	//! any multi-line text message from computation to be shown in renderer (e.g. time, solver, ...)
	virtual std::string GetComputationMessage();

	//! if the system has changed or loaded, compute maximum box of all items and reset scene to the maximum box
	//virtual void UpdateMaximumSceneCoordinates();

	virtual void Print(std::ostream& os) const
	{
		os << "VisualizationCSystem:\n";
		os << "  VisualizationSystemData = \n" << vSystemData << "\n";
		os << "  graphicsData = \n" << graphicsData << "\n";
		os << "\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const VisualizationSystem& object)
	{
		object.Print(os);
		return os;
	}



};



