
//#include <regex.h>

    #include <iostream>
    #include <fstream>
    #include <string>

    #include "..\\re_lib\\regexp.h"
    #include "..\\re_lib\\regmagic.h"
    #include "Globals.h"
    #include "Core.h"

    using namespace std;

    #define NOMATCH     0
    #define HUNGRY      1
    #define STUFFED     2

    // these are to used to convert the internal game characters to
    // something a bit more useful
    char* convChars[] =
    {
        // codes 0x00 and up
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
        "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",

        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",

        // codes 0x3E and up
        "!", "?", "-", ".", ",", "[...]", ">", "(", ")",
        "[ahnk]", "[waves]", "[snake]", "[LinkL]", "[LinkR]",
        "\"", "[Up]", "[Down]", "[Left]", "[Right]", "'",

    
        // codes 52 and up
	    "[1HeartL]", "[1HeartR]", "[2HeartL]", "[3HeartL]",
        "[3HeartR]", "[4HeartL]", "[4HeartR]",

        " ", "<", "[A]", "[B]", "[X]", "[Y]",

        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0x67 and up
        "[NextPic]", "[Choose]", "[Item]", "[Name]", "[Window ",
        "[Number ", "[Position ", "[ScrollSpd ", "[SelChng]",
    
        // codes 0x70 and up
        "[Command 70]", "[Choose2]", "[Choose3]", "[Scroll]", 
        "[Line1]", "[Line2]", "[Line3]", "[Color ",
        "[Wait ", "[Sound ", "[Speed ",
        "[Command 7B]", "[Command 7C]", "[Command 7D]", "[WaitKey]", "[End]",

        // codes 0x80 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0x90 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0xA0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0xB0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0xC0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0xD0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",

        // codes 0xE0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
 
        // codes 0xF0 and up
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0",
        "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"

    };

    // this string is used in conjunction with strstr
    char* cmdStrings[13] =
    {
        "nextpic",
        "choose",
        "item",
        "name",
        "selchng",
        "auto",
        "choose2",
        "choose3",
        "scroll",
        "line1",
        "line2",
        "line3",
        "waitkey"
    };

    char cmdCodes[13] = 
    {
        0x67,
        0x68,
        0x69,
        0x6A,
        0x6F, 
        0x70,
        0x71,
        0x72,
        0x73,
        0x74,
        0x75,
        0x76,
        0x7E
    };

    char* argCmdStrings[9] =
    {
        "window",
        "number",
        "position",
        "scrollspd",
        "color",
        "wait",
        "sound",
        "speed",
        "command"
    };

    char argCmdCodes[9] =
    {
        0x6B,
        0x6C,
        0x6D,
        0x6E,
        0x77,
        0x78,
        0x79,
        0x7A,
        0x7B
    };

    char* symbolStrings[21] = 
    {
        "...",
        "ahnk",
        "waves",
        "snake",
        "linkl",
        "linkr",
        "up",
        "down",
        "left",
        "right",
        "1heartl",
        "1heartr",
        "2heartl",
        "3heartl",
        "3heartr",
        "4heartl",
        "4heartr",
        "b",
        "a",
        "x",
        "y"
    };

    char symbolCodes[21] =
    {
        0x43,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x5B,
        0x5C,
        0x5D,
        0x5E
    };

    // this array translates ascii characters to their zelda equivalents
    // naturally only 0x5F or so of these indexes are used
    // the rest of the ascii characters return a null character ( '\0' )
    u8 asciiToZchar[256] =
    {
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 

        0x59,0x3E,0x4C,'\0','\0','\0','\0',0x51,0x45,0x46,'\0','\0',0x42,0x40,0x41,'\0', 

        // [0-9] starts at 0x30 and goes to 0x39 along with ? -> 0x3F (incidentally the same)
        // 
        0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,'\0','\0',0x5A,'\0',0x44,0x3F, 

        // [A-Z] starts at 0x41 and goes to 0x5A
        '\0',0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E, 

        // 0x50, continuation of [A-Z] with
        0x0F,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,'\0','\0','\0','\0','\0', 

        // [a-z] starts at 0x61 and goes to 0x7A
        0x51,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,

        // 0x70, continuation of [a-z] with
        0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,0x30,0x31,0x32,0x33,'\0','\0','\0','\0','\0', 

        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0', 
        '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'
    };

    // these variables are for use in ParseZChars() and ValidSyntax()
    static bool autoParse = false;
    static bool realDeal = false;

    static regexp *comment = regcomp("\\i((\\s)|(\\[(\\s*)Comment([^]]*)\\]))*");
    static regexp *msg = regcomp("\\i\\[\\s*Msg\\s*(\\h+)\\s*\\]");
    static regexp *end = regcomp("\\i\\[\\s*End\\s*\\]");

    static regexp *cmds = regcomp("\\i(\\[\\s*(Scroll|NextPic|Choose[23]?|Item|Name|SelChng|Auto|WaitKey|Line[123])\\s*\\])?");
    static regexp *argcmds = regcomp("\\i(\\[\\s*(Window|Number|Position|ScrollSpd|Color|Wait|Sound|Speed|Command)\\s*(\\h+)\\s*\\])?");
    static regexp *symbols = regcomp("\\i(\\[\\s*([1-4]HeartL|[134]HeartR|ahnk|waves|snake|Down|Up|Left|Right|LinkL|LinkR|B|A|X|Y|\\.\\.\\.)\\s*\\])?");
    static regexp *zchars = regcomp("([- A-Za-z0-9,\\.><\\(\\)\\?'\\\"!])*");

    static char errorBuf[512];
    static char* fileBuf = NULL;

    static u32 msgNum       = 0;
    static u32 lineNum      = 0;
    static u32 curMsgLen    = 0;
    static u32 curMsg       = 0;

    static bufPtr outputBuf = NULL;
    static ifstream inFile;
    static ofstream outFile;

    static DialogueFile newDF;

    int matchReg(regexp *r, char **text, u32 *lineRequest)
    {
        char *start = 0;
        int ret = 0;
        int oldLine = 0;
        u32 line = *lineRequest;
        static regexp *newLine = regcomp("\xD\xA");

        //memcpy(
        // reset if we're working with a new buffer

        // don't use invalid pointers
        if(!r || !(*text))
            ret = 0;
        // see if the regex matches in the text
        // then see if it matches the beginning of the text
        else if(regexec(r, *text))
        {
            start = r->startp[0];

            if(start == *text)
            {
                // return value of two indicates it actually ate some characters
                if(start < r->endp[0])
                    ret = 2;
                else
                    ret = 1;

                *text = r->endp[0];

                oldLine = line;

                while(1)
                {
                    if(regexec(newLine, start))
                    {   
                        line++;
                        start = newLine->endp[0];
                    }
                    else
                    {
                        *lineRequest = line;
                        ret = 0;
                        break;
                    }

                    if(start > r->endp[0])
                    {
                        line = oldLine;
                        break;
                    }
                    else
                    {
                        oldLine++;

                    }

                }

                *lineRequest = line;
            }
            else
                ret = 0;
        }
        else
            ret = 0;

        return ret;
    }


