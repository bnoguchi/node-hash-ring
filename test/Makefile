test:
	cd .. && node-waf configure build
	node test.js > test.js.txt
	python test.py > test.py.txt
	diffstat test.js.txt test.py.txt
