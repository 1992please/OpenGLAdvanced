#pragma once
#include "util.h"
#include "graphics_app.h"
#include "callbacks.h"

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class MainApp :public ICallbacks, public GraphicsApp
{
public:
	MainApp();
	~MainApp();
	bool Init();
	void Run();
	void RenderScene_callback() override;
	void Keyboard_callback(KEY key, KEY_STATE keyState = KEY_STATE_PRESS) override;

private:
	//BasicLightingTechnique m_LightingTech;
	//Camera* m_pGameCamera;
	//DirectionalLight m_directionalLight;
	//BasicMesh m_mesh;
	//PersProjInfo m_persProjInfo;
	//Pipeline m_pipeline;
};

