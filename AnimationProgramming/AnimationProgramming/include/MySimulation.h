#pragma once

#include <Simulation.h>
#include "Animator.h"
#include "Event.h"

class MySimulation : public ISimulation
{
private:
	Animator m_animator;
	std::thread m_inputReader;
	std::optional<std::string> m_input;
	std::unordered_map<std::string, Event<std::string>> m_commands;

public:
	MySimulation();
	~MySimulation();

	virtual void Init() override;
	void InitCommands();

	virtual void Update(float p_frameTime) override;

	void DrawAxis() const;
	void ParseInput();

private:
	void WaitForInput();

	void OnAnimCommand(std::string p_animation);
	void OnSpeedCommand(std::string p_speed);
};
