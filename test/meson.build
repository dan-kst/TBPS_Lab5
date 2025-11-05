#project('ConsoleMenuTest', 'cpp')

# 1. Find GTest
gtest_dep = dependency('gtest', main: true, required: true)

# 2.1 Form dependency
tests_dep = [gtest_dep, lab5_dep]

# 2.2 Form source files
test_sources = [
	'test_lab5.cpp'
]

# 3.1. Import the filesystem module
fs = import('fs')

# 3.2. Define the path to your samples
sample_dir_name = 'samples'
sample_dir_path = meson.current_source_dir() / sample_dir_name

# 3.3. Get all files in the directory
all_files = fs.listdir(sample_dir_path)

# 3.4. Loop over all files and copy *only* the .txt files
foreach f : all_files
  if f.ends_with('.txt')
    # This is the path to the original file, e.g., 'samples/matrix1.txt'
    input_file = sample_dir_name / f
    
    # This is the destination, e.g., 'samples/matrix1.txt' in the build dir
    output_file = sample_dir_name / f

    configure_file(
      input : input_file,
      output : output_file,
      copy : true
    )
  endif
endforeach



# 4. Build the test executable
test_exe = executable(
	'lab5_tests_run', 
	test_sources, 
	dependencies: tests_dep,
	include_directories: 'samples'
)

# 4. Register it as a test
test(
	'Lab5 Tests',
	test_exe)