u32 ParseZChars(zgPtr game)
{
    // i is a flag that as long as it is set (1) we are still finding actual characters
    // in the message body
    // the flag is let low (0) if we fail to find actual commands, symbols, or characters
    // on the current iteration, in which case we move on to try to find the [End]
    // k is a counter for looping through string arrays to match commands
    // tokenLen tracks how wide (in pixels) a the current token is.
    // tokens are space delimited character strings. 
    // inMemLen is the in memory length of the text message
    // corresponds to $7F1200 in the game
    // this should not exceed 0x800 or you will corrupt the game's other areas of RAM
    // hence why I'm tracking it. It's pretty hard to generate a message that
    // long unless you're purposely trying to.
    // each line of text has room for 21 tiles, which is 168 (0xA8) pixels wide
    // this variable keeps track of how many pixels we've used on the current line
    // [Line1], [Line2], [Line3], and [Scroll] are the only things that reset this
    u32 i = 1;
    u32 k = 0; 
    u32 tokenLen = 0;   
    u32 inMemLen = 0;
    u32 lineLen = 0;
    u32 zCharWidth = 0;
    
    // j is a char pointer used to loop through the base character set when there's 
    // a match. parse is a variable that contains the byte result of a
    // command or ascii char to a zelda char (zchar)
    // fileBuf is our current position while parsing the file
    // base is a string container for commands and symbols that are parsed
    // arg does the same for hexadecimal arguments for those commands that take them.
    char *lineStart = NULL;
    char *j = NULL;
    char parse = 0;
    char base[256];
    char arg[256];
 
    // regular expressions used for parsing the textfile's body
    // for cmds the command is \2
    // for argcmds the command is \2, the argument is \3
    // for symbols the command is \2
    // for zchars the content in question is doesn't need a parenthesis specifier
 
    while(i == 1)
    {
        i = 0;

        // Commands with no arguments ------------------------
        if(matchReg(cmds, &fileBuf, &lineNum) == STUFFED)
        {
            i = 1;

            regsub(cmds, "\\2", base);
            strlwr(base);

            parse = 0;

            for(k = 0; k < 13; k++)
            {       
                if(strlen(base) == strlen(cmdStrings[k]))
                {
                    if(!strstr(base, cmdStrings[k]))
                        continue;

                    parse = cmdCodes[k];
                    break;
                }
            }

            // all commands make us use at least one byte
            inMemLen++;

            switch(parse)
            {            
            case 0x6A: // [Name]

                // since we really have no way of knowing what size name
                // the player will pick, we go conservative and do six characters
                // at max width (7 pixels)
                
                if(lineLen == 0)
                    lineStart = cmds->startp[0];
                
                lineLen += (6 * 7);

                break;

            case 0x70: // [Auto]

                autoParse = true;

                // error condition??

                break;

            case 0x73: // [Scroll] all these commands reset the line
            case 0x74: // [Line1]
            case 0x75: // [Line2]
            case 0x76: // [Line3]
            
                lineLen = 0;

                break;

            case 0x67: // [NextPic]
            case 0x68: // [Choose]
            case 0x69: // [Item]
            case 0x6F: // [SelChng]
            case 0x71: // [Choose2]
            case 0x72: // [Choose3]
            case 0x7E: // [WaitKey]

                break;

            case 0: // error condition (not valid command)
            default:

                // this shouldn't happen at all.
                break;
            }

            if(realDeal)
               PutByte(newDF.data[curMsg], curMsgLen++, parse);

            goto checks;
        }

        // Commands that have arguments ---------------------------------
        if(matchReg(argcmds, &fileBuf, &lineNum) == STUFFED)
        {
            i = 1;

            regsub(argcmds, "\\2", base);
            strlwr(base);

            regsub(argcmds, "\\3", arg);
            
            parse = 0;

            for(k = 0; k < 9; k++)
            {       
                if(strlen(base) == strlen(argCmdStrings[k]))
                {
                    if(!strstr(base, argCmdStrings[k]))
                        {   continue;   }
                    else
                        {   parse = argCmdCodes[k]; break; }
                }
            }

            // all commands make us use at least one byte
            inMemLen++;

            switch(parse)
            {   
            case 0x6B: // [Window XX] valid input is 0,1, or 2
            case 0x6C: // [Number XX] valid input is 0,1, 2, or 3
            case 0x6D: // [Position XX] valid input is 0 or 1
            case 0x6E: // [ScrollSpd XX] valid input is 0 through 7
            case 0x77: // [Color XX] valid input is 0 through 7
            case 0x78: // [Wait XX] valid input is 0 through F
            case 0x79: // [Sound XX] need to know what valid sounds are :(
            case 0x7A: // [Speed XX] 
            
                // accounts for the parameter byte
                inMemLen++;
                
                if(realDeal)
                {
                    PutByte(newDF.data[curMsg], curMsgLen++, parse);

                    parse = hextodec((u8*)arg, 8);

                    PutByte(newDF.data[curMsg], curMsgLen++, parse);
                }

                break;

            case 0x7B: // [Command XX] (only valid XX's for this are 7B, 7C, and 7D
            
                break;

            default:

                break;            
            }



            goto checks;
        }

  
        // Bracket enclosed sequences that represent symbols not in the 
        // ASCII character set ----------------------------------------------
        if(matchReg(symbols, &fileBuf, &lineNum) == STUFFED)
        {
            i = 1;

            if(lineLen == 0)
                lineStart = symbols->startp[0];

            regsub(symbols, "\\2", base);

            for(k = 0, parse = 0; k < 21; k++)
            {       
                if(strlen(base) == strlen(symbolStrings[k]))
                {
                    if(!strstr(base, symbolStrings[k]))
                        {   continue;   }
                    else
                        {   parse = symbolCodes[k]; break; }
                }
            }
            
            zCharWidth = GetByte(game->image,
                                 0x74ADF + asciiToZchar[parse]);

            lineLen += zCharWidth;
            inMemLen++;

            if(realDeal)
                PutByte(newDF.data[curMsg], curMsgLen++, parse);

            goto checks;
        }

        // Everything else that is a valid character in ASCII
        // that's translatable into Zelda 3's symbols -------------------------
        if(matchReg(zchars, &fileBuf, &lineNum) == STUFFED)
        {
            i = 1;

            if(lineLen == 0)
                lineStart = zchars->startp[0];
 
            for(j = zchars->startp[0]; j < zchars->endp[0]; j++)
            {
                // since 'A' maps to 0x00, naturally it maps to the 
                // null character. Hence its just easier to catch 
                // so it doesn't cause false positives

                parse = asciiToZchar[*j];

                if((parse == '\0') && (*j != 'A'))
                    MessageBox(0, "internal error!", "debug", MB_OK);

                zCharWidth = GetByte(game->image,
                                     0x74ADF + parse);

                lineLen += zCharWidth;
                inMemLen++;

                if(realDeal)
                    PutByte(newDF.data[curMsg],
                            curMsgLen++,
                            parse);
            }

            goto checks;    
        }


    checks:

        if( (lineLen > 0xA8) && !autoParse)
        {
            wsprintf(errorBuf, "In message number: 0x%04X\nLine number: %d\nIn-game line starting with: \"", msgNum, lineNum); 
            strncat(errorBuf, lineStart, 40);
            strcat(errorBuf, 
                    "\"\nExceeds the number of pixels available per line.\nUse [Scroll], or [Line1, 2, or 3] commands to correct this.\nAlso, [Name] commands are assumed to be 42 pixels wide (the maximum).");

            return 0;
        }

        if(inMemLen > 0x800)
        {
            wsprintf(errorBuf, "Message number: 0x%04X\nLine number: %d\nThis message exeeds the 0x800 byte limit of the Zelda 3 messaging system.", msgNum, lineNum); 
            return 0;
        }
    }

    return 1;

}

