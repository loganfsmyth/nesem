C000 : D8		cld			;
C001 : 78		sei			;
C002 : A2 FF		ldx	#$FF		;
C004 : 9A		txs			;
  C005 : AD 02 20		lda	$2002		;PPU Status Reg.
C008 : 10 FB		bpl	$C005		;
C00A : A2 00		ldx	#$00		;
C00C : 8E 00 20		stx	$2000		;PPU CtrlReg.#1
C00F : 8E 01 20		stx	$2001		;PPU CtrlReg.#2
C012 : A0 06		ldy	#$06		;
C014 : 84 01		sty	$01		;
C016 : A0 00		ldy	#$00		;
C018 : 84 00		sty	$00		;
C01A : A9 40		lda	#$40		;
C01C : 85 10		sta	$10		;
C01E : A9 48		lda	#$48		;
C020 : 85 12		sta	$12		;

; Initialize memory to 0x20
C022 : A9 20		lda	#$20		;
C024 : 85 20		sta	$20		;
  C026 : 91 00		sta	($00),y		;
  C028 : 88		dey			;
  C029 : D0 FB		bne	$C026		;
  C02B : C6 01		dec	$01		;
C02D : 10 F7		bpl	$C026		;

; Initialize Palettes to 0x27
C02F : A2 3F		ldx	#$3F		;
C031 : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C034 : A2 00		ldx	#$00		;
C036 : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C039 : A2 27		ldx	#$27		;
C03B : A0 20		ldy	#$20		;
  C03D : 8E 07 20		stx	$2007		;VRAM I/O Reg.
  C040 : 88		dey			;
C041 : D0 FA		bne	$C03D		;

; Initialize Palettes with 32 bytes starting at 0xC247
C043 : A2 3F		ldx	#$3F		;
C045 : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C048 : A2 00		ldx	#$00		;
C04A : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C04D : A2 00		ldx	#$00		;
  C04F : BD 47 C2		lda	$C247,x		;
  C052 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
  C055 : E8		inx			;
  C056 : E4 20		cpx	$20		;
C058 : D0 F5		bne	$C04F		;


; Initialize Attribute table 0 with 0x40 bytes starting at 0xC267
C05A : A2 23		ldx	#$23		;
C05C : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C05F : A2 C0		ldx	#$C0		;
C061 : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C064 : A2 00		ldx	#$00		;
  C066 : BD 67 C2		lda	$C267,x		;
  C069 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
  C06C : E8		inx			;
  C06D : E4 10		cpx	$10		;
C06F : D0 F5		bne	$C066		;

; Initialize Name table 1

; Set up VRAM Pointer
C071 : A2 00		ldx	#$00		;
C073 : A2 20		ldx	#$20		;
C075 : 8E 06 20		stx	$2006		;VRAM Addr.Reg.
C078 : A2 00		ldx	#$00		;
C07A : 8E 06 20		stx	$2006		;VRAM Addr.Reg.

; Copy 0x100 bytes from 0xC2A7
C07D : A2 00		ldx	#$00		;
C07F : A0 00		ldy	#$00		;
C081 : BD A7 C2		lda	$C2A7,x		;
C084 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C087 : E8		inx			;
C088 : E0 00		cpx	#$00		;
C08A : D0 F5		bne	$C081		;

; Copy 0x100 bytes from 0xC3F7
C08C : A0 00		ldy	#$00		;
C08E : BD F7 C3		lda	$C3F7,x		;
C091 : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C094 : E8		inx			;
C095 : E0 00		cpx	#$00		;
C097 : D0 F5		bne	$C08E		;

; Copy 0x100 bytes from 0xC3F7
C099 : A0 00		ldy	#$00		;
C09B : BD F7 C3		lda	$C3F7,x		;
C09E : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C0A1 : E8		inx			;
C0A2 : E0 00		cpx	#$00		;
C0A4 : D0 F5		bne	$C09B		;

; Copy 0xC0 bytes from 0xC547
C0A6 : A0 00		ldy	#$00		;
C0A8 : BD 47 C5		lda	$C547,x		;
C0AB : 8D 07 20		sta	$2007		;VRAM I/O Reg.
C0AE : E8		inx			;
C0AF : E0 C0		cpx	#$C0		;
C0B1 : D0 F5		bne	$C0A8		;

; Initialize 0x48 bytes from 0xC1FF to Sprite RAM
C0B3 : A2 00		ldx	#$00		;
C0B5 : A9 00		lda	#$00		;
C0B7 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C0BA : BD FF C1		lda	$C1FF,x		;
C0BD : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C0C0 : E8		inx			;
C0C1 : E4 12		cpx	$12		;
C0C3 : D0 F5		bne	$C0BA		;



C0C5 : A2 00		ldx	#$00		;

; Set bit 4,7 => pt 0x1000,  NMI on vblank
C0C7 : A9 90		lda	#$90		;
C0C9 : 8D 00 20		sta	$2000		;PPU CtrlReg.#1

; Set bit 1,2,3,4 => Show background on left 8 px, show sprites on left 8 px, show bg, show sprites
C0CC : A9 1E		lda	#$1E		;
C0CE : 8D 01 20		sta	$2001		;PPU CtrlReg.#2

; $36 = 0xFF
; $70 = 0x05
; $32 = 0xCF
; $34 = 0x65
; $24 = 0xC7
; $28 = 0xC7
; $26 = 0xCF
; $30 = 0xCF
; $38 = 0x01
; $50 = 0xFA
; $64 = 0x30
; $52 = 0x00
; $54 = 0x00
; $56 = 0x00
; $58 = 0x00
; $62 = 0x00
; $72 = 0x00
; $74 = 0x00
C0D1 : A9 FF		lda	#$FF		;
C0D3 : 85 36		sta	$36		;
C0D5 : A9 05		lda	#$05		;
C0D7 : 85 70		sta	$70		;
C0D9 : A9 CF		lda	#$CF		;
C0DB : 85 32		sta	$32		;
C0DD : A9 65		lda	#$65		;
C0DF : 85 34		sta	$34		;
C0E1 : A9 C7		lda	#$C7		;
C0E3 : 85 24		sta	$24		;
C0E5 : 85 28		sta	$28		;
C0E7 : A9 CF		lda	#$CF		;
C0E9 : 85 26		sta	$26		;
C0EB : 85 30		sta	$30		;
C0ED : A9 01		lda	#$01		;
C0EF : 85 38		sta	$38		;
C0F1 : A9 FA		lda	#$FA		;
C0F3 : 85 50		sta	$50		;
C0F5 : A9 30		lda	#$30		;
C0F7 : 85 64		sta	$64		;
C0F9 : A9 00		lda	#$00		;
C0FB : 85 52		sta	$52		;
C0FD : 85 54		sta	$54		;
C0FF : 85 56		sta	$56		;
C101 : 85 58		sta	$58		;
C103 : 85 62		sta	$62		;
C105 : 85 72		sta	$72		;
C107 : 85 74		sta	$74		;

  ; Strobe the controller ports to latch the controller state
  C109 : A9 01		lda	#$01		;
  C10B : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg. Strobe Joypad 1
  C10E : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg. Strobe Joypad 2
  C111 : A9 00		lda	#$00		;
  C113 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg. Unstrobe 1
  C116 : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg. Unstrobe 2

  ; Shift 7 bits out of the controller to get value for the left arrow of 2nd controller to trigger game start.
  C119 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C11C : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C11F : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C122 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C125 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C128 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C12B : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
  C12E : 29 01		and	#$01		;
C130 : F0 D7		beq	$C109		;

