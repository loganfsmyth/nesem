C000 : 78		sei			;
C001 : A2 FF		ldx	#$FF		;
C003 : 9A		txs			;
C004 : D8		cld			;
C005 : A9 00		lda	#$00		;
C007 : 8D 00 20		sta	$2000		;PPU CtrlReg.#1
C00A : 20 8B C0		jsr	$C08B		;	- Zero RAM
C00D : 20 A7 C0		jsr	$C0A7		;	- Zero VRAM
C010 : 20 F1 C8		jsr	$C8F1		;	- Write Palette
C013 : 20 BC C0		jsr	$C0BC		;	- Copy Patterntable
C016 : 20 23 C9		jsr	$C923		;	- Copy Nametable data
C019 : 20 4E C9		jsr	$C94E		;	- Do Calculations
C01C : A9 21		lda	#$21		;
C01E : A0 68		ldy	#$68		;
C020 : 20 0E C9		jsr	$C90E		;	- Set VRAM address to 0x2168
C023 : A9 4F		lda	#$4F		;
C025 : 85 00		sta	$00		;
C027 : A9 C0		lda	#$C0		;
C029 : 85 01		sta	$01		;
C02B : 20 15 C9		jsr	$C915		;	Copy from 0xC04F into VRAM
C02E : A9 21		lda	#$21		;
C030 : A0 89		ldy	#$89		;
C032 : 20 0E C9		jsr	$C90E		;	Set address to 0x2189
C035 : A5 02		lda	$02		;
C037 : 20 59 C0		jsr	$C059		;	Magic
C03A : A9 21		lda	#$21		;
C03C : A0 8F		ldy	#$8F		;
C03E : 20 0E C9		jsr	$C90E		;	Set VRAM to 0x218F
C041 : A5 03		lda	$03		;
C043 : 20 59 C0		jsr	$C059		;
C046 : 20 7C C0		jsr	$C07C		;
C049 : 20 85 C0		jsr	$C085		;
C04C : 18		clc			;
C04D : 90 FE		bcc	$C04D		;


#10 and #12
C059 : 48		pha			;
C05A : 6A		ror	a		;
C05B : 6A		ror	a		;
C05C : 6A		ror	a		;
C05D : 6A		ror	a		;
C05E : 20 62 C0		jsr	$C062		;
C061 : 68		pla			;
C062 : 29 0F		and	#$0F		;
C064 : AA		tax			;
C065 : BD 6C C0		lda	$C06C,x		;
C068 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C06B : 60		rts			;


#13
C07C : A9 00		lda	#$00		;
C07E : 8D 05 20		sta	$2005		;BG Scrool Reg.
C081 : 8D 05 20		sta	$2005		;BG Scrool Reg.
C084 : 60		rts			;


#14
C085 : A9 08		lda	#$08		;
C087 : 8D 01 20		sta	$2001		;PPU CtrlReg.#2
C08A : 60		rts			;


#1 - Zero all sections of RAM except Stack
C08B : A9 00		lda	#$00		;
C08D : A2 00		ldx	#$00		;
C08F : 95 00		sta	$00,x		;
C091 : 9D 00 02		sta	$0200,x		;
C094 : 9D 00 03		sta	$0300,x		;
C097 : 9D 00 04		sta	$0400,x		;
C09A : 9D 00 05		sta	$0500,x		;
C09D : 9D 00 06		sta	$0600,x		;
C0A0 : 9D 00 07		sta	$0700,x		;
C0A3 : CA		dex			;
C0A4 : D0 E9		bne	$C08F		;
C0A6 : 60		rts			;


#2 - Write zeroes to 0x0000 - 0x3000 of the VRAM
C0A7 : A9 00		lda	#$00		;
C0A9 : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C0AC : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C0AF : A2 30		ldx	#$30		;
C0B1 : A8		tay			;
C0B2 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C0B5 : 88		dey			;
C0B6 : D0 FA		bne	$C0B2		;
C0B8 : CA		dex			;
C0B9 : D0 F6		bne	$C0B1		;
C0BB : 60		rts			;


