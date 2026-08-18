add_library(stlfix_core)

target_sources(
    stlfix_core
    PRIVATE
        src/Geometry.cpp
        src/Mesh.cpp
        src/STLReader.cpp
)

target_include_directories(
    stlfix_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(
    stlfix_core
    PUBLIC
        cxx_std_20
)
