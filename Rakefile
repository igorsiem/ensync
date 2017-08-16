# This file is part of the enSync project, written by Igor Siemienowicz.
#
# Copyright (c) 2017 Igor Siemienowicz All rights reserved Distributed under
# the Boost Software License, Version 1.0 (see 
# http://www.boost.org/LICENSE_1_0.txt)

# This is a Rakefile to automate enSync development operations.

# --- Configuration ---

# Change stuff in this section to suite your build environment
puts "--- Configuration ---"
if Rake::Win32::windows?
    puts "environment: Windows"
else
    # TODO Get a bit more specific with non-windows environments
    puts "environment: Assuming posix-compliant"
end

# A directory in which file artefacts from tests may be placed. It is cleared
# before every test run.
$test_artefacts_path = "test-artefacts"

# Use the ruby 'pack' trick to determine platform bit-ness / word size
wordsize = 32
wordsize = 64 if ['foo'].pack("p").size == 8
puts "word size: #{wordsize} bits"

puts "---\n\n"

# --- Build and Clean ---

# CMake build directory
directory "build"

desc "build all binaries"
task :binaries => "build" do

    # Default make is simple
    cmake_generator = nil
    make_command = "make"

    # We have some extra stuff to do for Windows
    if Rake::Win32::windows?
        
        # Make sure that our environment is set up for VS 2017.
        vsyear = 2017   # Visual Studio year, ...
        vernum = 15     # ... version number, ...
        envnum = 14     # ... and environment variable number.
        env_var_name = "VS#{envnum}0COMNTOOLS"

        raise "could not find environment variable \"#{env_var_name}\" - " +
            "is Visual Studio 2017 installed?" \
            if ENV[env_var_name].to_s.empty?

        raise "could not find environment variable \"VCINSTALLDIR\" - " +
            "has \"vcvars.bat\" been run?" if ENV['VCINSTALLDIR'].to_s.empty?

        # Work out the cmake generator and msbuild details
        platform = "Win32"
        gen_platform = "Win32"
        if wordsize == 64
            platform = "x64"
            gen_platform = "Win64"
        end
        cmake_generator = "-G \"Visual Studio #{vernum} #{vsyear} " +
            "#{gen_platform}"

        make_command =
            "msbuild enSync.sln /p:Configuration=Release " +
            "/p:Platform=\"#{platform}\" /m"

    end # if we are in Windows

    Dir.chdir "build"
    sh "cmake ../src #{cmake_generator}"
    sh make_command
    Dir.chdir ".."    

end

desc "clean all build artefacts"
task :clean do |t|
    FileUtils.rm_rf "build"
end

# --- Testing ---

# TODO This will be expanded sigificantly as we add different kinds of tests

desc "Run tests"
task :test => :binaries do

    # Delete and re-create the test artefacts directory
    FileUtils.rm_rf $test_artefacts_path
    FileUtils.mkpath $test_artefacts_path

    config = nil
    config = "Release" if Rake::Win32::windows?

    test_command = "build/test/#{config}/test-ensync"

    sh test_command
    
end

task :default