; Start of Main Loop
C132 : 20 67 C6		jsr	$C667		;
C135 : 20 D5 C8		jsr	$C8D5		; Sound control?
C138 : 20 0E C8		jsr	$C80E		; Move paddles if LEFT pressed
C13B : 20 D2 C7		jsr	$C7D2		; Update paddle 0 position
C13E : 20 F0 C7		jsr	$C7F0		; Update paddle 1 position
C141 : 20 63 C8		jsr	$C863		; Move paddles if RIGHT pressed
C144 : 20 D2 C7		jsr	$C7D2		; Update paddle 0 position
C147 : 20 F0 C7		jsr	$C7F0		; Update paddle 1 position
C14A : 4C 4D C1		jmp	$C14D		;


; JMP from C14A
C14D : A5 56		lda	$56		;
C14F : C9 00		cmp	#$00		;
C151 : F0 13		beq	$C166		;
  C153 : A9 15		lda	#$15		;
  C155 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
  C158 : A9 07		lda	#$07		;
  C15A : 18		clc			;
  C15B : 65 56		adc	$56		;
  C15D : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C160 : A5 56		lda	$56		;
  C162 : C9 09		cmp	#$09		;
  C164 : F0 1C		beq	$C182		;
C166 : A5 58		lda	$58		;
C168 : C9 00		cmp	#$00		;
C16A : F0 13		beq	$C17F		;
C16C : A9 19		lda	#$19		;
C16E : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C171 : A9 07		lda	#$07		;
C173 : 18		clc			;
C174 : 65 58		adc	$58		;
C176 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C179 : A5 58		lda	$58		;
C17B : C9 09		cmp	#$09		;
C17D : F0 03		beq	$C182		;
  C17F : 4C 32 C1		jmp	$C132		;


C182 : A0 00		ldy	#$00		;
C184 : A2 00		ldx	#$00		;
C186 : A9 01		lda	#$01		;
C188 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C18B : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C18E : A9 00		lda	#$00		;
C190 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C193 : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C196 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C199 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C19C : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C19F : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C1A2 : 29 01		and	#$01		;

C1A4 : F0 09		beq	$C1AF		;
  C1A6 : 4C A9 C1		jmp	$C1A9		; Restart

  C1A9 : 4C 00 C0		jmp	$C000		;
  C1AC : 4C 00 C0		jmp	$C000		;
  
C1AF : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C1B2 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C1B5 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C1B8 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C1BB : 29 01		and	#$01		;
C1BD : F0 03		beq	$C1C2		;
  C1BF : 4C AC C1		jmp	$C1AC		; Restart Game
C1C2 : A9 11		lda	#$11		;
C1C4 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C1C7 : A9 00		lda	#$00		;
C1C9 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C1CC : A9 25		lda	#$25		;
C1CE : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C1D1 : A2 00		ldx	#$00		;
  
  C1D3 : BD F7 C1		lda	$C1F7,x		;
  C1D6 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C1D9 : 18		clc			;
  C1DA : E8		inx			;
  C1DB : 8A		txa			;
  C1DC : 0A		asl	a		;
  C1DD : 0A		asl	a		;
  C1DE : 69 25		adc	#$25		;
  C1E0 : C9 45		cmp	#$45		;
  C1E2 : F0 06		beq	$C1EA		;

    C1E4 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
    C1E7 : 4C D3 C1		jmp	$C1D3		;

C1EA : A9 45		lda	#$45		;
C1EC : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C1EF : A9 17		lda	#$17		;
C1F1 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C1F4 : 4C 82 C1		jmp	$C182		;



; Start of Sprite RAM source block
C1FF : 38		sec			;
C200 : 01 00		ora	($00,x)		;
C202 : C7		db	#$C7		;
C203 : 38		sec			;
C204 : 01 00		ora	($00,x)		;
C206 : CF		db	#$CF		;
C207 : C8		iny			;
C208 : 02		db	#$02		;
C209 : 00		brk			;
C20A : C7		db	#$C7		;
C20B : C8		iny			;
C20C : 02		db	#$02		;
C20D : 00		brk			;
C20E : CF		db	#$CF		;
C20F : 7C		db	#$7C		;
C210 : 03		db	#$03		;
C211 : 00		brk			;
C212 : CF		db	#$CF		;
C213 : 15 07		ora	$07,x		;
C215 : 00		brk			;
C216 : 50 15		bvc	$C22D		;
C218 : 07		db	#$07		;
C219 : 00		brk			;
C21A : C2		db	#$C2		;
C21B : 08		php			;
C21C : 08		php			;
C21D : 10 50		bpl	$C26F		;
C21F : 08		php			;
C220 : 09 20		ora	#$20		;
C222 : C2		db	#$C2		;
C223 : 77		db	#$77		;
C224 : 00		brk			;
C225 : 00		brk			;
C226 : 57		db	#$57		;
C227 : 77		db	#$77		;
C228 : 00		brk			;
C229 : 00		brk			;
C22A : 5F		db	#$5F		;
C22B : 77		db	#$77		;
C22C : 00		brk			;
C22D : 00		brk			;
C22E : 67		db	#$67		;
C22F : 77		db	#$77		;
C230 : 00		brk			;
C231 : 00		brk			;
C232 : 6F		db	#$6F		;
C233 : 77		db	#$77		;
C234 : 00		brk			;
C235 : 00		brk			;
C236 : 77		db	#$77		;
C237 : 77		db	#$77		;
C238 : 00		brk			;
C239 : 00		brk			;
C23A : 7F		db	#$7F		;
C23B : 77		db	#$77		;
C23C : 00		brk			;
C23D : 00		brk			;
C23E : 87		db	#$87		;
C23F : 77		db	#$77		;
C240 : 00		brk			;
C241 : 00		brk			;
C242 : 8F		db	#$8F		;
C243 : 77		db	#$77		;
C244 : 00		brk			;
C245 : 00		brk			;
C246 : 97		db	#$97		;
; End Sprite RAM block


; Start Palette Table
C247 : 2E 30 2D		rol	$2D30		;
C24A : 06 2E		asl	$2E		;
C24C : 22		db	#$22		;
C24D : 04		db	#$04		;
C24E : 14		db	#$14		;
C24F : 2E 05 17		rol	$1705		;
C252 : 24 2E		bit	$2E		;
C254 : 15 27		ora	$27,x		;
C256 : 16 2E		asl	$2E,x		;
C258 : 30 2D		bmi	$C287		;
C25A : 06 2E		asl	$2E		;
C25C : 22		db	#$22		;
C25D : 04		db	#$04		;
C25E : 14		db	#$14		;
C25F : 2E 05 17		rol	$1705		;
C262 : 24 2E		bit	$2E		;
C264 : 15 27		ora	$27,x		;
C266 : 16
; End Palette Table

; Start Attribute table
C267 : 00		brk			;
C268 : 00		brk			;
C269 : 00		brk			;
C26A : 00		brk			;
C26B : 00		brk			;
C26C : 00		brk			;
C26D : 00		brk			;
C26E : 00		brk			;
C26F : 00		brk			;
C270 : 00		brk			;
C271 : 00		brk			;
C272 : 00		brk			;
C273 : 00		brk			;
C274 : 00		brk			;
C275 : 00		brk			;
C276 : 00		brk			;
C277 : 00		brk			;
C278 : 00		brk			;
C279 : 00		brk			;
C27A : 00		brk			;
C27B : 00		brk			;
C27C : 00		brk			;
C27D : 00		brk			;
C27E : 00		brk			;
C27F : 00		brk			;
C280 : 00		brk			;
C281 : 00		brk			;
C282 : 00		brk			;
C283 : 00		brk			;
C284 : 00		brk			;
C285 : 00		brk			;
C286 : 00		brk			;
C287 : 00		brk			;
C288 : 00		brk			;
C289 : 00		brk			;
C28A : 00		brk			;
C28B : 00		brk			;
C28C : 00		brk			;
C28D : 00		brk			;
C28E : 00		brk			;
C28F : 00		brk			;
C290 : 00		brk			;
C291 : 00		brk			;
C292 : 00		brk			;
C293 : 00		brk			;
C294 : 00		brk			;
C295 : 00		brk			;
C296 : 00		brk			;
C297 : 00		brk			;
C298 : 00		brk			;
C299 : 00		brk			;
C29A : 00		brk			;
C29B : 00		brk			;
C29C : 00		brk			;
C29D : 00		brk			;
C29E : 00		brk			;
C29F : 00		brk			;
C2A0 : 00		brk			;
C2A1 : 00		brk			;
C2A2 : 00		brk			;
C2A3 : 00		brk			;
C2A4 : 00		brk			;
C2A5 : 00		brk			;
C2A6 : 00		brk			;
; End Attribute Table

