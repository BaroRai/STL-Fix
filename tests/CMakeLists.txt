find_package(Catch2 3 CONFIG REQUIRED)

add_executable(
    stlfix_tests
    core/FixtureRepairTests.cpp
    core/GeometryTests.cpp
    core/MeshRepairTests.cpp
    core/MeshTests.cpp
    core/STLReaderTests.cpp
)

target_compile_definitions(
    stlfix_tests
    PRIVATE
        STLFIX_TEST_FIXTURE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/fixtures"
)

target_link_libraries(
    stlfix_tests
    PRIVATE
        stlfix_core
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(stlfix_tests)
