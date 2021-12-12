#pragma once
#include <vector>
#include <map>
#include <string>
#include <stack>
//#include "Utils\Container\Tree.hpp"

namespace scuff3d {
	class PerformanceTracker {
	public:
		PerformanceTracker();
		~PerformanceTracker();
		void reset();

		void beginFrame();
		void endFrame();

		void begin(const std::string& name);
		void end(const std::string& name, const bool count = true);

		const float& getDT() const;

		class Measurement {
		public:
			Measurement();
			~Measurement();

			void begin();
			void end(const bool count);

			void setLayer(const int layer);

			const float& getAverage();
			const float& getMin();
			const float& getMax();
			const std::vector<float>& getHistory();
			const int& getLayer();

			void reset();

		private:
			LARGE_INTEGER m_start, m_end;
			std::vector<float> m_history;
			float m_avg;
			float m_max;
			float m_min;
			int m_layer;
		};

		struct Layer {
			std::string name;
			std::vector<Layer> next;
		};

		void renderImGui();

	private:

		LARGE_INTEGER m_timestamp, m_lastFrame;
		float m_presentCounter;
		float m_dt;
		float m_average;
		float m_max;
		float m_min;
		float m_updateAcc;
		std::vector<float> m_history;


		std::map<std::string, Measurement> m_data;
		std::stack<std::string> m_current;
		Layer m_structure;
		void renderLayer(const Layer& layer, const std::string previous = "");

		Layer& getCurrent(std::stack<std::string> tempStack);
		bool exists(const std::string& name);
		void add(const std::string& name);

	};
}