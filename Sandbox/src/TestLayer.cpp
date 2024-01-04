#include <Daybreak.h>

#include "TestLayer.h"
#include <glad/glad.h> //Temporary for RenderTest() and RenderTest2()

TestLayer::TestLayer() : Layer("TestLayer")
{
	Daybreak::TextureSpecifications textSpec; // WARNING: Struct has hardcoded references to width,height, and format. CHANGE ASAP
	texture = Daybreak::Texture::Create(textSpec, "../Sandbox/assets/Test.png");
	shader = Daybreak::Shader::Create("Texture Shader", "../Sandbox/assets/TextureShader.glsl");

	float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
	};

		
	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	Daybreak::BufferLayout layout{
		{ Daybreak::RenderDataTypes::Float3, std::string("Position") },
		{ Daybreak::RenderDataTypes::Float2, std::string("Texture Position")}
	};

	va = Daybreak::VertexArray::Create();

	vb = Daybreak::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout(layout);
	va->AddVertexBuffer(vb);
	ib = Daybreak::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	va->SetIndexBuffer(ib);
	shader->Bind();
}

void TestLayer::OnUpdate()
{
	Daybreak::RenderCommand::SetClearColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	Daybreak::RenderCommand::Clear();

	texture->Bind();
	shader->Bind();
	shader->SetInt1("u_Texture", 0);
	shader->SetFloat4("u_Color", glm::vec4(r_color, g_color, 1.0f, 1.0f));
	Daybreak::RenderCommand::DrawIndexed(va);

	//TestLayer::RenderTest();
	//TestLayer::Test2();
	//DB_INFO("TestLayer::Update");
	//if (Daybreak::Input::IsKeyPressed(DB_KEY_TAB))
	//	DB_LOG("Tab key is pressed (poll)!");
}

void TestLayer::OnEvent(Daybreak::Event& event)
{
	
	if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	{
		Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		if (e.GetKeyCode() == DB_KEY_ESCAPE)
		{
			Daybreak::Application::Get().Close();
		}
	}
	//DB_LOG("{0}", event);
	//if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	//{
	//	Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
	//	if (e.GetKeyCode() == DB_KEY_TAB)
	//		DB_LOG("Tab key is pressed (event)!");
	//	DB_LOG("{0}", (char)e.GetKeyCode());
	//}
	//if (event.GetEventType() == Daybreak::EventType::KeyTyped)
	//{
	//	DB_LOG("Key Typed!");
	//}
}

void TestLayer::RenderTest2()
{
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
		"}\n\0";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	Daybreak::BufferLayout layout{ 
		{Daybreak::RenderDataTypes::Float3, std::string("Position")} 
	};

	Daybreak::Ref<Daybreak::VertexArray> va = Daybreak::VertexArray::Create();

	Daybreak::Ref<Daybreak::VertexBuffer> vb = Daybreak::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout(layout);
	va->AddVertexBuffer(vb);
	Daybreak::Ref<Daybreak::IndexBuffer> ib = Daybreak::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	va->SetIndexBuffer(ib);

	//DB_LOG(vb->GetLayout().GetElements()[0].Name);
	glUseProgram(shaderProgram);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void TestLayer::RenderTest()
{
	shader = Daybreak::Shader::Create("From File Shader", "../Sandbox/src/TestShader.glsl");
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	uint32_t indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	Daybreak::BufferLayout layout{
		{ Daybreak::RenderDataTypes::Float3, std::string("Position") }
	};
	if (r_color >= 1.0f)
		r_color = 0;
	else
		r_color += 0.01f;

	if (g_color >= 1.0f)
		g_color = 0;
	else
		g_color += 0.01f;


	va = Daybreak::VertexArray::Create();

	vb = Daybreak::VertexBuffer::Create(vertices, sizeof(vertices));
	vb->SetLayout(layout);
	va->AddVertexBuffer(vb);
	ib = Daybreak::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	va->SetIndexBuffer(ib);
	shader->Bind();
	shader->SetFloat4("u_Color", glm::vec4(r_color, g_color, 1.0f, 1.0f));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// This will be replaced with Render::Submit(shader, vertexArray, transform)
	Daybreak::RenderCommand::DrawIndexed(va);
}