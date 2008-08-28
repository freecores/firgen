
[info]

userid    =     xyz;

username  =     Mustermann;
project   =     TestFIR;
version   =     1 ;
revision  =     0;
description  =  "Project";
company =       company  ;
date    =       04/07/09;

[optimizer]

mode    = 0;          # 0-4 möglich
effort  = 1000 1000;      # 0-100 möglich

logging = 4;          # loglevel 0-9

resources  :                    # das heißt jetzt kommt estwas mehrzeiliges / mehrspaltiges
            LogicBlocks => 10;      # synopsis:  name der ressource  => gewichtung
            FlipFlops   => 10;
            ShortLines  => 10;      # synopsis:  name der ressource  => gewichtung
            LongLines   => 10;


[architecture]

HDLang = VHDL87;                          # VHDL87; VHDL93; matlab; plainC  ... sofern supported

in_width  = 14;                          # 1-128 maximalwert kann verändert werden
out_width = 0;

scaling_limit = 0;                     # 0<scaling_limit   type float
deviation = 0;                           # 0=<deviation<scaling_limit/2  type float




[technology]

partno  = xilinx,XCVE;

synthesis :

            DELAY   => 0  12  11  12;     #s
            P_MULT  => 12  11  11  12;     #Sie sollten unterschiedlich heissen
            S_MULT  => 12  11  11  12;
            ADDER   => 12  12  33  12;
            A_SUB   => 12  12  44  12;
            B_SUB   => 12  12  55  12;



specification = "irgendeine Virtex";

coefficients:
0.0078;
-0.0117;
-0.0039;
0.0156;
-0.0469;
0.0469;
0.0352;
-0.1016;
0.1484;
-0.0938;
-0.1953;
0.5273;
-0.8320;
1.0000;
-0.8320;
0.5273;
-0.1953;
-0.0938;
0.1484;
-0.1016;
0.0352;
0.0469;
-0.0469;
0.0156;
-0.0039;
-0.0117;
0.0078;

# extras für filehandling und den der so ein file per hand schreibt


debug   = 0;            # 0  no debug mode
                        # 1  adding 20 coefficients by random
                        # 2  adding 40 coefficients by random
                        # 3  adding 100 coefficients by random
                        # 4  adding 1000 coefficients by random




