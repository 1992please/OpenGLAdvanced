#ifndef MAIN_APP_H
#define MAIN_APP_H

#include "util.h"
#include "graphics_app.h"
#include "callbacks.h"
#include <GL\glew.h>
#include "Texture.h"
#include "techniques/basic_lighting_technique.h"
#include "techniques/custom_techniques.h"
#include "camera.h"
#include "basic_mesh.h"

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 720

struct Orientation;

class MainApp :public ICallbacks, public GraphicsApp
{
public:
	MainApp();
	~MainApp();
	bool Init();
	void Run();
	void RenderScene_callback() override;
	void Keyboard_callback(KEY key) override;
	void PassiveMouse_callback(float x, float y) override;
private:
	Camera* GameCamera;
	DirectionalLight mDirectionalLight;
	PersProjInfo mPersProjInfo;
	BasicMesh* mPlanetMesh;
	BasicMesh* mRockMesh;
	UnlitTechnique* mUnlitTechnique;
	InstancedUnlitTechnique* mInstUnlitTechnique;
	Orientation* Orientations;
	uint mAmount;
	glm::mat4* Models;
	glm::mat4* MVP;
	bool bFirstRender;
};

#endif // !MAIN_APP_H