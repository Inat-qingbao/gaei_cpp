﻿// gaei_cpp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
#include <memory>
#include "vertex.hpp"
#include "color.hpp"
#include "dat_loader.hpp"
#include "vrml_writer.hpp"
#include "ouchilib/program_options/program_options_parser.hpp"
#include "ouchilib/result/result.hpp"

[[nodiscard]]
ouchi::result::result<std::monostate, std::string>
load(std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& buf,
     const std::filesystem::path& p)
{
    using namespace std::string_literals;
    std::error_code err;
    bool d = std::filesystem::is_directory(p, err);
    // file not found
    if (err) return ouchi::result::err(err.message());
    // path is directory
    if (d) {
        for (auto&& subp : std::filesystem::directory_iterator(p)) {
            if(auto r = load(buf, subp.path()); !r) return ouchi::result::err(r.unwrap_err());
        }
    }
    // path is file
    if (p.extension() != ".dat") return ouchi::result::ok(std::monostate{});
    std::cout << "loading " << p.string() << std::endl;
    buf.reserve(buf.size() + std::filesystem::file_size(p) / 32/* line size */);
    gaei::dat_loader dl;
    if (auto r = dl.load(p); !r) return ouchi::result::err(std::string(r.unwrap_err()));
    else {
        for (auto&& v : r.unwrap()) buf.push_back(v);
    }
    return ouchi::result::ok(std::monostate{});
}

[[nodiscard]]
ouchi::result::result<std::vector<gaei::vertex<gaei::vec3f, gaei::color>>, std::string>
load(const std::vector<std::string>& path)
{

    std::vector<gaei::vertex<gaei::vec3f, gaei::color>> ret;
    for (auto&& p : path) {
        std::filesystem::path fp{ p };
        if (auto r = load(ret, fp); !r) return ouchi::result::err(r.unwrap_err());
    }
    return ouchi::result::ok(std::move(ret));
}

bool write(const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vs,
           std::string path)
{
    namespace vrml = gaei::vrml;
    std::ofstream of{ path };
    vrml::vrml_writer vw;
    vrml::shape<vrml::point_set, vrml::appearance<>> sp;
    sp.geometry().points.reserve(vs.size());
    // 欠損点をコピーしない
    for (auto& i : vs) {
        if (i.position.z() > -50)
            sp.geometry().points.push_back(i);
    }
    vw.push(std::move(sp));
    return vw.write(path);
}

int main(int argc, const char** const argv)
{
    namespace po = ouchi::program_options;
    using namespace std::literals;
    po::options_description d;
    d
        .add("", "file or directory to read", po::multi<std::string>)
        .add("out;o", "name of output file.", po::default_value = "out.wrl"s, po::single<std::string>);

    po::arg_parser p;
    p.parse(d, argv, argc); 
    auto in = p.get<std::vector<std::string>>("");
    if (in.size() == 0) {
        std::cout << "少なくとも一つ以上のファイルまたはディレクトリが入力されていなければなりません\n";
        std::cout << d << std::endl;
        return -1;
    }    
    auto r = load(in);
    if (!r) {
        std::cout << r.unwrap_err() << std::endl;
        for (auto i : r.unwrap_err())
            std::cout << std::hex << (int)i << ' ';
        return -1;
    }
    auto out_path = p.get<std::string>("out");
    std::cout << "out:" << out_path << std::endl;
    write(r.unwrap(), out_path);
	return 0;
}
