#pragma once

namespace sysinfo
{
	class ARGV : public std::vector<std::string>
	{
		public:
			ARGV(int arc, char* argv[]);
		private:
	};

	inline ARGV::ARGV(int argc, char* argv[])
	{
		this->resize(argc);
		for (int i = 0; i < argc; i++)
		{
			(*this)[i] = argv[i];
		}
	}
}
