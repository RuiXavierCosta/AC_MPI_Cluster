CC_MPI=mpicc -I/opt/local/include
CFLAGS=-lm -L/opt/local/lib -lnetpbm
BUILD_DIR = build
IMAGES_DIR = build/images

.PHONY: main_MPI
main_MPI: main.c pbm/funcs.c dft/dft.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	$(CC_MPI) pbm/funcs.c dft/dft.c main.c $(CFLAGS) -o $(BUILD_DIR)/filter_image

setup:
	make -C ./pbm/
	make all

clean:
	rm -r build