; First 0x100 For Name table
C2A7 : 00		brk			;
C2A8 : 00		brk			;
C2A9 : 00		brk			;
C2AA : 00		brk			;
C2AB : 00		brk			;
C2AC : 00		brk			;
C2AD : 00		brk			;
C2AE : 00		brk			;
C2AF : 00		brk			;
C2B0 : 00		brk			;
C2B1 : 00		brk			;
C2B2 : 00		brk			;
C2B3 : 00		brk			;
C2B4 : 00		brk			;
C2B5 : 00		brk			;
C2B6 : 00		brk			;
C2B7 : 00		brk			;
C2B8 : 00		brk			;
C2B9 : 00		brk			;
C2BA : 00		brk			;
C2BB : 00		brk			;
C2BC : 00		brk			;
C2BD : 00		brk			;
C2BE : 00		brk			;
C2BF : 00		brk			;
C2C0 : 00		brk			;
C2C1 : 00		brk			;
C2C2 : 00		brk			;
C2C3 : 00		brk			;
C2C4 : 00		brk			;
C2C5 : 00		brk			;
C2C6 : 00		brk			;
C2C7 : 00		brk			;
C2C8 : 00		brk			;
C2C9 : 00		brk			;
C2CA : 00		brk			;
C2CB : 00		brk			;
C2CC : 00		brk			;
C2CD : 00		brk			;
C2CE : 0C		db	#$0C		;
C2CF : 0D 0E 00		ora	$000E		;
C2D2 : 00		brk			;
C2D3 : 00		brk			;
C2D4 : 00		brk			;
C2D5 : 00		brk			;
C2D6 : 00		brk			;
C2D7 : 00		brk			;
C2D8 : 00		brk			;
C2D9 : 00		brk			;
C2DA : 00		brk			;
C2DB : 06 0C		asl	$0C		;
C2DD : 0D 0E 00		ora	$000E		;
C2E0 : 00		brk			;
C2E1 : 00		brk			;
C2E2 : 00		brk			;
C2E3 : 00		brk			;
C2E4 : 00		brk			;
C2E5 : 00		brk			;
C2E6 : 00		brk			;
C2E7 : 00		brk			;
C2E8 : 00		brk			;
C2E9 : 00		brk			;
C2EA : 00		brk			;
C2EB : 00		brk			;
C2EC : 00		brk			;
C2ED : 00		brk			;
C2EE : 10 0F		bpl	$C2FF		;
C2F0 : 0F		db	#$0F		;
C2F1 : 11 00		ora	($00),y		;
C2F3 : 00		brk			;
C2F4 : 06 00		asl	$00		;
C2F6 : 00		brk			;
C2F7 : 00		brk			;
C2F8 : 00		brk			;
C2F9 : 00		brk			;
C2FA : 00		brk			;
C2FB : 00		brk			;
C2FC : 10 0F		bpl	$C30D		;
C2FE : 0F		db	#$0F		;
C2FF : 11 00		ora	($00),y		;
C301 : 00		brk			;
C302 : 00		brk			;
C303 : 00		brk			;
C304 : 00		brk			;
C305 : 00		brk			;
C306 : 00		brk			;
C307 : 00		brk			;
C308 : 00		brk			;
C309 : 00		brk			;
C30A : 00		brk			;
C30B : 00		brk			;
C30C : 00		brk			;
C30D : 00		brk			;
C30E : 00		brk			;
C30F : 00		brk			;
C310 : 00		brk			;
C311 : 00		brk			;
C312 : 00		brk			;
C313 : 00		brk			;
C314 : 00		brk			;
C315 : 00		brk			;
C316 : 00		brk			;
C317 : 00		brk			;
C318 : 00		brk			;
C319 : 00		brk			;
C31A : 00		brk			;
C31B : 00		brk			;
C31C : 00		brk			;
C31D : 00		brk			;
C31E : 00		brk			;
C31F : 06 00		asl	$00		;
C321 : 00		brk			;
C322 : 00		brk			;
C323 : 00		brk			;
C324 : 00		brk			;
C325 : 00		brk			;
C326 : 06 00		asl	$00		;
C328 : 00		brk			;
C329 : 00		brk			;
C32A : 00		brk			;
C32B : 00		brk			;
C32C : 00		brk			;
C32D : 00		brk			;
C32E : 00		brk			;
C32F : 00		brk			;
C330 : 00		brk			;
C331 : 00		brk			;
C332 : 00		brk			;
C333 : 00		brk			;
C334 : 00		brk			;
C335 : 00		brk			;
C336 : 00		brk			;
C337 : 00		brk			;
C338 : 06 00		asl	$00		;
C33A : 00		brk			;
C33B : 00		brk			;
C33C : 00		brk			;
C33D : 00		brk			;
C33E : 00		brk			;
C33F : 00		brk			;
C340 : 00		brk			;
C341 : 00		brk			;
C342 : 00		brk			;
C343 : 00		brk			;
C344 : 00		brk			;
C345 : 00		brk			;
C346 : 00		brk			;
C347 : 00		brk			;
C348 : 00		brk			;
C349 : 00		brk			;
C34A : 00		brk			;
C34B : 00		brk			;
C34C : 06 00		asl	$00		;
C34E : 00		brk			;
C34F : 00		brk			;
C350 : 00		brk			;
C351 : 00		brk			;
C352 : 00		brk			;
C353 : 00		brk			;
C354 : 00		brk			;
C355 : 00		brk			;
C356 : 00		brk			;
C357 : 00		brk			;
C358 : 00		brk			;
C359 : 00		brk			;
C35A : 00		brk			;
C35B : 00		brk			;
C35C : 00		brk			;
C35D : 00		brk			;
C35E : 00		brk			;
C35F : 00		brk			;
C360 : 00		brk			;
C361 : 06 00		asl	$00		;
C363 : 00		brk			;
C364 : 00		brk			;
C365 : 00		brk			;
C366 : 00		brk			;
C367 : 06 00		asl	$00		;
C369 : 00		brk			;
C36A : 00		brk			;
C36B : 00		brk			;
C36C : 00		brk			;
C36D : 00		brk			;
C36E : 06 00		asl	$00		;
C370 : 00		brk			;
C371 : 00		brk			;
C372 : 00		brk			;
C373 : 00		brk			;
C374 : 00		brk			;
C375 : 00		brk			;
C376 : 00		brk			;
C377 : 00		brk			;
C378 : 00		brk			;
C379 : 00		brk			;
C37A : 00		brk			;
C37B : 00		brk			;
C37C : 00		brk			;
C37D : 00		brk			;
C37E : 00		brk			;
C37F : 00		brk			;
C380 : 06 00		asl	$00		;
C382 : 00		brk			;
C383 : 00		brk			;
C384 : 00		brk			;
C385 : 00		brk			;
C386 : 00		brk			;
C387 : 00		brk			;
C388 : 00		brk			;
C389 : 00		brk			;
C38A : 00		brk			;
C38B : 00		brk			;
C38C : 00		brk			;
C38D : 00		brk			;
C38E : 00		brk			;
C38F : 00		brk			;
C390 : 00		brk			;
C391 : 00		brk			;
C392 : 00		brk			;
C393 : 00		brk			;
C394 : 06 00		asl	$00		;
C396 : 00		brk			;
C397 : 00		brk			;
C398 : 00		brk			;
C399 : 00		brk			;
C39A : 00		brk			;
C39B : 00		brk			;
C39C : 00		brk			;
C39D : 00		brk			;
C39E : 00		brk			;
C39F : 00		brk			;
C3A0 : 00		brk			;
C3A1 : 00		brk			;
C3A2 : 00		brk			;
C3A3 : 00		brk			;
C3A4 : 00		brk			;
C3A5 : 00		brk			;
C3A6 : 00		brk			;
; End first 0x100 for name table

