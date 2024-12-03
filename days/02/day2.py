#!/usr/bin/env python3

import sys

def main():
    if len(sys.argv) != 2:
        print("Expected <input file>")
        exit(0)

    safe_levels, unsafe_levels = extract_levels(sys.argv[1])
    print(f"part 1: {part_1(safe_levels)}")
    print(f"part 2: {part_2(safe_levels, unsafe_levels)}")

def cmp(op_pair):
    return ((op_pair[0] > op_pair[1]) - (op_pair[0] < op_pair[1]))

def validate(dir, pair):
    return ((dir == 0) or (cmp(pair) != dir) or (abs(pair[0] - pair[1]) > 3))

def first_error_idx(level):
    dir = cmp((int(level[0]), int(level[1])))
    for i in range(len(level) - 1):
        if validate(dir, (int(level[i]), int(level[i + 1]))):
            return i

    return -1

def extract_levels(pathname):
    levels, safe, unsafe = [], [], []
    with open(pathname) as file:
        levels = [line.strip().split() for line in file]

    for level in levels:
        err_idx = first_error_idx(level)
        safe.append(level) if err_idx == -1 else unsafe.append([err_idx, level])

    return safe, unsafe

def part_1(safe_levels):
    return len(safe_levels)

def part_2(safe_levels, unsafe_levels):
    safe = len(safe_levels)
    for err_idx, level in unsafe_levels:
        for i in range(err_idx - 1, err_idx + 2):
            print(i - err_idx)
            if first_error_idx(level[:i] + level[i + 1:]) == -1:
                safe += 1
                break

    return safe

main()
