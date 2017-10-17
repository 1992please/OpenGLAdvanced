#include "glfw_backend.h"
#include "camera.h"
#include "pipeline.h"
#include "main_app.h"

MainApp::MainApp()
{
	m_pGameCamera = NULL;
	m_pTexture = NULL;
	m_pEffect = NULL;
	m_scale = 0.0f;
	m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_directionalLight.AmbientIntensity = 0.5f;

	m_persProjInfo.FOV = 60.0f;
	m_persProjInfo.Height = WINDOW_HEIGHT;
	m_persProjInfo.Width = WINDOW_WIDTH;
	m_persProjInfo.zNear = 1.0f;
	m_persProjInfo.zFar = 100.0f;
}

MainApp::~MainApp()
{
	delete m_pEffect;
	delete m_pGameCamera;
	delete m_pTexture;
}

bool MainApp::Init()
{
	m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	CreateVertexBuffer();
	CreateIndexBuffer();

	m_pEffect = new LightingTechnique();

	if (!m_pEffect->Init())
	{
		return false;
	}

	m_pEffect->Enable();

	m_pEffect->SetTextureUnit(0);

	m_pTexture = new Texture(GL_TEXTURE_2D);

	if (!m_pTexture->Load("content/test.png")) {
		return false;
	}

	return true;
}

void MainApp::Run()
{
	GLFWBackendRun(this);
}

void MainApp::RenderScene_callback()
{
	//m_pGameCamera->OnRender();

	//glClear(GL_COLOR_BUFFER_BIT);

	//m_scale += 0.1f;

	//Pipeline p;
	//p.Rotate(0.0f, m_scale, 0.0f);
	//p.WorldPos(0.0f, 0.0f, 3.0f);
	//p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
	//p.SetPerspectiveProj(m_persProjInfo);
	//m_pEffect->SetWVP(p.GetWVPTrans());
	//m_pEffect->SetDirectionalLight(m_directionalLight);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//m_pTexture->Bind(GL_TEXTURE0);
	//glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	//glutSwapBuffers();
}

void MainApp::Keyboard_callback(KEY key, KEY_STATE keyState)
{
}
