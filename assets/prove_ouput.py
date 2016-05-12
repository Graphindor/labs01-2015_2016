fl = open("output.txt", "r")

with fl:
	lt = fl.readlines()
	li = [int(lt[n]) - 1 for n in range(len(lt))]

	fl2 = open("input5.txt", "r")
	with fl2:
		li2 = fl2.readlines()
		for a in li:
			print("[{}] => {}".format(a, li2[a]))

		fl2.close()

	fl.close()