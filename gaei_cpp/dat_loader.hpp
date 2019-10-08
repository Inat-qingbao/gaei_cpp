#pragma once
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string_view>
#include "ouchilib/result/result.hpp"
#include "ouchilib/tokenizer/tokenizer.hpp"
#include "ouchilib/utl/translator.hpp"
#include "vertex.hpp"
#include "color.hpp"

namespace gaei {

class dat_loader {
    static ouchi::tokenizer::separator<char> init_sep()
    {
        using namespace std::literals;
        return ouchi::tokenizer::separator<char>(std::in_place, { " "s, "\r\n"s });
    }

public:
    static inline const ouchi::tokenizer::separator<char> sep_ = init_sep();

    dat_loader() = default;
    
    /// <summary>
    /// ファイルをオープンし、<see cref="load(std::istream&amp;)"/>に渡す。
    /// </summary>
    ouchi::result::result<std::vector<vertex<vec3f, color>>, std::string>
    load(const std::filesystem::path& path) const
    {
        using namespace std::string_literals;
        if (!std::filesystem::exists(path))
            return ouchi::result::err("no such file or directory"s);
        std::ifstream s(path, std::ios::binary);
        return std::move(load(s));
    }
    /// <summary>
    /// ストリームからデータを読み取り、パースして点集合を返す。
    /// </summary>
    /// <exception cref="std::exception">復帰不可能なエラーが標準ライブラリから送出される可能性があります</exception>
    /// <returns>
    /// 成功した場合、戻り値はtrueに評価される。`unwrap()`して点集合にアクセスできる。
    /// エラーの場合、戻り値はfalseに評価される。このエラーは復帰可能である場合がある。エラーの詳細は`unwrap_err()`でアクセスする。
    /// </returns>
    /// <example>
    /// <code>
    /// dat_loader dl;
    /// auto r = dl.load(file_stream);
    /// if(r) {// 成功
    ///     some_work(r.unwrap());
    /// } else {
    ///     print_error(r.unwrap_err());
    /// }
    /// </code>
    /// </example>
    ouchi::result::result<std::vector<vertex<vec3f, color>>, std::string>
    load(std::istream& s) const
    {
        char line[33] = {};
        std::vector<vertex<vec3f, color>> vertexes;
        while (true) {
            std::memset(line, 0, sizeof(line));
            s.read(line, 32);
            if (s.eof())
                break;
            if (auto ver = load_line(line))
                vertexes.push_back(ver.unwrap());
            else return ouchi::result::err(ver.unwrap_err() + line);
        }
        return ouchi::result::ok(std::move(vertexes));
    }
private:

    ouchi::result::result<vertex<vec3f, color>, std::string>
    load_line(std::string_view line) const
    {
        using namespace std::string_literals;
        vec3f pos{};
        std::string buffer; // to reduce memory allocation (MSVC's SSO is up to 15 byte)
        typename ouchi::translator_between<std::string, float>::type translator;
        unsigned vec_c = 0;
        constexpr color def_c = colors::white;
        while (line.size()) {
            auto [tk, it] = sep_(line);
            auto token = line.substr(0, std::distance(line.begin(), it));
            line.remove_prefix(token.size());
            if (tk != ouchi::tokenizer::primitive_token::word) continue;
            if (auto value = translator.get_value(buffer.assign(token))) {
                pos.coord[vec_c++] = value.value();
            } else return ouchi::result::err("cannot translate string into float: "s);
        }
        if (vec_c < 3) return ouchi::result::err("too short line!"s);
        return ouchi::result::ok(vertex<vec3f, color>{pos, def_c});
    }
};

}
