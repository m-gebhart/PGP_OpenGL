#include "PGP_Time.h"
#include <iostream>

std::chrono::time_point<std::chrono::steady_clock> PGP_Time::startTime = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> PGP_Time::currentTimePoint = std::chrono::steady_clock::now();
std::chrono::time_point<std::chrono::steady_clock> PGP_Time::previousTimePoint = std::chrono::steady_clock::now();

bool PGP_Time::bTicking = false;
float PGP_Time::previousElapsedTime = 0.0f;
float PGP_Time::deltaTime = 0.0f;
float PGP_Time::elapsedTime = 0.0f;
int PGP_Time::FPS = 30;

void PGP_Time::StartTime()
{
	PGP_Time::bTicking = true;
	PGP_Time::startTime =  PGP_Time::currentTimePoint = std::chrono::steady_clock::now();
	elapsedTime = 0.f;
}

void PGP_Time::UpdateTime()
{
	if (!PGP_Time::bTicking)
	{
		PGP_Time::StartTime();
		return;
	}

	previousElapsedTime = elapsedTime;
	previousTimePoint = currentTimePoint;
	currentTimePoint = std::chrono::steady_clock::now();

	PGP_Time::deltaTime = std::chrono::duration<float>(currentTimePoint - previousTimePoint).count();
	elapsedTime = std::chrono::duration<float>(currentTimePoint - startTime).count();
}

void PGP_Time::SleepUntilFrameEnd()
{
	float passedTimeOfCurrentFrame = std::chrono::duration<float>(std::chrono::steady_clock::now() - currentTimePoint).count();
	float sleepTimeMS = (1.0f / PGP_Time::FPS - passedTimeOfCurrentFrame)*1000.f;
	std::this_thread::sleep_for(std::chrono::milliseconds((int)sleepTimeMS));
}