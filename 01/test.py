import subprocess

def check(expr):
	process = subprocess.run(["./calc", expr], stdout=subprocess.PIPE, universal_newlines=True)
	err = process.returncode
	if err == 0:
		return float(process.stdout)
	else:
		return None

assert(check("2 + 3 * 4 - -2") == 16)
assert(check("3 * 4 / 2 + 1") == 7)
assert(check("1 + 2") == 3)
assert(check("1 / 0") == float("Inf"))
assert(check("-1 - 2 / 2") == -2)
assert(check("-1 * -2 * -3 + 10") == 4)
assert(check("8 * 8 - 120 / 2 - 3") == 1)
assert(check("1 + 2 + 3 + 4 + 5 + 6") == 21)
assert(check("21 - 6 - 5 - 4 - 3 - 2 - 1") == 0)
assert(check("2 +") == None)