C3A7 : 00		brk			;
C3A8 : 00		brk			;
C3A9 : 06 00		asl	$00		;
C3AB : 00		brk			;
C3AC : 00		brk			;
C3AD : 00		brk			;
C3AE : 00		brk			;
C3AF : 06 00		asl	$00		;
C3B1 : 00		brk			;
C3B2 : 00		brk			;
C3B3 : 00		brk			;
C3B4 : 00		brk			;
C3B5 : 00		brk			;
C3B6 : 06 00		asl	$00		;
C3B8 : 00		brk			;
C3B9 : 00		brk			;
C3BA : 00		brk			;
C3BB : 00		brk			;
C3BC : 00		brk			;
C3BD : 00		brk			;
C3BE : 00		brk			;
C3BF : 00		brk			;
C3C0 : 00		brk			;
C3C1 : 00		brk			;
C3C2 : 00		brk			;
C3C3 : 00		brk			;
C3C4 : 00		brk			;
C3C5 : 00		brk			;
C3C6 : 00		brk			;
C3C7 : 00		brk			;
C3C8 : 06 00		asl	$00		;
C3CA : 00		brk			;
C3CB : 00		brk			;
C3CC : 00		brk			;
C3CD : 00		brk			;
C3CE : 00		brk			;
C3CF : 00		brk			;
C3D0 : 00		brk			;
C3D1 : 00		brk			;
C3D2 : 06 00		asl	$00		;
C3D4 : 00		brk			;
C3D5 : 00		brk			;
C3D6 : 00		brk			;
C3D7 : 00		brk			;
C3D8 : 00		brk			;
C3D9 : 00		brk			;
C3DA : 00		brk			;
C3DB : 00		brk			;
C3DC : 06 00		asl	$00		;
C3DE : 00		brk			;
C3DF : 00		brk			;
C3E0 : 00		brk			;
C3E1 : 00		brk			;
C3E2 : 00		brk			;
C3E3 : 00		brk			;
C3E4 : 00		brk			;
C3E5 : 00		brk			;
C3E6 : 00		brk			;
C3E7 : 00		brk			;
C3E8 : 00		brk			;
C3E9 : 00		brk			;
C3EA : 00		brk			;
C3EB : 00		brk			;
C3EC : 00		brk			;
C3ED : 00		brk			;
C3EE : 00		brk			;
C3EF : 00		brk			;
C3F0 : 00		brk			;
C3F1 : 00		brk			;
C3F2 : 00		brk			;
C3F3 : 00		brk			;
C3F4 : 00		brk			;
C3F5 : 00		brk			;
C3F6 : 00		brk			;


; Start second and third 0x100 for Name table
C3F7 : 00		brk			;
C3F8 : 00		brk			;
C3F9 : 00		brk			;
C3FA : 00		brk			;
C3FB : 00		brk			;
C3FC : 00		brk			;
C3FD : 00		brk			;
C3FE : 00		brk			;
C3FF : 00		brk			;
C400 : 00		brk			;
C401 : 00		brk			;
C402 : 00		brk			;
C403 : 00		brk			;
C404 : 00		brk			;
C405 : 00		brk			;
C406 : 06 00		asl	$00		;
C408 : 00		brk			;
C409 : 00		brk			;
C40A : 00		brk			;
C40B : 00		brk			;
C40C : 00		brk			;
C40D : 00		brk			;
C40E : 00		brk			;
C40F : 00		brk			;
C410 : 06 00		asl	$00		;
C412 : 00		brk			;
C413 : 00		brk			;
C414 : 00		brk			;
C415 : 00		brk			;
C416 : 00		brk			;
C417 : 00		brk			;
C418 : 06 00		asl	$00		;
C41A : 00		brk			;
C41B : 00		brk			;
C41C : 00		brk			;
C41D : 00		brk			;
C41E : 00		brk			;
C41F : 00		brk			;
C420 : 00		brk			;
C421 : 00		brk			;
C422 : 00		brk			;
C423 : 00		brk			;
C424 : 00		brk			;
C425 : 00		brk			;
C426 : 00		brk			;
C427 : 00		brk			;
C428 : 00		brk			;
C429 : 00		brk			;
C42A : 00		brk			;
C42B : 00		brk			;
C42C : 00		brk			;
C42D : 00		brk			;
C42E : 00		brk			;
C42F : 00		brk			;
C430 : 00		brk			;
C431 : 00		brk			;
C432 : 00		brk			;
C433 : 00		brk			;
C434 : 00		brk			;
C435 : 00		brk			;
C436 : 00		brk			;
C437 : 00		brk			;
C438 : 00		brk			;
C439 : 00		brk			;
C43A : 00		brk			;
C43B : 00		brk			;
C43C : 00		brk			;
C43D : 00		brk			;
C43E : 00		brk			;
C43F : 00		brk			;
C440 : 00		brk			;
C441 : 06 00		asl	$00		;
C443 : 00		brk			;
C444 : 00		brk			;
C445 : 00		brk			;
C446 : 00		brk			;
C447 : 00		brk			;
C448 : 00		brk			;
C449 : 00		brk			;
C44A : 00		brk			;
C44B : 06 00		asl	$00		;
C44D : 00		brk			;
C44E : 06 00		asl	$00		;
C450 : 00		brk			;
C451 : 00		brk			;
C452 : 00		brk			;
C453 : 00		brk			;
C454 : 00		brk			;
C455 : 00		brk			;
C456 : 00		brk			;
C457 : 00		brk			;
C458 : 00		brk			;
C459 : 00		brk			;
C45A : 00		brk			;
C45B : 00		brk			;
C45C : 00		brk			;
C45D : 00		brk			;
C45E : 00		brk			;
C45F : 00		brk			;
C460 : 06 00		asl	$00		;
C462 : 00		brk			;
C463 : 00		brk			;
C464 : 00		brk			;
C465 : 00		brk			;
C466 : 00		brk			;
C467 : 00		brk			;
C468 : 00		brk			;
C469 : 00		brk			;
C46A : 00		brk			;
C46B : 00		brk			;
C46C : 00		brk			;
C46D : 00		brk			;
C46E : 00		brk			;
C46F : 00		brk			;
C470 : 00		brk			;
C471 : 00		brk			;
C472 : 06 00		asl	$00		;
C474 : 00		brk			;
C475 : 00		brk			;
C476 : 00		brk			;
C477 : 00		brk			;
C478 : 00		brk			;
C479 : 00		brk			;
C47A : 00		brk			;
C47B : 00		brk			;
C47C : 00		brk			;
C47D : 00		brk			;
C47E : 00		brk			;
C47F : 00		brk			;
C480 : 00		brk			;
C481 : 00		brk			;
C482 : 00		brk			;
C483 : 00		brk			;
C484 : 00		brk			;
C485 : 06 00		asl	$00		;
C487 : 00		brk			;
C488 : 00		brk			;
C489 : 06 00		asl	$00		;
C48B : 00		brk			;
C48C : 00		brk			;
C48D : 00		brk			;
C48E : 00		brk			;
C48F : 06 00		asl	$00		;
C491 : 00		brk			;
C492 : 00		brk			;
C493 : 00		brk			;
C494 : 00		brk			;
C495 : 00		brk			;
C496 : 00		brk			;
C497 : 00		brk			;
C498 : 00		brk			;
C499 : 00		brk			;
C49A : 00		brk			;
C49B : 00		brk			;
C49C : 00		brk			;
C49D : 00		brk			;
C49E : 00		brk			;
C49F : 00		brk			;
C4A0 : 00		brk			;
C4A1 : 00		brk			;
C4A2 : 00		brk			;
C4A3 : 00		brk			;
C4A4 : 00		brk			;
C4A5 : 00		brk			;
C4A6 : 00		brk			;
C4A7 : 00		brk			;
C4A8 : 06 00		asl	$00		;
C4AA : 00		brk			;
C4AB : 00		brk			;
C4AC : 00		brk			;
C4AD : 00		brk			;
C4AE : 00		brk			;
C4AF : 00		brk			;
C4B0 : 00		brk			;
C4B1 : 00		brk			;
C4B2 : 00		brk			;
C4B3 : 00		brk			;
C4B4 : 00		brk			;
C4B5 : 00		brk			;
C4B6 : 00		brk			;
C4B7 : 00		brk			;
C4B8 : 00		brk			;
C4B9 : 00		brk			;
C4BA : 00		brk			;
C4BB : 00		brk			;
C4BC : 06 00		asl	$00		;
C4BE : 00		brk			;
C4BF : 00		brk			;
C4C0 : 00		brk			;
C4C1 : 00		brk			;
C4C2 : 00		brk			;
C4C3 : 00		brk			;
C4C4 : 00		brk			;
C4C5 : 00		brk			;
C4C6 : 00		brk			;
C4C7 : 00		brk			;
C4C8 : 00		brk			;
C4C9 : 00		brk			;
C4CA : 00		brk			;
C4CB : 00		brk			;
C4CC : 00		brk			;
C4CD : 00		brk			;
C4CE : 00		brk			;
C4CF : 00		brk			;
C4D0 : 00		brk			;
C4D1 : 06 00		asl	$00		;
C4D3 : 00		brk			;
C4D4 : 00		brk			;
C4D5 : 00		brk			;
C4D6 : 00		brk			;
C4D7 : 00		brk			;
C4D8 : 00		brk			;
C4D9 : 00		brk			;
C4DA : 00		brk			;
C4DB : 00		brk			;
C4DC : 06 00		asl	$00		;
C4DE : 00		brk			;
C4DF : 00		brk			;
C4E0 : 00		brk			;
C4E1 : 00		brk			;
C4E2 : 00		brk			;
C4E3 : 00		brk			;
C4E4 : 00		brk			;
C4E5 : 00		brk			;
C4E6 : 00		brk			;
C4E7 : 00		brk			;
C4E8 : 00		brk			;
C4E9 : 00		brk			;
C4EA : 00		brk			;
C4EB : 00		brk			;
C4EC : 00		brk			;
C4ED : 00		brk			;
C4EE : 00		brk			;
C4EF : 00		brk			;
C4F0 : 00		brk			;
C4F1 : 06 00		asl	$00		;
C4F3 : 00		brk			;
C4F4 : 00		brk			;
C4F5 : 00		brk			;
C4F6 : 00		brk			;
; End second and third 0x100 for name table


