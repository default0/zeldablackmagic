    #include <direct.h>
    #include "Music.h"

    #define GetTrackByte(a,b,c)       c = GetByte(a,b); b++;

    u32 song = 0;
    u32 pattern = 0;
    u32 track_byte = 0;

    u32 track[8]        =   {0, 0, 0, 0, 0, 0, 0, 0};
    u32 call_return[8]  =   {0, 0, 0, 0, 0, 0, 0, 0};
    u32 call_iter[8]    =   {0, 0, 0, 0, 0, 0, 0, 0};
    u32 call_addr[8]    =   {0, 0, 0, 0, 0, 0, 0, 0};
    u32 sustain[8]      =   {0, 0, 0, 0, 0, 0, 0, 0};
    u32 note_length[8]  =   {0, 0, 0, 0, 0, 0, 0, 0};

    u32 j = 0;

    u32 a = 0, b = 0, c = 0;


    char output[8][0x2000];
    char temp[0x200];
    bufPtr gRam = NULL;

// **************************************************

    void ExportSamples(bufPtr ram)
    {

    



    }

// **************************************************

    void SetInstrument()
    {
        GetTrackByte(gRam, track[j], track_byte);
        sprintf(temp, "Inst %02X\n", track_byte);
        strcat(output[j], temp);
    }

    void SetPan()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Pan I=%d L=%d R=%d\n", a, a % 0x15, 0x14 - (a % 0x15));
        strcat(output[j], temp);
    }

    void SetPanSweep()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        sprintf(temp, "PanSweep T=%02X L=%d R=%d\n", a, b % 0x15, 0x14 - (b % 0x15));
        strcat(output[j], temp);
    }

    void SetVibrato()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        GetTrackByte(gRam, track[j], c);
        sprintf(temp, "Vibr. %02X %02X %02X\n", a, b, c);
        strcat(output[j], temp);
    }

    void VibratoOff()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Vibr. Off %02X\n", a);
        strcat(output[j], temp);
    }

    void SetGlobalVol()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Glob. Vol %02X\n", a);
        strcat(output[j], temp);
    }

    void SetGlobalVolRamp()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        sprintf(temp, "Glob. Vol Ramp T=%02X V=%02X", a, b);
        strcat(output[j], temp);
    }

    void SetTempo()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Tempo %02X\n", a);
        strcat(output[j], temp);
    }

    void SetTempoRamp()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        sprintf(temp, "Tempo Ramp T=$02X V=%02X\n", a, b);
        strcat(output[j], temp);
    }

    void SetGlobalPitch()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Global Pitch %02X\n", a);
        strcat(output[j], temp);
    }

    void SetChannelPitch()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Channel Pitch %02X\n", a);
        strcat(output[j], temp);
    }

    void SetTremelo()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        GetTrackByte(gRam, track[j], c);
        sprintf(temp, "Tremelo S=%02X R=%02X I=%02X\n", a, b, c);
        strcat(output[j], temp);
    }

    void SetTremeloLevel()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Tremelo I=%02X\n", a);
        strcat(output[j], temp);
    }

    void SetChannelVolume()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Channel Volume I=%02X\n", a);
        strcat(output[j], temp);
    }

    void SetEcho()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "Fast Instrument O=%02X\n", a);
        strcat(output[j], temp);
    }

    void SetPitchSlide()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        GetTrackByte(gRam, track[j], c);
        sprintf(temp, "Pitch Slide  T=%02X P=%02X\n", a, b, c);
        strcat(output[j], temp);
    }

    void SetVolumeRamp()
    {
       GetTrackByte(gRam, track[j], a);
       GetTrackByte(gRam, track[j], b);
       sprintf(temp, "FineTune T=%02X V=%02X\n", a, b);
       strcat(output[j], temp);
    }


    void CallLoop()
    {
        GetTrackByte(gRam, track[j], track_byte);
        call_addr[j] = track_byte;

        GetTrackByte(gRam, track[j], track_byte);
        call_addr[j] |= (track_byte << 8);

        GetTrackByte(gRam, track[j], track_byte);
        call_iter[j] = track_byte;

        call_return[j] = track[j];
        track[j] = call_addr[j];

        // add text output soon
    }

    void SetVibratoStep()
    {
        GetTrackByte(gRam, track[j], a);
        sprintf(temp, "VibratoStep V=%02X\n", a);
        strcat(output[j], temp);
    }

    void SetPSlideTo()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        GetTrackByte(gRam, track[j], b);
        sprintf(temp, "PSlideTo D=%02X T=%02X S=%02X\n", a, b, c);
        strcat(output[j], temp);
    }

    void SetPSlideFr()
    {
        GetTrackByte(gRam, track[j], a);
        GetTrackByte(gRam, track[j], b);
        GetTrackByte(gRam, track[j], c);
        sprintf(temp, "PSlideFr D=%02X T=%02X S=%02X\n", a, b, c);
        strcat(output[j], temp);
    }

    void SetPSlideTicks()
    {
       GetTrackByte(gRam, track[j], a);
       sprintf(temp, "PSlideTicks T=%02X\n", a);
       strcat(output[j], temp);
    }

    void SetFineTune()
    {
       GetTrackByte(gRam, track[j], a);
       sprintf(temp, "FineTune P=%02X\n", a);
       strcat(output[j], temp);
    }

    void SetEchoControl()
    {
       GetTrackByte(gRam, track[j], a);
       GetTrackByte(gRam, track[j], b);
       GetTrackByte(gRam, track[j], c);
       sprintf(temp, "EchoControl D=%02X L=%02X R=%02X\n", a, b, c);
       strcat(output[j], temp);
    }

    void SetEchoSilence()
    {
       strcat(output[j], "MuteEcho");
    }

    void SetEchoFilter()
    {
       GetTrackByte(gRam, track[j], a);
       GetTrackByte(gRam, track[j], b);
       GetTrackByte(gRam, track[j], c);
       sprintf(temp, "EchoFilter D=%02X V=%02X F=%02X\n", a, b, c);
       strcat(output[j], temp);
    }

    void SetEchoVolume()
    {
       GetTrackByte(gRam, track[j], a);
       GetTrackByte(gRam, track[j], b);
       GetTrackByte(gRam, track[j], c);
       sprintf(temp, "EchoVolume T=%02X L=%02X R=%02X\n", a, b, c);
       strcat(output[j], temp);
    }

    void SetPSlide()
    {
       GetTrackByte(gRam, track[j], a);
       GetTrackByte(gRam, track[j], b);
       GetTrackByte(gRam, track[j], c);
       sprintf(temp, "PSlide D=%02X T=%02X S=%02X\n", a, b, c);
       strcat(output[j], temp);
    }

    void SetInstrumentOffset()
    {
       GetTrackByte(gRam, track[j], a);
       sprintf(temp, "InstrumentOffset I=%02X\n", a);
       strcat(output[j], temp);
    }

