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

template <typename T, typename Allocator>
inline void set_or_extend(std::vector<T, Allocator> &vthis, size_t index, T &&data) {
    if(vthis.size() <= index) vthis.resize(index+1);
    vthis[index] = std::forward<T>(data);
}

inline string json_to_string(json j) {
    if(j.is_array()) return "ARRAY";
    if(j.is_object()) return "OBJECT";
    if(j.is_string()) return j.get<string>();
    if(j.is_number_float()) return std::to_string(j.get<double>());
    if(j.is_number()) return std::to_string(j.get<long>());
    if(j.is_boolean()) return std::to_string(j.get<bool>());
    if(j.is_null()) return "null";
    return "UNKNOWN";
}

int main(int argc, char **argv) {
    rlib::opt_parser args(argc, argv);
    if(args.getBoolArg("-h", "--help")) {
        rlib::println("Usage: cat xxx.json | json2table");
        rlib::println("Usage: curl https://myapi/getJson | json2table /path/to/subobject");
        return 1;
    }

    auto json_path = args.getSubCommand("").replace("\\", "/").strip("/");
    json input;
    std::cin >> input;
    for(auto &next : json_path.split('/')) {
        if(!next.empty())
            input = input[next];
    }

    //////////////////////////////////////////////////////////////////////

    vector<string> headers;
    vector<vector<string>> rows(1);
    size_t curr_row_pos = 0;
    if(input.is_array()) {
        for(auto &[_, item] : input.items()) {
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
        // single-row
        for(auto &[key, value] : input.items()) {
            // Add key-value into table.
            headers.emplace_back(key);
            rows[curr_row_pos].emplace_back(json_to_string(value));
        }
    }
    else {
        // gg. Just print and go.
        rlib::println("Single value:", json_to_string(input));
        return 0;
    }

    // Print the table.
    fort::char_table table;
    table << fort::header;
    for(auto &ele : headers) table << ele;
    table << fort::endr;
    for(auto &row : rows) {
        for(auto &ele : row) table << ele;
        table << fort::endr;
    }

    rlib::println(table.to_string());

}
