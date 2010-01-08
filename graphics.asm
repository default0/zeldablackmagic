
    ; total size = 0x53 bytes when compiled

    GetSprGfxPtr:
    {
        PHX : PHY : PHP : PHB
    
        SEP #$30
    
        ; set data bank to $00 (just a precaution)
        LDA.b #$00 : PHA : PLB
    
        REP #$30
    
        PHA : PHY
    
        ; multiply the graphics pack number by 3 to index into the table
        LDA $01, S : ASL A : !ADD $01, S : TAX
    
        ; set the direct page address (usually $00-$02 or $C8-$CA)
        LDA $03, S : TAY    
    
        ; Save the pointer to the graphics pack to a location in direct page memory.
        LDA.l SprGfxPtrTable, X   : STA $0000, Y
        LDA.l SprGfxPtrTable+2, X : STA $0002, Y
    
        PLY : PLA
    
        ; restore all the settings we changed.
        PLB : PLP : PLY : PLX
    }

    GetBgGfxPtr:
    {
        PHX : PHY : PHP : PHB
    
        SEP #$30
    
        ; set data bank to $00 (just a precaution)
        LDA.b #$00 : PHA : PLB
    
        REP #$30
    
        PHA : PHY
    
        ; multiply the graphics pack number by 3 to index into the table
        LDA $01, S : ASL A : !ADD $01, S : TAX
    
        ; set the direct page address (usually $00-$02 or $C8-$CA)
        LDA $03, S : TAY    
    
        ; Save the pointer to the graphics pack to a location in direct page memory.
        LDA.l BgGfxPtrTable, X   : STA $0000, Y
        LDA.l BgGfxPtrTable+2, X : STA $0002, Y
    
        PLY : PLA
    
        ; restore all the settings we changed.
        PLB : PLP : PLY : PLX
    }
 