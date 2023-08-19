add_executable(p1 main.cpp)

target_include_directories(p1 PRIVATE "include")

target_sources(p1 PRIVATE src/Student.cpp)
