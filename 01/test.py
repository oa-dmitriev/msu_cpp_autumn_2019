import subprocess

def check(expr):
	process = subprocess.run(["./calc", expr], check=True, stdout=subprocess.PIPE, universal_newlines=True)
	output = process.stdout
	try:
		n = float(output)
		return n
	except ValueError:
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
