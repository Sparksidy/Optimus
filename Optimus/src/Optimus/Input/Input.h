#pragma once
#include "Core.h"

#include "Log.h"

namespace OP
{
	class OPTIMUS_API InputManager
	{
	public:
		InputManager();
		~InputManager();

		void Initialize();
		void Update();
		void Shutdown();

		inline static InputManager& Get() { return *s_Instance; }

	private:
		static InputManager* s_Instance;
		Keyboard m_keyBoard;
	};
}
