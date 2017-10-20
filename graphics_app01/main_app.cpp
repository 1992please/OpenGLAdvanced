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
	m_pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	m_scale += 0.1f;

	Pipeline p;
	p.Rotate(0.0f, m_scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 3.0f);
	p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetForward(), m_pGameCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);
	m_pEffect->SetMVP(p.GetMVPTrans());
	m_pEffect->SetDirectionalLight(m_directionalLight);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_pTexture->Bind(0);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void MainApp::Keyboard_callback(KEY key, KEY_STATE keyState)
{
	switch (key)
	{
		case KEY_ESCAPE:
		case KEY_q:
			GLFWBackendLeaveMainLoop();
			break;

		case KEY_a:
			m_directionalLight.AmbientIntensity += 0.05f;
			break;

		case KEY_s:
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;
		default:
			break;
	}
}

void MainApp::PassiveMouse_callback(int x, int y)
{
	m_pGameCamera->OnMouse(x, y);
}

TestApp::TestApp()
{

}

TestApp::~TestApp()
{

}

bool TestApp::Init()
{
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	return true;
}

void TestApp::Run()
{
	GLFWBackendRun(this);
}

void TestApp::RenderScene_callback()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw our first triangle
	//glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TestApp::Keyboard_callback(KEY key, KEY_STATE keyState)
{
}

void TestApp::PassiveMouse_callback(int x, int y)
{
}
