function compare_fft (bits) 
	system(cstrcat("/home/goexrois/Documentos/dsp/fft/fft ",bits));
	x = load("signal") ;
	y = abs(fft(x)) ; 
	my_y = load("out") ;
	subplot(1,2,1) ;
	plot(y) ;
	title("octave") ;
	subplot(1,2,2) ;
	plot(my_y) ;
	title("fft para retards"); 
endfunction
