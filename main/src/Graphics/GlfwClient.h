/** ***********************************************************************************************
* @class        GlfwClient
* @brief        Hub to glfw class for 3D visualization using OpenGL
*
* @author       Gerstmayr Johannes
* @date         2019-05-24 (generated)
*
* @copyright    This file is part of Exudyn. Exudyn is free software: you can redistribute it and/or modify it under the terms of the Exudyn license. See "LICENSE.txt" for more details.
* @note         Bug reports, support and further information:
                - email: johannes.gerstmayr@uibk.ac.at
                - weblink: missing
                
************************************************************************************************ */
#pragma once

#include "Utilities/ReleaseAssert.h"
#include "Utilities/BasicDefinitions.h"

#ifdef USE_GLFW_GRAPHICS

#include <ostream>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include "Graphics/GraphicsData.h"
//#include "Graphics/GlfwClient.h"

#include "Autogenerated/VisualizationSettings.h"
#include "Graphics/VisualizationSystemContainerBase.h" //common interface to visualization system

#include "Graphics/GlfwClientText.h" //link to external library; include only if copyright is appropriate


enum class RendererMode {
	None, Move, Rotate, ZoomView, Select
};


//! variables that describe the renderer's state machine ==> zoom, move, etc. with mouse
class RendererStateMachine
{
public:
	bool leftMousePressed;
	bool rightMousePressed;
	bool shiftPressed;
	bool ctrlPressed;
	RendererMode mode;			//!< determines the state of any action

	double mousePositionX;		//!< currently recorded mouse position; use double for GLFW compatibility
	double mousePositionY;		//!< currently recorded mouse position; use double for GLFW compatibility
	double lastMousePressedX;	//!< last mouse button position pressed
	double lastMousePressedY;	//!< last mouse button position pressed

	float storedCenterPointX;	//!< stored centerpoint position during mouse-move
	float storedCenterPointY;	//!< stored centerpoint position during mouse-move

	Float16 storedModelRotation;//!< stored rotation matrix before right mouse button pressed
};


//! this is the rendering module for displaying 3D model data
class GlfwRenderer
{
private:
	//static RendererState state;
	static bool rendererActive;			//!< signal that shows that renderer is active
	static bool stopRenderer;			//!< signal that shows that renderer should quit
	static GLFWwindow* window;
	static RendererState* state;		//!< this represents the current OpenGL parameters
	static RendererStateMachine stateMachine; //!< all variables (mouse, keyboard, ...) used for state machine (zoom, zoom-view, move, ...)
	static std::thread rendererThread;	//!< std::thread variable for rendererThread
	static Index rendererError;			//!< 0 ... no error, 1 ... glfwInit() failed, 2 ... glfwCreateWindow failed, 3 ... other error

	//done in graphicsData: static uint64_t visualizationCounter; //!< counter showing number for pulling last successful graphics content; must be set to zero when system is reset, otherwise nothing is drawn unless counter is higher than 

	//+++++++++++++++++++++++++++++++++++++++++
	//link to GraphicsData and Settings:
	static ResizableArray<GraphicsData*>* graphicsDataList;					//!< link to graphics data; only works for one MainSystem, but could also be realized for several systems
	static VisualizationSettings* visSettings;  //!< link to visualization settings
	static VisualizationSystemContainerBase* basicVisualizationSystemContainer;
	//+++++++++++++++++++++++++++++++++++++++++

public:
	GlfwRenderer();
	~GlfwRenderer() 
	{	
		//glfwTerminate(); //move to destructor
	};

	//! Initializes and starts the Renderer in a separate thread;
	//  Returns false, if problems with glfw library or windows creation, otherwise true; 
	//  @todo test with apple and linux
	static bool SetupRenderer();

	//! stop the renderer engine and its thread; @todo StopRenderer currently also stops also main thread (python)
	static void StopRenderer() 
	{ 
		if (window)
		{
			stopRenderer = true;
			glfwSetWindowShouldClose(window, 1);
			Index timeOut = visSettings->window.startupTimeout / 10;

			Index i = 0;
			while (i++ < timeOut && rendererActive) //wait 5 seconds for thread to answer
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			if (rendererActive) { SysError("OpenGL Renderer could not be stopped safely\n"); }
			//else { pout << "Renderer Stopped\n"; }

			glfwDestroyWindow(window);
			//not necessary: glfwTerminate(); //test if this helps; should not be needed

			//delete window; //will not work? VS2017 reports warning that destructor will not be called, since window is only a struct
			window = nullptr; //this is used to identify if window has already been generated

			//after this command, this thread is terminated! ==> nothing will be done any more
			if (rendererThread.joinable()) //thread is still running from previous call ...
			{
				//pout << "join thread ...\n";
				rendererThread.join();
				//pout << "thread joined\n";
				//not necessary: rendererThread.~thread(); //check if this is necessary/right ==> will not be called after .joint() ...
			}
		}
	}

