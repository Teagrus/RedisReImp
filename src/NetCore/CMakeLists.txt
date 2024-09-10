# add_executable(NetCoreTest)
# target_sources(NetCoreTest PRIVATE NetCoreTest.cpp)


add_library(NetCore STATIC)
target_sources(NetCore PRIVATE NetCore.cpp)

# target_include_directories(NetCoreTest PUBLIC ${PROJECT_SOURCE_DIR}/include/NetCore 
#                              ${PROJECT_SOURCE_DIR}/include/General)

target_include_directories(NetCore PUBLIC ${PROJECT_SOURCE_DIR}/include/NetCore 
                             ${PROJECT_SOURCE_DIR}/include/General)

target_link_libraries(NetCore PUBLIC General)
target_link_libraries(NetCore PUBLIC RESPSolver)
# target_link_libraries(NetCoreTest PUBLIC NetCore)
# target_link_libraries(NetCoreTest PUBLIC General)
# target_link_libraries(NetCoreTest PUBLIC NetCore) 