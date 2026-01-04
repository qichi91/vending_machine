cmake_minimum_required(VERSION 3.15)
project(VendingMachineCleanArch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# カバレッジ計測用オプション
option(ENABLE_COVERAGE "Enable coverage reporting" ON)
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

file(GLOB_RECURSE USECASES_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/usecases/*.cpp"
)

file(GLOB_RECURSE INTERFACE_ADAPTERS_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/interface_adapters/**/*.cpp"
)

file(GLOB_RECURSE FRAMEWORKS_DRIVERS_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/frameworks_drivers/**/*.cpp"
)

message(STATUS "USECASES_SOURCES: ${USECASES_SOURCES}")
message(STATUS "INTERFACE_ADAPTERS_SOURCES: ${INTERFACE_ADAPTERS_SOURCES}")
message(STATUS "FRAMEWORKS_DRIVERS_SOURCES: ${FRAMEWORKS_DRIVERS_SOURCES}")

# ライブラリの作成
add_library(domain STATIC ${DOMAIN_SOURCES})
target_include_directories(domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(USECASES_SOURCES)
    add_library(usecases STATIC ${USECASES_SOURCES})
    target_include_directories(usecases PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(usecases PUBLIC domain)
endif()

if(INTERFACE_ADAPTERS_SOURCES)
    add_library(interface_adapters STATIC ${INTERFACE_ADAPTERS_SOURCES})
    target_include_directories(interface_adapters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(interface_adapters PUBLIC domain usecases)
endif()

if(FRAMEWORKS_DRIVERS_SOURCES)
    add_library(frameworks_drivers STATIC ${FRAMEWORKS_DRIVERS_SOURCES})
    target_include_directories(frameworks_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
    target_link_libraries(frameworks_drivers PUBLIC interface_adapters)
endif()

# 実行可能ファイルの作成
add_executable(vending_machine src/main.cpp)
target_include_directories(vending_machine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(vending_machine PRIVATE 
    domain 
    usecases 
    interface_adapters
    frameworks_drivers
)

# テストの追加
add_subdirectory(test)

# カバレッジ計測用オプション (Clang Source-based)
if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Clang Source-based coverage enabled")
    add_compile_options("-fprofile-instr-generate" "-fcoverage-mapping")
    add_link_options("-fprofile-instr-generate" "-fcoverage-mapping")
endif()