// **************************************************

    void NspcCommand(bufPtr ram)
    {
        gRam = ram;

        switch(track_byte)
        {
        case 0xE0:
            SetInstrument();
            break;

        case 0xE1:
            SetPan();
            break;

        case 0xE2:
            SetPanSweep();
            break;

        case 0xE3:
            SetVibrato();
            break;

        case 0xE4:
            VibratoOff();
            break;

        case 0xE5:
            SetGlobalVol();
            break;

        case 0xE6:
            SetGlobalVolRamp();
            break;

        case 0xE7:
            SetTempo();
            break;

        case 0xE8:
            SetTempoRamp();
            break;

        case 0xE9:
            SetGlobalPitch();
            break;

        case 0xEA:
            SetChannelPitch();
            break;

        case 0xEB:
            SetTremelo();
            break;

        case 0xEC:
            SetTremeloLevel();
            break;

        case 0xED:
            SetChannelVolume();
            break;

        case 0xEE:
            SetVolumeRamp();
            break;

        case 0xEF:
            CallLoop();
            break;

        case 0xF0:
            SetVibratoStep();
            break;

        case 0xF1:
            SetPSlideTo();
            break;

        case 0xF2:
            SetPSlideFr();
            break;

        case 0xF3:
            SetPSlideTicks();
            break;

        case 0xF4:
            SetFineTune();
            break;

        case 0xF5:
            SetEchoControl();
            break;

        case 0xF6:
            SetEchoSilence();
            break;

        case 0xF7:
            SetEchoFilter();
            break;

        case 0xF8:
            SetEchoVolume();
            break;

        case 0xF9:
            SetPitchSlide();
            break;

        case 0xFA:
            SetInstrumentOffset();
            break;

        default:
            strcpy(temp, "");
            break;
        }
    }

