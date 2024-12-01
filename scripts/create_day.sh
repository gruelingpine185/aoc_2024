#!/usr/bin/env zsh

create_day() {
  local day="$1"
  local part="$2"

  printf -v padded_day "%02d" "$day"
  if [[ ! -e "./days/${padded_day}" ]]; then
    mkdir -p "days/${padded_day}"
  fi

  if [[ -e "/days/${padded_day}/input_2.txt" ]]; then
    echo "Day ${day} files already downloaded and present at ./days/${padded_day}/"
    exit 0
  elif [[ -e "/days/${padded_day}/input_1.txt" ]]; then
    part="2"
  else
    part="1"
  fi

  if [[ -z "$AOC_SESSION" ]]; then
    echo "Please export your AoC session"
    echo "  export AOC_SESSION=<session>"
    exit 0
  fi

  local year=$(date "+%Y")
  local aoc_url="https://adventofcode.com/${year}/day/${day}"
  local aoc_session=$(printenv AOC_SESSION)

  # download the prompt of the given day, extract text within <p> tags,
  # format the output to 80 columns
  curl "$aoc_url" | \
    hxselect -c -s "\n\n" p, pre | \
    fold -s > "days/${padded_day}/prompt.md"

  # download puzzle input
  curl --cookie session=$(printenv AOC_SESSION) "${aoc_url}/input" \
    > "days/${padded_day}/input_${part}.txt"
}

if [[ $# != 2 ]]; then
  echo "create_day.sh <day> <part>"
  exit 0
fi

create_day "$1" "$2"
exit 0
