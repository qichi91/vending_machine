# テストファイルの収集
file(GLOB_RECURSE TEST_SOURCES 
    "domain/**/*.cpp"
    "application/**/*.cpp"
    "infrastructure/**/*.cpp"
)

# テスト実行ファイルの作成
add_executable(unit_tests ${TEST_SOURCES})

target_include_directories(unit_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

target_link_libraries(unit_tests PRIVATE
    domain
    application
    infrastructure
    gtest_main
    gmock_main
)

# Google Test の検出と登録
include(GoogleTest)
gtest_discover_tests(unit_tests)
