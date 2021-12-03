#include "pch.h"
#include "PerformanceTracker.h"
namespace scuff3d {

	// frequency for quantum
	LARGE_INTEGER frequency;
	float saveInterval = 1/10.f;
	float smoothing = 0.90f;

	PerformanceTracker::PerformanceTracker() {
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&m_timestamp);
		QueryPerformanceCounter(&m_lastFrame);

		m_structure = {"Root",std::vector<Layer>()};
		m_current.push("Root");
	}
	PerformanceTracker::~PerformanceTracker() { }
	void PerformanceTracker::reset() { 
	
	}

	void PerformanceTracker::beginFrame() {
		QueryPerformanceCounter(&m_timestamp);
		m_dt = (float)((m_timestamp.QuadPart - m_lastFrame.QuadPart) * 1.0 / frequency.QuadPart);
		m_average = (m_average * smoothing) + (m_dt * (1.0f - smoothing));

		m_lastFrame = m_timestamp;
	}
	void PerformanceTracker::endFrame() { 
		//m_data["Frame"].end();
	}

	void PerformanceTracker::begin(const std::string& name) {
		bool found = exists(name);
		m_data[name].begin();
		if (!found)
			add(name);
		m_current.push(name);
	}
	void PerformanceTracker::end(const std::string& name, const bool count) {
		m_data[name].end(count);
		m_current.pop();
	}

	const float& PerformanceTracker::getDT() const {
		return m_dt;
	}

	void PerformanceTracker::renderImGui() {

		ImGui::Text("fps");
		ImGui::SameLine();
		ImGui::rText(toString(1.0f/m_average));

		ImGui::SetWindowFontScale(1.0f);
		ImGui::Text("Saveinterval");
		ImGui::SameLine(110);
		float hz = 1.0f / saveInterval;
		if (ImGui::SliderFloat("##interval", &hz, 1, 60)) {
			saveInterval = 1.0f / hz;
		}
		ImGui::Text("Smoothing");
		ImGui::SameLine(110);
		if (ImGui::SliderFloat("##smoothing", &smoothing, 0.0001f, 0.9999f, "%.5f")) {
		}
		
		for (auto& layer : m_structure.next) {
			renderLayer(layer);
		}
		


		
		
	}
	void PerformanceTracker::renderLayer(const Layer& layer, const std::string previous) {

		const std::string& n = layer.name;
		//ImGui::Indent(10);
		ImGui::SetWindowFontScale(1.0f);
		//if (layer.next.size() > 0) {
			ImGui::SetWindowFontScale(1.3f);
			if (ImGui::TreeNode(n.c_str())) {
				ImGui::SetWindowFontScale(1.0f);

				//ImGui::Text("Elapsed Time");
				ImGui::Text(std::to_string(m_data[n].getAverage()) + "s");
				if (previous != "") {
					ImGui::Text(toString((m_data[n].getAverage() / m_data[previous].getAverage())*100.0f,0) + "%%");
				}
				for (auto& layr : layer.next) {
					renderLayer(layr, n);
					
				}
				ImGui::TreePop();
			}
			/*if (ImGui::CollapsingHeader(n.c_str())) {
				ImGui::rText(std::to_string(m_data[n].getAverage()) + "s");
				ImGui::Text("");
				ImGui::SameLine();
				ImGui::Separator();
				for (auto& layr : layer.next) {
					renderLayer(layr);
				
				}
			}*/
		//}
		//else {
		//	ImGui::Indent(10);
		//	ImGui::Text(n);
		//	ImGui::Indent(10);
		//	ImGui::Text(std::to_string(m_data[n].getAverage()) + "s"); 
		//	ImGui::Unindent(20);
		//	//ImGui::rText(std::to_string(1.0f / m_data[n].getAverage()) + "fps");
		//}
		//ImGui::Unindent(10);
	}


	PerformanceTracker::Layer& PerformanceTracker::getCurrent(std::stack<std::string> tempStack) {
		Layer* ptr = &m_structure;
		std::vector<std::string> vec;
		while (tempStack.size() > 1) {
			vec.insert(vec.begin(), tempStack.top());
			tempStack.pop();
		}
		for (auto& step : vec) {
			for (auto& layr : ptr->next) {
				if (layr.name == step) {
					ptr = &layr;
					break;
				}
			}
		}
		
		return *ptr;
	}
	bool PerformanceTracker::exists(const std::string& name) {
		return m_data.find(name) != m_data.end();
	}
	void PerformanceTracker::add(const std::string& name) { 
		Layer& top = getCurrent(m_current);
		std::vector<Layer>& vec = top.next;
		vec.push_back({ name, {} });
	}
















	PerformanceTracker::Measurement::Measurement() {
		reset();
	}

	PerformanceTracker::Measurement::~Measurement() { }

	void PerformanceTracker::Measurement::begin() { 
		QueryPerformanceCounter(&m_start);
	}
	void PerformanceTracker::Measurement::end(const bool count) {
		if (count) {
			QueryPerformanceCounter(&m_end);
			float dt = (float)((m_end.QuadPart - m_start.QuadPart) * 1.0 / frequency.QuadPart);
			m_avg = (m_avg * smoothing) + (dt * (1.0f - smoothing));
		}
	}

	
	const float& PerformanceTracker::Measurement::getAverage() {
		return m_avg;
	}

	const float& PerformanceTracker::Measurement::getMin() {
		return m_min;
	}

	const float& PerformanceTracker::Measurement::getMax() {
		return m_max;
	}

	const int& PerformanceTracker::Measurement::getLayer() {
		return m_layer;
	}


	void PerformanceTracker::Measurement::reset() {
		m_avg = 0.0f;
		m_max = 0.0f;
		m_min = 1.0f;
		m_layer = 0;
		m_history.clear();
	}


}