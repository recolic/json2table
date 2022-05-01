#include "lib/json.hpp"
#include "lib/fort.hpp"
#include <rlib/stdio.hpp>
#include <rlib/opt.hpp>
#include <algorithm>
#include <string>

using nlohmann::json;
using std::string;
using std::vector;
using namespace std::literals;

inline bool program_mode = false;

template <typename T, typename Allocator>
inline void set_or_extend(std::vector<T, Allocator> &vthis, size_t index, T &&data) {
    if(vthis.size() <= index) vthis.resize(index+1);
    vthis[index] = std::forward<T>(data);
}

inline string json_to_string(json j) {
    if(j.is_array()) return program_mode ? j.dump() : "ARRAY";
    if(j.is_object()) return program_mode ? j.dump() : "OBJECT";
    if(j.is_string()) return j.get<string>();
    if(j.is_number_float()) return std::to_string(j.get<double>());
    if(j.is_number()) return std::to_string(j.get<long>());
    if(j.is_boolean()) return std::to_string(j.get<bool>());
    if(j.is_null()) return "null";
    return program_mode ? j.dump() : "UNKNOWN";
}

void json_decay_single_element_array(json &input) {
    while(input.is_array() && input.size() == 1)
        input = input[0];
}
void _json_get_subset(json &input, rlib::string subset_key) {
    // This function steps one-level deeper in the path
    json_decay_single_element_array(input);
    if(input.is_object()) {
        // Supports multiple col selection
        auto subset_keys = subset_key.split(',');
        if(subset_keys.size() == 1)
            // Simplest case. we must decay to the next level, if there's only one subset key. 
            input = input[subset_keys[0]];
        else {
            // Multiple subset keys. The result is a table in the same level, EVEN IF only one of them exists in result. 
            json result = {}; // Not-null even if none of keys exists. 
            for(auto &&k : subset_keys) {
                if(!k.empty()) result[k] = input[k];
            }
            input = std::move(result);
        }
    }
    else if(input.is_array()) {
        // Do element-wise-operation for every element.
        json result_json_arr = json::array();
        for(auto &[_, item] : input.items()) {
            json_decay_single_element_array(item);
            _json_get_subset(item, subset_key);
            result_json_arr.emplace_back(std::move(item));
        }
        input = std::move(result_json_arr);
    }
    else {
        throw std::invalid_argument("json_path is not valid for json. No element `" + subset_key + "` found in json input. (the result of previous level is neither object nor array)");
    }
}
void naive_json_access_path(json &input, rlib::string json_path) {
    for(auto &next : json_path.split('/')) {
        if(!next.empty()) {
            _json_get_subset(input, next);
        }
    }
}

int main(int argc, char **argv) {
    rlib::opt_parser args(argc, argv);
    if(args.getBoolArg("-h", "--help")) {
        rlib::println("json2table version 1.0.6-2, maintainer Recolic Keghart <root@recolic.net>");
        rlib::println("Usage: cat xxx.json | json2table");
        rlib::println("Usage: curl https://myapi/getJson | json2table /path/to/subobject");
        rlib::println("Set --programming / -p to make the output easier for program to process. ");
        rlib::println("You can use /path/to/col1,col2,col8 to select multiple columns. ");
        return 1;
    }
    program_mode = args.getBoolArg("-p", "--programming");

    auto json_path = args.getSubCommand("").replace("\\", "/").strip("/ \t");
    json input;
    std::cin >> input;
    naive_json_access_path(input, json_path);

    //////////////////////////////////////////////////////////////////////

    vector<string> headers; // it can be empty
    vector<vector<string>> rows(1);
    size_t curr_row_pos = 0;
    json_decay_single_element_array(input);
    if(input.is_array()) {
        // multi-row complete table
        for(auto &[_, item] : input.items()) {
            json_decay_single_element_array(item);
            if(item.is_object()) {
                // Perfect schema: [{}, {}, ...]
                for(auto &[key, value] : item.items()) {
                    // Add key-value into table.
                    auto iter = std::find(headers.cbegin(), headers.cend(), key);
                    auto col_pos = iter - headers.cbegin();
                    if(iter == headers.cend())
                        headers.emplace_back(key);
                    set_or_extend(rows[curr_row_pos], col_pos, json_to_string(value));
                }
            }
            else if(item.is_array()) {
                // It's an array of array. Print the matrix out!
                for(auto &[_, value] : item.items()) {
                    rows[curr_row_pos].push_back(json_to_string(value));
                }
            }
            else {
                rows[curr_row_pos].emplace_back("VAL: "s + json_to_string(item));
            }
            // TABLE: end this row.
            rows.emplace_back();
            ++curr_row_pos;
        }
    }
    else if(input.is_object()) {
        // single-row table
        for(auto &[key, value] : input.items()) {
            // Add key-value into table.
            headers.emplace_back(key);
            rows[curr_row_pos].emplace_back(json_to_string(value));
        }
    }
    else {
        // No way to create table. It's a single value, just put it in a 1x1 table. 
        rows[curr_row_pos].emplace_back("VAL: "s + json_to_string(input));
    }

    ////////////////////// Print-out the table

    if(program_mode) {
        for(auto &row : rows) {
            if(!row.empty())
                // Print \n instead of \r\n in Windows. Not using println. 
                std::cout << rlib::printable_iter(row, "|") << '\n';
        }
    }
    else {
        // Print the table.
        fort::char_table table;
        if(!headers.empty()) table << fort::header;
        for(auto &ele : headers) table << ele;
        if(!headers.empty()) table << fort::endr;
        for(auto &row : rows) {
            for(auto &ele : row) table << ele;
            table << fort::endr;
        }

        rlib::print(table.to_string());
    }

}
