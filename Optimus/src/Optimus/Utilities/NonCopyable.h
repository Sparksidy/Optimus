#pragma once

#include <pch.h>
#include <Optimus/Core.h>


namespace OP
{
	class OPTIMUS_API NonCopyable
	{
	protected:
		NonCopyable() {}
		virtual ~NonCopyable() {}

	public:
		NonCopyable(const NonCopyable& other) = delete;
		NonCopyable& operator=(const NonCopyable& other) = delete;

		NonCopyable(NonCopyable&& other) = default;
		NonCopyable& operator=(NonCopyable&& other) = default;
	};
}