	//! Set all light functions for openGL
	static void SetGLLights();

	static bool WindowIsInitialized()
	{
		if (window && rendererActive) { return true; }
		else { return false; }
	}

	//! Links the Renderer to a specific GraphicsData/settings; 
	//! Only one data linked at one time
	//! Returns true on success and false, if data is already linked (==> call DetachVisualizationSystem first)
	static bool LinkVisualizationSystem(ResizableArray<GraphicsData*>* graphicsDataListInit, VisualizationSettings* settingsInit,
										VisualizationSystemContainerBase* basicVisualizationSystemContainerInit, RendererState* rendererStateInit)
	{
		if (graphicsDataList == nullptr)
		{
			graphicsDataList = graphicsDataListInit;
			visSettings = settingsInit;
			basicVisualizationSystemContainer = basicVisualizationSystemContainerInit;
			state = rendererStateInit;
			return true;
		}
		else { return false; }
	}

	//! Detach the GraphicsData/settings; enables the visualization of different MainSystems; return true on success
	static bool DetachVisualizationSystem()
	{
		StopRenderer();

		if (graphicsDataList == nullptr) { return false; }//this just shows that no system was linked yet
		else 
		{ 
			graphicsDataList = nullptr;
			visSettings = nullptr;
			return true;
		}
	}

	static void UpdateGraphicsDataNow()
	{
		basicVisualizationSystemContainer->UpdateGraphicsDataNow();
	}

private: //to be called internally only!
	static void error_callback(int error, const char* description)
	{
		std::cout << description << "\n";
	}

	//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void window_close_callback(GLFWwindow* window)
	{
		//StopRenderer();
		//rendererActive = false;
		//basicVisualizationSystemContainer->StopSimulation(); //if user waits for termination of render engine, it tells that window is closed
		//stopRenderer = false;	//if stopped by user

		glfwSetWindowShouldClose(window, GLFW_FALSE); //do not close ....
		pout << "\n+++++++++++++++++++++\npress ESC or Q to close window!\n+++++++++++++++++++++\n";

	}

	//! GlfwInit and glfw->CreateWindow() calls; returns false, if functions fail
	static void InitCreateWindow();

	//! loop which checks for keyboard/mouse input; check for visualization updates (new data, window size changed, zoom, mouse move, etc.) and calls Render()
	static void RunLoop();

	//! Render function called for every update of OpenGl window
	static void Render(GLFWwindow* window); //GLFWwindow* needed in argument, because of glfwSetWindowRefreshCallback

	//! check if frame shall be grabed and saved to file using visualization options
	static void SaveImage();

	//! save scene to a file with filename
	static void SaveSceneToFile(const STDstring& filename);
	
	//! Render particulalry the graphics data of multibody system
	static void RenderGraphicsData();

	//! Zoom all graphics objects (for current configuration)
	static void ZoomAll();

	//get lines for character (0-9NOML) inside a 2 x 4 matrix (x/y) of 'lines' and the number of lines 'nLines'
	//(0,0) is located left bottom
	//static void GetCharacterLines(char c, ConstSizeVector<2 * maxTextPoints>& lines)

	//! draw a 0-terminated text string with scaling (size=1: height=1; width=0.5 for one character; distance = 0.25)
	static void DrawString(const char* text, float scale, const Float3& p, const Float4& color)
	{
#ifdef OPENGLTEXT_EXISTS //defined in GlfwClientText
		glLineWidth(visSettings->openGL.textLineWidth);
		if (visSettings->openGL.textLineSmooth) { glEnable(GL_LINE_SMOOTH); }
		OpenGLText::DrawString(text, scale, p, color);
		if (visSettings->openGL.textLineSmooth) { glDisable(GL_LINE_SMOOTH); }
#endif
	}
};

extern GlfwRenderer glfwRenderer; //this is the (static) location of the renderer class; could also be made dynamic



#endif //USE_GLFW_GRAPHICS
