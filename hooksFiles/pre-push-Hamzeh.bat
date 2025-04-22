#!/bin/sh

echo "Starting test runner..."

test_runner="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\TestWindow\>

tests_dll="C:\Users\Hamze\source\repos\summer24-sft221-zaa-1\root\SourceCode\sft-milestone\x64\Debug\UnitTest2.dll"

testout=$("$test_runner" "$tests_dll")


if echo "$testout" | grep -q "Failed:"; then
    echo "Unit tests are not passing! Push aborted!" >&2
    exit 1
fi

echo "All tests run successfully." >&2
exit 0