int ValidSyntax(ifstream *textFile, zgPtr game)
{
    u32 i = 0;
    u32 match = 0;
    u32 msgCount = 0;
    long a = 0;
    char b = 0;
    char c = 0;
    char* stepper = NULL;
    char* lineBuf = NULL;
    char* saver = NULL;
    u32 maxMsg = 0;

    // this buffer needs enough flags for all 0x2AAA possible messages
    bufPtr msgFlags = CreateBuffer(0x2AAB / 8);

    // obtain file size
    textFile->seekg(0, ios::end);
    a = textFile->tellg();
    textFile->seekg(0, ios::beg);
    a -= textFile->tellg();

    //dump the file to a char*
    fileBuf             =   (char*) malloc(a+1);
                            memset(fileBuf, 0, a+1);
                            textFile->read(fileBuf, a);

    realDeal = false;

doActualImport:

    i                   =   strlen(fileBuf);
    lineNum             =   1;
    saver               =   fileBuf;

    while(1)
    {
        autoParse = false;

        // eat comment tags
        matchReg(comment, &fileBuf, &lineNum);
       
        if(!matchReg(msg, &fileBuf, &lineNum))
        {
            wsprintf(errorBuf, "Expected: [Msg (message ID)]\nLine: %d", lineNum);
            break;
        }
        else
        {   
            regsub(msg, "\\1", errorBuf);
            msgNum = hextodec((u8*) errorBuf, 8);

            if(msgNum > 0x2AA9)
            {
                wsprintf(errorBuf,"A message index of 0x%04X is too large.\n0x2AA9 is the upper limit.\nLine: %d", msgNum, lineNum);
                break;
            }
            else
            {
                if(realDeal)
                {
                    newDF.data[msgNum] = CreateBuffer(0x800);
                    curMsgLen = 0;
                    curMsg = msgNum;

                    if(_CrtCheckMemory() == FALSE)
                        MessageBox(0, errorBuf, "InsertScript()", MB_OK);
                }
                else if( GetBit(msgFlags, msgNum) == 1)
                {
                     wsprintf(errorBuf, "Message index 0x%04X has been redefined.\nLine: %d\nPlease give this message a unique index.", msgNum, lineNum);
                     break;
                }
                else
                {
                    PutBit(msgFlags, 1, msgNum);
                    msgCount = max(msgCount, msgNum);
                }                
            }

        }

        // eat valid zelda character sequences
        // and also eat any (command / special symbol) tags
        if(!ParseZChars(game))
        {   break;  }
        
        if(!matchReg(end, &fileBuf, &lineNum))
        {
            wsprintf(errorBuf, "Expected: [End]\nLine: %d", lineNum);
            break;
        }
        else
        {
            if(realDeal)
            {
                PutByte(newDF.data[curMsg], curMsgLen++, 0x7F);
                ResizeBuffer(newDF.data[curMsg], curMsgLen);
            }
        }
    }

    if( strlen(fileBuf) || (msgCount == 0))
    {
        MessageBox(0, errorBuf, "prob", MB_OK);
        goto error;
    }
 
    // in this situation, we have passed the validation step
    // and can begin actually storing the data to buffers

if(realDeal == true)
    goto finished;

    realDeal    =   true;
    fileBuf     =   saver;

    // account for the fact that the last message index is one less
    // then the total number of messages we need to allocate for
    msgCount++; 

    newDF.data = (bufPtr*) malloc(sizeof(bufPtr) * msgCount);
    newDF.numMsgs = msgCount;
    memset(newDF.data, 0, sizeof(bufPtr) * msgCount);

    goto doActualImport;

finished:

    free(saver);

    DestroyBuffer(msgFlags);

    return msgCount;

error:

    free(saver);
    DestroyBuffer(msgFlags);

    return 0;
}



