#!python3

from itertools import combinations_with_replacement
from random import shuffle
from os import system

falshivka = "./s21_grep"
original = "grep"

file_name = 'TEST-FILE'
file_format = ".testing"

template = '[0-9]'
suite = 'TEST-FILE_001'
test_template = './TEST-SUITE/TEST-TEMPLATE'

suite_flag = [
    f'-l {template} {suite}',
    f'-v {template} {suite}',
    f'-c {template} {suite}',
    f'-n {template} {suite}',
    f'-o {template} {suite}',
    f'-e {template} -e {test_template} {suite}',
    f'-f {test_template} s21_grep.c s21_grep.h',
    f'-vf {test_template} {suite}',
    f'-cf {test_template} {suite}',
    f'-lf {test_template} {suite}',
    f'-nf {test_template} {suite}',
    f'-hf {test_template} {suite}',
    f'-h if {suite} s21_grep.h',
    f'-ie {template} {suite}',
    f'-ve {template} {suite}',
    f'-ce {template} {suite}',
    f'-le {template} {suite}',
    f'-ne {template} {suite}',
    f'-he {template} {suite}',
    f'-se {template} {suite}',
    f'-oe {template} {suite}',
    f'-iv {template} {suite}',
    f'-ic {template} {suite}',
    f'-il {template} {suite}',
    f'-in {template} {suite}',
    f'-ih {template} {suite}',
    f'-is {template} {suite}',
    f'-io {template} {suite}',
    f'-vc {template} {suite}',
    f'-vl {template} {suite}',
    f'-vn {template} {suite}',
    f'-vh {template} {suite}',
    f'-vs {template} {suite}',
    f'-vo {template} {suite}',
    f'-cl {template} {suite}',
    f'-cn {template} {suite}',
    f'-ch {template} {suite}',
    f'-cs {template} {suite}',
    f'-co {template} {suite}',
    f'-ln {template} {suite}',
    f'-lh {template} {suite}',
    f'-ls {template} {suite}',
    f'-lo {template} {suite}',
    f'-nh {template} {suite}',
    f'-ns {template} {suite}',
    f'-no {template} {suite}',
    f'-hs {template} {suite}',
    f'-ho {template} {suite}',
    f'[0-9] {suite}',
    f'-i RY {suite}'
]

# all_var = list(combinations_with_replacement(suite_flag, len(suite_flag)))

def comp(a, b):
    flag = 0
    with open(a) as file_1:
        with open(b) as file_2:
            a, b = file_1.read(), file_2.read()
            if a != b:
                print("\033[41m\033[1m FAIL! \033[0m")

            else:
                flag = 1
                print("\033[42m\033[1m YES \033[0m")
    return flag

all_tests = 0
succeses = 0
for index in range(len(suite_flag)):
    all_tests += 1
    print(f'Current TEST [{index + 1} / {len(suite_flag)}] - {suite_flag[index]} - ', end='')
    system(f'{falshivka} {suite_flag[index]} > {file_name + "-0" + file_format}')
    system(f'{original} {suite_flag[index]} > {file_name + "-1" + file_format}')
    succeses += comp(file_name+'-0'+file_format, file_name+'-1'+file_format)

print("succes: ", succeses, "/", all_tests)

system('rm -rf *' + file_format)