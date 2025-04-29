import re

def replace_repeated_digits(s):
    res = []
    for match in re.finditer(r'(\d)\1+', s):
        d = match.group(1)
        cnt = match.end() - match.start()
        res.append(f"{d}...{d} ({cnt})")
    return "\n".join(res)

input = input()
print(replace_repeated_digits(input))