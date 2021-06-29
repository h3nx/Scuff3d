#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H
#include <queue>
#include <vector>
#include <string>
#include <mutex>

namespace scuff3d {
	class Console {
	public:

		const std::vector<std::string>& getMessages() {
			update();
			return m_messages;
		}
		void log(const std::string& message) {
			std::lock_guard<std::mutex> guard(logMutex);
			m_queue.emplace(message);
		}
	private:

		void update() {
			std::lock_guard<std::mutex> guard(logMutex);
			while (!m_queue.empty()) {
				m_messages.emplace_back(m_queue.front());
				m_queue.pop();
			}
		}

		std::vector<std::string> m_messages;
		std::queue<std::string> m_queue;
		std::mutex logMutex;
	};
}




#endif