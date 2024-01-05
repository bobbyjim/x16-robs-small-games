5 CLS
10 REM --------------------------------------
15 ? "  ÕÃÃÕÀÉÕÀÉÂ ÂÕÀÀ  ÕÃÃÕÀÉÕÃÃÕÀÀÕÀÃÀ²À "
20 ? "  Â  ÊÀËÊÀÛÊÀË«À   «À ÊÀËÂ  «À ÊÀÉ Â  "
25 ? "         ÀË   ÊÀÀ  Â        ÊÀÀÃÀË1.5 "
30 REM --------------------------------------
35 REM  INITIALIZE PLAYER
40 LONGVAR INVENTORY_WEAPONS%(10)
45 LONGVAR INVENTORY_ARMOR%(10)
50 GOSUB 1075  :REM INIT GAME
55 GOSUB 2160  :REM INIT SOUND
60 GOSUB 1620 :REM INIT PANELS
65 REM  LINE-CLEANER
70 LI$ = "                                                           "
75 GOSUB 2270 :REM SPLASH SCREEN
80 GOSUB 520  :REM BUILD FOREST
85 R1=INT(RND(1)*50+10)
90 R2=INT(RND(1)*30+10)
95 R3  = 0
100 R4 = 0
105 R5   = 1
110 R6 = 1
115 R7  = 0
120 R6$ = "RUSTY SPORK"
125 R7$ = "TUXEDO"
130 C1    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM STR
135 C2  = C1
140 C3    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM DEX
145 C4    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM END
150 C5    = INT(RND(1)*6)+INT(RND(1)*6)+2 :REM INT
155 C0(1) = C1
160 C0(2) = C3
165 C0(3) = C4
170 C0(4) = C5
175 R8$ = "NOT YET"
180 REM =====================================================================
185 REM
190 REM
195 REM                       DRAW THE SCREEN
200 REM
205 REM
210 REM =====================================================================
215 REM HIDE BACKGROUND
220 COLOR 1,6 :CLS :REM WHITE ON BLUE
225 GOSUB 2100 
230 L= 1  :T= 44 :W= 76  :H= 15  :L$= "" :GOSUB 2030
235 GOSUB 1850 :REM DRAW PANELS
240 GOSUB 430
245 REM  ----------------------------------
250 REM  PRINT PLAYER AND POSITION CURSOR
255 REM  ----------------------------------
260 LOCATE R2-3
265 COLOR 1,0 :REM TRANSPARENT (CLEARS FOG)
270 ? TAB(R1-4) C9$;
275 COLOR 1,6 :REM BLUE UNDER WHITE
280 ? L5$ TAB(R1) "@"
285 LOCATE 4
290 ? LI$ : ? "\X91   STR" C1 "SCORE:" R3 "HUNGER:" R4
295 IF RND(1)>0.95 THEN R4 = R4 + 1
300 T0 = T0 - 1
305 IF T0 = 0 THEN ? VP$(5) LI$
310 GET A$ :IF A$="" GOTO 310
315 XX=0 :YY=0
320 IF A$="\X11" THEN YY=+1 :REM DOWN
325 IF A$="\X91" THEN YY=-1 :REM UP
330 IF A$="\X1D" THEN XX=+1 :REM RIGHT
335 IF A$="\X9D" THEN XX=-1 :REM LEFT
340 IF A$="E" THEN GOSUB 470
345 IF (A$="." OR A$=" ") AND C1 < C2 THEN C1 = C1 + 1
350 LO = VPEEK(1,(R2+YY)*$100 + (R1+XX)*2)
355 IF LO < $20 THEN NN=LO :GOSUB 1475 :GOTO 370
360 IF LO > $22 AND LO < $2E THEN :GOSUB 2465 :GOSUB 430
365 IF LO < $41 THEN R1=R1+XX :R2=R2+YY :HU=HU+1: LO = VPEEK(1,R2*$100 + R1*2)
370 FOR NN=0 TO 31
375    AA=MA(NN)
380    XX=XM(NN)-R1
385    YY=YM(NN)-R2
390    IF ABS(XX)+ABS(YY) <= 1 THEN GOSUB 1400 :GOTO 405
395    IF RND(1)>0.5 THEN 405
400    IF ABS(XX) < 9 AND ABS(YY) < 9 THEN GOSUB 1290
405 NEXT
410 IF C1 < 1 GOTO 420
415 IF (R1<70) AND (R1>7) AND (R2<42) AND (R2>5) GOTO 260
420 GOSUB 950
425 END
430 LOCATE 46
435 ? SPC(6) LI$: ? "\X91" SPC(6) "WEAPON: " R6$ " (" R6 ")"
440 ? SPC(6) LI$: ? "\X91" SPC(6) "ARMOR : " R7$ " (" R7 ")"
445 ? SPC(6) LI$: ? "\X91" SPC(6) "FOOD  : " R5
450 ? SPC(6) LI$: ? "\X91" SPC(6) "AMULET: " R8$ 
455 ? SPC(6) LI$: ? "\X91" SPC(6) "SCORE : " R3
460 ?
465 RETURN
470 IF R5 < 1 THEN RETURN
475 IF R4 < 1 THEN RETURN
480 R5 = R5 - 1
485 R4 = R4 - 1
490 T0 = 5
495 LOCATE 5
500 ? LI$ :?"\X91   YOU FEEL BETTER."
505 GOSUB 430
510 RETURN
515 REM                         BUILD THE FOREST
520 REM  SET UP VIDEO REGISTERS
525 POKE $9F2D, %01100000 :REM MAP HEIGHT=1, WIDTH=2 = 64X128 TILES
530 POKE $9F2E, %10000000 :REM MAP BASE ADDR = 128 X512 = $10000.
535 POKE $9F2F, %11111000 :REM TILE BASE ADDR = $1F000.
540 REM ALSO, TILE HT=0, WD=0, SO 8 PIXELS X 8 PIXELS
545 REM  NO SCROLLING, PLEASE ($9F30-$9F33)
550 POKE $9F30, 0
555 POKE $9F31, 0
560 POKE $9F32, 0
565 POKE $9F33, 0
570 COLOR 1,0 :CLS  :REM 0=TRANSPARENT
575 POKE $9F29, %00110001 :REM $31=LAYERS 1,0. OUTPUT MODE=1 (VGA)
580 CC=4:CD=45
585 IF VPEEK(1,0) = 32 THEN CC=8 :CD=40
590 REM  UNROLLING THIS LOOP SHAVES 5 SECS
595 CL=$05  :REM BG/FG COLOR NYBBLES ($15=WHITE UNDER GREEN)
600 REM MONSTER DATA
605 MN=0
610 DIM MA(31)
615 DIM XM(31),YM(31)
620 DIM SO(31) :REM TERRAIN THE MONSTER IS .S.TANDING .O.N
625 POKE $9F25, PEEK($9F25) AND 254 : REM SELECT PORT 0
630 POKE $9F22, %00010001 : REM INCR 1 AND VRAM ADDR BIT 16
635 FOR Y=CC TO CD
640    Y0=Y*$100
645    FOR X=0 TO 158 STEP 8
650       P=Y0+X
655       POKE $9F21, INT(P/256) AND 255 :REM VRAM 15:8
660       POKE $9F20, P AND 255          :REM VRAM 7:0
665       IF Y<8 OR Y>40 GOTO 780
670       IF X<14 OR X>140 GOTO 780
675          FF=3
680          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
685          POKE $9F23, FO(FF) :REM CHARACTER INDEX
690          POKE $9F23, CL   :REM BG/FG COLOR NYBBLES
695          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
700          POKE $9F23, FO(FF)
705          POKE $9F23, CL
710          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
715          POKE $9F23, FO(FF)
720          POKE $9F23, CL
725          IF Y>8 AND Y<40 THEN FF=INT(RND(1)*3)
730          POKE $9F23, FO(FF)
735          POKE $9F23, CL
740          IF Y<9 OR Y>39 GOTO 820
745          REM TREASURE
750          IF RND(1) > 0.08 GOTO 820
755             XX = INT(RND(4))
760             VPOKE 1,P+2*XX,FO(5+INT(RND(1)*4))
765             POKE $9F25, PEEK($9F25) AND 254 : REM RE-SELECT PORT 0
770             POKE $9F22, %00010001 : REM INCR 1 AND VRAM ADDR BIT 16
775             GOTO 820
780             POKE $9F23, $20
785             POKE $9F23, CL
790             POKE $9F23, $20
795             POKE $9F23, CL
800             POKE $9F23, $20
805             POKE $9F23, CL
810             POKE $9F23, $20
815             POKE $9F23, CL
820    NEXT
825 NEXT
830 REM  PLACE MONSTERS
835 FOR M = 1 TO 26 :REM 0 TO 31
840    X = 10 + INT(RND(1)*60)
845    Y = 10 + INT(RND(1)*30)
850    XX = ABS(X-R1)
855    YY = ABS(Y-R2)
860    IF XX<6 AND YY<6 GOTO 840 :REM TOO CLOSE!
865    MA = Y * $100 + X * 2
870    MA(M) = MA
875    XM(M)=X
880    YM(M)=Y
885    SO(M) = VPEEK(1,MA)
890    VPOKE 1,MA,M
895 NEXT
900 REM AMULET OF YENDOR
905 VPOKE 1, INT(RND(1)*30+10) * $100 + INT(RND(1)*60+10) * 2, FO(10)
910 RETURN
915 REM ===================================
920 REM
925 REM
930 REM     GAME OVER - PRINT SCORE
935 REM
940 REM
945 REM ===================================
950 COLOR 1,$06 :REM WHITE ON BLUE
955 CLS
960 GOSUB 2100
965 L= 1  :T= 1  :W= 76  :H= 17  :L$= "GAME OVER"   :GOSUB 2030
970 GOSUB 1850
975 LOCATE 3,6
980 ? "YOUR ACCOMPLISHMENTS"
985 LOCATE 6,6
990 ? "ESCAPED THE FOREST: ";
995 IF C1 < 1 THEN ? "NO"
1000 IF C1 > 0 THEN ? "YES"
1005 LOCATE 9,6
1010 ? "FOUND THE AMULET:   ";
1015 IF R8 = 0 THEN ? "NO"
1020 IF R8 = 1 THEN ? "YES"
1025 LOCATE 12,6
1030 ? "SCORE:             " R3
1035 GOSUB 2215
1040 LOCATE 15,6
1045 ? "THANK YOU FOR PLAYING ROGUE FOREST!"
1050 LOCATE 18
1055 RETURN
1060 REM  ---------------------------------
1065 REM  INITIALIZE 
1070 REM  ---------------------------------
1075 L5$ = "\X91\X91\X91\X91\X91" :REM LEFT 5
1080 C9$ = "         \X11\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1085 C9$ = C9$+C9$+C9$+C9$+C9$+C9$+C9$+C9$+C9$
1090 DIM M$(31)
1095 FOR X=0 TO 31 :READ M$(X) :NEXT
1100 DATA  DOPPLEGANGER,  ABALONE,  BASILISK,    CENTAUR,    DRAGON
1105 DATA  ETTIN,         FUZZY,    GHOUL,       HELLHOUND,  IMP
1110 DATA  JINN,          KOBOLD,   LEPRECHAUN,  MINOTAUR,   NYMPH
1115 DATA  ORC,           PYTHON,   QUAGGA,      RATTLER,    SKELETON
1120 DATA  TROLL,         UR-VILE,  VAMPIRE,     WRAITH,     XEROC
1125 DATA  YETI,          ZOMBIE,   WYVERN,      MANTICORE,  VELOCIRAPTOR
1130 DATA  SPHINX,        SPIDER
1135 FOR X=0 TO 7 :READ N$(X) :NEXT
1140 DATA CHENEY, DURIN, FARGOL, KHAALO, REJNALDI, SARA, SHARIK, ZOG
1145 FOR X=0 TO 7 :READ C$(X) :NEXT
1150 DATA "","RED ","GREEN ","BLUE ","TIN ","WHITE ","PALE ","GOLD "
1155 FOR X=0 TO 7 :READ D$(X) :NEXT
1160 DATA ""," OF WAR"," OF AGILITY"," OF PAIN"," OF CUNNING"
1165 DATA " OF SKILL",""," OF MAGIC"
1170 FOR X=0 TO 7 :READ W$(X) :NEXT
1175 DATA DAGGER, STAFF, HAMMER, MACHETE, AXE, SWORD, LABRYS, HALBERD
1180 FOR X=0 TO 7 :READ A$(X) :NEXT
1185 DATA LEATHER, HAUBERK, RINGMAIL, MESH, MAIL, SPLINT, PLATE, MITHRIL
1190 REM FOREST ELEMENTS
1195 FO(0) = $41 :REM TREE
1200 FO(1) = $2E :REM DOT
1205 FO(2) = $2E :REM DOT
1210 FO(3) = $43 :REM HORIZ LINE
1215 FO(4) = $42 :REM VERT LINE
1220 FO(5) = $23 :REM ARMOR (#)
1225 FO(6) = $24 :REM FOOD ($)
1230 FO(7) = $27 :REM WEAPON (')
1235 FO(8) = $27
1240 FO(9) = $24
1245 FO(10) = $2A :REM AMULET OF YENDOR
1250 RETURN
1255 REM =================================================================
1260 REM
1265 REM
1270 REM       MOVE MONSTER:NN ADDRESS:AA RELATIVE_POS:XX,YY
1275 REM
1280 REM
1285 REM =================================================================
1290 VPOKE 1,AA,SO(NN)  :REM REDRAW TILE
1295 DX=0 :DY=0
1300 IF RND(1)>0.5 GOTO 1315
1305 IF XX<0 THEN AA=AA+2 :DX=1 :GOTO 1325
1310 IF XX>0 THEN AA=AA-2 :DX=-1 :GOTO 1325
1315 IF YY<0 THEN AA=AA+$100 :DY=1 :GOTO 1325
1320 IF YY>0 THEN AA=AA-$100 :DY=-1 :GOTO 1325
1325 V=VPEEK(1,AA)
1330 IF V < $20 OR V = $2A GOTO 1360
1335 SO(NN)=VPEEK(1,AA)
1340 MA(NN)=AA
1345 XM(NN)=XM(NN)+DX
1350 YM(NN)=YM(NN)+DY
1355 VPOKE 1,AA,NN
1360 RETURN
1365 REM =================================================================
1370 REM
1375 REM
1380 REM                  MONSTER NN ATTACKS
1385 REM
1390 REM
1395 REM =================================================================
1400 LOCATE 51, 6
1405 ? LI$ : ? "\X91";
1410 W = INT(RND(1)*4)
1415 IF INT(RND(1)*40) < R7 THEN W=0
1420 IF W=0 THEN ? SPC(6) "THE " M$(NN) " MISSES."
1425 IF W>0 THEN ? SPC(6) "THE " M$(NN) " HITS YOU FOR" W "POINTS." :GOSUB 2190
1430 C1 = C1 - W
1435 RETURN
1440 REM =================================================================
1445 REM
1450 REM
1455 REM                 ATTACK MONSTER NN
1460 REM
1465 REM
1470 REM =================================================================
1475 LOCATE 52, 6
1480 ? LI$ : ? "\X91";
1485 T1 = R6 AND 7
1490 T2 = (R6/8) AND 7
1495 T3 = (R6/64) AND 7
1500 T4 = (R6/256) AND 7
1505 T5 = T1+T2+T3+T4+1 - R4
1510 IF T5 < 2 THEN T5 = 2
1515 W = INT(RND(1)*T5)
1520 HU=HU+1
1525 N=RND(1) * NN
1530 ? SPC(6) "YOU HIT THE " M$(NN) " FOR" W "POINTS"; :GOSUB 2190
1535 IF W > N THEN ? ", KILLING IT";: VPOKE 1,MA(NN),$2E :YM(NN)=-1000 :R3=R3+W
1540 ? "."
1545 RETURN
1550 REM -----------------------------------------
1555 REM
1560 REM  PANEL UTILITY 
1565 REM
1570 REM -----------------------------------------
1575 REM
1580 REM  NIFTY DISPLAY PANELS
1585 REM
1590 REM 
1595 REM ÕÃÃÃÃÃÃÃÉ
1600 REM Â       Â
1605 REM Â       Â
1610 REM ÊÃÃÃÃÃÃÃË
1615 REM 
1620 REM  ------------------------------------
1625 REM  SET UP PANEL ARRAYS BASED ON MAX
1630 REM  NUMBER OF PANELS (N=6)
1635 REM  ------------------------------------
1640 N = 6
1645 P7 = 0
1650 DIM P0$(N)
1655 DIM P1(N), P2(N), P3(N), P4(N)
1660 DIM P5$(N,30), P6(N)
1665 REM  -----------------------------------
1670 REM  HORIZONTAL BAR ARRAY
1675 REM  -----------------------------------
1680 DIM HB$(80)
1685 HB$(0) = "\XC3"
1690 FOR V=1 TO 80
1695    HB$(V) = HB$(V-1) + "\XC3"
1700 NEXT
1705 REM  -----------------------------------
1710 REM  VERTICAL BAR ARRAY
1715 REM  -----------------------------------
1720 DIM VB$(30)
1725 VB$(0) = "\XC2\X11\X9D"
1730 FOR V=1 TO 30
1735    VB$(V) = VB$(V-1) + "\XC2\X11\X9D"
1740 NEXT
1745 REM  ----------------------------------
1750 REM  PANEL EDGES
1755 REM  ----------------------------------
1760 BA$ = "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1765 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1770 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1775 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1780 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1785 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1790 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1795 BA$ = BA$ + "\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D\X9D"
1800 SP$ = "                    "
1805 SP$ = SP$ + "                    "
1810 SP$ = SP$ + "                    "
1815 SP$ = SP$ + "                    "
1820 RETURN
1825 REM ------------------------------------
1830 REM
1835 REM DRAW PANELS FROM VARIABLE DATA
1840 REM
1845 REM ------------------------------------
1850 FOR N = 0 TO P7 - 1
1855    LOCATE P1(N), P2(N)
1860    ? "\XD5\XC3" P0$(N) "\XC3";
1865    ? HB$(P3(N) - 3 - LEN(P0$(N)));
1870    ? "\XC9\X11";
1875    B2$ = LEFT$(BA$, P3(N)+2)
1880    S2$ = LEFT$(SP$, P3(N))
1885    FOR RR=3 TO P4(N)
1890    REM DON'T FORGET THE LINES !!
1895    ? B2$ "\XC2" S2$ "\XC2\X11";
1900    NEXT
1905    ? B2$ "\XCA";
1910    ? HB$(P3(N)-1);
1915    ? "\XCB";
1920 NEXT
1925 RETURN
1930 REM ------------------------------------
1935 REM
1940 REM SHOW THE PANEL TEXTS
1945 REM
1950 REM ------------------------------------
1955 FOR N = 0 TO P7 - 1
1960    LOCATE P1(N)
1965    ?
1970    FOR X = 0 TO P4(N)-3
1975       ? SPC(P2(N)+2) P5$(N,X)
1980    NEXT
1985 NEXT
1990 RETURN
1995 REM ------------------------------------
2000 REM
2005 REM ADD A PANEL TO THE LIST!
2010 REM T: TOP, L: LEFT, W: WIDTH, H:HEIGHT
2015 REM L$: LABEL
2020 REM
2025 REM ------------------------------------
2030 N = P7
2035 P0$(N) = L$
2040 P1(N) = T
2045 P2(N) = L
2050 P3(N) = W
2055 P4(N) = H
2060 P6(N) = -1
2065 P7 = N + 1
2070 RETURN
2075 REM ------------------------------------------------------
2080 REM
2085 REM "DELETE" ALL PANELS
2090 REM
2095 REM ------------------------------------------------------
2100 P7 = 0
2105 RETURN
2110 REM ------------------------------------------------------
2115 REM
2120 REM SET CONTENTS OF A PANEL
2125 REM N:PANEL NUM, BU$(): LINES, LC: LINE COUNT
2130 REM
2135 REM ------------------------------------------------------
2140 FOR X = 0 TO LC-1
2145    P5$(N,X) = BU$(X)
2150 NEXT
2155 RETURN
2160 IF PEEK($400)=0 THEN LOAD "RF-EFFECTS.PRG",8,1,$0400
2165 RETURN
2170 SYS $0400
2175 T = TI 
2180 IF TI < T + 100 GOTO 2180
2185 RETURN
2190 SYS $0403
2195 T = TI 
2200 IF TI < T + 100 GOTO 2200
2205 RETURN
2210 RETURN
2215 SYS $0409
2220 T = TI
2225 IF TI < T + 300 GOTO 2225
2230 RETURN
2235 REM =================================================================
2240 REM
2245 REM
2250 REM                       "SPLASH" SCREEN
2255 REM
2260 REM
2265 REM =================================================================
2270 L= 1  :T= 5  :W= 76  :H= 13  :L$= "WELCOME"     :GOSUB 2030
2275 L= 1  :T= 19 :W= 76  :H= 15  :L$= "MAP KEY"     :GOSUB 2030
2280 L= 1  :T= 35 :W= 76  :H= 12  :L$= "COMMAND KEY" :GOSUB 2030
2285 GOSUB 1850 :REM DRAW PANELS
2290 LOCATE 8
2295 ? SPC(6) "FIND THE AMULET OF YENDOR AND EXIT THE FOREST ALIVE!"
2300 ?
2305 ? SPC(6) "POINTS ARE AWARDED FOR ITEMS COLLECTED AND MONSTERS KILLED."
2310 LOCATE 21
2315 ? SPC(6) "@ ... YOU"
2320 ? SPC(6) CHR$(FO(0)) " ... TREE (IMPASSABLE BY YOU)"
2325 ? SPC(6) CHR$(FO(1)) " ... CLEARING"
2330 ? SPC(6) CHR$(FO(5)) " ... ARMOR"
2335 ? SPC(6) CHR$(FO(6)) " ... FOOD"
2340 ? SPC(6) CHR$(FO(7)) " ... WEAPON"
2345 ? SPC(6) CHR$(FO(10))" ... AMULET OF YENDOR"
2350 ?:?
2355 ? SPC(6) "-- ANYTHING THAT MOVES IS A MONSTER --"
2360 LOCATE 37
2365 ? SPC(6) "? ............ HELP"
2370 ? SPC(6) "CURSOR KEYS .. MOVE / ATTACK"
2375 ? SPC(6) ". ............ REST"
2380 ? SPC(6) "E ............ EAT FOOD"
2385 ? SPC(6) "X ............ EXIT"
2390 ?
2395 LOCATE 50
2400 ? "   GOOD LUCK!"
2405 ?
2410 GOSUB 2215
2415 INPUT "   PLEASE TYPE YOUR NAME"; R0$
2420 X=RND(-TI)
2425 RETURN
2430 REM =================================================================
2435 REM
2440 REM
2445 REM                   TREASURE FINDS (TYPE IN 'LO')
2450 REM
2455 REM
2460 REM =================================================================
2465 COLOR 1,6
2470 REM CLEAR THE SQUARE THE PLAYER IS *LOOKING AT*
2475 VPOKE 1,(R2+YY)*$100 + (R1+XX)*2, $2E
2480 GOSUB 2170
2485 REM  ----------------------
2490 REM  PRE-CALC SOME VALUES
2495 REM  ----------------------
2500 T1=INT(RND(1)*8)
2505 T2=INT(RND(1)*8)
2510 T3=INT(RND(1)*8)
2515 T4=INT(RND(1)*8)
2520 REM  ------------------------
2525 REM  FIGURE OUT WHAT WE GOT
2530 REM  ------------------------
2535 IF LO=FO(10) GOTO 2560
2540 IF LO=FO(5)  GOTO 2620
2545 IF LO=FO(6)  GOTO 2590
2550 IF LO=FO(7)  GOTO 2675
2555 RETURN
2560 LOCATE 5
2565 ? LI$ :? "\X91   YOU FOUND THE AMULET OF YENDOR! "
2570 R8$ = "FOUND IT!"
2575 R8=1  :REM PLAYER-YENDOR
2580 T0 = 5
2585 RETURN
2590 T1 = INT(RND(1)*8+1)
2595 LOCATE 5
2600 ? LI$ :?"\X91   YOU FOUND" T1 "FOOD."
2605 R5=R5 + T1
2610 T0 = 5
2615 RETURN
2620 R3 = R3 + T1+T2+T3+T4
2625 AD$ = N$(T1) + "'S " + C$(T2) + A$(T3) + D$(T4)
2630 GOSUB 2730
2635 ? SPC(6) "YOU FOUND " AD$
2640 ?:? SPC(6) "REPLACE YOUR CURRENT ARMOR WITH IT? [YN] "; 
2645 GET YN$ :IF YN$<> "Y" AND YN$ <> "N" GOTO 2645
2650 GOSUB 2760
2655 IF YN$ = "N" THEN RETURN
2660 R7 = T1 + T2 +T3 + T4 
2665 R7$ = AD$
2670 RETURN
2675 R3 = R3 + T1+T2+T3+T4
2680 WD$ = N$(T1) + "'S " + C$(T2) + W$(T3) + D$(T4)
2685 GOSUB 2730
2690 ? SPC(6) "YOU FOUND " WD$
2695 ?:? SPC(6) "REPLACE YOUR CURRENT WEAPON WITH IT? [YN] "; 
2700 GET YN$ :IF YN$<> "Y" AND YN$ <> "N" GOTO 2700
2705 GOSUB 2760
2710 IF YN$ = "N" THEN RETURN
2715 R6 = T1 + T2 * 8 + T3 * 64 + T4 * 256
2720 R6$ = WD$
2725 RETURN
2730 LOCATE 51
2735 ?SPC(6) LI$
2740 ?SPC(6) LI$
2745 ?SPC(6) LI$
2750 ?"\X91\X91\X91";
2755 RETURN
2760 ? "\X91\X91\X91\X91"
2765 ?SPC(6) LI$ 
2770 ?SPC(6) LI$ 
2775 ?SPC(6) LI$ 
2780 ?SPC(6) LI$
2785 RETURN
