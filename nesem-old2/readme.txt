***** Minachun Disassembler for 6502 Famicom(NES)   Version 0.04a ******

							Programmed by Minachun


1. ���̃\�t�g�ɂ���

	���̃\�t�g�́A���̓t�@�C���� 6502 �o�C�i���ƌ��Ȃ��ċt�A�Z���u��
	���܂��B�����́A
	�E�o�͂����R�����g���t�@�~�R���ɓ������Ă���B
	�E���荞�݃x�N�^�[�̏���������B
	�E�\�[�X�R�[�h���ꏏ�Ɍ��J����̂ŁAC/C++ �R���p�C���������Ă���
	  �l�Ȃ�A�e�ՂɃR�����g�����J�X�^�}�C�Y�ł���B
	�ENSF �����l�̂��߂̋@�\������i�ق�̂�����Ƃ����j�B
	�E����񂱂Ƃ�����Ă���֌W��A�������x�͒x�������B
	�EGUI�ł͂���܂���B


2. �����

	�����̎��s�t�@�C���́AWindows95 �� DOS����ł̂ݓ�����m�F���Ă��܂��B
	�\�[�X���̂��̂� OS �Ɉˑ�����悤�ȃR�[�h���ɗ͔r�����Ă���͂��i���j
	�Ȃ̂ŁA���̊��ł��R���p�C������Ύ��s�ł���\���͂���܂��B
	�܂��A���삳�������ʂɊւ��ẮA�����Ȃ�ꍇ�ɂ����Ă��ۏ؂͂������܂���
	�̂ł����ӂ��B


3. �g����

	��{�I�ɂ� yd6502.com �Ǝ����Ă���̂ŁA
	
	>md6502 ���̓t�@�C�� �o�̓t�@�C��
	
	�Ƃ���΁Ayd6502.com �ł̏o�͂Ɏ������ʂ������邩�Ǝv���܂��B
	���ꂾ�����ƁA�����̃f�J���Ēx���v���O�����ł��̂ŁA�ȉ��̂悤��
	�@�\������܂��B�w�肷�鐔���́A���ׂ� 16 �i���ł��肢���܂��B
	
	Usage:md6502.exe [input file] [output file] [options]
	[input file] : ���̓t�@�C����
	[output file] : �o�̓t�@�C����
	[options] :
	    -s????...�t�A�Z���u���J�n�A�h���X���w�肷��B�f�t�H���g�́A�t�@�C��
		     �̍Ōオ 0xffff �ƂȂ�悤�ɂ���B
	    -l????...�t�A�Z���u������o�C�g�����w�肷��B�f�t�H���g�́A�t�@�C��
		     ���ׂĂ�ǂݍ��ނ܂ŏo�͂���B
	    -o????...�t�A�Z���u���J�n�I�t�Z�b�g���w�肷��B�f�t�H���g�́A
		     �t�@�C���̐擪�A�܂� 0 �B
	    -t...PSG���g���e�[�u����T���A����炵����������΃e�[�u���ɂ��܂��B
		 �f�t�H���g�ł͂��܂���B
	    -m??...�}�b�p�[�ԍ����w�肵�܂��B�f�t�H���g�ł́A�}�b�p�[�ʂ�
		   �R�����g���o�͂��܂���B�ő�8�܂ŕ����̎w�肪�\�ł��B
		   Ex) -m4b -mff ... KONAMI VRC1 �� NSF Mapper

	���Ȃ݂ɁA�P��
	>md6502
	�Ƃ������s���Ă��A��L�Ɠ��l�ȏo�͂�������Ǝv���܂��i�΁j�B

	�����_�ŃR�����g�o�͂ɑΉ����Ă���}�b�p�[�͈ȉ��̒ʂ�ł��B�������A
	���ׂĂ��������킯�ł͂Ȃ��̂ł����ӂ��������B

	MapperNo.	Type
	01(1)		Nintendo MMC1
	02(2)		UNROM
	03(3)		CNROM
	04(4)		Nintendo MMC3
	05(5)		Nintendo MMC5
	07(7)		AOROM
	08(8)		FFE F3xxx
	09(9)		Nintendo MMC2
	0A(10)		Nintendo MMC4
	0F(15)		100-in-1
	10(16)		Bandai
	12(18)		Jaleco SS8806
	13(19)		Namcot 106
	14(20)		Famicom Disk System
	15(21)		Konami VRC4 2A ���������ɕs���m
	16(22)		Konami VRC2 Type A ( VRC4 1B )
	17(23)		Konami VRC2 Type B ( VRC4 1A )
	18(24)		Konami VRC6
	19(25)		Konami VRC4
	20(32)		IREM G-101
	21(33)		Taito TC0190
	42(66)		GNROM
	44(68)		Sunsoft Mapper #4
	45(69)		Sunsoft FME-7 ( Mapper #5 )
	4B(75)		Konami VRC1 / Jaleco SS8805
	4E(78)		Irem 74HC161/32
	55(85)		Konami VRC7
	59(89)		Sunsoft Early Mapper
	184(B8)		Sunsoft Madoola Mapper
	FF(255)		NSF Bank


4. �g�p�E�z�z����

	�S�������Ď��R�ł����A�� Minachun �ɕs���v���o�Ȃ��悤�ɂ��肢���܂��B
	�Ĕz�z�����R�ł����A����ȊO�̋��K�̎��󂪂Ȃ��悤�ɂ��肢���܂��B
	�\�[�X�R�[�h�̉��ς���щ��ς������̂��R���p�C���������s�t�@�C���̔z�z
	�͎��R�Ɉ����Ă�����Ă��\���܂��񂪁A���Ɉ��e�����o�Ȃ��͈͂ł��肢��
	�܂��B���悢���ς��Ȃ��������́A�����ɉ��ϓ��e�����񂭂�����Ɣ���
	������܂��B


5. ���̑�

	�}�b�p�[��񓙂̃R�����g�́A�����_�Ŏ��������Ă�����݂̂��Q�Ƃ���
	���܂��B�S�������ĕs���m�ł���\���������̂ŁA���L�p�ȏ���������
	�̂����͐��񋳂��Ă��������B���̃v���O�����̓R�����g�����ł��B

	�����������̂�����ƁA������ ripping �����₷���Ȃ�ł��傤���H

	�Ȃ��A�ꎞ�z�z���i�܂�A���� Web page�j�́A�ȉ��� URL �ɂ���܂��̂�
	�@�����ΖK��Ă݂Ă��������B

	http://minachun.8m.com/index.html
	E-Mail : nsf@minachun.8m.com

	�g�����A�R���p�C���̎d���Ɋւ��鎿��͂��󂯂ł��܂���B�g�����͊ȒP��
	���낤���A�R���p�C���̎d�����\�[�X�t�@�C����������Ȃ��̂ō�������
	�͂����Ȃ��낤���B


6. �ӎ�

	���ɗL�p�ȏ��� Web ��Ɍ��J���Ă���������F�l�B���ɁAYoshi����A
	goroh ����̏��͔��ɎQ�l�ɂȂ�܂����B
	
	FWNES �̍�ҁAFan Wen Yang ����̃h�L�������g�B
	
	�����������邫�������ɂȂ����ARipper�ւ̓��������J���Ă���������
	Izumi. ����B


7. ����

	2000/2/12  Version 0.04a
		INC/DEC $xx,x(#$F6/D6)�̋t�A�Z���u���� INC/DEC $xx,y �Ƃ��Ă���
		�o�O��Fix.
	2000/1/8   Version 0.04
		BN:0xb8 (�}�h�D�[���̗�) �̃��b�Z�[�W��t��
		-s���w�肹��-l�݂̂��w�肵���Ƃ��́A�x�[�X�A�h���X�̕s����Fix.
		MMC5 �̃��b�Z�[�W��ǉ��A�ύX
	2000/1/3   Version 0.03
		VRC2B �̃��b�Z�[�W�A�A�h���X��ύX
		NSF (�ԍ��F0xff(255)) �o���N���b�Z�[�W�̕t��
		�}�b�p�[�ԍ��𕡐��w��ł���悤�ɂ���
	1999/12/31 Version 0.02
		VRC6 �̉������b�Z�[�W��t���B
		Namcot106 �̃��b�Z�[�W��ǉ�(ExI/O Addr.)
		VRC7,VRC1,Sun EarlyMapper ��ǉ�
		fds �̃}�b�p�[�ԍ��� 0x14(20) �ɃZ�b�g
		�������ݓǂݍ��ݑ����̒ǉ�(���R�����g�����m��)
	1999/12/27 Version 0.01
		�����J�B�}�b�p�[���Ƃ̃R�����g�A�J�n�A�h���X�A�T�C�Y�A�I�t�Z�b�g
		���g���e�[�u�������Ȃǂ�����B
	1999/12/25 Version 0.00
		�����J�Byd6502.com�Ɠ�������B�Q���Ԃ̂�����d���i�΁j�B


[EOF]