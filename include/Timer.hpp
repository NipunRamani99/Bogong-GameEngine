#pragma once
#include <chrono>
#include <vector>
#include <utility>
#include <string>
namespace bogong {
	class Timer {
		static std::chrono::steady_clock::time_point tp;
	public:
		static void Start();
		static void LogTimeElapsed(std::string name);
		static void Clear();
		static std::vector<std::pair<std::string,std::chrono::duration<float,std::nano>>> durations;
	};
}