void DumpScript(zgPtr game, HINSTANCE prog)
{
    u32 i = 0, j = 0;
    u32 textPointer = Get3Bytes(game->image, asm_text_ref);
    u32 msgPointer = 0;
    u8  a = 0;
    u32 b = 0, c = 0, d = 0;
    u32 e = 0;
    u32 f = 0;

    bufPtr textFileName = CreateBuffer(MAX_PATH);
    bufPtr digits = CreateBuffer(0x08);
    bufPtr textData = NULL;

    // use the currently loaded in text data from the game
    // This is loaded into memory when the game is loaded into Black magic
    DialogueFile *df = &game->dialogueFile;

    ofstream textDump;

	bmOFN.hInstance = prog;
	bmOFN.lpstrFilter = NULL;
    bmOFN.lpstrFilter = "Text dumps\0*.log\0All files\0*.*\0";;
	bmOFN.lpstrFile = (LPSTR) textFileName->contents;
	bmOFN.lpstrFile[0] = 0; // a particularity of using the open file dialogs.
	bmOFN.lpstrTitle = "Export dialogue and text";
	bmOFN.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    bmOFN.lpstrDefExt = "log";

    if(!GetSaveFileName(&bmOFN))
    {
        MessageBox(NULL, "Failed to save dialogue", "error", MB_OK);
        goto dumpScriptCleanup;
    }

    textDump.open((const char*) textFileName->contents);

    for(i = 0; i < df->numMsgs; i++)
    {
        textData = df->data[i];
        msgPointer = 0;

        // output the Msg's ID number
        makeHexStr(digits, i);
        textDump << "[Msg " << ToString(digits) << "]";

        j = 0;

        while(1)
        {
            j++;

            // safeguard 
            if(j > textData->length)
                break;

            a = GetByte(textData, msgPointer++);
   
            if(a == 0x7F)
            {
                textDump << convChars[a] << "\n\n";
                break;
            }
            else if( (a >= 0x67) && (a < 0x80) )
            {
                // check for command bytes

                // b is the code length,
                b = GetByte(game->image, 
                            asm_code_lengths + a) - 1;
                
                textDump << convChars[a];

                if(b == 0)
                    continue;

                while(b > 0)
                {
                    a = GetByte(textData, msgPointer++);
                    makeHexStr(digits, (u32) a);
                    textDump << ToString(digits);

                    b--; e++; f++;
                }

                textDump << "]";
            }
            else if( (a >= 0x88) && (a <= 0xE8) )
            {
                // the original condition is >= 0x80 but
                // these conditions are safer in the event 
                // of junk data

                // utilize the dictionary entries

                a -= 0x88;

                b = Get2Bytes(game->image, 0x74703 + (a << 1));
                c = Get2Bytes(game->image, 0x74705 + (a << 1));

                while(b < c)
                {
                    d = CpuToRomAddr(0x0E0000 + b);
                    a = GetByte(game->image, d);

                    textDump << convChars[a];                    

                    b++; f++;
                }
            }
            else
                textDump << convChars[a];

            // end of the j loop
        }
        
    }
    
    textDump.close();

dumpScriptCleanup:

    DestroyBuffer(digits);
    DestroyBuffer(textFileName);

    return;
}

