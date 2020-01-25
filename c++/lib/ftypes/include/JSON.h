#pragma once

#define JSON_GET(json, storage, ...) storage = json.get<decltype(storage)>(__VA_ARGS__)

namespace storage {
	class JSON {
	public:
		JSON();
        ~JSON() = default;

		/* Parses the document. Returns true if no parse errors */
		bool parse(const std::string& data);

		/* Sets the member with the specified value */
		template <typename T, typename... Args>
		void set(T value, Args&& ... args) {
			const char* path[sizeof...(args)] = { args... };
			auto member = getOrCreateMember<sizeof...(args)>(path);
			assert(member != nullptr);
			setValue(member, value);
		}

		/* Retrieves the memebr with the specified value */
		template <typename T, typename... Args>
		T get(Args&& ... args) {
			const char* path[sizeof...(args)] = { args... };
			auto member = getMember<sizeof...(args)>(path);
			assert(member != nullptr);
			return getValue<T>(member);
		}

		/* Checks if the member exists */
		template <typename... Args>
		bool memberExists(Args&& ... args) {
			const char* path[sizeof...(args)] = { args... };
			return getMember<sizeof...(args)>(path) != nullptr;
		}

		/* Erases the specifed member in the path*/
		template <typename... Args>
		bool erase(std::string mem, Args&& ... args) {
			const char* path[sizeof...(args)] = { args... };
			auto member = getMember<sizeof...(args)>(path);
			assert(member != nullptr);
			if (member->HasMember(mem.c_str())) {
				member->EraseMember(mem.c_str());
				return true;
			} else {
				return false;
			}
		}

		/* Outputs the document as a compressed string */
		std::string toString();

		/* Outputs the document as a formatted string */
		std::string toPrettyString();

	private:
		rapidjson::Document mDoc;

		template <typename T>
		void setValue(rapidjson::Value* member, T value);

		template <typename T>
		T getValue(rapidjson::Value* member);

		template <size_t size>
		rapidjson::Value* getOrCreateMember(const char* path[size]) {
			rapidjson::Value* val = &mDoc;
			for (size_t i = 0; i < size; i++) {
				auto& str = path[i];

				if (val->GetType() != rapidjson::Type::kObjectType) {
					val->SetObject();
				}

				if (!val->HasMember(str)) {
					val->AddMember(rapidjson::StringRef(str), rapidjson::Value(rapidjson::kNullType), mDoc.GetAllocator());
				}

				val = &(*val)[str];
			}
			return val;
		}

		template <size_t size>
		rapidjson::Value* getMember(const char* path[size]) {
			rapidjson::Value* val = &mDoc;
			for (size_t i = 0; i < size; i++) {
				auto& str = path[i];
				if (val->GetType() != rapidjson::Type::kObjectType || !val->HasMember(str)) {
					return nullptr;
				}
				val = &(*val)[str];
			}
			return val;
		}
	};
}