C4F7 : 06 00		asl	$00		;
C4F9 : 00		brk			;
C4FA : 00		brk			;
C4FB : 00		brk			;
C4FC : 00		brk			;
C4FD : 00		brk			;
C4FE : 06 00		asl	$00		;
C500 : 00		brk			;
C501 : 00		brk			;
C502 : 06 00		asl	$00		;
C504 : 00		brk			;
C505 : 00		brk			;
C506 : 00		brk			;
C507 : 00		brk			;
C508 : 00		brk			;
C509 : 00		brk			;
C50A : 00		brk			;
C50B : 00		brk			;
C50C : 00		brk			;
C50D : 00		brk			;
C50E : 00		brk			;
C50F : 00		brk			;
C510 : 00		brk			;
C511 : 00		brk			;
C512 : 00		brk			;
C513 : 00		brk			;
C514 : 00		brk			;
C515 : 00		brk			;
C516 : 00		brk			;
C517 : 00		brk			;
C518 : 00		brk			;
C519 : 00		brk			;
C51A : 00		brk			;
C51B : 00		brk			;
C51C : 00		brk			;
C51D : 00		brk			;
C51E : 00		brk			;
C51F : 00		brk			;
C520 : 00		brk			;
C521 : 00		brk			;
C522 : 00		brk			;
C523 : 00		brk			;
C524 : 00		brk			;
C525 : 00		brk			;
C526 : 00		brk			;
C527 : 00		brk			;
C528 : 00		brk			;
C529 : 00		brk			;
C52A : 00		brk			;
C52B : 00		brk			;
C52C : 00		brk			;
C52D : 00		brk			;
C52E : 00		brk			;
C52F : 00		brk			;
C530 : 00		brk			;
C531 : 00		brk			;
C532 : 00		brk			;
C533 : 06 00		asl	$00		;
C535 : 00		brk			;
C536 : 00		brk			;
C537 : 00		brk			;
C538 : 00		brk			;
C539 : 06 00		asl	$00		;
C53B : 00		brk			;
C53C : 00		brk			;
C53D : 00		brk			;
C53E : 00		brk			;
C53F : 00		brk			;
C540 : 00		brk			;
C541 : 00		brk			;
C542 : 00		brk			;
C543 : 00		brk			;
C544 : 00		brk			;
C545 : 00		brk			;
C546 : 06