void LoadDialogueFile(zgPtr game)
{
    u32 i = 0;
    u32 j = 0;
    u32 k = 0;
    u32 msgPointer = 0;
    u32 dataPointer = 0;
    u8  a = 0;
    u32 b = 0, c = 0, d = 0;
    u32 e = 0;
    u32 f = 0;

    bool first = false;

    // obtain the CPU address of where the text pointers are at
    // By default this is $7F71C0, b/c by default the text pointers reside in RAM
    // Black Magic modifies the game to leave the pointers in ROM when it saves
    // This frees up some RAM and means that savestates are not broken as easily.
    u32 textPointer = Get3Bytes(game->image, asm_text_ref);

    bufPtr textPtrs = NULL;   
    bufPtr digits   = NULL;

    if(textPointer == 0x7F71C0)
    {
        // pointer table is using the old formatting method
        textPtrs = GetOldTextTable(game);
    }
    else
    {
        // pointer table is in ROM b/c Black Magic has previously saved it.
        // but without the BM_HEADER struct we don't know where it is
        textPtrs = GetNewTextTable(game);
    }

    if(!textPtrs)
    {
        MessageBox(NULL, "Error in LoadDialogueFile() obtaining text pointers", "error", MB_OK);
        return;
    }

    DialogueFile *df = &game->dialogueFile;

    df->numMsgs = textPtrs->length / 3;
    df->data = (bufPtr*) malloc(sizeof(bufPtr) * df->numMsgs);
    
    for(i = 0, j = 0; i < textPtrs->length; i += 3, j++)
    {
        msgPointer = CpuToRomAddr(Get3Bytes(textPtrs, i));
        dataPointer = 0;

        if(GetByte(game->image, msgPointer) == 0x7F)
        {
            if(first == false)
            {
                first = true;
                k = j;
                df->data[j] = CreateBuffer(1);
                PutByte(df->data[j], 0, 0x7F);
            }

            df->data[j] = df->data[k];
            continue;
        }

        df->data[j] = CreateBuffer(0x800);

        while(1)
        {       
            a = GetByte(game->image, msgPointer++);
            e++; f++;

            if(a == 0x7F)
            {
                PutByte(df->data[j], dataPointer++, a);
                break;
            }

            else if( (a >= 0x67) && (a < 0x80) )
            {
                // check for command bytes

                // b is the code length,
                b = GetByte(game->image, asm_code_lengths + a) - 1;
                
                PutByte(df->data[j], dataPointer++, a);

                if(b == 0)
                    continue;

                while(b > 0)
                {
                    a = GetByte(game->image, msgPointer++);
                    PutByte(df->data[j], dataPointer++, a);
    
                    e++;
                    f++;
                    b--;
                }
            }
            else if( (a >= 0x88) && (a <= 0xE8) )
            {
                // the original condition is >= 0x80 but
                // these conditions are safer in the event 
                // of junk data

                // utilize the dictionary entries

                a -= 0x88;

                b = Get2Bytes(game->image, 0x74703 + (a << 1));
                c = Get2Bytes(game->image, 0x74705 + (a << 1));

                while(b < c)
                {
                    d = CpuToRomAddr(0x0E0000 + b);
                    a = GetByte(game->image, d);
                    PutByte(df->data[j], dataPointer++, a);

                    b++; f++;
                }
            }
            else
            {
                PutByte(df->data[j], dataPointer++, a);
            }
        }

        ResizeBuffer(df->data[j], dataPointer);        
    }

    DestroyBuffer(textPtrs);

    return;
}

