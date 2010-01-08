
    lorom
    
    !ADD = "CLC : ADC"
    !SUB = "SEC : SBC"

; =====================================================

    incsrc overworld/owData.asm
    incsrc graphics.asm
    
; =====================================================

    ; patch the sprite graphics loading location
    ; GetSprGfxPtr and GetBgGfxPtr are externally defined routines that Black Magic will output at a dynamic location.
    org $00D53B
    
        LDA.b #$00

        JSL GetSprGfxPtr
        
        LDA $02 : STA $05
        
        NOP #7

    org $00DA99

        LDA.b #$00

        JSL GetSprGfxPtr
        
        LDA $02 : STA $05
        
        NOP #7
    
    org $00E2D8

        LDA.b #$00

        JSL GetSprGfxPtr
        
        NOP #9
    
    org $00E462
    
        LDA.b #$00

        JSL GetSprGfxPtr
        
        LDA $02 : STA $05
        
        NOP #7

    org $00E6BA
    
        LDA.b #$00

        JSL GetSprGfxPtr
        
        NOP #9

    org $00E72E
    
        LDA.b #$00

        JSL GetSprGfxPtr
        
        NOP #9

    org $00E772

        LDA.b #$C8

        JSL GetSprGfxPtr
        
        NOP #9

    ; Patch the background graphics loading location
    org $00E78F
    
        LDA.b #$C8

        JSL GetBgGfxPtr
        
        NOP #9
    
; =====================================================

; overworld map32 to map16 conversion arrays hooks

    ; Overworld patch to expand the number of map32 tiles available.    

    org $02F691

    ConvertMap32ToMap16:
    {

        PHA : AND.w #$FFF8 : CMP $4440  ; map32 value
    	
    	BNE .notPrevTile    ; if(A != $4440) goto BRANCH_1;

    	JMP .sameTile       ; this is a shortcut to load the same data if the last
    	; map32 value matches the current one (more or less)

    .notPrevTile

        ; $4440 = input, $4442 = input >> 1;
        ; X = (input >> 2) + (input >> 1);
    	STA $4440 : LSR A : STA $4442 : LSR A : ADC $4442 : TAX
    	
        SEP #$20
    	
    	LDA.l owMap32To16UpperLeft+0, X : STA $4400
    	LDA.l owMap32To16UpperLeft+1, X : STA $4402
    	LDA.l owMap32To16UpperLeft+2, X : STA $4404
    	LDA.l owMap32To16UpperLeft+3, X : STA $4406

    	LDA.l owMap32To16UpperLeft+4, X : PHA : LSR #4 : STA $4401
    	PLA : AND.b #$0F : STA $4403

    	LDA.l owMap32To16UpperLeft+5, X : PHA : LSR #4 : STA $4405
    	PLA : AND.b #$0F : STA $4407

    	LDA.l owMap32To16UpperRight+0, X : STA $4410
    	LDA.l owMap32To16UpperRight+1, X : STA $4412
    	LDA.l owMap32To16UpperRight+2, X : STA $4414
    	LDA.l owMap32To16UpperRight+3, X : STA $4416

    	LDA.l owMap32To16UpperRight+4, X : PHA : LSR #4 : STA $4411
    	PLA : AND.b #$0F : STA $4413

    	LDA.l owMap32To16UpperRight+5, X : PHA : LSR #4 : STA $4415
    	PLA : AND.b #$0F : STA $4417

    	LDA.l owMap32To16LowerLeft+0, X : STA $4420
    	LDA.l owMap32To16LowerLeft+1, X : STA $4422
    	LDA.l owMap32To16LowerLeft+2, X : STA $4424
    	LDA.l owMap32To16LowerLeft+3, X : STA $4426

    	LDA.l owMap32To16LowerLeft+4, X : PHA : LSR #4 : STA $4421
    	PLA : AND.b #$0F : STA $4423

    	LDA.l owMap32To16LowerLeft+5, X : PHA : LSR #4 : STA $4425
    	PLA : AND.b #$0F : STA $4427

    	; $23400, X THAT IS
    	LDA.l owMap32To16LowerRight+0, X : STA $4430
    	LDA.l owMap32To16LowerRight+1, X : STA $4432
    	LDA.l owMap32To16LowerRight+2, X : STA $4434
    	LDA.l owMap32To16LowerRight+3, X : STA $4436

    	LDA.l owMap32To16LowerRight+4, X : PHA : LSR #4 : STA $4431
    	PLA : AND.b #$0F : STA $4433

    	LDA.l owMap32To16LowerRight+5, X : PHA : LSR #4 : STA $4435
    	PLA : AND.b #$0F : STA $4437

    	REP #$30

.sameTile

    	PLA : AND.w #$0007 : TAX
    	
    	LDA $4400, X : STA [$00], Y
    	LDA $4420, X : STA [$03], Y : INY #2
    	LDA $4410, X : STA [$00], Y
    	LDA $4430, X : STA [$03], Y : INY #2
    	
    	RTS
    }

