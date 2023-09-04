
b_add_library(reverse STATIC src/reverse.cpp)
target_include_directories(reverse PUBLIC include)
target_link_libraries(reverse PUBLIC battery::core)
b_precompile_headers(reverse include/pch.h)