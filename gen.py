from sys import argv
import random

with open("list.txt", "w") as file:
    file.write(argv[1] + '\n')
    for i in range(int(argv[1])):
        file.write(str(random.randint(-99999, 99999)) + '\n')