#4 - Copy PRG rom into VRAM...I guess
C0BC : A9 00		lda	#$00		;
C0BE : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C0C1 : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C0C4 : A9 F1		lda	#$F1		;
C0C6 : 85 00		sta	$00		;
C0C8 : A9 C0		lda	#$C0		;
C0CA : 85 01		sta	$01		;
C0CC : A2 00		ldx	#$00		;
C0CE : A0 00		ldy	#$00		;
C0D0 : B1 00		lda	($00),y		;
C0D2 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C0D5 : C8		iny			;
C0D6 : C0 08		cpy	#$08		;
C0D8 : D0 F6		bne	$C0D0		;
C0DA : A9 00		lda	#$00		;
C0DC : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C0DF : 88		dey			;
C0E0 : D0 FA		bne	$C0DC		;
C0E2 : A5 00		lda	$00		;
C0E4 : 18		clc			;
C0E5 : 69 08		adc	#$08		;
C0E7 : 85 00		sta	$00		;
C0E9 : 90 02		bcc	$C0ED		;
C0EB : E6 01		inc	$01		;
C0ED : CA		dex			;
C0EE : D0 E0		bne	$C0D0		;
C0F0 : 60		rts			;




#3 - Write "0x0E", "0x30", "0x00", and "0x00" into the palette
C8F1 : A9 3F		lda	#$3F		;
C8F3 : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C8F6 : A9 00		lda	#$00		;
C8F8 : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C8FB : A9 0E		lda	#$0E		;
C8FD : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C900 : A9 30		lda	#$30		;
C902 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C905 : A9 00		lda	#$00		;
C907 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C90A : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C90D : 60		rts			;


#5.1 and #7 and # 9 and #11 - Set VRAM Address to (Y A)
C90E : 8D 06 20		sta	$2006		;VRAM Addr.Reg.
C911 : 8C 06 20		sty	$2006		;VRAM Addr.Reg.
C914 : 60		rts			;

#5.2 and #8 - Copy 0xFF elements from ($01 $00) into VRAM
C915 : A0 00		ldy	#$00		;
C917 : B1 00		lda	($00),y		;
C919 : F0 07		beq	$C922		;
C91B : C8		iny			;
C91C : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C91F : 4C 17 C9		jmp	$C917		;
C922 : 60		rts			;


#5 - Copy 0xFF bytes from 0xC935 to 0x2122
C923 : A9 21		lda	#$21		;
C925 : A0 22		ldy	#$22		;
C927 : 20 0E C9		jsr	$C90E		;
C92A : A9 35		lda	#$35		;
C92C : 85 00		sta	$00		;
C92E : A9 C9		lda	#$C9		;
C930 : 85 01		sta	$01		;
C932 : 4C 15 C9		jmp	$C915		;




#NMI
C94D : 40		rti			;


