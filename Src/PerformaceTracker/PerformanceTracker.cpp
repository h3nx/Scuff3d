#include "pch.h"
#include "PerformanceTracker.h"
namespace scuff3d
{
	static LARGE_INTEGER frequency;
	static float smoothing;
	//const std::map<std::string, PerformanceTracker::Measurement>& PerformanceTracker::getData()
	//{
	//	return m_data;
	//}

	PerformanceTracker::PerformanceTracker()
	{
		QueryPerformanceFrequency(&frequency);
	}

	void PerformanceTracker::beginFrame()
	{
		m_frame.addFrame();
	}

	const float PerformanceTracker::getDT()
	{
		return m_frame.getDT();
	}
















	PerformanceTracker::Measurement::Measurement() {
		reset();
	}

	PerformanceTracker::Measurement::~Measurement() {

	}

	void PerformanceTracker::Measurement::addFrame() {
		QueryPerformanceCounter(&m_start);
		m_dt = (float)((m_start.QuadPart - m_end.QuadPart) * 1.0 / frequency.QuadPart);
		m_avg += 

		
		m_avg = (m_avg * smoothing) + (m_dt * (1.0f - smoothing));

		m_end = m_start;
	}

	const float PerformanceTracker::Measurement::getDT() {
		return m_dt;
	}

	void PerformanceTracker::Measurement::reset() {
		m_avg = 0.0f;
		m_dt = 0.0f;
		m_max = 0.0f;
		m_min = 1.0f;
	}


}