#  DMITRIY BERNSTEYN         #
#  MATTHEW WEINGARTEN        #
#  LOGAN OWENS				 #
#  CIS4914 Senior Project    #

MAKE = make
CFLAGS = -L./wavelib -lwavelib -L. -lm

imgexe: img_main.o img_file_opps.o img_rmse.o img_help.o img_manip.o img_rmse_annu.o img_cpx.o img_fft.o img_wht.o img_dwt.o wavelib.a
	gcc -o img.exe img_main.o img_file_opps.o img_rmse.o img_help.o img_manip.o img_rmse_annu.o img_cpx.o img_fft.o img_wht.o img_dwt.o $(CFLAGS)

img_main.o: img_main.c image.h
	cc -c img_main.c
img_file_opps.o: img_file_opps.c image.h
	cc -c img_file_opps.c
img_rmse.o: img_rmse.c image.h
	cc -c img_rmse.c
img_rmse_annu.o: img_rmse_annu.c image.h
	cc -c img_rmse_annu.c
img_help.o: img_help.c image.h
	cc -c img_help.c
img_manip.o: img_manip.c image.h
	cc -c img_manip.c
img_cpx.o: img_cpx.c img_cpx.h image.h
	cc -c img_cpx.c
img_fft.o: img_fft.c image.h
	cc -c img_fft.c
img_wht.o: img_wht.c image.h
	cc -c img_wht.c
imp_dwt.o: img_dwt.c image.h
	cc -c img_dwt.c
image.h:

wavelib.a: 
	cd wavelib && $(MAKE)

wall:
	g++ -Wall -o img.exe img_main.c img_file_opps.c img_rmse.c img_help.c img_manip.c 

clean:
	rm *.o