; =====================================================

    ; Overworld patch to expand the number of OW areas available from 0xA0 to 0xC0

    ; modifies the pointers for the large areas (1024x1024)
    ; each pointer points to compressed map32 data
    
    org $02F59D

    	LDA owMap32Ptrs_1, X   : STA $C8
    	LDA owMap32Ptrs_1+1, X : STA $C9

    org $02F5C8
        
    	LDA owMap32Ptrs_2, X   : STA $C8
    	LDA owMap32Ptrs_2+1, X : STA $C9

    org $02F7E3
    
    	LDA owMap32Ptrs_1, X   : STA $C8
    	LDA owMap32Ptrs_1+1, X : STA $C9

    org $02F80E
        
    	LDA owMap32Ptrs_2, X   : STA $C8
    	LDA owMap32Ptrs_2+1, X : STA $C9    
    
; =====================================================

    ; Overworld patch to expand the number of hole markers
    org $1BB860

    HolePatch:
    {
        ; Patch to expand the number of Overworld holes
        ; Sets the entrance for when Link falls into a hole

        !numHoles = holeAreaNumbers-holeMap16Coords
        
        REP #$31

        LDA $20 : AND.w #$FFF8 : STA $00
        !SUB $0708 : AND $070A : ASL #3 : STA $06

        LDA $22 : AND.w #$FFF8 : LSR #3 : STA $02
        !SUB $070C : AND $070E : !ADD $06 : STA $00

        LDX.w #!numHoles-2

    .nextHole

        LDA $00 : CMP.l holeMap16Coords, X

        BNE .noMap16Match

        LDA $040A : CMP.l holeAreaNumbers, X

        BEQ .matched

    .noMap16Match

        DEX #2

        BPL .nextHole

        LDX.w #!numHoles           ; Send us to the Chris Houlihan room

        SEP #$20

        LDA.b #$00 : STA $7EF3CA   ; Put Link in the Light World

    .matched

        SEP #$30

        TXA : LSR A : TAX

        ; Set the entrance Link will come into the building via
        LDA.l holeEntranceNumbers, X : STA $010E                
        STZ $010F

        RTL
    }

; =====================================================
    
    ; Overworld patch to expand the number of entrances
    org $1BBD07

        !numEntrances = entranceMap16Coords-entranceAreaNumbers
    
        LDX.w #!numEntrances-2    

    org $1BBD19

        CMP.l entranceAreaNumbers, X
        
    org $1BBD10

        CMP.l entranceMap16Coords, X
        
    org $1BBD5F

        LDA.l entranceNumbers, X   

; =====================================================

    ; Overworld patch to expand the amount of secrets (item) data.
    org $1BC8B6
    
        ; go to a hook that Black Magic will insert
        JSL LoadOwSecretPointer
        
        ; skips over the old code
        BRA $07

