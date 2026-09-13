#!/bin/sh
set -eu

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <slides.md> <output-directory>" >&2
    exit 2
fi

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
input=$1
output_dir=$2

case "$input" in
    *.md) ;;
    *)
        echo "Slide source must end in .md: $input" >&2
        exit 2
        ;;
esac

if [ ! -f "$script_dir/$input" ]; then
    echo "Slide source not found: $script_dir/$input" >&2
    exit 2
fi

cd "$script_dir"
output="${input##*/}"
output="${output%.md}.pdf"
mkdir -p "$output_dir"

marp "$input" \
    --pdf \
    --allow-local-files \
    --theme-set style/sioux/style.css \
    --output "$output_dir/$output"