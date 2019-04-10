#include "stdafx.h"
#include "MySimulation.h"

MySimulation::MySimulation()
	: m_inputReader(&MySimulation::WaitForInput, this), m_input(std::nullopt) { }

MySimulation::~MySimulation()
{
	m_inputReader.detach();
}

void MySimulation::Init()
{
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1s);

	m_animator.AddAnimation("ThirdPersonWalk.anim", "Walk");
	m_animator.AddAnimation("ThirdPersonRun.anim", "Run");
	m_animator.Init("Walk");

	InitCommands();
}

void MySimulation::InitCommands()
{
	m_commands["anim"].AddListener(std::bind(&MySimulation::OnAnimCommand, this, std::placeholders::_1));
	m_commands["speed"].AddListener(std::bind(&MySimulation::OnSpeedCommand, this, std::placeholders::_1));
}

void MySimulation::Update(float p_frameTime)
{
	ParseInput();
	DrawAxis();
	m_animator.Update(p_frameTime);
}

void MySimulation::DrawAxis() const
{
	// X axis
	DrawLine(0, 0, 0, 100, 0, 0, 1, 0, 0);

	// Y axis
	DrawLine(0, 0, 0, 0, 100, 0, 0, 1, 0);

	// Z axis
	DrawLine(0, 0, 0, 0, 0, 100, 0, 0, 1);
}

void MySimulation::ParseInput()
{
	if (m_input.has_value())
	{
		size_t index = m_input->find("anim=");
		if (index != std::string::npos)
			m_commands["anim"].Invoke(m_input->substr(index + 5));

		index = m_input->find("speed=");
		if (index != std::string::npos)
			m_commands["speed"].Invoke(m_input->substr(index + 6));

		m_input = std::nullopt;
	}
}

void MySimulation::WaitForInput()
{
	std::string buffer;
	while (true)
	{
		std::cin >> buffer;
		m_input = buffer;
	}
}

void MySimulation::OnAnimCommand(std::string p_animation)
{
	m_animator.SetCurrentAnimation(p_animation);
}

void MySimulation::OnSpeedCommand(std::string p_speed)
{
	m_animator.GetCurrentAnimation()->SetSpeed(std::stof(p_speed));
}
