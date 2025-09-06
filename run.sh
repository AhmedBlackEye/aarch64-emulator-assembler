#!/bin/bash
set -e

# Check for debug flag
DEBUG=false
if [[ "${1:-}" == "--debug" ]]; then
  DEBUG=true
fi

# Helper to run cb with optional log
run_cb() {
  local dir=$1
  local msg=$2
  shift 2
  (
    cd "$dir" || exit 1
    if $DEBUG; then
      echo "➡️  $msg"
      cb "$@"
      echo "✅ $msg succeeded."
    else
      if ! cb "$@" &>/dev/null; then
        echo "❌ $msg failed!"
        exit 1
      else
        echo "✅ $msg succeeded."
      fi
    fi
  )
}

# Build directories
run_cb ./emulate "Building Emulate"
run_cb ./assemble "Building Assemble"
run_cb ./common "Building Common"

# Copy binaries
cp -f ./emulate/emulate ./testsuite/solution/emulate
cp -f ./assemble/assemble ./testsuite/solution/assemble

# Clean only if NOT debug
if ! $DEBUG; then
  run_cb ./emulate "Cleaning Emulate" --clean
  run_cb ./assemble "Cleaning Assemble" --clean
  run_cb ./common "Cleaning Common" --clean
fi

# Run testsuite with full log
(
  cd ./testsuite || exit 1
  ./run -s
)
