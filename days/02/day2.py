#!/usr/bin/env python3

import sys

def main():
    if len(sys.argv) != 2:
        print("Expected <input file>")
        exit(0)

    safe_levels, unsafe_levels = extract_levels(sys.argv[1])
    print(f"part 1: {part_1(safe_levels)}")
    print(f"part 2: {part_2(safe_levels, unsafe_levels)}")

def cmp(l, r):
    if l == r:
        return 0
    elif l > r:
        return -1
    else:
        return 1

def validate(dir, l, r):
    if dir == 0:
        return False
    elif cmp(l, r) != dir:
        return False
    elif abs(l - r) > 3:
        return False

    return True

def first_error_idx(level):
    l = int(level[0])
    r = int(level[1])
    dir = cmp(l, r)
    if not validate(dir, l, r):
        return 1

    is_safe = True
    for i in range(1, len(level) - 1):
        l = int(level[i])
        r = int(level[i + 1])
        if not validate(dir, l, r):            
            return i

    if is_safe == True:
        return -1

def extract_levels(pathname):
    levels = []
    safe_levels = []
    unsafe_levels = []
    with open(pathname) as file:
        for line in file:
            levels.append(line.split("\n")[0].split(" "))

    for level in levels:
        err_idx = first_error_idx(level)
        if err_idx != -1:
            unsafe_levels.append([err_idx, level])
        else:
            safe_levels.append(level)

    return safe_levels, unsafe_levels

def part_1(safe_levels):
    return len(safe_levels)

def part_2(safe_levels, unsafe_levels):
    safe = len(safe_levels)
    for err_idx, level in unsafe_levels:
        for i in range(err_idx - 1,len(level)):
            copy = level[:i] + level[i + 1:]
            if first_error_idx(copy) == -1:
                print(f"orig: {level}\ncopy: {copy}\n")
                safe += 1
                break

    return safe

main()
