/*

#ifndef _included_adpcm_
	#define _included_adpcm_
 
	//
	//
	//
 
	namespace adpcm
	{
		//
		//
		//
 
		struct packet
		{
			enum
			{
				flag_end=1,
				flag_loop=2,
				flag_loop_start=4
			};
 
			unsigned char info,
										flags,
										data[14];
		};
 
		//
		//
		//
 
		class decoder
		{
			int l0,l1;
 
		public:
			decoder()
			{
				reset();
			}
 
			decoder(const decoder &other)
			{
				operator =(other);
			}
 
			decoder &operator =(const decoder &other)
			{
				l0=other.l0;
				l1=other.l1;
				return *this;
			}
 
			void reset()
			{
				l0=l1=0;
			}
 
			signed short *decode_packet(packet *ap, signed short *dp);
		};
 
		//
		//
		//
 
		extern const int filter_coef[5][2];
	};
 
#endif

  */