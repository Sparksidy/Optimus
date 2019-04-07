#include "pch.h"
#include "Input.h"

namespace OP
{
	InputManager* InputManager::s_Instance = nullptr;

	InputManager::InputManager()
	{
		OP_ASSERT(s_Instance);
		s_Instance = this;
	}
	InputManager::~InputManager()
	{
	}
	void InputManager::Initialize()
	{
		//Initialize Keyboard

		//Initialize Controller

		//Initialize Mouse
	}
	void InputManager::Update()
	{
		//Update Keyboard

		//Update Controller

		//Update Mouse

	}
	void InputManager::Shutdown()
	{
		//Shut Keyboard

		//Shut Controller

		//Shut Mouse
	}
}

