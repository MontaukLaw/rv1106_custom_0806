all: app

date=`date +%m%d`

app:  ai
	cd ../demo && make clean && make
	
ai:
	cd ../ai && make
clean:
	cd ../demo && make clean
	cd ../ai && make clean
