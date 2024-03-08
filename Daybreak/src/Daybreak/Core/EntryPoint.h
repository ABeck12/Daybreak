#pragma once

#include "Daybreak/Core/Application.h"

extern Daybreak::Application* Daybreak::CreateApplication();

int main(int argc, char** argv)
{
	Daybreak::Log::Init();
	DB_CORE_INFO("Daybreak Engine Start");

	auto app = Daybreak::CreateApplication();
	app->Run();
	delete app;
}