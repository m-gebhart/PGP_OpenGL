#include <chrono>
#include <thread>

static class PGP_Time {
private:
	static bool bTicking;
	static std::chrono::time_point<std::chrono::steady_clock> startTime;
	static std::chrono::time_point<std::chrono::steady_clock> previousTimePoint;
	static std::chrono::time_point<std::chrono::steady_clock> currentTimePoint;
	static float previousElapsedTime;

public:
	static float deltaTime;
	static float elapsedTime;
	static int FPS;

private:
	static void StartTime(void);
public:
	static void UpdateTime(void);
	static void SleepUntilFrameEnd(void);
};