; Start last 0xC0 of the name table
C547 : 00		brk			;
C548 : 00		brk			;
C549 : 00		brk			;
C54A : 00		brk			;
C54B : 00		brk			;
C54C : 00		brk			;
C54D : 00		brk			;
C54E : 00		brk			;
C54F : 00		brk			;
C550 : 00		brk			;
C551 : 00		brk			;
C552 : 00		brk			;
C553 : 00		brk			;
C554 : 00		brk			;
C555 : 00		brk			;
C556 : 00		brk			;
C557 : 00		brk			;
C558 : 06 00		asl	$00		;
C55A : 00		brk			;
C55B : 00		brk			;
C55C : 00		brk			;
C55D : 00		brk			;
C55E : 00		brk			;
C55F : 00		brk			;
C560 : 00		brk			;
C561 : 00		brk			;
C562 : 00		brk			;
C563 : 00		brk			;
C564 : 00		brk			;
C565 : 00		brk			;
C566 : 00		brk			;
C567 : 06 00		asl	$00		;
C569 : 00		brk			;
C56A : 00		brk			;
C56B : 00		brk			;
C56C : 00		brk			;
C56D : 00		brk			;
C56E : 00		brk			;
C56F : 00		brk			;
C570 : 00		brk			;
C571 : 00		brk			;
C572 : 00		brk			;
C573 : 00		brk			;
C574 : 00		brk			;
C575 : 00		brk			;
C576 : 00		brk			;
C577 : 00		brk			;
C578 : 00		brk			;
C579 : 00		brk			;
C57A : 00		brk			;
C57B : 00		brk			;
C57C : 00		brk			;
C57D : 00		brk			;
C57E : 00		brk			;
C57F : 00		brk			;
C580 : 00		brk			;
C581 : 06 00		asl	$00		;
C583 : 00		brk			;
C584 : 00		brk			;
C585 : 00		brk			;
C586 : 00		brk			;
C587 : 00		brk			;
C588 : 00		brk			;
C589 : 00		brk			;
C58A : 00		brk			;
C58B : 00		brk			;
C58C : 00		brk			;
C58D : 00		brk			;
C58E : 00		brk			;
C58F : 00		brk			;
C590 : 00		brk			;
C591 : 00		brk			;
C592 : 00		brk			;
C593 : 00		brk			;
C594 : 00		brk			;
C595 : 00		brk			;
C596 : 06 00		asl	$00		;
C598 : 00		brk			;
C599 : 00		brk			;
C59A : 00		brk			;
C59B : 00		brk			;
C59C : 00		brk			;
C59D : 00		brk			;
C59E : 00		brk			;
C59F : 00		brk			;
C5A0 : 00		brk			;
C5A1 : 00		brk			;
C5A2 : 06 00		asl	$00		;
C5A4 : 00		brk			;
C5A5 : 00		brk			;
C5A6 : 00		brk			;
C5A7 : 00		brk			;
C5A8 : 06 00		asl	$00		;
C5AA : 00		brk			;
C5AB : 00		brk			;
C5AC : 00		brk			;
C5AD : 00		brk			;
C5AE : 00		brk			;
C5AF : 00		brk			;
C5B0 : 00		brk			;
C5B1 : 00		brk			;
C5B2 : 00		brk			;
C5B3 : 00		brk			;
C5B4 : 00		brk			;
C5B5 : 00		brk			;
C5B6 : 00		brk			;
C5B7 : 00		brk			;
C5B8 : 00		brk			;
C5B9 : 00		brk			;
C5BA : 00		brk			;
C5BB : 00		brk			;
C5BC : 06 00		asl	$00		;
C5BE : 00		brk			;
C5BF : 00		brk			;
C5C0 : 00		brk			;
C5C1 : 00		brk			;
C5C2 : 00		brk			;
C5C3 : 00		brk			;
C5C4 : 00		brk			;
C5C5 : 00		brk			;
C5C6 : 00		brk			;
C5C7 : 00		brk			;
C5C8 : 00		brk			;
C5C9 : 00		brk			;
C5CA : 00		brk			;
C5CB : 00		brk			;
C5CC : 00		brk			;
C5CD : 00		brk			;
C5CE : 00		brk			;
C5CF : 00		brk			;
C5D0 : 00		brk			;
C5D1 : 06 00		asl	$00		;
C5D3 : 00		brk			;
C5D4 : 00		brk			;
C5D5 : 00		brk			;
C5D6 : 00		brk			;
C5D7 : 06 00		asl	$00		;
C5D9 : 00		brk			;
C5DA : 00		brk			;
C5DB : 00		brk			;
C5DC : 00		brk			;
C5DD : 00		brk			;
C5DE : 06 00		asl	$00		;
C5E0 : 00		brk			;
C5E1 : 00		brk			;
C5E2 : 00		brk			;
C5E3 : 00		brk			;
C5E4 : 00		brk			;
C5E5 : 00		brk			;
C5E6 : 00		brk			;
C5E7 : 00		brk			;
C5E8 : 00		brk			;
C5E9 : 00		brk			;
C5EA : 00		brk			;
C5EB : 00		brk			;
C5EC : 00		brk			;
C5ED : 00		brk			;
C5EE : 00		brk			;
C5EF : 00		brk			;
C5F0 : 00		brk			;
C5F1 : 00		brk			;
C5F2 : 00		brk			;
C5F3 : 00		brk			;
C5F4 : 00		brk			;
C5F5 : 00		brk			;
C5F6 : 00		brk			;
C5F7 : 00		brk			;
C5F8 : 00		brk			;
C5F9 : 00		brk			;
C5FA : 06 00		asl	$00		;
C5FC : 00		brk			;
C5FD : 00		brk			;
C5FE : 00		brk			;
C5FF : 00		brk			;
C600 : 00		brk			;
C601 : 00		brk			;
C602 : 00		brk			;
C603 : 00		brk			;
C604 : 00		brk			;
C605 : 00		brk			;
C606 : 00		brk			;
; End last 0xC0 of the name table

C607 : 00		brk			;
C608 : 00		brk			;
C609 : 00		brk			;
C60A : 00		brk			;
C60B : 00		brk			;
C60C : 00		brk			;
C60D : 00		brk			;
C60E : 00		brk			;
C60F : 00		brk			;
C610 : 00		brk			;
C611 : 00		brk			;
C612 : 00		brk			;
C613 : 00		brk			;
C614 : 00		brk			;
C615 : 00		brk			;
C616 : 00		brk			;
C617 : 00		brk			;
C618 : 00		brk			;
C619 : 06 00		asl	$00		;
C61B : 00		brk			;
C61C : 00		brk			;
C61D : 00		brk			;
C61E : 00		brk			;
C61F : 06 00		asl	$00		;
C621 : 00		brk			;
C622 : 00		brk			;
C623 : 00		brk			;
C624 : 00		brk			;
C625 : 00		brk			;
C626 : 06 00		asl	$00		;
C628 : 00		brk			;
C629 : 00		brk			;
C62A : 00		brk			;
C62B : 00		brk			;
C62C : 00		brk			;
C62D : 00		brk			;
C62E : 00		brk			;
C62F : 00		brk			;
C630 : 00		brk			;
C631 : 00		brk			;
C632 : 00		brk			;
C633 : 00		brk			;
C634 : 00		brk			;
C635 : 00		brk			;
C636 : 00		brk			;
C637 : 00		brk			;
C638 : 00		brk			;
C639 : 00		brk			;
C63A : 00		brk			;
C63B : 00		brk			;
C63C : 00		brk			;
C63D : 00		brk			;
C63E : 00		brk			;
C63F : 00		brk			;
C640 : 00		brk			;
C641 : 00		brk			;
C642 : 00		brk			;
C643 : 00		brk			;
C644 : 00		brk			;
C645 : 00		brk			;
C646 : 00		brk			;
C647 : 00		brk			;
C648 : 00		brk			;
C649 : 00		brk			;
C64A : 06 00		asl	$00		;
C64C : 00		brk			;
C64D : 00		brk			;
C64E : 00		brk			;
C64F : 00		brk			;
C650 : 00		brk			;
C651 : 00		brk			;
C652 : 00		brk			;
C653 : 00		brk			;
C654 : 00		brk			;
C655 : 00		brk			;
C656 : 00		brk			;
C657 : 00		brk			;
C658 : 00		brk			;
C659 : 00		brk			;
C65A : 06 00		asl	$00		;
C65C : 00		brk			;
C65D : 00		brk			;
C65E : 00		brk			;
C65F : 00		brk			;
C660 : 00		brk			;
C661 : 00		brk			;
C662 : 00		brk			;
C663 : 00		brk			;
C664 : 06 00		asl	$00		;
C666 : 00		brk			;

; Function call
C667 : 4C B8 C6		jmp	$C6B8		;

