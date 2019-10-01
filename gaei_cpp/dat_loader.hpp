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
#include "vertex.hpp"
#include "color.hpp"

namespace gaei {

class dat_loader {
    static ouchi::tokenizer::separator<char> init_sep()
    {
        using namespace std::literals;
        return ouchi::tokenizer::separator<char>(std::in_place, { " "s, "\r\n" });
    }
    ouchi::tokenizer::tokenizer<char> tok_;

public:
    static inline const ouchi::tokenizer::separator<char> sep_ = init_sep();

    dat_loader() = default;
    
    /// <summary>
    /// ファイルをオープンし、<see cref="load(std::istream&amp;)"/>に渡す。
    /// </summary>
    ouchi::result::result<std::vector<vertex<vec3f, color>>, std::string_view>
    load(const std::filesystem::path& path) const;
    /// <summary>
    /// ストリームからデータを読み取り、パースして点集合を返す。
    /// </summary>
    /// <exception cref="std::exception">復帰不可能なエラーが標準ライブラリから送出される可能性があります</exception>
    /// <returns>
    /// 成功した場合、戻り値はtrueに評価される。`unwrap()`して点集合にアクセスできる。
    /// エラーの場合、戻り値はfalseに評価される。このエラーは復帰可能である場合がある。エラーの詳細は`unwrap_err()`でアクセスする。
    /// </returns>
    ouchi::result::result<std::vector<vertex<vec3f, color>>, std::string_view>
    load(std::istream& s) const;
private:

    ouchi::result::result<vertex<vec3f, color>, std::string_view>
    load_line(std::string_view line) const
    {
        vec3f pos;
        constexpr color def_c = colors::white;
    }
};

}
