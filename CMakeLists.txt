# CMakeList.txt : CMake project for SQLSimulatorCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SQLSimulatorCMake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add source to this project's executable.
add_executable (SQLSimulatorCMake
	src/main.cpp
	src/BazaDeDate.cpp
	src/Coloana.cpp
	src/Interogare.cpp
	src/Rand.cpp
	src/Tabela.cpp
	src/Conditie.cpp
    src/ResultSet.cpp)

target_include_directories(SQLSimulatorCMake PRIVATE include)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SQLSimulatorCMake PROPERTY CXX_STANDARD 20)
  if(MSVC)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  endif()
endif()

# TODO: Add tests and install targets if needed.