; Break from C6A8
C66A : A9 01		lda	#$01		;
C66C : 85 54		sta	$54		;
C66E : A5 36		lda	$36		;
C670 : C9 01		cmp	#$01		;
C672 : F0 3E		beq	$C6B2		;
C674 : 38		sec			;
C675 : A5 36		lda	$36		;
C677 : E5 70		sbc	$70		;
C679 : 85 36		sta	$36		;
C67B : 4C B2 C6		jmp	$C6B2		;
C67E : A2 00		ldx	#$00		;
C680 : A9 00		lda	#$00		;
C682 : 38		sec			;
C683 : A5 24		lda	$24		;
C685 : E9 06		sbc	#$06		;
C687 : 18		clc			;
C688 : 86 60		stx	$60		;
C68A : 65 60		adc	$60		;
C68C : C5 32		cmp	$32		;
C68E : F0 79		beq	$C709		;
C690 : E8		inx			;
C691 : E0 18		cpx	#$18		;
C693 : D0 ED		bne	$C682		;
C695 : 4C D3 C6		jmp	$C6D3		;
C698 : A2 00		ldx	#$00		;
C69A : A9 00		lda	#$00		;
C69C : 38		sec			;
C69D : A5 28		lda	$28		;
C69F : E9 06		sbc	#$06		;
C6A1 : 18		clc			;
C6A2 : 86 60		stx	$60		;
C6A4 : 65 60		adc	$60		;
C6A6 : C5 32		cmp	$32		;
C6A8 : F0 C0		beq	$C66A		;
C6AA : E8		inx			;
C6AB : E0 18		cpx	#$18		;
C6AD : D0 ED		bne	$C69C		;
C6AF : 4C D3 C6		jmp	$C6D3		;

; Break from all over
C6B2 : 20 94 C7 	jsr	$C794		;
C6B5 : 4C 62 C7		jmp	$C762		;

; JMP from C667 and others
C6B8 : A9 34		lda	#$34		;
C6BA : C5 34		cmp	$34		;
C6BC : F0 6D		beq	$C72B		;
C6BE : A9 3B		lda	#$3B		;
C6C0 : C5 34		cmp	$34		;
C6C2 : F0 BA		beq	$C67E		;
C6C4 : A9 C4		lda	#$C4		;
C6C6 : C5 34		cmp	$34		;
C6C8 : F0 CE		beq	$C698		;
C6CA : A9 CB		lda	#$CB		;
C6CC : C5 34		cmp	$34		;
C6CE : F0 75		beq	$C745		;
C6D0 : 4C D3 C6		jmp	$C6D3		;

C6D3 : A9 00		lda	#$00		;
C6D5 : C5 32		cmp	$32		;
C6D7 : D0 03		bne	$C6DC		;
C6D9 : 20 80 C7		jsr	$C780		; Call
C6DC : A5 64		lda	$64		;
C6DE : C5 34		cmp	$34		;
C6E0 : D0 09		bne	$C6EB		;
C6E2 : E6 58		inc	$58		;
C6E4 : A9 01		lda	#$01		;
C6E6 : 85 72		sta	$72		;
C6E8 : 20 85 C7		jsr	$C785		; Call
C6EB : A5 50		lda	$50		;
C6ED : 38		sec			;
C6EE : E9 06		sbc	#$06		;
C6F0 : C5 32		cmp	$32		;
C6F2 : D0 03		bne	$C6F7		;
C6F4 : 20 8A C7		jsr	$C78A		; Call
C6F7 : A9 D6		lda	#$D6		;
C6F9 : C5 34		cmp	$34		;
C6FB : D0 09		bne	$C706		;
C6FD : E6 56		inc	$56		;
C6FF : A9 01		lda	#$01		;
C701 : 85 72		sta	$72		;
C703 : 20 8F C7		jsr	$C78F		; Call
C706 : 4C B2 C6		jmp	$C6B2		;

; Break from C68E
C709 : A9 00		lda	#$00		;
C70B : 85 54		sta	$54		;
C70D : A5 36		lda	$36		;
C70F : C9 01		cmp	#$01		;
C711 : F0 9F		beq	$C6B2		; Break to above
C713 : 38		sec			;
C714 : A5 36		lda	$36		;
C716 : E5 70		sbc	$70		;
C718 : 85 36		sta	$36		;
C71A : 4C B2 C6		jmp	$C6B2		; Break to above

C71D : A9 01		lda	#$01		;
C71F : 85 54		sta	$54		;
C721 : 4C B2 C6		jmp	$C6B2		; Break to above

C724 : A9 00		lda	#$00		;
C726 : 85 54		sta	$54		;
C728 : 4C 5F C7		jmp	$C75F		; Animate ball


C72B : A2 00		ldx	#$00		;
C72D : A9 00		lda	#$00		;
  C72F : 38		sec			;
  C730 : A5 24		lda	$24		;
  C732 : E9 06		sbc	#$06		;
  C734 : 18		clc			;
  C735 : 86 60		stx	$60		;
  C737 : 65 60		adc	$60		;
  C739 : C5 32		cmp	$32		;
  C73B : F0 E0		beq	$C71D		;  $54 = 0x01 to C75F
  C73D : E8		inx			;
  C73E : E0 18		cpx	#$18		;
  C740 : D0 ED		bne	$C72F		;
C742 : 4C D3 C6		jmp	$C6D3		;


do {
  x = 0
  a = 0
  
  a = $28
  a = a - 0x06 - 1

  $60 = x
  a = a + $60


} 
C745 : A2 00		ldx	#$00		;
C747 : A9 00		lda	#$00		;
  C749 : 38		sec			;
  C74A : A5 28		lda	$28		;
  C74C : E9 06		sbc	#$06		;
  C74E : 18		clc			;
  C74F : 86 60		stx	$60		;
  C751 : 65 60		adc	$60		;
  C753 : C5 32		cmp	$32		;
  C755 : F0 CD		beq	$C724		; $54 = 0x00 to C75F
  C757 : E8		inx			;
  C758 : E0 18		cpx	#$18		;
  C75A : D0 ED		bne	$C749		;
C75C : 4C D3 C6		jmp	$C6D3		;

; Ball animation
; animate_ball($52, %54, $36)  ; $52 = 
; Call C794 which updates $32 and $34
; 
; y = 0x00
; for (x = 0; x < $36; x++) {
;   $2003 = 0x13
;   $2004 = $32
; 
;   $2003 = 0x10
;   $2004 = $34
; }
;
C75F : 20 94 C7		jsr	$C794		; Call
C762 : A2 00		ldx	#$00		;
C764 : A0 00		ldy	#$00		;
C766 : A9 13		lda	#$13		;
C768 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C76B : A5 32		lda	$32		;
C76D : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C770 : A9 10		lda	#$10		;
C772 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
C775 : A5 34		lda	$34		;
C777 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
C77A : E8		inx			;
C77B : E4 36		cpx	$36		;
C77D : D0 E7		bne	$C766		;
C77F : 60		rts			;

; Function from C6D9
C780 : A9 00		lda	#$00		;
C782 : 85 52		sta	$52		;
C784 : 60		rts			;

; Function from C6E8
C785 : A9 00		lda	#$00		;
C787 : 85 54		sta	$54		;
C789 : 60		rts			;

; Function from C6F4
C78A : A9 01		lda	#$01		;
C78C : 85 52		sta	$52		;
C78E : 60		rts			;

; Function from C703
C78F : A9 01		lda	#$01		;
C791 : 85 54		sta	$54		;
C793 : 60		rts			;

; Called from C6B2 and C75F
; if $52 != 1:
;   if $54 == 0:
;     $32++
;     $34++
;   else if $54 == 1:
;     $32++
;     $34--
; else if $52 == 1:
;   if $54 == 0:
;     $34++
;     $32--
;   else if $54 == 1:
;     $34--
;     $32--
C794 : A9 00		lda	#$00		;
C796 : C5 52		cmp	$52		;
C798 : F0 06		beq	$C7A0		;
C79A : A9 01		lda	#$01		;
C79C : C5 52		cmp	$52		;
C79E : F0 0C		beq	$C7AC		;
C7A0 : A9 00		lda	#$00		;
C7A2 : C5 54		cmp	$54		;
C7A4 : F0 12		beq	$C7B8		;
C7A6 : A9 01		lda	#$01		;
C7A8 : C5 54		cmp	$54		;
C7AA : F0 1A		beq	$C7C6		;
C7AC : A9 00		lda	#$00		;
C7AE : C5 54		cmp	$54		;
C7B0 : F0 1B		beq	$C7CD		;
C7B2 : A9 01		lda	#$01		;
C7B4 : C5 54		cmp	$54		;
C7B6 : F0 07		beq	$C7BF		;
C7B8 : E6 32		inc	$32		;
C7BA : E6 34		inc	$34		;
C7BC : 4C D1 C7		jmp	$C7D1		; Return
C7BF : C6 32		dec	$32		;
C7C1 : C6 34		dec	$34		;
C7C3 : 4C D1 C7		jmp	$C7D1		; Return
C7C6 : E6 32		inc	$32		;
C7C8 : C6 34		dec	$34		;
C7CA : 4C D1 C7		jmp	$C7D1		; Return
C7CD : E6 34		inc	$34		;
C7CF : C6 32		dec	$32		;
C7D1 : 60		rts			;