bufPtr GetNewTextTable(zgPtr game)
{
    u32 m = game->bm_Header.textNumMsgs * 3;

    if(m > 0xFFFF)
        return NULL;

    if(!m)
        return NULL;
    
    bufPtr output = CreateBuffer(m);

    CopyBuffer(output,  game->image,
               0,       CpuToRomAddr(game->bm_Header.textOffset),
               m);

    return output;
}


bufPtr GetOldTextTable(zgPtr game)
{
    u32 m = 0; // this is the position within the output buffer
    u32 oSize = 0x600; // this is the nubmer of pointers found so far
    u32 pos = 0xE0000; // this is our current position within the rom
    unsigned char code = 0;
    unsigned char codeLen = 0;

    bufPtr output = CreateBuffer(oSize);

    while(1)
    {
        // slightly excessive but oh well
        if(m > (oSize - 0x50) )
        {
            oSize += 0x200;
            ResizeBuffer(output, oSize);
        }

        Put3Bytes(output, m, RomToCpuAddr(pos));
        m += 3;

        while(1)
        {
            code = GetByte(game->image, pos);
            codeLen = GetByte(game->image,
                              asm_code_lengths + code);

            if( (code < 0x67) || (code >= 0x80) )
                pos++;
            else
                pos += codeLen;

            if( code == 0x7F )
                break;

            if( code != 0x80 )
                if( code == 0xFF)
                    break;
                else
                    continue;

            m -= 3;
            pos = 0x75F40;
            break;

        }

        // m is decremented b/c that would mean we're adding another pointer
        if( code == 0xFF )
        {   
            m -= 3;
            break;
        }
    }

    ResizeBuffer(output, m);

    return output;

}

