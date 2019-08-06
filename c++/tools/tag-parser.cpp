#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_map>

typedef std::vector<std::string> string_vec;
typedef std::unordered_map<std::string, Tag> TagMap;
typedef std::unordered_map<std::string, std::string> AttributeMap;

void fill_vector(string_vec& vec, int count);

class Tag {
  public:
    Tag() = default;
    Tag(std::string name) : mName(name) { }

    Tag& operator[](const std::string& key) {
      return children[key];
    }

    std::string& operator()(const std::string& key) {
      return attributes[key];
    }

    TagMap children;
    AttributeMap attributes;

  private:
    std::string mName;
};

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

    TagMap getTags(const std::string& src) {
      TagMap roots;
      TagData tag_data;

      do {
	tag_data = get_tag_data(working_string, tag_data.next_pos);

	std::string body = src.substr(tag_data.close_start_tag_indx + 1, tag_data.open_end_tag_indx);

	Tag current(tag_data.tag_name);

	if (body.length()) {
	  current.children = std::move(getTags(tag_body));
	}

	roots[tag_data.tag_name] = current;
      } while(tag_data.next_pos != std::string::npos);

      return roots;
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

struct QueryCommmand {
  bool next_is_child_access = false;
  bool next_is_attr_access = false;
  
  std::string current_member_name;
};

class QueryRunner {
  public:
    QueryRunner(const TagMap& data) : mData(data) { }

    void runQueries(std::vector<std::string>& cmds) {
      for (auto& cmd : cmds) {
	std::vector<QueryCommand> queries = parseCommand(cmd);
	runQuery(queries, 0);
      }
    }

  private:
    const TagMap& mData;

    void runQuery(const std::vector<QueryCommand>& cmds, size_t indx, bool is_child_access, bool is_attr_access) {
      if (indx < cmds.size()) {
	QueryCommand cmd = cmds[indx];
	if (is_child_access) {

	} else if (is_attr_access) {

	}
	runQuery(cmds, indx + 1, cmd.is_child_access, cmd.is_attr_access);
      }
    }

    std::vector<QueryCommand> parseCommand(const std::string& line) {
      std::vector<QueryCommand> retval;

      size_t next_pos = 0;
      do {
	QueryCommand cmd;

	size_t tmp = line.find(".", next_pos);
	if (tmp != std::string::npos) {
	  cmd.next_is_child_access = true;
	} else {
	  tmp = line.find("~", next_pos);
	  if (tmp != std::string::npos) {
	    cmd.next_is_attr_access = true;
	  }
	}

	cmd.current_member_name = line.substr(next_pos, tmp);
	next_pos = tmp + 1;

	retval.push_back(cmd);
      } while (next_pos != std::string::npos);

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

  std::string_stream ss;
  for (const auto& line : src_code) {
    ss << line;
  }

  TagParser parser;

  TagMap tags = parser.getTags(ss.str());

  QueryRunner runner;
  runner.runQueries(
  return 0;
}

void fill_vector(string_vec& vec, int count) {
  for (int i = 0; i < count; i++) {
    std::string line;
    std::getline(std::cin, line);
    vec.push_back(line);
  }
}


