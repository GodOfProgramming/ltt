#pragma once
#include <unordered_map>
#include <string>

extern char** environ;

namespace sysinfo
{
	class Env: public std::unordered_map<std::string, std::string>
	{
	public:
		Env();
	};

	inline Env::Env()
	{
		for (int i = 0; environ[i]; i++) {
			std::string env = environ[i];

			auto eqpos = env.find_first_of('=');
			auto k = env.substr(0, eqpos);
			auto v = env.substr(eqpos + 1);

			(*this)[k] = v;
		}
	}
}  // namespace info

