#pragma once

#include <pch.h>
#include <Optimus/Core.h>

#include <type_traits>
#include <typeindex>

namespace OP
{
	using TypeId = std::size_t;

	template <typename T>
	class TypeInfo
	{
	public:
		TypeInfo() = delete;

		template <typename R>
		static TypeId GetTypeID() noexcept
		{
			auto index = std::type_index(typeid(R));
			if (auto it = m_TypeMap.find(index); it != m_TypeMap.end())
			{
				return it->second;
			}

			const auto id = NextTypeID();
			m_TypeMap[index] = id;
			return id;
		}

	private:

		static TypeId NextTypeID()noexcept
		{
			const auto id = m_NextTypeID;
			++m_NextTypeID;
			return id;
		}

		static TypeId m_NextTypeID;
		static std::unordered_map<std::type_index, TypeId> m_TypeMap;
	};

	template <typename R>
	TypeId TypeInfo<R>::m_NextTypeID = 0;

	template <typename R>
	std::unordered_map<std::type_index, TypeId> TypeInfo<R>::m_TypeMap = {};
}