// ***************************************

    void Songs(bufPtr ram)
    {
        int i = 0;
        int pat = 0;
        bool done = false;
    
        FILE *f;

        for(i = 0;  ; i += 2)
        {
            // get the current song pointer
            song = Get2Bytes(ram, 0xD000 + i);

            if(song <= 0xFF)
                break;

            sprintf(temp, "F:\\Song %d.log", i/2);
            f = fopen(temp, "wt");

            pat = 0;

            for( ; ; )
            {  
                //3 tabs in
                sprintf(temp, "Pattern %d\n\n", pat);
                fwrite(temp, strlen(temp), 1, f);
                pattern = Get2Bytes(ram, song);

                if(!pattern)
                {
                    break;
                }
                else if( (pattern == 0x81) || (pattern == 0x80) )
                {
                    strcpy(temp, "Dave Waggoner\n\n");
                    fwrite(temp, strlen(temp), 1, f);
                    song += 2; continue;
                }
                else if( pattern <= 0xFF )
                {
                    // this is a looping mechanism, makes you go to a
                    // previous pattern
                    song += 2; song = Get2Bytes(ram, song);
                    sprintf(temp, "Loop to %04X\n\n", song);
                    fwrite(temp, strlen(temp), 1, f);

                    break;
                }
            
                // load up to 8 track pointers
                for(j = 0; j < 0x08; j++)
                {
                    track[j] = Get2Bytes(ram, pattern + (j << 1) );
                    sprintf(output[j], "Track %d\n\n", j);
                }

                for(j = 0, done = false; done == false; j++ )
                {
                    if(strlen(output[j]) > 0x1F80)
                        break;

                    j = (j > 7) ? 0 : j;

                    if((track[j] & 0xFF00) == 0)
                        continue;

                    if(sustain[j])
                    {
                        sustain[j]--;

                        if(sustain[j])
                            continue;
                    }
                
                    while(1)
                    {
                        GetTrackByte(ram, track[j], track_byte);

                        if(track_byte & 0x80)
                        {
                        note_or_command:

                            if(track_byte >= 0xE0)
                            {
                                NspcCommand(ram);
                            }
                            else if(track_byte >= 0xCA)
                            {
                                // set the instrument a bassackwards way and play 
                                //C4 (middle C)

                                sprintf(temp, "Fast Inst I=%02X P=A4 T=$02X\n", track_byte - 0xCA, note_length[j]);
                                strcat(output[j], temp);
                                sustain[j] = note_length[j];
                                break;
                            }
                            else if(track_byte == 0xC9)
                            {
                                sprintf(temp, "Extend %02X\n", note_length[j]);
                                strcat(output[j], temp);
                                sustain[j] = note_length[j];                             
                                break; // rest
                            }
                            else if(track_byte == 0xC8)
                            {
                                sprintf(temp, "Rest %02X\n", note_length[j]);
                                strcat(output[j], temp);
                                sustain[j] = note_length[j];                             
                                break; // rest
                            }
                            else if(track_byte)
                            {
                                sprintf(temp, "Note %02X\n", track_byte);
                                sustain[j] = note_length[j];
                                strcat(output[j], temp);
                                break;
                            }                            
                        }
                        else
                        {
                            if(!track_byte)
                            {
                                // faced with a null track_byte and a null call loop iterator
                                // we load the next pattern
                                if(!call_iter[j])
                                {
                                    done = true; break;
                                }
                            
                                // decrement the call iteration counter
                                call_iter[j]--;

                                // if zero, go to the return address
                                if(!call_iter[j])
                                {
                                    track[j] = call_return[j];
                                }
                                // otherwise, do another call in the call loop
                                else
                                {
                                    track[j] = call_addr[j];
                                }
                            }

                            note_length[j] = track_byte;
                            // set new sustain time (in ticks)

                            sprintf(temp, "Length %02X\n", track_byte);
                            strcat(output[j], temp);

                            GetTrackByte(ram, track[j], track_byte);

                            if(track_byte & 0x80)
                                goto note_or_command;

                            sprintf(temp, "NoteVolume %02X\n", track_byte >> 4);
                            strcat(output[j], temp);

                            sprintf(temp, "Staccato %02X\n", track_byte & 0x07);
                            strcat(output[j], temp);

                            // upper 4 bits are note level volume modulation
                            // lower 4 bits are note level sustain modulation
                            // however, only the bottom 3 bits of that are used.

                            GetTrackByte(ram, track[j], track_byte);

                            // the third byte is forced into being a note or command
                            goto note_or_command;                        
                        }
                    }       
                }

                for(j = 0; j < 8; j++)
                {
                    if(fwrite(output[j], strlen(output[j]), 1, f))
                        j = j;
                    output[j][0] = 0;
                }

                song += 2;
            }

            fclose(f);
        }
    }

