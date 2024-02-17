#pragma once

#include "Event.hpp"

namespace VX {
	class VX_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const {
			return m_KeyCode;
		}
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		int m_KeyCode;
		KeyEvent(int keyCode) : m_KeyCode(keyCode) {};
	};

	class VX_API KeyPressedEvent : public KeyEvent
	{
	public:
		EVENT_CLASS_TYPE(KeyPressed)
			KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {};

		inline int GetRepeatCount() const {
			return m_RepeatCount;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << "repeats)";
			return ss.str();
		}

	private:
		int m_RepeatCount;
	};

	class VX_API KeyReleasedEvent : public KeyEvent
	{
	public:
		EVENT_CLASS_TYPE(KeyReleased)
			KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {};

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
	};

}