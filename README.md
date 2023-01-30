# Multi-Client TCP Server

[![ci](https://github.com/EricoDeMecha/N-Client-TCP-Server/actions/workflows/ci.yml/badge.svg)](https://github.com/cpp-best-practices/cmake_conan_boilerplate_template/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/EricoDeMecha/N-Client-TCP-Server/branch/main/graph/badge.svg)](https://codecov.io/gh/cpp-best-practices/cmake_conan_boilerplate_template)
[![Language grade: C++](https://img.shields.io/lgtm/grade/cpp/github/cpp-best-practices/cmake_conan_boilerplate_template)](https://lgtm.com/projects/g/cpp-best-practices/cmake_conan_boilerplate_template/context:cpp)
[![CodeQL](https://github.com/EricoDeMecha/N-Client-TCP-Server/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/cpp-best-practices/cmake_conan_boilerplate_template/actions/workflows/codeql-analysis.yml)

## About Multi-Client TCP Server application

This is a multi-client TCP server with support for priority communication between clients
such that a client with a lower rank, determined by the time of entry, cannot communicate
with a client of a higher rank.

## Getting Started

* [Dependency Setup](README_dependencies.md)

### Building

* [Building Details](README_building.md)
* [Docker](README_docker.md)
* 
### Troubleshooting

 * [Troubleshooting](README_troubleshooting.md)

## Testing

See [Catch2 tutorial](https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md)

## Fuzz testing

See [libFuzzer Tutorial](https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md)


