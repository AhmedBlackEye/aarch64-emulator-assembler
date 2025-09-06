# ARMv8 AArch64 Emulator & Assembler

## About
A C implementation of a simple toolchain for a subset of the **ARMv8-A AArch64 instruction set**:

- **Assembler** – converts AArch64 assembly (`.s`) into binary (`.bin`).  
- **Emulator** – simulates execution of AArch64 binaries on a virtual CPU.  

Built as part of a systems programming course, with correctness verified against a provided test suite.
Along with a **Flask-based interactive testing server** for easier debugging and visualization.


## Project Structure
```
.
├── assemble/   # Assembler source
├── emulate/    # Emulator source
├── common/     # Shared utilities
├── testsuite/  # Test assembly, binaries, expected outputs
├── run.sh      # Build + run helper
├── clean.sh    # Cleanup
```

## Usage
### Assemble
```bash
./assemble input.s output.bin
```
### Emulate

```bash
./emulate input.bin [output.out]
```

* If `output.out` is omitted, results are printed to stdout.

**Example:**

```bash
./assemble add01.s add01.bin
./emulate add01.bin add01.out
```

---

## Testing

Run all tests:

```bash
./run.sh
```

The suite checks assembler + emulator output against expected results. It launches a flask web server that for an interactive testing

## Features

* Subset of **AArch64 instructions**:

  * Data processing (immediate & register)
  * Load/store
  * Branch (conditional & unconditional)
* Two-pass assembler with label resolution
* Emulator outputs:

  * Registers (X0–X30, PC)
  * Condition flags (NZCV)
  * Non-zero memory
* Interactive testing via Flask web interface
