cmake_minimum_required(VERSION 3.16)
project(boid VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")

find_package(SFML COMPONENTS graphics REQUIRED)
#vvvvvv da controllare se giusto vvvvvvv
find_package(TGUI REQUIRED)

add_executable(boid source/point.cpp source/boid.cpp source/quadtree.cpp source/statistics.cpp source/sfml.cpp source/gui.cpp source/main.cpp)

#vvvvvv da controllare se giusto vvvvvvv
target_link_libraries(boid PRIVATE sfml-graphics tgui)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione

if (BUILD_TESTING)

  # aggiungi l'eseguibile boid.t
  add_executable(boid.t source/point.cpp source/boid.cpp source/quadtree.cpp source/statistics.cpp source/sfml.cpp source/gui.cpp source/test/boids.test.cpp)
  target_link_libraries(boid.t PRIVATE sfml-graphics tgui)
  # aggiungi l'eseguibile boid.t alla lista dei test
  add_test(NAME boid.t COMMAND boid.t)


endif()

# se le statistiche sono abilitate...
#   per abilitare le statistiche, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_STATISTICS)

  # aggiungi l'eseguibile boids.s
  add_executable(boid.s source/point.cpp source/boid.cpp source/quadtree.cpp source/statistics.cpp source/sfml.cpp source/gui.cpp source/statistics/statistics_main.cpp)
  target_link_libraries(boid.s PRIVATE sfml-graphics tgui)
  #todo: not a test, figure out how to create it without making it a test
  add_test(NAME boid.s COMMAND boid.s)

endif()