#6 - Does the Calculations
C94E : 18		clc			;
C94F : A9 00		lda	#$00		;
C951 : 69 00		adc	#$00		;
C953 : 50 06		bvc	$C95B		;
C955 : A5 02		lda	$02		;
C957 : 09 01		ora	#$01		;
C959 : 85 02		sta	$02		;
C95B : 18		clc			;
C95C : A9 00		lda	#$00		;
C95E : 69 FF		adc	#$FF		;
C960 : 50 06		bvc	$C968		;
C962 : A5 02		lda	$02		;
C964 : 09 02		ora	#$02		;
C966 : 85 02		sta	$02		;
C968 : 18		clc			;
C969 : A9 00		lda	#$00		;
C96B : 69 01		adc	#$01		;
C96D : 50 06		bvc	$C975		;
C96F : A5 02		lda	$02		;
C971 : 09 04		ora	#$04		;
C973 : 85 02		sta	$02		;
C975 : 18		clc			;
C976 : A9 7F		lda	#$7F		;
C978 : 69 00		adc	#$00		;
C97A : 50 06		bvc	$C982		;
C97C : A5 02		lda	$02		;
C97E : 09 08		ora	#$08		;
C980 : 85 02		sta	$02		;
C982 : 18		clc			;
C983 : A9 80		lda	#$80		;
C985 : 69 00		adc	#$00		;
C987 : 50 06		bvc	$C98F		;
C989 : A5 02		lda	$02		;
C98B : 09 10		ora	#$10		;
C98D : 85 02		sta	$02		;
C98F : 18		clc			;
C990 : A9 7F		lda	#$7F		;
C992 : 69 01		adc	#$01		;
C994 : 70 06		bvs	$C99C		;
C996 : A5 02		lda	$02		;
C998 : 09 20		ora	#$20		;
C99A : 85 02		sta	$02		;
C99C : 18		clc			;
C99D : A9 80		lda	#$80		;
C99F : 69 FF		adc	#$FF		;
C9A1 : 70 06		bvs	$C9A9		;
C9A3 : A5 02		lda	$02		;
C9A5 : 09 40		ora	#$40		;
C9A7 : 85 02		sta	$02		;
C9A9 : 38		sec			;
C9AA : A9 00		lda	#$00		;
C9AC : E9 00		sbc	#$00		;
C9AE : 50 06		bvc	$C9B6		;
C9B0 : A5 03		lda	$03		;
C9B2 : 09 01		ora	#$01		;
C9B4 : 85 03		sta	$03		;
C9B6 : 38		sec			;
C9B7 : A9 FF		lda	#$FF		;
C9B9 : E9 FF		sbc	#$FF		;
C9BB : 50 06		bvc	$C9C3		;
C9BD : A5 03		lda	$03		;
C9BF : 09 02		ora	#$02		;
C9C1 : 85 03		sta	$03		;
C9C3 : 38		sec			;
C9C4 : A9 00		lda	#$00		;
C9C6 : E9 01		sbc	#$01		;
C9C8 : 50 06		bvc	$C9D0		;
C9CA : A5 03		lda	$03		;
C9CC : 09 04		ora	#$04		;
C9CE : 85 03		sta	$03		;
C9D0 : 38		sec			;
C9D1 : A9 7F		lda	#$7F		;
C9D3 : E9 00		sbc	#$00		;
C9D5 : 50 06		bvc	$C9DD		;
C9D7 : A5 03		lda	$03		;
C9D9 : 09 08		ora	#$08		;
C9DB : 85 03		sta	$03		;
C9DD : 38		sec			;
C9DE : A9 80		lda	#$80		;
C9E0 : E9 00		sbc	#$00		;
C9E2 : 50 06		bvc	$C9EA		;
C9E4 : A5 03		lda	$03		;
C9E6 : 09 10		ora	#$10		;
C9E8 : 85 03		sta	$03		;
C9EA : 38		sec			;
C9EB : A9 80		lda	#$80		;
C9ED : E9 01		sbc	#$01		;
C9EF : 70 06		bvs	$C9F7		;
C9F1 : A5 03		lda	$03		;
C9F3 : 09 20		ora	#$20		;
C9F5 : 85 03		sta	$03		;
C9F7 : 38		sec			;
C9F8 : A9 7F		lda	#$7F		;
C9FA : E9 FF		sbc	#$FF		;
C9FC : 70 06		bvs	$CA04		;
C9FE : A5 03		lda	$03		;
CA00 : 09 40		ora	#$40		;
CA02 : 85 03		sta	$03		;
CA04 : 60		rts			;


FFFA : 4C C9		dw	$C94C		;NMI(VBlank)
FFFC : 00 C0		dw	$C000		;Reset
FFFE : 4D C9		dw	$C94D		;IRQ/BRK