; =====================================================
    
    ; Overworld patch to expand the number of exits (dungeon to overworld linkage or overworld to overworld linkage)
    org $02E4A3
    ExitPatch:
    {
    	; Loads a bunch of exit data (e.g. Link's coordinates)

        ; Data Bank = Program Bank
    	PHB : PHK : PLB

        ; Set it so that we are outdoors…
    	STZ $1B : STZ $0458 ;
    	
    	REP #$20
    	
    	LDA.w #$0000 : STA $7EC017
    	
        ; We are not in a dungeon.
    	LDA.w #$00FF : STA $040C
    	STZ $04AC
    	
        ; If in Link’s house...
    	LDA $A0 : CMP.w #$0104
    	
    	BEQ .alpha

        ; special outdoor areas like Zora falls
    	CMP.w #$0180
    		
    	BCS .alpha

        ; If less than #$0100, branch
    	CMP.w #$0100
    	
    	BCC .alpha
    	; This code apparently executes for all rooms >= #$0100 and < 
    	; #$0180. (Excluding Link's house)

    	; these rooms only have one exit.

    	JSR $E5D4; $165D4 IN ROM
    	JMP .noExplicitExit

    .alpha

    	LDX.b #$9E;

    .beta

        ; Tries to find the appropriate room in a large array. X in this case becomes the exit number * 2
    	DEX #2 : CMP $DD8A, X    	

    	BNE .beta

    	; Load things like scroll data
    	LDA $DF15, X : STA $E6 : STA $E8 : STA $0122 : STA $0124
    	
    	LDA $DFB3, X : STA $E0 : STA $E2 : STA $011E : STA $0120
    	
    	; Loads up Link's coordinates
    	LDA $E051, X : STA $20
    	
		; See the data document for details
    	LDA $E0EF, X : STA $22
    	
    	LDA $DE77, X : STA $84 : !SUB.w #$0400 : AND.w #$0F80 : ASL A : XBA : STA $88
    	
    	LDA $84 : !SUB.w #$0010 : AND.w #$003E : LSR A : STA $86
    	
    	LDA $E18D, X : STA $0618 : DEC #2 : STA $061A
    	
    	LDA $E22B, X : STA $061C : DEC #2 : STA $061E;
    	
		; Make Link face the downwards direction
    	LDA.w #$0002 : STA $2F
    	
    	LDA $E367, X : STA $0696
    	
    	LDA $E405, X : STA $0698
    	
    	TXA : LSR A : TAX
    	
    	SEP #$20
    	
		; These are the overworld areas that the exits lead to.
    	LDA $DE28, X : STA $8A : STA $040A
    	STZ $8B : STZ $040B
    	
    	LDA $E2C9, X : STA $0624 : STZ $0625 : ASL A
    	
    	BCC .gamma

    	DEC $0625   ; sign extends to 16-bit
    	
    .gamma

    	LDA $E318, X : STA $0628 : STZ $0629 : ASL A
    	
    	BCC .delta

    	DEC $0629   ; sign extend to 16-bit

    .delta

    	REP #$20
    	
    	LDA.w #$0000 : !SUB $0624 : STA $0626
    	
    	LDA.w #$0000 : !SUB $0628 : STA $062A
    	
    .noExplicitExit

        PLB
    	
    ; *$1658B ALTERNATE ENTRY POINT

    	LDA #$FFF8 : STA $EC		; $EC = -8
    	
    	SEP #$30
    	
    	PHB : PHK : PLB
    	
    	JSR $AB08; $12B08 IN ROM
    		
    	LDA.b #$E4 : STA $0716

    	STZ $0713
    	
    	LDA $8A : AND.b #$3F : ASL A : TAY
    	
    	REP #$20

    	LDX.b #$00 : LDA $0712
    	
    	BEQ .largeOwMap
    	
    	INX #2

    .largeOwMap
    	
    	JSR $C0C3      ; $140C3 IN ROM
    	
    	SEP #$20
    	
    	PLB

    	STZ $A9
    	
    	LDA.b #$02 : STA $AA : STA $A6 : STA $A7
    	
    	LDA.b #$80 : STA $45 : STA $44
    	
    	LDA.b #$0F : STA $42 : STA $43
    	
    	LDA.b #$FF : STA $24 : STA $29
    	
        RTS
    }

; =====================================================

    org $09C4AC

        ; Overworld patch to expand the number of sprites that can be stored in the ROM.
        ; This is a rewritten version of the original routine, except this one uses 3-byte pointers rather than 2-byte pointers.
    
    LoadOwData:
    {
        JSL LoadOwData2
        
        RTS
    }
        
    org $3F8000
        
    LoadOwData2:
    {
        PHP
    
        REP #$30
        
        PEI ($03)

    	LDA $040A
        TAY ; save this register so it's easily retrieved later.
        AND.w #$0007 : ASL A : XBA : STA $0FBC

    	TYA ; restoring from $040A
        AND.w #$003F : LSR #2 : AND.w #$000E : XBA : STA $0FBE

    	TYX ; restoring from $040A
    	LDA $09C635, X : AND.w #$00FF : XBA : STA $0FB8 : STA $0FBA
    
        ; What Overworld area are we in? (BTW, X = $040A * 3)
    	TYA : ASL A : !ADD $040A : TAX

        PHP
        SEP #$20
        
    	; Check progress indicator 1.
    	LDA $7EF3C5 : CMP.b #$03

    	BEQ .part2      ; Have we defeated Agahnim?

    	CMP.b #$02		; Nope, did we save Zelda (the first time)?

    	BEQ .part1      ; Yes...

        PLP
        
        ; Nope. Load a pointer from the "Beginning" sprites for the Overworld.
    	LDA.l owBeginningSpritePtrs, X : TAY
    	LDA.l owBeginningSpritePtrs+1, X

    	BRA .continue

    .part2

        PLP
        
        ; // Load the "Second part" sprites for the Overworld.
        LDA.l owPart2SpritePtrs, X : TAY
    	LDA.l owPart2SpritePtrs+1, X

        BRA .continue

    .part1

        PLP
    
        ; Load the "First Part" sprites for the Overworld.
        LDA.l owPart1SpritePtrs, X : TAY
        LDA.l owPart1SpritePtrs+1, X

    .continue

        STY $00 ; little hack to save a byte, goddamit :(
    	STA $01

    	LDY.w #$0000

        SEP #$20
        
    .load_loop

        ; Read off the sprite information until we reach a #$FF byte.
    	LDA [$00], Y : INC A

    	BEQ .return ; Stop loading, basically.

    	INY #2

    	; Is this a "Falling Rocks" sprite?
    	LDA [$00], Y :  CMP.b #$F4

    	BNE .notFallingRocks

    	INC $0FFD       ; Set a falling rocks flag apparently and skip past this sprite
    	INY #1

    	BRA .load_loop

    .notFallingRocks    ; Anything other than falling rocks.
    
        DEY #2
    
    	LDA [$00], Y : PHA : AND.b #$F0 : LSR #2 : STA $03 : INY

    	LDA [$00], Y : LSR #4 : CLC : ADC $03 : STA $06

    	PLA : ASL #4 : STA $07

    	; All this is to tell us where to put the sprite in the sprite map.
    	LDA [$00], Y : AND.b #$0F : ORA $07 : STA $05

    	; The sprite / overlord index as stored as one plus it’s normal index. Don’t ask me why yet.
    	INY : LDA [$00], Y : LDX $05 : INC A : STA $7FDF80, X   ; Load them into what I guess you might call a sprite map.

    	INY ; Move on to the next sprite / overlord.

    	BRA .load_loop

    .return
    
        REP #$20
        
        PLA : STA $03
    
        PLP
    
    	RTL
    }
        
; =====================================================
 