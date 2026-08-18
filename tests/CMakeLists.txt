find_package(Catch2 3 CONFIG REQUIRED)

add_executable(
    stlfix_tests
    core/GeometryTests.cpp
    core/MeshTests.cpp
    core/STLReaderTests.cpp
)

target_link_libraries(
    stlfix_tests
    PRIVATE
        stlfix_core
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(stlfix_tests)
