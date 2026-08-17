add_executable(
    stlfix
    main.cpp
)

target_link_libraries(
    stlfix
    PRIVATE
        stlfix_core
)