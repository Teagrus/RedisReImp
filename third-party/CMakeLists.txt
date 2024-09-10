add_library(ThirdParty STATIC)
target_sources(ThirdParty PRIVATE src/ini.cpp src/INIReader.cpp )

target_include_directories(ThirdParty PUBLIC ${PROJECT_SOURCE_DIR}/third-party/include)
