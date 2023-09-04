
b_add_executable(bytereverse src/main.cpp)
target_link_libraries(bytereverse PUBLIC reverse)
target_precompile_headers(bytereverse REUSE_FROM reverse)