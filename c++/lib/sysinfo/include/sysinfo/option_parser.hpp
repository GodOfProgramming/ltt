#pragma once
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <istream>
#include <sstream>
#include <iterator>
#include <iostream>

namespace sysinfo
{
	struct Opt
	{
		std::string SName;
		std::string LName;
		std::string ArgName;
		std::string Desc;
		std::function<void(bool)> OnFindBool;
		std::function<void(std::string)> OnFindStr;

		bool Advanced = false;  // follows the format "arg [some_field]"
	};

	class Options
	{
	public:
		std::string Banner;

		void on(std::string s_name, std::string l_name, std::string desc, std::function<void(bool)> onfind);
		void on(std::string s_name, std::string l_name, std::string desc, std::function<void(std::string)> onfind);

		void printHelp();

		std::shared_ptr<Opt> find(std::string name);

		bool isAdvancedOpt(
		 std::string s_name, std::string l_name, std::string& new_s_name, std::string& new_l_name, std::string& argument);

		bool shouldShowHelp(int argc, const char* const argv[]);

	private:
		std::unordered_map<std::string, std::shared_ptr<Opt>> mShortNames;
		std::unordered_map<std::string, std::shared_ptr<Opt>> mLongNames;

		size_t mMaxOptLength = 0;
	};

	class OptionParser
	{
	public:
		OptionParser(int argc, const char* const argv[], std::function<void(Options&)> initializer);

		std::vector<std::string> parse();

		std::string getArgValue(int& indx);

	private:
		int mArgc;
		const char* const* mArgv;
		Options mOpts;
	};

	inline OptionParser::OptionParser(int argc, const char* const argv[], std::function<void(Options&)> initializer)
	 : mArgc(argc), mArgv(argv)
	{
		initializer(mOpts);
	}

	inline std::vector<std::string> OptionParser::parse()
	{
		if (mOpts.shouldShowHelp(mArgc, mArgv)) {
			mOpts.printHelp();
			std::exit(0);
		}

		std::vector<std::string> remaining;
		for (int i = 1; i < mArgc; i++) {
			std::string arg = mArgv[i];
			auto opt = mOpts.find(arg);

			if (opt) {
				if (opt->OnFindBool) {
					opt->OnFindBool(true);
				} else if (opt->OnFindStr) {
					opt->OnFindStr(getArgValue(i));
				}
			} else {
				remaining.push_back(arg);
			}
		}

		return remaining;
	}

	inline std::string OptionParser::getArgValue(int& i)
	{
		auto is_short_opt = [](std::string& opt) -> bool { return opt.length() == 2 && opt[0] == '-' && opt[1] != '-'; };
		auto is_long_opt = [](std::string& opt) -> bool { return opt.length() >= 3 && opt[0] == '-' && opt[1] == '-'; };

		std::string current_arg = mArgv[i];
		auto eqpos = current_arg.find_first_of('=');

		if (eqpos != std::string::npos && is_long_opt(current_arg)) {
			return current_arg.substr(eqpos + 1);
		} else if (i + 1 < mArgc && (is_short_opt(current_arg) || is_long_opt(current_arg))) {
			auto retval = std::string(mArgv[++i]);          // retval is the next option
			if (retval.length() > 1 && retval[0] == '-') {  // retval is another option -- invalid thus return empty string
				return std::string();
			}
			return retval;
		}
		return std::string();
	}

	inline void Options::on(std::string s_name, std::string l_name, std::string desc, std::function<void(bool)> onfind)
	{
		if (s_name.length() + l_name.length() > mMaxOptLength) {
			mMaxOptLength = s_name.length() + l_name.length();
		}
		auto opt = std::make_shared<Opt>();
		opt->Desc = desc;
		opt->OnFindBool = onfind;
		opt->SName = s_name;
		opt->LName = l_name;
		mShortNames[s_name] = opt;
		mLongNames[l_name] = opt;
	}

	inline void Options::on(std::string s_name, std::string l_name, std::string desc, std::function<void(std::string)> onfind)
	{
		std::string new_s_name, new_l_name, arg;
		if (isAdvancedOpt(s_name, l_name, new_s_name, new_l_name, arg)) {
			if (new_s_name.length() + new_l_name.length() + arg.length() > mMaxOptLength) {
				mMaxOptLength = s_name.length() + l_name.length();
			}
			auto opt = std::make_shared<Opt>();
			opt->Desc = desc;
			opt->OnFindStr = onfind;
			opt->SName = new_s_name;
			opt->LName = new_l_name;
			opt->Advanced = true;
			opt->ArgName = arg;
			mShortNames[new_s_name] = opt;
			mLongNames[new_l_name] = opt;
		}
	}

	inline void Options::printHelp()
	{
		std::cout << Banner << '\n';

		for (auto& pair : mShortNames) {
			auto& opt = pair.second;
			std::cout << '\t' << opt->SName << ", " << opt->LName << ' ' << opt->ArgName;
			std::cout.width(mMaxOptLength - opt->SName.length() - opt->LName.length());
			std::cout << '\t' << opt->Desc << '\n';
		}
	}

	inline std::shared_ptr<Opt> Options::find(std::string name)
	{
		auto eqpos = name.find_first_of('=');

		if (eqpos != std::string::npos) {
			name = name.substr(0, eqpos);
		}

		if (mShortNames.find(name) != mShortNames.end()) {
			return mShortNames[name];
		}

		if (mLongNames.find(name) != mLongNames.end()) {
			return mLongNames[name];
		}

		return nullptr;
	}

	inline bool Options::isAdvancedOpt(
	 std::string s_name, std::string l_name, std::string& new_s_name, std::string& new_l_name, std::string& argument)
	{
		auto verify_complex_opt = [](std::string opt_name) { return opt_name[0] == '[' && opt_name[opt_name.length() - 1] == ']'; };

		// short results
		{
			std::istringstream iss(s_name);
			std::vector<std::string> results(std::istream_iterator<std::string>{ iss }, std::istream_iterator<std::string>());
			if (results.size() > 1 && verify_complex_opt(results[1])) {  // opt name is complex is valid
				new_l_name = l_name;
				new_s_name = results[0];
				argument = results[1];
				return true;
			}
		}

		// long results
		{
			std::istringstream iss(l_name);
			std::vector<std::string> results(std::istream_iterator<std::string>{ iss }, std::istream_iterator<std::string>());
			if (results.size() > 1 && verify_complex_opt(results[1])) {  // opt name is complex is valid
				new_s_name = s_name;
				new_l_name = results[0];
				argument = results[1];
				return true;
			}
		}

		return false;
	}

	inline bool Options::shouldShowHelp(int argc, const char* const argv[])
	{
		std::string s = "-h", l = "--help";
		bool short_overriden = mShortNames.find(s) != mShortNames.end();
		bool long_overriden = mLongNames.find(l) != mLongNames.end();

		for (int i = 1; i < argc; i++) {
			if (s == argv[i] && !short_overriden) {
				return true;
			}

			if (l == argv[i] && !long_overriden) {
				return true;
			}
		}

		return false;
	}
}  // namespace sys
