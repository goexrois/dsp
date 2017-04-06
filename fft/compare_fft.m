function compare_fft (bits) 
	system(cstrcat("/home/goexrois/Documentos/dsp/fft/fft ",bits));
	x = load("signal") ;
	numbits = str2num(bits) ; 	
	y = abs(fft(x,numbits)) ;
	my_y = load("out") ;
	x_axis = 0:1:numbits-1;
	f_axis = (-numbits/2:1:(numbits-1)/2);
	subplot(1,3,1) ;
	plot(x_axis,x) ;
	title("signal") ;
	subplot(1,3,2) ;
	plot(y) ;
    y;
	title("octave") ;
	subplot(1,3,3) ;
	plot(my_y) ;
	my_y;
	title("fft para retards"); 
endfunction
