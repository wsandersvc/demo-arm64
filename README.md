# ARM64 C++ Sample Project
1.0.2
## Requirements

### Linux/macOS Build
- GCC 13 (gcc-13)
- Linux ARM64 target architecture
- Make build system

### Windows Build
- Visual Studio 2022 (or later) with C++ ARM64 support
- MSBuild
- Windows 10 SDK

## Building

To build the project:

```bash
make
```

### Building on macOS

```bash
docker build . -t gcc-veracode
docker run --rm -it -v .:/app -v ~/.veracode/veracode.yml:/root/.veracode/veracode.yml --user root --workdir /app gcc-veracode:latest bash
`

```bash
make docker-build
```

### Building on Windows

On Windows, open the solution file in Visual Studio:

```powershell
# Open solution in Visual Studio
start arm64_sample.sln
```

Or build from command line using MSBuild:

```powershell
# Build Debug configuration
msbuild arm64_sample.sln /p:Configuration=Debug /p:Platform=ARM64

# Build Release configuration
msbuild arm64_sample.sln /p:Configuration=Release /p:Platform=ARM64
```

## Running

After building, run the executable:

```bash
./build/arm64_sample
```

## Cleaning

To clean all build artifacts:

```bash
make clean
```