; Function call from C13B and C144
; update_paddle0($24, $26)  ; $24 = x coord of sprite 0, $26 = x coord of sprite 1
; y = 0
; for (x = 0; x < 0x20; x++) {
;   $2003 = 0x03
;   $2004 = $24
; 
;   $2003 = 0x07
;   $2004 = $26
; }

C7D2 : A2 00		ldx	#$00		;
C7D4 : A0 00		ldy	#$00		;
  C7D6 : A9 03		lda	#$03		;
  C7D8 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
  C7DB : A5 24		lda	$24		;
  C7DD : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C7E0 : A9 07		lda	#$07		;
  C7E2 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
  C7E5 : A5 26		lda	$26		;
  C7E7 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C7EA : E8		inx			;
  C7EB : E0 20		cpx	#$20		;
  C7ED : D0 E7		bne	$C7D6		;
C7EF : 60		rts			;


; Function call from C13E and C147
; update_paddle1($28, $30) ; $28 = x coord of sprite 2, $30 = x coord of sprite 3
; y = 0
; for (x = 0; x < 0x20; x++) {
;   $2003 = 0x0B
;   $2004 = $28
;
;   $2003 = 0x0F
;   $2004 = $30
; }

C7F0 : A2 00		ldx	#$00		;
C7F2 : A0 00		ldy	#$00		;
  C7F4 : A9 0B		lda	#$0B		;
  C7F6 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
  C7F9 : A5 28		lda	$28		;
  C7FB : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C7FE : A9 0F		lda	#$0F		;
  C800 : 8D 03 20		sta	$2003		;SPR-RAM Addr.Reg.
  C803 : A5 30		lda	$30		;
  C805 : 8D 04 20		sta	$2004		;SPR-RAM I/O Reg.
  C808 : E8		inx			;
  C809 : E0 20		cpx	#$20		;
  C80B : D0 E7		bne	$C7F4		;
C80D : 60		rts			;


; Function call from C138
; 
; Strobe $4016 and $4017
;
; if ( controller 0 'Left') {
;   $22 = 0x01
;   $24--
;   $26--
; }
;
; if ( controller 1 'Left') {
;   $22 = 0x02
;   $28--
;   $30--
; }

C80E : A9 01		lda	#$01		;
C810 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C813 : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C816 : A9 00		lda	#$00		;
C818 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C81B : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C81E : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C821 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C824 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C827 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C82A : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C82D : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C830 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C833 : 29 01		and	#$01		;
C835 : F0 08		beq	$C83F		;
  C837 : A2 01		ldx	#$01		;
  C839 : 86 22		stx	$22		;
  C83B : C6 24		dec	$24		;
  C83D : C6 26		dec	$26		;
C83F : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C842 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C845 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C848 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C84B : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C84E : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C851 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C854 : 29 01		and	#$01		;
C856 : F0 0A		beq	$C862		; Return
  C858 : A2 02		ldx	#$02		;
  C85A : 86 22		stx	$22		;
  C85C : A2 00		ldx	#$00		;
  C85E : C6 28		dec	$28		;
  C860 : C6 30		dec	$30		;
C862 : 60		rts			; Return

; Function call from C141
; 
; Strobe controllers
; 
; if controller 0 RIGHT
;   $22 = 0x01
;   $24++
;   $26++
; 
; if controller 1 RIGHT
;   $22 = 0x02
;   $28++
;   $30++

C863 : A9 01		lda	#$01		;
C865 : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C868 : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C86B : A9 00		lda	#$00		;
C86D : 8D 16 40		sta	$4016		;Joy-Pad #1 CtrlReg.
C870 : 8D 17 40		sta	$4017		;Joy-Pad #2 CtrlReg.
C873 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C876 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C879 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C87C : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C87F : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C882 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C885 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C888 : AD 16 40		lda	$4016		;Joy-Pad #1 CtrlReg.
C88B : 29 01		and	#$01		;
C88D : F0 0A		beq	$C899		;
  C88F : A2 01		ldx	#$01		;
  C891 : 86 22		stx	$22		;
  C893 : A2 00		ldx	#$00		;
  C895 : E6 24		inc	$24		;
  C897 : E6 26		inc	$26		;
C899 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C89C : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C89F : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8A2 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8A5 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8A8 : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8AB : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8AE : AD 17 40		lda	$4017		;Joy-Pad #2 CtrlReg.
C8B1 : 29 01		and	#$01		;
C8B3 : F0 0A		beq	$C8BF		; Return
  C8B5 : A2 02		ldx	#$02		;
  C8B7 : 86 22		stx	$22		;
  C8B9 : A2 00		ldx	#$00		;
  C8BB : E6 28		inc	$28		;
  C8BD : E6 30		inc	$30		;
C8BF : 60		rts			; Return

; Function call from C8C0
; 
; $4015 = 0x0F   ; Vertical clock signal register
; $400C = 0x0F   ; Noise Control register
; $400E = 0xF4   ; Noise Frequency reg 1
; $400E = 0xFD

C8C0 : A9 0F		lda	#$0F		;
C8C2 : 8D 15 40		sta	$4015		;SoundChannel CtrlReg.
C8C5 : A9 0F		lda	#$0F		;
C8C7 : 8D 0C 40		sta	$400C		;NoizeWave ch4 CtrlReg.#1
C8CA : A9 F4		lda	#$F4		;
C8CC : 8D 0E 40		sta	$400E		;NoizeWave ch4 Freq.Reg.L
C8CF : A9 FD		lda	#$FD		;
C8D1 : 8D 0E 40		sta	$400E		;NoizeWave ch4 Freq.Reg.L
C8D4 : 60		rts			;

; Function call from C135
;
; if ($72 != 0x01) return;
;
; Call 0xC8C0 (sound control)
; $74++
; if ($74 != 0x08) return
; $74 = 0
; $72 = 0


C8D5 : A5 72		lda	$72		;
C8D7 : C9 01		cmp	#$01		;
C8D9 : D0 11		bne	$C8EC		; Return
C8DB : 20 C0 C8		jsr	$C8C0		;
C8DE : E6 74		inc	$74		;
C8E0 : A5 74		lda	$74		;
C8E2 : C9 08		cmp	#$08		;
C8E4 : D0 06		bne	$C8EC		; Return
C8E6 : A9 00		lda	#$00		;
C8E8 : 85 74		sta	$74		;
C8EA : 85 72		sta	$72		;
C8EC : 60		rts			;
; End function

; VBlank Handler
FF00 : A2 00		ldx	#$00		;
FF02 : 8E 05 20		stx	$2005		;BG Scrool Reg.
FF05 : 8E 05 20		stx	$2005		;BG Scrool Reg.
FF08 : 40		rti			;
; Done handler

FFFA : 00 FF		dw	$FF00		;NMI(VBlank)
FFFC : 00 C0		dw	$C000		;Reset
FFFE : 00 C0		dw	$C000		;IRQ/BRK
