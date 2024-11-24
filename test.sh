#!/bin/bash
assert() {
    expected="$1"
    input="$2"

    ./aucc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo -e "$input => $actual \e[92mOK\e[0m"
    else
        echo -e "\e[91m\e[1m! $input => $expected expected, but got $actual\e[93m\n"
        exit 1
    fi
    
}

echo -e "\e[93m --- Running test --- \e[0m"
assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "
assert 47 '5+6*7'
assert 15 '5*(9-6)'
assert 4 '(3+5)/2'
assert 1 '-4+1*5'
assert 1 '11+2 == 13'
assert 1 '11+2 >= 12'
assert 0 '11+2 <= 12'
assert 0 '11+2 < 12'
assert 1 '11+2 > 12'
assert 1 '11+2 >= 13'
assert 2 '1 + (11+2 >= 10)'


echo -e "\e[92m\e[1m- All tests OK!\e[0m\n"