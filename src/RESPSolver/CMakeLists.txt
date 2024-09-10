

add_library(RESPSolver STATIC)
target_sources(RESPSolver PRIVATE RESPData.cpp)


target_include_directories(RESPSolver PUBLIC ${PROJECT_SOURCE_DIR}/include/RESPSolver 
                             ${PROJECT_SOURCE_DIR}/include/General)

target_link_libraries(RESPSolver PUBLIC General)