// **************************************************

    void WriteSPCHeader(bufPtr header)
    {
        const char* headText = "SNES-SPC700 Black Magic version";

        if(strlen(headText) < 0x20)
            memcpy(header->contents, headText, strlen(headText));
    
        Put4Bytes(header, 0x21, 0x001B1A1A); // indicates no ID666
        Put2Bytes(header, headPC, 0x0800);
        PutByte(header, headA,      0x00);
        PutByte(header, headX,      0x00);
        PutByte(header, headY,      0x00); 
        PutByte(header, headPSW,    0x00); 
        PutByte(header, headSP,     0x00); 
    
        Put2Bytes(header, headReserve,0x0000); 

        // ID666 information would go here normally if we were handling it
    }

// **************************************************

    void ExportSpc(zgPtr game)
    {
        char *oldPath = NULL;
        char sampName[MAX_PATH];

        u32 length = 0;
        u32 address = 0;
        u32 offset = 0;
        u32 loopStart = 0;
        u32 i = 0;
        u32 numSamp = 0; // number of samples found so far

        FILE *f = fopen("C:\\test_bm.spc", "wb");
    
        if(!f)
            return;

        bufPtr header = CreateBuffer(0x100);
        bufPtr ram    = CreateBuffer(0x10000);
        bufPtr dsp = CreateBuffer(0x100);
        bufPtr rom = game->image;

        WriteSPCHeader(header);

        // write the data to the RAM portion now

        offset = 0xC8000;

        while(1)
        {   
            length = Get2Bytes(rom, offset); offset += 2;
            address = Get2Bytes(rom, offset); offset += 2;

            if(length == 0x0000)
            {
                Put2Bytes(header, headPC, address);
                break; // zero length block means we're done loading data.
            }            

            CopyBuffer(ram,     rom,
                       address, offset, length);

            offset += length;
        }
    
        // finally, handle the DSP registers

        fwrite(header->contents, header->length, 1, f);
        fwrite(ram->contents, ram->length, 1, f);
        fwrite(dsp->contents, dsp->length, 1, f);
  
        fclose(f);

        i = mkdir("sound");
        oldPath = getcwd(NULL, MAX_PATH);
        i = chdir("sound");

        for(i = 0x3C00;  ; i += 4, numSamp++)
        {
            address = Get2Bytes(ram, i);

            // loopstart is the distance from the start of the sample to the point where it loops
            loopStart = Get2Bytes(ram, i + 2);
            loopStart = loopStart - address; 

            if(address == 0xFFFF)
                break;

            // acquire the length of the sample in bytes
            for(j = address; j < 0xFFFF; j += 9)
            {
                if(GetByte(ram, j) & end_flag)
                {
                    j += 9;
                    break;
                }
            }

            sprintf(sampName, "Sample%02X.brr", numSamp);

            f = fopen(sampName, "wb");

            fwrite(ram->contents + address, j - address, 1, f);
            fwrite(&loopStart, 2, 1, f);

            fclose(f);
        }

        Songs(ram);

        free(oldPath);

        oldPath = getcwd(NULL, MAX_PATH);

        free(oldPath);

        chdir("..");

        oldPath = getcwd(NULL, MAX_PATH);

        free(oldPath);    

        DeallocBuffer(header);
        DeallocBuffer(ram);
        DeallocBuffer(dsp);

        return;
    }

// **************************************************

    void ImportSpc(zgPtr game)
    {
    








    }

// **************************************************
