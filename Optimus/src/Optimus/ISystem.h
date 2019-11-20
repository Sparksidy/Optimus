#pragma once

#include <Optimus/Core.h>

#include <string>

namespace OP
{
	class OPTIMUS_API ISystem
	{
	public:
		ISystem():m_isInitialized(false) {};

		virtual ~ISystem(){};

		virtual bool Initialize() = 0;
		virtual void Update() = 0;
		virtual void Unload() = 0;

		virtual std::string GetName()const = 0;

	protected:
		bool m_isInitialized;

	};
}

