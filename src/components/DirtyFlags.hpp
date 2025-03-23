#pragma once

#include <chrono>

struct UpdateEntityEvent
{
	UpdateEntityEvent() : timeStart(std::chrono::steady_clock::now()), maxDuration(0) {}
	UpdateEntityEvent(std::chrono::milliseconds duration) : timeStart(std::chrono::steady_clock::now()), maxDuration(duration) {}
	std::chrono::steady_clock::time_point timeStart;
	std::chrono::milliseconds maxDuration;

	bool isReady()
	{
		const auto currDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeStart);
		if (currDuration <= maxDuration) return false;
		timeStart = std::chrono::steady_clock::now();
		return true;
	}
};

struct UpdateEntityPolling
{
	UpdateEntityPolling() : timeStart(std::chrono::steady_clock::now()), maxDuration(0) {}
	UpdateEntityPolling(std::chrono::milliseconds duration, bool readyOnStart = true) : timeStart(std::chrono::steady_clock::now()), maxDuration(duration) {}
	std::chrono::steady_clock::time_point timeStart;
	const std::chrono::milliseconds maxDuration;

	bool isReady()
	{
		const auto currDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - timeStart);
		if (currDuration <= maxDuration) return false;
		timeStart = std::chrono::steady_clock::now();
		return true;
	}
};