
/*
#include "common.h"
#include "adpcm.h"
 
//	<pSXAuthor>	you use the decoder class like:
//	<pSXAuthor>	adpcm::decoder d;
//	<pSXAuthor>	signed short buffer[...number of pcm bytes....];
//	<pSXAuthor>	packet *p=... pointer to start of adpcm data ...;
//  <pSXAuthor>	signed short *ptr=buffer; while ((p->flags&flag_end)==0) ptr=d.decode_packet(p++,ptr);


//
//
//
 
const int adpcm::filter_coef[5][2]=
{
	{ 0,0 },
	{ 60,0 },
	{ 115,-52 },
	{ 98,-55 },
	{ 122,-60 },
};
 
//
//
//
 
signed short *adpcm::decoder::decode_packet(adpcm::packet *ap, signed short *dp)
{
	int shift=ap->info&0xf,
			filter=ap->info>>4,
			f0=filter_coef[filter][0],
			f1=filter_coef[filter][1];
 
	for (int i=0; i<14; i++)
	{
		unsigned char b=ap->data[i];
		short bl=(b&0xf)<<12,
			 		bh=(b>>4)<<12;
 
		bl=(bl>>shift)+(((l0*f0)+(l1*f1)+32)>>6);
		if (bl<-32768) bl=-32768; else if (bl>32767) bl=32767;
		*dp++=bl;
		l1=l0;
		l0=bl;
 
		bh=(bh>>shift)+(((l0*f0)+(l1*f1)+32)>>6);
		if (bh<-32768) bh=-32768; else if (bh>32767) bh=32767;
		*dp++=bh;
		l1=l0;
		l0=bh;
	}
 
	return dp;
} */