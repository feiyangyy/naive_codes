import numpy as np

# 设置要生成的数字数量
num_count = 65536 * 10  # 例如生成 100 万个数字
min_value = 1        # 数字的最小值
max_value = 1000000    # 数字的最大值

# 生成随机数字
numbers = np.random.randint(min_value, max_value + 1, size=num_count + 1)
numbers[0] = num_count
# 将数字保存到文件
np.savetxt('large_numbers.txt', numbers, fmt='%d', delimiter=' ')

print("File 'large_numbers.txt' has been created with", num_count, "numbers.")