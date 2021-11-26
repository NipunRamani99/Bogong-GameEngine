#include "Timer.hpp"
#include <string>
namespace bogong {
	std::chrono::steady_clock::time_point Timer::tp = std::chrono::steady_clock::time_point();
	std::vector<std::pair<std::string, std::chrono::duration<float,std::nano>>> Timer::durations;
	void Timer::Start() {
		tp = std::chrono::steady_clock::now();
	}
	void Timer::Clear() {
		durations.clear();
	}
	void Timer::LogTimeElapsed(std::string name) {
		auto now = std::chrono::steady_clock::now();
		auto diff = now - tp;
		std::chrono::system_clock::duration t;
		auto duration = (now - tp);
		durations.push_back(std::make_pair(name,duration));
	}
}