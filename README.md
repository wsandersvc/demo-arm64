# ARM64 C++ Sample Project
1.0.2
## Requirements

### Linux/macOS Build
- GCC 13 (gcc-13)
- Linux ARM64 target architecture
- Make build system

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
