#Labs/Lab5/meson.build

omp_dep = dependency('openmp')

gausssolver_lib = library(
	'gausssolver_lib',
	['GaussSolver.cpp'],
	dependencies: omp_dep,
	include_directories: '.'
)

gausssolver_dep = declare_dependency(
	link_with: gausssolver_lib,
	include_directories: '.'
)

lab5_dep = [gausssolver_dep]

executable(
	'lab5_program',
	'main.cpp',
	dependencies: lab5_dep
)
