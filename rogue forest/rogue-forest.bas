5 REM
10 REM "ÕÃÃÕÀÉÕÀÉÂ ÂÕÀÀ  ÕÃÃÕÀÉÕÃÃÕÀÀÕÀÃÀ²À "
15 REM "Â  ÊÀËÊÀÛÊÀË«À   «À ÊÀËÂ  «À ÊÀÉ Â  "
20 REM "       ÀË   ÊÀÀ  Â        ÊÀÀÃÀË    "
25 REM
30 REM  INITIALIZE PLAYER
35 GOSUB 960  :REM INIT GAME
40 GOSUB 1745  :REM INIT SOUND
45 GOSUB 1340 :REM INIT PANELS
50 REM  LINE-CLEANER
55 LI$ = "                                                           "
60 GOSUB 1825 :REM SPLASH SCREEN
65 GOSUB 465  :REM BUILD FOREST
70 R1=INT(RND(1)*50+10)
75 R2=INT(RND(1)*30+10)
80 R3  = 0
85 R4 = 0
90 R5   = 1
95 R6 = 1
100 R7  = 0
105 R6$ = "RUSTY SPORK"
110 R7$ = "TUXEDO"
115 C1    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM STR
120 C2  = C1
125 C3    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM DEX
130 C4    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM END
135 C5    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM INT
140 C0(1) = C1
145 C0(2) = C3
150 C0(3) = C4
155 C0(4) = C5
160 R8$ = "NOT YET"
165 DIM IW%(10), IA%(10) :REM WEAPONS, ARMOR
170 REM                       DRAW THE SCREEN
175 REM HIDE BACKGROUND
180 COLOR 1,6 :CLS :REM WHITE ON BLUE
185 GOSUB 2000 :REM TITLE
190 GOSUB 1705 
195 L= 1  :T= 44 :W= 76  :H= 15  :L$= "" :GOSUB 1655
200 GOSUB 1515 :REM DRAW PANELS
205 GOSUB 380
210 REM  PRINT PLAYER AND POSITION CURSOR
215 ? VP$(R2-4);
220 COLOR 1,0 :REM TRANSPARENT (CLEARS FOG)
225 ? TAB(R1-4) C9$;
230 COLOR 1,6 :REM BLUE UNDER WHITE
235 ? L5$ TAB(R1) "@"
240 ? VP$(4) LI$ : ? "\X91   STR" C1 "SCORE:" R3 "HUNGER:" R4
245 IF RND(1)>0.95 THEN R4 = R4 + 1
250 T0 = T0 - 1
255 IF T0 = 0 THEN ? VP$(5) LI$
260 GET A$ :IF A$="" GOTO 260
265 XX=0 :YY=0
270 IF A$="\X11" THEN YY=+1 :REM DOWN
275 IF A$="\X91" THEN YY=-1 :REM UP
280 IF A$="\X1D" THEN XX=+1 :REM RIGHT
285 IF A$="\X9D" THEN XX=-1 :REM LEFT
290 IF A$="E" THEN GOSUB 420
295 IF (A$="." OR A$=" ") AND C1 < C2 THEN C1 = C1 + 1
300 LO = VPEEK(1,(R2+YY)*$100 + (R1+XX)*2)
305 IF LO < $20 THEN NN=LO :GOSUB 1270 :GOTO 320
310 IF LO > $22 AND LO < $2E THEN :GOSUB 2025 :GOSUB 380
315 IF LO < $41 THEN R1=R1+XX :R2=R2+YY :HU=HU+1: LO = VPEEK(1,R2*$100 + R1*2)
320 FOR NN=0 TO 31
325    AA=MA(NN)
330    XX=XM(NN)-R1
335    YY=YM(NN)-R2
340    IF ABS(XX)+ABS(YY) <= 1 THEN GOSUB 1230 :GOTO 355
345    IF RND(1)>0.5 THEN 355
350    IF ABS(XX) < 9 AND ABS(YY) < 9 THEN GOSUB 1150
355 NEXT
360 IF C1 < 1 GOTO 370
365 IF (R1<70) AND (R1>7) AND (R2<42) AND (R2>5) GOTO 215
370 GOSUB 865
375 END
380 ? VP$(46);
385 ? SPC(6) LI$: ? "\X91" SPC(6) "WEAPON: " R6$ " (" R6 ")"
390 ? SPC(6) LI$: ? "\X91" SPC(6) "ARMOR : " R7$ " (" R7 ")"
395 ? SPC(6) LI$: ? "\X91" SPC(6) "FOOD  : " R5
400 ? SPC(6) LI$: ? "\X91" SPC(6) "AMULET: " R8$ 
405 ? SPC(6) LI$: ? "\X91" SPC(6) "SCORE : " R3
410 ?
415 RETURN
420 IF R5 < 1 THEN RETURN
425 IF R4 < 1 THEN RETURN
430 R5 = R5 - 1
435 R4 = R4 - 1
440 T0 = 5
445 ? VP$(5) LI$ :?"\X91   YOU FEEL BETTER."
450 GOSUB 380
455 RETURN
460 REM                         BUILD THE FOREST
465 REM  SET UP VIDEO REGISTERS
470 POKE $9F2D, %01100000 :REM MAP HEIGHT=1, WIDTH=2 = 64X128 TILES
475 POKE $9F2E, %10000000 :REM MAP BASE ADDR = 128 X512 = $10000.
480 POKE $9F2F, %11111000 :REM TILE BASE ADDR = 30 X2K  = $1F000.
485 REM ALSO, TILE HT=0, WD=0, SO 8 PIXELS X 8 PIXELS
490 REM  NO SCROLLING, PLEASE ($9F30-$9F33)
495 POKE $9F30, 0
500 POKE $9F31, 0
505 POKE $9F32, 0
510 POKE $9F33, 0
515 COLOR 1,0 :CLS  :REM 0=TRANSPARENT
520 POKE $9F29, %00110001 :REM $31=LAYERS 1,0. OUTPUT MODE=1 (VGA)
525 CC=4:CD=45
530 IF VPEEK(1,0) = 32 THEN CC=8 :CD=40
535 REM  UNROLLING THIS LOOP SHAVES 5 SECS
540 CL=$05  :REM BG/FG COLOR NYBBLES ($15=WHITE UNDER GREEN)
545 REM MONSTER DATA
550 MN=0
555 DIM MA(31)
560 DIM XM(31),YM(31)
565 DIM SO(31) :REM TERRAIN THE MONSTER IS .S.TANDING .O.N
570 POKE $9F25, PEEK($9F25) AND 254 : REM SELECT PORT 0
575 POKE $9F22, %00010001 : REM INCR 1 AND VRAM ADDR BIT 16
580 FOR Y=CC TO CD
585    Y0=Y*$100
590    FOR X=0 TO 158 STEP 8
595       P=Y0+X
600       POKE $9F21, INT(P/256) AND 255 :REM VRAM 15:8
605       POKE $9F20, P AND 255          :REM VRAM 7:0
610       IF Y<8 OR Y>40 GOTO 725
615       IF X<14 OR X>140 GOTO 725
620          FF=3
625          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
630          POKE $9F23, FO(FF) :REM CHARACTER INDEX
635          POKE $9F23, CL   :REM BG/FG COLOR NYBBLES
640          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
645          POKE $9F23, FO(FF)
650          POKE $9F23, CL
655          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
660          POKE $9F23, FO(FF)
665          POKE $9F23, CL
670          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
675          POKE $9F23, FO(FF)
680          POKE $9F23, CL
685          IF Y<9 OR Y>39 GOTO 765
690          REM TREASURE
695          IF RND(1) > 0.08 GOTO 765
700             XX = INT(RND(4))
705             VPOKE 1,P+2*XX,FO(5+INT(RND(1)*4))
710             POKE $9F25, PEEK($9F25) AND 254 : REM RE-SELECT PORT 0
715             POKE $9F22, %00010001 : REM INCR 1 AND VRAM ADDR BIT 16
720             GOTO 765
725             POKE $9F23, $20
730             POKE $9F23, CL
735             POKE $9F23, $20
740             POKE $9F23, CL
745             POKE $9F23, $20
750             POKE $9F23, CL
755             POKE $9F23, $20
760             POKE $9F23, CL
765    NEXT
770 NEXT
775 REM  PLACE MONSTERS
780 FOR M = 1 TO 26 :REM 0 TO 31
785    X = 10 + INT(RND(1)*60)
790    Y = 10 + INT(RND(1)*30)
795    XX = ABS(X-R1)
800    YY = ABS(Y-R2)
805    IF XX<6 AND YY<6 GOTO 785 :REM TOO CLOSE!
810    MA = Y * $100 + X * 2
815    MA(M) = MA
820    XM(M)=X
825    YM(M)=Y
830    SO(M) = VPEEK(1,MA)
835    VPOKE 1,MA,M
840 NEXT
845 REM AMULET OF YENDOR
850 VPOKE 1, INT(RND(1)*30+10) * $100 + INT(RND(1)*60+10) * 2, FO(10)
855 RETURN
860 REM                      GAME OVER - PRINT SCORE
865 COLOR 1,$06 :REM WHITE ON BLUE
870 ? CHR$(147) :REM CLR
875 GOSUB 1705
880 L= 1  :T= 1  :W= 76  :H= 17  :L$= "GAME OVER"   :GOSUB 1655
885 GOSUB 1515
890 ? VP$(3)
895 ?SPC(6) "YOUR ACCOMPLISHMENTS"
900 ?:?:? SPC(6) "ESCAPED THE FOREST: ";
905 IF C1 < 1 THEN ? "NO"
910 IF C1 > 0 THEN ? "YES"
915 ?:? SPC(6) "FOUND THE AMULET:   ";
920 IF R8 = 0 THEN ? "NO"
925 IF R8 = 1 THEN ? "YES"
930 ?:?:? SPC(6) "SCORE:             " R3
935 GOSUB 1800
940 ?:?:? SPC(6) "THANK YOU FOR PLAYING ROGUE FOREST!"
945 ?:?:?
950 RETURN
955 REM  INITIALIZE 
960 GOSUB 2325 :REM INIT VP$()
965 L5$ = "\X91\X91\X91\X91\X91" :REM LEFT 5
970 C9$ = "         \X11\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
975 C9$ = C9$+C9$+C9$+C9$+C9$+C9$+C9$+C9$+C9$
980 DIM M$(31)
985 FOR X=0 TO 31 :READ M$(X) :NEXT
990 DATA  DOPPLEGANGER,  ABALONE,  BASILISK,    CENTAUR,    DRAGON
995 DATA  ETTIN,         FUZZY,    GHOUL,       HELLHOUND,  IMP
1000 DATA  JINN,          KOBOLD,   LEPRECHAUN,  MINOTAUR,   NYMPH
1005 DATA  ORC,           PYTHON,   QUAGGA,      RATTLER,    SKELETON
1010 DATA  TROLL,         UR-VILE,  VAMPIRE,     WRAITH,     XEROC
1015 DATA  YETI,          ZOMBIE,   WYVERN,      MANTICORE,  VELOCIRAPTOR
1020 DATA  SPHINX,        SPIDER
1025 FOR X=0 TO 7 :READ N$(X) :NEXT
1030 DATA CHENEY, DURIN, FARGOL, KHAALO, REJNALDI, SARA, SHARIK, ZOG
1035 FOR X=0 TO 7 :READ C$(X) :NEXT
1040 DATA "","RED ","GREEN ","BLUE ","TIN ","WHITE ","PALE ","GOLD "
1045 FOR X=0 TO 7 :READ D$(X) :NEXT
1050 DATA ""," OF WAR"," OF AGILITY"," OF PAIN"," OF CUNNING"
1055 DATA " OF SKILL",""," OF MAGIC"
1060 FOR X=0 TO 7 :READ W$(X) :NEXT
1065 DATA DAGGER, STAFF, HAMMER, MACHETE, AXE, SWORD, LABRYS, HALBERD
1070 FOR X=0 TO 7 :READ A$(X) :NEXT
1075 DATA LEATHER, HAUBERK, RINGMAIL, MESH, MAIL, SPLINT, PLATE, MITHRIL
1080 REM FOREST ELEMENTS
1085 FO(0) = $41 :REM TREE
1090 FO(1) = $2E :REM DOT
1095 FO(2) = $2E :REM DOT
1100 FO(3) = $43 :REM HORIZ LINE
1105 FO(4) = $42 :REM VERT LINE
1110 FO(5) = $23 :REM ARMOR (#)
1115 FO(6) = $24 :REM FOOD ($)
1120 FO(7) = $27 :REM WEAPON (')
1125 FO(8) = $27
1130 FO(9) = $24
1135 FO(10) = $2A :REM AMULET OF YENDOR
1140 RETURN
1145 REM        MOVE MONSTER:NN ADDRESS:AA RELATIVE_POS:XX,YY
1150 VPOKE 1,AA,SO(NN)  :REM REDRAW TILE
1155 DX=0 :DY=0
1160 IF RND(1)>0.5 GOTO 1175
1165 IF XX<0 THEN AA=AA+2 :DX=1 :GOTO 1185
1170 IF XX>0 THEN AA=AA-2 :DX=-1 :GOTO 1185
1175 IF YY<0 THEN AA=AA+$100 :DY=1 :GOTO 1185
1180 IF YY>0 THEN AA=AA-$100 :DY=-1 :GOTO 1185
1185 V=VPEEK(1,AA)
1190 IF V < $20 OR V = $2A GOTO 1220
1195 SO(NN)=VPEEK(1,AA)
1200 MA(NN)=AA
1205 XM(NN)=XM(NN)+DX
1210 YM(NN)=YM(NN)+DY
1215 VPOKE 1,AA,NN
1220 RETURN
1225 REM                  MONSTER NN ATTACKS
1230 ? VP$(51) SPC(6) LI$ : ? "\X91";
1235 W = INT(RND(1)*4)
1240 IF INT(RND(1)*40) < R7 THEN W=0
1245 IF W=0 THEN ? SPC(6) "THE " M$(NN) " MISSES."
1250 IF W>0 THEN ? SPC(6) "THE " M$(NN) " HITS YOU FOR" W "POINTS." :GOSUB 1775
1255 C1 = C1 - W
1260 RETURN
1265 REM                  ATTACK MONSTER NN
1270 ? VP$(52) SPC(6) LI$ : ? "\X91";
1275 T1 = R6 AND 7
1280 T2 = (R6/8) AND 7
1285 T3 = (R6/64) AND 7
1290 T4 = (R6/256) AND 7
1295 T5 = T1+T2+T3+T4+1 - R4
1300 IF T5 < 2 THEN T5 = 2
1305 W = INT(RND(1)*T5)
1310 HU=HU+1
1315 N=RND(1) * NN
1320 ? SPC(6) "YOU HIT THE " M$(NN) " FOR" W "POINTS"; :GOSUB 1775
1325 IF W > N THEN ? ", KILLING IT";: VPOKE 1,MA(NN),$2E :YM(NN)=-1000 :R3=R3+W
1330 ? "."
1335 RETURN
1340 REM SET UP PANEL ARRAYS BASED ON MAX
1345 REM NUMBER OF PANELS (N=6)
1350 N = 6
1355 P7 = 0
1360 DIM P0$(N)
1365 DIM P1(N), P2(N), P3(N), P4(N)
1370 DIM P5$(N,30), P5SELECTED(N)
1375 GOSUB 2325 :REM INIT VP$()
1380 REM HORIZONTAL BAR ARRAY
1385 DIM HB$(80)
1390 HB$(0) = "\XC3"
1395 FOR V=1 TO 80
1400    HB$(V) = HB$(V-1) + "\XC3"
1405 NEXT
1410 REM VERTICAL BAR ARRAY
1415 DIM VB$(30)
1420 VB$(0) = "\XC2\X11\X9D"
1425 FOR V=1 TO 30
1430    VB$(V) = VB$(V-1) + "\XC2\X11\X9D"
1435 NEXT
1440 REM  PANEL EDGES
1445 BA$ = "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1450 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1455 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1460 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1465 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1470 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1475 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1480 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1485 SP$ = "                    "
1490 SP$ = SP$ + "                    "
1495 SP$ = SP$ + "                    "
1500 SP$ = SP$ + "                    "
1505 RETURN
1510 REM DRAW PANELS FROM VARIABLE DATA
1515 FOR N = 0 TO P7 - 1
1520    ? VP$(P1(N)) SPC(P2(N))
1525    ? "\XD5\XC3" P0$(N) "\XC3";
1530    ? HB$(P3(N) - 3 - LEN(P0$(N)));
1535    ? "\XC9\X11";
1540    B2$ = LEFT$(BA$, P3(N)+2)
1545    S2$ = LEFT$(SP$, P3(N))
1550    FOR RR=3 TO P4(N)
1555    REM DON'T FORGET THE LINES !!
1560    ? B2$ "\XC2" S2$ "\XC2\X11";
1565    NEXT
1570    ? B2$ "\XCA";
1575    ? HB$(P3(N)-1);
1580    ? "\XCB";
1585 NEXT
1590 RETURN
1595 REM SHOW THE PANEL TEXTS
1600 FOR N = 0 TO P7 - 1
1605    ? VP$(P1(N));
1610    ?
1615    FOR X = 0 TO P4(N)-3
1620       ? SPC(P2(N)+2) P5$(N,X)
1625    NEXT
1630 NEXT
1635 RETURN
1640 REM ADD A PANEL TO THE LIST!
1645 REM T: TOP, L: LEFT, W: WIDTH, H:HEIGHT
1650 REM L$: LABEL
1655 N = P7
1660 P0$(N) = L$
1665 P1(N) = T
1670 P2(N) = L
1675 P3(N) = W
1680 P4(N) = H
1685 P5SELECTED(N) = -1
1690 P7 = N + 1
1695 RETURN
1700 REM "DELETE" ALL PANELS
1705 P7 = 0
1710 RETURN
1715 REM SET CONTENTS OF A PANEL
1720 REM N:PANEL NUM, BU$(): LINES, LC: LINE COUNT
1725 FOR X = 0 TO LC-1
1730    P5$(N,X) = BU$(X)
1735 NEXT
1740 RETURN
1745 IF PEEK($400)=0 THEN LOAD "RF-EFFECTS.PRG",8,1,$0400
1750 RETURN
1755 SYS $0400
1760 T = TI 
1765 IF TI < T + 100 GOTO 1765
1770 RETURN
1775 SYS $0403
1780 T = TI 
1785 IF TI < T + 100 GOTO 1785
1790 RETURN
1795 RETURN
1800 SYS $0409
1805 T = TI
1810 IF TI < T + 300 GOTO 1810
1815 RETURN
1820 REM                       SPLASH SCREEN!
1825 ? CHR$(147)
1830 L= 1  :T= 5  :W= 76  :H= 13  :L$= "WELCOME"     :GOSUB 1655
1835 L= 1  :T= 19 :W= 76  :H= 15  :L$= "MAP KEY"     :GOSUB 1655
1840 L= 1  :T= 35 :W= 76  :H= 12  :L$= "COMMAND KEY" :GOSUB 1655
1845 GOSUB 1515 :REM DRAW PANELS
1850 ? VP$(0); :GOSUB 2000 :REM TITLE
1855 ? VP$(8)
1860 ? SPC(6) "FIND THE AMULET OF YENDOR AND EXIT THE FOREST ALIVE!"
1865 ?
1870 ? SPC(6) "POINTS ARE AWARDED FOR ITEMS COLLECTED AND MONSTERS KILLED."
1875 ? VP$(20)
1880 ? SPC(6) "@ ... YOU"
1885 ? SPC(6) CHR$(FO(0)) " ... TREE (IMPASSABLE BY YOU)"
1890 ? SPC(6) CHR$(FO(1)) " ... CLEARING"
1895 ? SPC(6) CHR$(FO(5)) " ... ARMOR"
1900 ? SPC(6) CHR$(FO(6)) " ... FOOD"
1905 ? SPC(6) CHR$(FO(7)) " ... WEAPON"
1910 ? SPC(6) CHR$(FO(10))" ... AMULET OF YENDOR"
1915 ?:?
1920 ? SPC(6) "-- ANYTHING THAT MOVES IS A MONSTER --"
1925 ? VP$(36)
1930 ? SPC(6) "? ............ HELP"
1935 ? SPC(6) "CURSOR KEYS .. MOVE / ATTACK"
1940 ? SPC(6) ". ............ REST"
1945 ? SPC(6) "E ............ EAT FOOD"
1950 ? SPC(6) "X ............ EXIT"
1955 ?
1960 ? VP$(50)
1965 ? "   GOOD LUCK!"
1970 ?
1975 GOSUB 1800
1980 INPUT "   PLEASE TYPE YOUR NAME"; R0$
1985 X=RND(-TI)
1990 RETURN
1995 REM PRINT TITLE
2000 ? "  ÕÃÃÕÀÉÕÀÉÂ ÂÕÀÀ  ÕÃÃÕÀÉÕÃÃÕÀÀÕÀÃÀ²À "
2005 ? "  Â  ÊÀËÊÀÛÊÀË«À   «À ÊÀËÂ  «À ÊÀÉ Â  "
2010 ? "         ÀË   ÊÀÀ  Â        ÊÀÀÃÀË1.1 "
2015 RETURN
2020 REM                   TREASURE FINDS (TYPE IN 'LO')
2025 COLOR 1,6
2030 REM CLEAR THE SQUARE THE PLAYER IS *LOOKING AT*
2035 VPOKE 1,(R2+YY)*$100 + (R1+XX)*2, $2E
2040 GOSUB 1755
2045 REM PRE-CALC SOME VALUES
2050 T1=INT(RND(1)*8)
2055 T2=INT(RND(1)*8)
2060 T3=INT(RND(1)*8)
2065 T4=INT(RND(1)*8)
2070 REM FIGURE OUT WHAT WE GOT
2075 IF LO=FO(10) GOTO 2100
2080 IF LO=FO(5)  GOTO 2150
2085 IF LO=FO(6)  GOTO 2125
2090 IF LO=FO(7)  GOTO 2205
2095 RETURN
2100 ? VP$(5) LI$ :? "\X91   YOU FOUND THE AMULET OF YENDOR! "
2105 R8$ = "FOUND IT!"
2110 R8=1  :REM PLAYER-YENDOR
2115 T0 = 5
2120 RETURN
2125 T1 = INT(RND(1)*8+1)
2130 ? VP$(5) LI$ :?"\X91   YOU FOUND" T1 "FOOD."
2135 R5=R5 + T1
2140 T0 = 5
2145 RETURN
2150 R3 = R3 + T1+T2+T3+T4
2155 AD$ = N$(T1) + "'S " + C$(T2) + A$(T3) + D$(T4)
2160 GOSUB 2260
2165 ? SPC(6) "YOU FOUND " AD$
2170 ?:? SPC(6) "REPLACE YOUR CURRENT ARMOR WITH IT? [YN] "; 
2175 GET YN$ :IF YN$<> "Y" AND YN$ <> "N" GOTO 2175
2180 GOSUB 2290
2185 IF YN$ = "N" THEN RETURN
2190 R7 = T1 + T2 +T3 + T4 
2195 R7$ = AD$
2200 RETURN
2205 R3 = R3 + T1+T2+T3+T4
2210 WD$ = N$(T1) + "'S " + C$(T2) + W$(T3) + D$(T4)
2215 GOSUB 2260
2220 ? SPC(6) "YOU FOUND " WD$
2225 ?:? SPC(6) "REPLACE YOUR CURRENT WEAPON WITH IT? [YN] "; 
2230 GET YN$ :IF YN$<> "Y" AND YN$ <> "N" GOTO 2230
2235 GOSUB 2290
2240 IF YN$ = "N" THEN RETURN
2245 R6 = T1 + T2 * 8 + T3 * 64 + T4 * 256
2250 R6$ = WD$
2255 RETURN
2260 ? VP$(51);
2265 ?SPC(6) LI$
2270 ?SPC(6) LI$
2275 ?SPC(6) LI$
2280 ?"\X91\X91\X91";
2285 RETURN
2290 ? "\X91\X91\X91\X91"
2295 ?SPC(6) LI$ 
2300 ?SPC(6) LI$ 
2305 ?SPC(6) LI$ 
2310 ?SPC(6) LI$
2315 RETURN
2320 REM INITIALIZE VERTICAL POSITION ARRAY
2325 IF VP=1 THEN RETURN
2330 DIM VP$(60)
2335 VP$(0)="\X13"
2340 FOR V=1 TO 60
2345    VP$(V) = VP$(V-1) + "\X11"
2350 NEXT
2355 VP=1
2360 RETURN
