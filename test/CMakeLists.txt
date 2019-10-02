cmake_minimum_required (VERSION 3.8)

project ("gaei_cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_EXTENSIONS OFF) 
find_package(Threads REQUIRED)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()

include_directories("./ouchitest/include")
include_directories("../gaei_cpp/thirdparty/ouchilib/include")
include_directories("../gaei_cpp")

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (
	gaei_test 
	"test_main.cpp"
	"test_vertex.cpp"
	"test_dat_loader.cpp"
	"test_color.cpp"
)
