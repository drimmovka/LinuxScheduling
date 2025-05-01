with open('output.txt') as f:
    nums = f.read().strip().split('|')

result = []
current = nums[0]
count = 1

for n in nums[1:]:
    if n == current:
        count += 1
    else:
        if count > 1:
            result.append(f"{current}...{current} {count}")
        current = n
        count = 1

if count > 1:
    result.append(f"{current}...{current} {count}")

print('\n'.join(result))