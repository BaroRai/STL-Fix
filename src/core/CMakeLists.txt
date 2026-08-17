add_library(stlfix_core)

target_sources(
    stlfix_core
    PRIVATE
        src/Mesh.cpp
        src/STLReader.cpp
)

target_compile_features(
    stlfix_core
    PUBLIC
        cxx_std_20
)