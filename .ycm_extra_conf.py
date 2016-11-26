def FlagsForFile(filename, **Kwargs):
    return {
        "flags": ['-isystem', '/usr/local/include', '-isystem', '/usr/lib/llvm-3.8/bin/../lib/clang/3.8.1/include', '-isystem', '/usr/include/x86_64-linux-gnu', '-isystem', '/usr/include', '-I/usr/include/libpng16', '-I/home/tony/Dev/normalmap/src', '-I.', '-Wall', '-Wextra', '-xc'],
        "do_cache": True
    }