char* command_strings[] =
{
    "NextPic", // code 0x67
    "Choose",
    "Item",
    "Name",
    "Window",
    "Number",
    "Position",
    "ScrollSpd",
    "SelChng",
    "Auto",
    "Choose2",
    "Choose3",
    "Scroll",
    "Line1",
    "Line2",
    "Line3",
    "Color", // code 0x77
    "Wait",
    "Sound",
    "Speed",
    "Command 7B",
    "Command 7C",
    "Command 7D",
    "WaitKey",
    "End", // code 0x7F 
};

void InsertScript(zgPtr game, HINSTANCE prog)
{
    bufPtr textFileName = CreateBuffer(MAX_PATH);

	bmOFN.hInstance = prog;
	bmOFN.lpstrFilter = NULL;
    bmOFN.lpstrFilter = "Text dumps\0*.log\0All files\0*.*\0";;
	bmOFN.lpstrFile = (LPSTR) textFileName->contents;
	bmOFN.lpstrTitle = "Import dialogue and text";
	bmOFN.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    bmOFN.lpstrDefExt = "log";

    if(!GetOpenFileName(&bmOFN))
        goto insertScriptFailure;

    inFile.open((const char*) textFileName->contents);

    if(inFile.fail())
        goto insertScriptFailure;

    ParseScript(game, prog);

    if(inFile.fail())
        inFile.clear();

    if(inFile.is_open())
        inFile.close();

    DestroyBuffer(textFileName);

    return;

insertScriptFailure:
    
    if(inFile.fail())
        inFile.clear();

    if(inFile.is_open())
        inFile.close();

    MessageBox(NULL, "Error opening text dump", "error", MB_OK);
    DestroyBuffer(textFileName);
    
    return;
}


void ParseScript(zgPtr game, HINSTANCE prog)
{
    char dummy[256];
    char *curMsg = (char*) malloc(256);

    int a = 0;
    int i = 0;
    int j = 0;

    u32 msgCount = 0;
    newDF.data = NULL;
    newDF.numMsgs = 0;

    bool first = false;

    msgCount = ValidSyntax(&inFile, game);

    if(!msgCount)
    {
        // The parser encountered an error or found no messages

        inFile.close();  return;
    }
    else
    {
        // At this point we have successfully parsed the file
        // Next we go through and figure out how many of these

        inFile.close();

        for(i = 0; i < msgCount; i++)
        {
            if(newDF.data[i] == NULL)
            {
                if(first == false)
                {
                    first = true;
                    newDF.data[i] = CreateBuffer(1);
                    PutByte(newDF.data[i], 0, 0x7F);
                    j = i;
                }

                newDF.data[i] = newDF.data[j];
            }
        }
    }

    
    //Check and see if the user wants to back up the old text data before loading in the new one
    i = MessageBox(0, "The existing text and dialogue data is about to be overwritten.\nWould you like to create a backup of it before the new data is loaded in?", NULL, MB_YESNO);

    if(i == IDYES)
        DumpScript(game, prog);

    // Since the number of messages in the old script could differ
    // simply free that table.

    for(i = 0; i < game->dialogueFile.numMsgs; i++)
    {
        DestroyBuffer(game->dialogueFile.data[i]); 

        if(i == 0x18C)
            continue;

    }


    free(game->dialogueFile.data);

    game->dialogueFile.data = newDF.data;
    game->dialogueFile.numMsgs = newDF.numMsgs;

    return;
}





