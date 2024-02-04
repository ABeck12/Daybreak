#include <Daybreak.h>

#include "AudioLayer.h"

#include <imgui.h>

AudioLayer::AudioLayer() : Layer("AudioLayer")
{

}

void AudioLayer::OnUpdate(Daybreak::DeltaTime dt)
{

}

void AudioLayer::OnEvent(Daybreak::Event& event)
{
	if (event.GetEventType() == Daybreak::EventType::KeyPressed)
	{
		Daybreak::KeyPressedEvent& e = (Daybreak::KeyPressedEvent&)event;
		if (e.GetKeyCode() == Daybreak::Key::Escape)
		{
			Daybreak::Application::Get().Close();
		}
		if (e.GetKeyCode() == Daybreak::Key::T)
		{
			auto clip = Daybreak::AudioClip::Create("../Sandbox/assets/mynamejeff.mp3");
			Daybreak::AudioEngine::Play(clip);
		}
	}
}

void AudioLayer::OnImGuiRender()
{
	
}
