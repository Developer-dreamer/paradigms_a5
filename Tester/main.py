import subprocess


def run_echo(input_data):
    process = subprocess.Popen(
        ['/home/serafym/Documents/CLionProjects/paradigms-a5/cmake-build-debug/paradigms_a5'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )

    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def test_echo():
    tests = [
        {
            "input": "12345\n",
            "expected_output": "12345"
        },
        {
            "input": "1 + 2 * 4\n",
            "expected_output": "9"
        },
        {
            "input": "1 + pow (1,2) - min ( max ( -1,2), 4) * abs (-3)\n",
            "expected_output": "-4"
        }
    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]
        output, error = run_echo(input_data)
        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")


if __name__ == "__main__":
    test_echo()
