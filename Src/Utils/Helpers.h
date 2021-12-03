#pragma once
#include <map>
#include <vector>

#ifdef _DEBUG
//#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define NEW new

#else
#define NEW new
#endif

namespace scuff3d {





	template<typename type>
	inline void safeDelete(type* item) {
		if (item)
			delete item;
		item = nullptr;
	}

	template<typename index, typename item>
	inline void safeDeleteMap(std::map<index, item*>& map) {
		for (auto& pair : map) {
			safeDelete(pair.second);
		}
		map.clear();
	}
	template<typename type>
	inline void safeDeleteVector(std::vector<type*>& vec) {
		for (auto& item : vec) {
			safeDelete(item);
		}
		vec.clear();
	}


	template<typename type>
	inline void safeRelease(type* item) {
		if (item) {
			item->Release();
		}
		item = nullptr;
	}

	template<typename index, typename item>
	inline void safeReleaseMap(std::map<index, item*>& map) {
		for (auto& pair : map) {
			safeRelease(pair.second);
		}
		map.clear();
	}
	template<typename type>
	inline void safeReleaseVector(std::vector<type*>& vec) {
		for (auto& item : vec) {
			safeRelease(item);
		}
		vec.clear();
	}


	template<typename index, typename tFunc>
	inline void runMap(std::map<index, tFunc> map) {
		for (auto& pair : map) {
			if (pair.second) {
				pair.second();
			}
		}
	}
	template<typename index, typename tFunc, typename... tArgs>
	inline void runMap(std::map<index,tFunc> map, tArgs... args) {
		for (auto& pair : map) {
			if (pair.second) {
				pair.second(args...);
			}
		}
	}


}
