#! /bin/bash

set -euo pipefail

BASE_PATH="$(dirname "$0")/../.."
cd "$BASE_PATH"

INPUT="$1"
shift 1

OUTPUT_DIR="${OUTPUT_DIR:-results}" # Default to 'results' if not set
export OUTPUT_DIR

cat "$INPUT" | build/Compiler "$@"