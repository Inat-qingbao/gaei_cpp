// gaei_cpp.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
#include <memory>
#include <chrono>
#include <random>
#include "vertex.hpp"
#include "vector_utl.hpp"
#include "color.hpp"
#include "dat_loader.hpp"
#include "vrml_writer.hpp"
#include "surface_structure_isolate.hpp"
#include "reduce_points.hpp"
#include "normalize.hpp"
#include "wall.hpp"

#include "ouchilib/geometry/triangulation.hpp"
#include "ouchilib/program_options/program_options_parser.hpp"
#include "ouchilib/result/result.hpp"

[[nodiscard]]
ouchi::result::result<std::monostate, std::string>
load(std::vector<gaei::vertex<>>& buf,
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
    if(auto size = std::filesystem::file_size(p) >> 5/* / 32*/; buf.capacity() <= size)
        buf.reserve(9 * size/* line size */);
    gaei::dat_loader dl;
    if (auto r = dl.load(p, buf); !r) return ouchi::result::err(std::string(r.unwrap_err()));
    return ouchi::result::ok(std::monostate{});
}

[[nodiscard]]
ouchi::result::result<std::vector<gaei::vertex<>>, std::string>
load(const std::vector<std::string>& path)
{

    std::vector<gaei::vertex<>> ret;
    std::filesystem::path fp;
    for (auto&& p : path) {
        fp.assign(p);
        if (auto r = load(ret, fp); !r) return ouchi::result::err(r.unwrap_err());
    }
    return ouchi::result::ok(std::move(ret));
}

void label(std::vector<gaei::vertex<>>& vs, const ouchi::program_options::arg_parser& p)
{
    gaei::surface_structure_isolate ssi{ p.get<float>("diff") };
    std::cout << "calclating " << vs.size() << " points...\n";
    gaei::remove_error_point(vs);
    std::cout << "labeling points..." << std::endl;
    auto label_cnt = ssi(vs);
    std::cout << label_cnt << " labels" << std::endl;
    std::cout << "reducing points..." << std::endl;
    auto lc = gaei::count_label(label_cnt, vs);
    gaei::remove_trivial_surface(lc, vs);
    gaei::remove_minor_labels(lc, vs, p.get<size_t>("remove_minor_labels_threshold"));
    gaei::thinout(vs, p.get<int>("thinout_width"));
}
std::vector<long> triangulate(std::vector<gaei::vertex<>>& vs,
                              const ouchi::program_options::arg_parser& p)
{
    std::vector<long> faces;
    if (p.exist("printer")) {
        gaei::bounding_box(vs);
    }
    gaei::normalize(vs);
    std::cout << "triangulate " << vs.size() << " points...\n";
    ouchi::geometry::triangulation<gaei::vertex<>, 1000> t(1.0e-5);
    auto v = t(vs.cbegin(), vs.cend(), t.return_as_idx);

    faces.reserve(v.size() * 4 + 128);

    std::cout << "post-processing..." << std::endl;
    std::sort(v.begin(), v.end());
    auto e = std::unique(v.begin(), v.end());
    std::cout << "fail:" << std::distance(e, v.end()) << std::endl;
    gaei::inv_normalize(vs);
    for (auto& f : v) {
        for (auto idx : f) {
            faces.push_back((long)idx);
        }
        faces.push_back(-1);
    }
    if (p.exist("printer")) {
        gaei::create_wall(vs, faces);
    }
    return std::move(faces);
}

ouchi::result::result<std::monostate, std::string>
write(const std::vector<gaei::vertex<>>& vs,
      const std::vector<long>& faces,
      std::string path)
{
    namespace vrml = gaei::vrml;
    vrml::vrml_writer vw;
    vrml::shape<vrml::indexed_face_set, vrml::appearance<>> sp;
    sp.geometry().coord_.assign(vs.begin(), vs.end());
    sp.geometry().coord_index_ = faces;
    vw.push(std::move(sp));
    std::cout << "writing " << vs.size() << " points to " << path << '\n';
    return vw.write(path);
}

int main(const int argc, const char** const argv)
{
    namespace po = ouchi::program_options;
    namespace chrono = std::chrono;
    using namespace std::literals;
    auto beg = chrono::high_resolution_clock::now();
    po::options_description d;
    d
        .add("", ".datファイルへのパス/.datファイルを含むディレクトリへのパス", po::multi<std::string>)
        .add("out;o", "出力ファイル", po::default_value = "out.wrl"s, po::single<std::string>)
        .add("diff;d", "指定された値[m]だけzが異なる点に異なるラベルを付けます", po::single<float>, po::default_value = 1.0f)
        .add("nooutput;N", "ファイルへの出力を行いません", po::flag)
        .add("remove_minor_labels_threshold;t", "指定された値以下のサイズのラベルを削除します", po::single<size_t>, po::default_value = (size_t)5)
        .add("thinout_width;w", "点を間引く幅を指定します", po::default_value = 2, po::single<int>)
        .add("printer;p", "3Dプリンター用にデータを加工します。", po::flag);

    po::arg_parser p;
    p.parse(d, argv, argc); 
    auto parse_time = chrono::high_resolution_clock::now();
    auto in = p.get<std::vector<std::string>>("");
    if (in.size() == 0) {
        std::cout << "少なくとも一つ以上のファイルまたはディレクトリが入力されていなければなりません\n";
        std::cout << d << std::endl;
        return -1;
    }
    auto r = load(in);
    auto load_time = chrono::high_resolution_clock::now();
    if (!r) {
        std::cout << r.unwrap_err() << std::endl;
        return -1;
    }
    auto v = r.unwrap();
    if (p.exist("printer")) std::cout << "out for 3D printer\n";
    label(v, p);
    auto label_time = chrono::high_resolution_clock::now();
    auto tri = triangulate(v, p);
    auto tri_time = chrono::high_resolution_clock::now();
    auto out_path = p.get<std::string>("out");
    if (!p.exist("nooutput")) write(v, tri,out_path).unwrap_or_else([](auto e)->std::monostate {std::cout << e; return {}; });
    auto write_time = chrono::high_resolution_clock::now();
    std::cout << "out:" << out_path << std::endl;
    std::cout << "elappsed time"
        << "\nparse\t" << (parse_time - beg).count() / (double)chrono::high_resolution_clock::period::den
        << "\nload\t" << (load_time - parse_time).count() / (double)chrono::high_resolution_clock::period::den
        << "\nlabel\t" << (label_time - load_time).count() / (double)chrono::high_resolution_clock::period::den
        << "\ntri\t" << (tri_time - load_time).count() / (double)chrono::high_resolution_clock::period::den
        << "\nwrite\t" << (write_time - tri_time).count() / (double)chrono::high_resolution_clock::period::den
        << "\ntotal\t" << (write_time - beg).count() / (double)chrono::high_resolution_clock::period::den;
	return 0;
}
