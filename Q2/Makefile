all: build

build:
	gcc workload.c policy.c checker.c -o check
	gcc workload.c policy.c main.c -o plt

checker:
	@echo "Checking the policy"
	gcc workload.c policy.c checker.c -o check
	./check

plot:
	@echo "Plotting..."
	gcc workload.c policy.c main.c -o plt
	./plt
	python plot.py

clean:
	@echo "Cleaning..."
	rm plt
	rm check
rmcsv:
	rm output.csv