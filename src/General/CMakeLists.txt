add_library(General STATIC)
target_sources(General PRIVATE ThreadPool.cpp BaseEvent.cpp RedisReImpGeneral.cpp)

# for multi thread
find_package (Threads REQUIRED)
target_link_libraries(General
    PUBLIC Threads::Threads
)

target_include_directories(General PUBLIC ${PROJECT_SOURCE_DIR}/include/General
                                        ${PROJECT_SOURCE_DIR}/include/RESPSolver
                                        ${PROJECT_SOURCE_DIR}/third-party/include)
# include_directories(${PROJECT_SOURCE_DIR}/include/General)

target_link_libraries(General PUBLIC RESPSolver)
target_link_libraries(General PUBLIC ThirdParty)