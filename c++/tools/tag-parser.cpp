#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_map>

typedef std::vector<std::string> string_vec;
typedef std::unordered_map<std::string, std::string> AttributeMap;

void fill_vector(string_vec& vec, int count);

class Tag {
  public:
    Tag() = default;

    Tag& operator[](const std::string& key) {
      return children[key];
    }

    std::string& operator()(const std::string& key) {
      return attributes[key];
    }

    std::unordered_map<std::string, Tag> children;
    AttributeMap attributes;
};

typedef std::unordered_map<std::string, Tag> TagMap;

struct TagData  {
  std::string tag_name;
  size_t open_start_tag_indx = std::string::npos;
  size_t close_start_tag_indx = std::string::npos;
  size_t open_end_tag_indx = std::string::npos;
  size_t close_end_tag_indx = std::string::npos;
  size_t next_pos = 0;
};

class TagParser {
  public:
    TagParser() = default;

    Tag getTags(const std::string& src) {
      Tag root;
      TagMap roots;
      TagData tag_data;

      do {
	tag_data = get_tag_data(src, tag_data.next_pos);

	std::string body = src.substr(tag_data.close_start_tag_indx + 1, tag_data.open_end_tag_indx);

	Tag current;

	if (body.length()) {
	  Tag inner = getTags(body);
	  current.children = std::move(inner.children);
	}

	roots[tag_data.tag_name] = current;
      } while(tag_data.next_pos != std::string::npos);

      root.children = std::move(roots);

      return root;
    }

  private:
    TagData get_tag_data(const std::string& src, size_t offset) {
      size_t open_start_tag_indx = src.find_first_of("<", offset);
      size_t close_start_tag_indx = src.find_first_of(">", offset + open_start_tag_indx + 1);
      std::string tag_name = src.substr(open_start_tag_indx + 1, offset + close_start_tag_indx);

      size_t open_end_tag_indx = src.find_last_of("</" + tag_name + ">", offset);
      size_t close_end_tag_indx = src.find_first_of(">", offset + open_end_tag_indx + 1);

      TagData retval;
      retval.tag_name = tag_name;
      retval.open_start_tag_indx = open_start_tag_indx;
      retval.close_start_tag_indx = close_start_tag_indx;
      retval.open_end_tag_indx = open_end_tag_indx;
      retval.close_end_tag_indx = close_end_tag_indx;
      retval.next_pos = src.find_first_of("<", close_end_tag_indx + 1);

      return retval;
    }
};

struct QueryCommand {
  bool is_child_access = false;
  bool is_attr_access = false;
  
  std::string member_name;
};

class QueryRunner {
  public:
    QueryRunner(const Tag& root) : mRootTag(root) { }

    void runQueries(std::vector<std::string>& cmds) {
      for (auto& cmd : cmds) {
	std::vector<QueryCommand> queries = parseCommand(cmd);
	Tag start = mRootTag;
	for(auto& query : queries) {
	  if (query.is_child_access) {
	    start = start[query.member_name];
	  } else if (query.is_attr_access) {
	    std::cout << start(query.member_name) << '\n';
	  }
	}
      }
    }

  private:
    const Tag& mRootTag;

    std::vector<QueryCommand> parseCommand(const std::string& line) {
      std::vector<QueryCommand> retval;

      std::vector<size_t> dot_locations;
      size_t next_pos = line.find(".", next_pos);
      while (next_pos != std::string::npos) {
	dot_locations.push_back(next_pos);
	next_pos = line.find(".", next_pos);
      }

      size_t last_dot_location = 0;
      for (auto loc : dot_locations) {
	QueryCommand cmd;
	cmd.is_child_access = true;
	cmd.member_name = line.substr(last_dot_location, loc);
	last_dot_location = loc;
	retval.push_back(cmd);
      }

      QueryCommand cmd;
      size_t attr_access = line.find("~", last_dot_location);

      cmd.is_attr_access = true;
      cmd.member_name = line.substr(attr_access);
      retval.push_back(cmd);

      return retval;
    }
};

int main(int argc, char* argv[]) {
  std::string input;

  std::string src_line_count_arg(argv[1]);
  std::string query_line_count_arg(argv[2]); 

  int src_line_count = std::stoi(src_line_count_arg);
  int query_line_count = std::stoi(query_line_count_arg);

  std::vector<std::string> src_code;
  fill_vector(src_code, src_line_count);

  std::vector<std::string> queries;
  fill_vector(queries, query_line_count);

  std::stringstream ss;
  for (const auto& line : src_code) {
    ss << line;
  }

  TagParser parser;

  Tag root = parser.getTags(ss.str());

  QueryRunner runner(root);
  runner.runQueries();
  return 0;
}

void fill_vector(string_vec& vec, int count) {
  for (int i = 0; i < count; i++) {
    std::string line;
    std::getline(std::cin, line);
    vec.push_back(line);
  }
}


