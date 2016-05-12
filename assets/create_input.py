from random import randint, uniform, choice, shuffle, random

def create_int_list(limit, elements, negative = False):

	li = []

	lower_bound = 0
	if negative == True:
		lower_bound = -limit

	for a in range(elements):
		li.append(randint(lower_bound, limit))

	return li

def create_float_list(limit, elements, negative = False):

	li = []

	lower_bound = 0
	if negative == True:
		lower_bound = -limit

	for a in range(elements):
		li.append(uniform(lower_bound, limit))

	return li


def create_string_list(elements, words_file):
	fl = open(words_file)

	retval = []

	with fl:
		li = fl.readlines()
		li = [s.strip() for s in li]
		for a in range(elements):
			retval.append(choice(li))

	return retval

def write_to_file(li, file):

	fl = open(file, "w")

	shuffle(li)

	print(len(li))

	with fl:
		for a in li:
			fl.write(str(a) + "\n")
		fl.close()

# l1 = create_int_list(10, 100, False)

# write_to_file(l1, "input1.txt")

l2 = create_float_list(50, 2000, True)

write_to_file(l2, "input2.txt")

# l3 = create_string_list(20, "words.txt")

# write_to_file(l3, "input3.txt")

# l4 = create_int_list(100, 3000, False)
# l4.extend(create_float_list(50, 1000, False))

# write_to_file(l4, "input4.txt")

# l5 = create_int_list(100, 1000, True)
# l5.extend(create_float_list(250, 2000, True))
# l5.extend(create_string_list(3000, "words.txt"))

# write_to_file(l5, "input5.txt")
