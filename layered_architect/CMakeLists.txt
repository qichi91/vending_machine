cmake_minimum_required(VERSION 3.15)
project(VendingMachineLayeredArch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# カバレッジ計測用オプション
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
  message(STATUS "Coverage reporting enabled")
    # コンパイラがClangであることを確認
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        # カバレッジ用フラグの設定
        add_compile_options("-fprofile-instr-generate" "-fcoverage-mapping")
        add_link_options("-fprofile-instr-generate" "-fcoverage-mapping")
    else()
        # カバレッジOnならDebugビルドに強制設定
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()
include(CTest)
enable_testing()

# ASan有効化オプション (デフォルト OFF)
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN)
    if(MSVC)
        add_compile_options(/fsanitize=address)
    else()
        # スペースではなく、セミコロンで区切る（または複数の引数として渡す）
        set(ASAN_FLAGS "-fsanitize=address" "-fno-omit-frame-pointer")
        
        # リストをそのまま渡せば、CMakeが適切に分解してくれます
        add_compile_options(${ASAN_FLAGS} -g)
        
        # リンク時も同様にリストで渡すのが安全です
        add_link_options("-fsanitize=address")
        
        message(STATUS "AddressSanitizer enabled (GCC/Clang)")
    endif()
endif()

# ユニットテスト用の設定（Google Test）
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# カバレッジの作成用ターゲット
find_program(GCOVR_PATH gcovr)
if(GCOVR_PATH)
    add_custom_target(coverage
        COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating HTML coverage report"
    )
    add_custom_target(coverage_html
        COMMAND ${GCOVR_PATH} -r ${CMAKE_SOURCE_DIR} --html-details -o coverage.html
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating HTML coverage report"
    )
endif()

# ソースファイルの収集
file(GLOB_RECURSE DOMAIN_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/domain/**/*.cpp"
)

file(GLOB_RECURSE APPLICATION_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/application/**/*.cpp"
)

file(GLOB_RECURSE INFRASTRUCTURE_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/infrastructure/**/*.cpp"
)

file(GLOB_RECURSE PRESENTATION_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/presentation/*.cpp"
)

message(STATUS "APPLICATION_SOURCES: ${APPLICATION_SOURCES}")
message(STATUS "PRESENTATION_SOURCES: ${PRESENTATION_SOURCES}")

# ライブラリの作成
add_library(domain STATIC ${DOMAIN_SOURCES})
target_include_directories(domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# application と infrastructure は後で追加予定
if(APPLICATION_SOURCES)
    add_library(application STATIC ${APPLICATION_SOURCES})
    target_include_directories(application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(application PUBLIC domain)
endif()

if(INFRASTRUCTURE_SOURCES)
    add_library(infrastructure STATIC ${INFRASTRUCTURE_SOURCES})
    target_include_directories(infrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(infrastructure PUBLIC domain)
endif()

if(PRESENTATION_SOURCES)
    add_library(presentation STATIC ${PRESENTATION_SOURCES})
    target_include_directories(presentation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(presentation PUBLIC domain application infrastructure)
endif()

# 実行可能ファイルの作成
add_executable(vending_machine src/main.cpp)
target_include_directories(vending_machine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(vending_machine PRIVATE 
    domain 
    application 
    infrastructure 
    presentation
)

# テストの追加
add_subdirectory(test)

# カバレッジ計測用オプション (Clang Source-based)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Clang Source-based coverage enabled")
    add_compile_options("-fprofile-instr-generate" "-fcoverage-mapping")
    add_link_options("-fprofile-instr-generate" "-fcoverage-mapping")
endif()
