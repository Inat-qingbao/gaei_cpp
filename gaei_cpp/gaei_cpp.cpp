// gaei_cpp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
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
            load(buf, subp.path);
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
}

bool write(const std::vector<gaei::vertex<gaei::vec3f, gaei::color>>& vs,
           std::string path)
{
    std::ofstream of{ path };
    gaei::vrml::vrml_writer vw;

    for (auto&& v : vs) {
    }
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
    
    if (!p.exist("")) {
        std::cout << "少なくとも一つ以上のファイルまたはディレクトリが入力されていなければなりません\n";
        return -1;
    }    
    load(p.get<std::vector<std::string>>(""));
    auto out_path = p.get<std::string>("out");


	return 0;
}
