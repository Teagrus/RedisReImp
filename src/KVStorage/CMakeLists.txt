add_library(KVStorage STATIC)
target_sources(KVStorage PRIVATE Containers.cpp)

target_include_directories(KVStorage PUBLIC ${PROJECT_SOURCE_DIR}/include/KVStorage 
                             ${PROJECT_SOURCE_DIR}/include/General)

target_link_libraries(KVStorage PUBLIC General)
