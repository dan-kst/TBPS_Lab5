#Labs/Lab5/test/meson.build

# 1. Find GTest
gtest_dep = dependency('gtest', main: true, required: true)

# 2.1 Form dependency
tests_dep = [gtest_dep, lab5_dep]


# 2.2 Form source files

#subdir('samples')

test_sources = [
	'test_lab5.cpp'
]

# 4. Build the test executable
test_exe = executable(
	'lab5_tests_run', 
	test_sources, 
	dependencies: tests_dep,
	include_directories: ['.', 'samples']
#	link_depends: test_matrixes
)

# 4. Register it as a test
test(
	'Lab5 Tests',
	test_exe
)
