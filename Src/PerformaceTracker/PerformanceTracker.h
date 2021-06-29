#pragma once
#include <vector>
#include <map>
#include <string>

namespace scuff3d {
	class PerformanceTracker {
	public:
		PerformanceTracker();
		~PerformanceTracker();

		void beginFrame();
		void endFrame();

		const float getDT();


		class Measurement {
		public:
			Measurement();
			~Measurement();

			void addFrame();
			const float getDT();

			void reset();

		private:
			LARGE_INTEGER m_start, m_end;
			//std::queue<float> m_frames;
			float m_avg;
			float m_dt;
			float m_max;
			float m_min;


		};

		const std::map<std::string, Measurement>& getData();

	private:

		Measurement m_frame;
		std::map<std::string, Measurement> m_data;


	};
}