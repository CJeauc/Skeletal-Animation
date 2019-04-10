#pragma once

#include <functional>

template<class... ArgTypes>
class Event
{
	using CallBack = std::function<void(ArgTypes...)>;
private:
	std::list<CallBack> m_callBacks;

public:
	void AddListener(CallBack p_callback)
	{
		m_callBacks.emplace_back(p_callback);
	}

	void RemoveListener(const CallBack& p_listener)
	{
		m_callBacks.remove(p_listener);
	}

	void Clear()
	{
		m_callBacks.clear();
	}

	void Invoke(ArgTypes... p_args)
	{
		for (CallBack const& val : m_callBacks)
			val(p_args...);
	}
};