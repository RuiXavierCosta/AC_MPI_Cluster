CC_MPI = mpicc -I/opt/local/include
CFLAGS = -lm -L/opt/local/lib -lnetpbm
BUILD_DIR = build
IMAGES_DIR = build/images
DEPLOY_DIR = build/tmp
DOWNLOAD_DIR = build/download
USER = ruicosta
USER_DIR = /home/2013/ruicosta/aula6
SERVER = 193.136.230.223

.PHONY: main_MPI
main_MPI: main.c pbm/funcs.c pbm/dft.c pbm/mpi.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	$(CC_MPI) pbm/funcs.c pbm/dft.c pbm/mpi.c main.c $(CFLAGS) -o $(BUILD_DIR)/filter_image

deploy:
	test -d "$(DEPLOY_DIR)" || mkdir $(DEPLOY_DIR)
	cp -r pbm $(DEPLOY_DIR)
	cp main.c $(DEPLOY_DIR)
	cp Makefile $(DEPLOY_DIR)
	cp cluster_right_1 $(DEPLOY_DIR)
	cp cluster_right_2 $(DEPLOY_DIR)
	cp cluster_right_xavier $(DEPLOY_DIR)
	scp -r $(DEPLOY_DIR)/* $(USER)@$(SERVER):$(USER_DIR)/
	rm -r $(DEPLOY_DIR)

download_image:
	test -d "$(DOWNLOAD_DIR)" || mkdir $(DOWNLOAD_DIR)
	scp -r $(USER)@$(SERVER):$(USER_DIR)/build/images/* $(DOWNLOAD_DIR)

setup:
	make -C ./pbm/
	make all

clean:
	rm -r build