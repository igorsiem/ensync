# This file is part of the enSync project, written by Igor Siemienowicz.
#
# Copyright (c) 2017 Igor Siemienowicz All rights reserved Distributed under
# the Boost Software License, Version 1.0 (see 
# http://www.boost.org/LICENSE_1_0.txt)

# This is a Rakefile to automate enSync development operations.

# --- Configuration ---

# Use the ruby 'pack' trick to determine platform bit-ness / word size
$wordsize = 32
$wordsize = 64 if ['foo'].pack("p").size == 8

# -- Directories --

# - Source Artefacrs -

# All separate and specific documentation files go under here
$docs_src_dir = "docs"

$high_level_docs_src_dir = "#{$docs_src_dir}"

# - Generated Artefacts -

# All build artefacts go here
$build_dir = "build"

# All generated documentation goes here
$docs_build_dir = "#{$build_dir}/docs"

puts "--- Configuration ---"
if Rake::Win32::windows?
    puts "platform: windows"
else
    puts "platform: assuming posix-compliant"
end
puts "word size: #{$wordsize} bits"
puts "directories:"
puts "    source:"
puts "        docs:            #{$docs_src_dir}"
puts "        high-level docs: #{$high_level_docs_src_dir}"
puts "    generated:"
puts "        build:           #{$build_dir}"
puts "        docs build:      #{$docs_build_dir}"

puts "---\n\n"

# --- Build and Clean ---

# Directories
directory $build_dir
directory $docs_build_dir

desc "clean all build artefacts"
task :clean do |t|
    FileUtils.rm_rf $build_dir
end

desc "build all binaries"
task :bin => $build_dir do

    # Default make is simple
    cmake_generator = nil
    make_command = "make"

    Dir.chdir "build"
    sh "cmake ../src #{cmake_generator}"
    sh make_command
    Dir.chdir ".."    

end

desc "build all documentation"
task :docs => $docs_build_dir do
    sh "doxygen"
end

# --- Testing ---

# TODO This will be expanded sigificantly as we add different kinds of tests

desc "run tests"
task :test => :bin do

    test_command = "build/test/test-ensync"
    sh test_command

end
