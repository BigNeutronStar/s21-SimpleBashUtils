#!python3

from itertools import combinations_with_replacement
from random import shuffle
from os import system

falshivka = "./s21_cat"
original = "cat"

file_name = 'TEST-FILE'
file_format = ".testing"

suite = [
    'TEST-FILE_001',
    'TEST-FILE_002',
    'TEST-FILE_003',
    'TEST-FILE_004'
]

flags = {
    '-b',
    '-e',
    '-n',
    '-s',
    '-v',
    '-t'
}

tests = list(combinations_with_replacement(flags, len(flags)))

def comp(a, b):
    flag = 0
    with open(a, encoding='latin-1') as file_1:
        with open(b, encoding='latin-1') as file_2:
            a, b = file_1.read(), file_2.read()
            if a != b:
                print("\033[41m\033[1m FAIL! \033[0m")

            else:
                flag = 1
                print("\033[42m\033[1m YES \033[0m")
    return flag

succeses = 0
all_tests = 0
for test in range(len(tests)):
    cur_flags_ = tests[test]
    for cur_flags in (cur_flags_, set(cur_flags_)):
        all_tests += 1
        shuffle(suite)
        print(f"Current TEST [{test + 1} / {len(tests)}] - ", end='')
        for i, func in (('0', falshivka), ('1', original)):
            m_str = f'{func} {" ".join(cur_flags)} {" ".join(suite)} > {file_name+"-"+i+file_format}'
            system("echo '{}' >> commands.testing".format(m_str))
            system(m_str)
        succeses += comp(file_name+'-0'+file_format, file_name+'-1'+file_format)
print("succes: ", succeses, "/", all_tests)
system('rm -rf *' + file_format)