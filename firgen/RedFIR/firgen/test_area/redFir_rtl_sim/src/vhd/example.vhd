--------------------------------------------------------------------------------
-- This sourcecode was automaticaly generated by wizard redFIR(TM)
-- optmizer version : 2.030908ALPHA_FHF
--------------------------------------------------------------------------------
-- redFIR constraints: 
--	date : 04/07/09
--	username xyz ,	userID  : Mustermann
--	project : TestFIR
--	version : 1,	revision: 0
--	revision: 0
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- disclaimer:
--	redFir in a registered trademark of 
--	Fraunhofer Institute for Integrated Circuits
--	Am Wolfsmantel 13 / 91058 Erlangen / Germany
-- http://www.iis.fraunhofer.de 	 redfir@iis.fhg.de
-- look an http://www.redfir.de for further information and updates 
--------------------------------------------------------------------------------



library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

library work;
use work.components_redfir.all; 	-- modify packagename for other technologies



--------------------------------------------------------------------------------
entity TestFIR is 
generic (
	flag_clk : integer := 1; 	-- this flag is to enable the clock for the coefficient structure
	flag_rst : integer := 1; 	-- this flag is to enable async reset in modules
	flag_en  : integer := 1); 	-- this flag is to enable the 'en' signale for data enable
port (
	rst      : in std_logic;  	-- asynchronous reset signal
	en       : in std_logic;  	-- data enable signal
	clk      : in std_logic;  	-- clock signal
	in_data  : in std_logic_vector  (13 downto 0);   	-- incomming data
	out_data : out std_logic_vector (27 downto 0) ); 	-- outgoing data
end  TestFIR;
--------------------------------------------------------------------------------



architecture TestFIR_arch of TestFIR is 

-- Signal declaration for coefficient structure --
-- structure will contain 3 lines and  27 coefficients




 -- signal identifications are

-- 0.00151961517002397600 
-- -0.00227942275503596400 
-- -0.00075980758501198800 
-- 0.00303923034004795201 
-- -0.00913717359623189562 
-- 0.00913717359623189562 
-- 0.00685775065975337446 
-- -0.01979396199960265293 
-- 0.02891165229397627684 
-- -0.01827434719246379125 
-- -0.03804882661597840804 
-- 0.10272988476676525593 
-- -0.16209229254410656429 
-- 0.19482246298047756206 
-- -0.16209229254410656429 
-- 0.10272988476676525593 
-- -0.03804882661597840804 
-- -0.01827434719246379125 
-- 0.02891165229397627684 
-- -0.01979396199960265293 
-- 0.00685775065975337446 
-- 0.00913717359623189562 
-- -0.00913717359623189562 
-- 0.00303923034004795201 
-- -0.00075980758501198800 
-- -0.00227942275503596400 
-- 0.00151961517002397600 


-- mapped coefficients 

  signal sg_in : signed (13 downto 0);


--------------------------------------------------------------------------------
  signal sg_0_1_module 	:	signed(14 downto 0);
  signal sg_0_3_module 	:	signed(15 downto 0);
  signal sg_0_5_module 	:	signed(16 downto 0);
  signal sg_0_7_module 	:	signed(16 downto 0);
  signal sg_0_9_module 	:	signed(17 downto 0);
  signal sg_0_127_module 	:	signed(20 downto 0);
  signal sg_1_7_module 	:	signed(16 downto 0);
  signal sg_1_11_module 	:	signed(17 downto 0);
  signal sg_1_13_module 	:	signed(17 downto 0);
  signal sg_1_29_module 	:	signed(18 downto 0);
  signal sg_1_45_module 	:	signed(19 downto 0);
  signal sg_1_103_module 	:	signed(20 downto 0);
  signal sg_1_105_module 	:	signed(20 downto 0);
  signal sg_1_169_module 	:	signed(21 downto 0);
  signal sg_1_351_module 	:	signed(22 downto 0);
  signal sg_1_2561_module 	:	signed(25 downto 0);
  signal sg_2_25_module 	:	signed(18 downto 0);
  signal sg_2_149_module 	:	signed(21 downto 0);
  signal sg_2_199_module 	:	signed(21 downto 0);
  signal sg_2_399_module 	:	signed(22 downto 0);
  signal sg_2_449_module 	:	signed(22 downto 0);
  signal sg_2_599_module 	:	signed(23 downto 0);
  signal sg_2_1247_module 	:	signed(24 downto 0);
  signal sg_2_1297_module 	:	signed(24 downto 0);
  signal sg_2_1895_module 	:	signed(24 downto 0);
  signal sg_2_6733_module 	:	signed(26 downto 0);
  signal sg_2_10623_module 	:	signed(27 downto 0);


  signal sg_0_coef	:	signed (20 downto 0);
  signal sg_1_coef	:	signed (21 downto 0);
  signal sg_2_coef	:	signed (22 downto 0);
  signal sg_3_coef	:	signed (22 downto 0);
  signal sg_4_coef	:	signed (24 downto 0);
  signal sg_5_coef	:	signed (24 downto 0);
  signal sg_6_coef	:	signed (25 downto 0);
  signal sg_7_coef	:	signed (25 downto 0);
  signal sg_8_coef	:	signed (26 downto 0);
  signal sg_9_coef	:	signed (26 downto 0);
  signal sg_10_coef	:	signed (27 downto 0);
  signal sg_11_coef	:	signed (27 downto 0);
  signal sg_12_coef	:	signed (28 downto 0);
  signal sg_13_coef	:	signed (29 downto 0);
  signal sg_14_coef	:	signed (29 downto 0);
  signal sg_15_coef	:	signed (29 downto 0);
  signal sg_16_coef	:	signed (29 downto 0);
  signal sg_17_coef	:	signed (29 downto 0);
  signal sg_18_coef	:	signed (29 downto 0);
  signal sg_19_coef	:	signed (29 downto 0);
  signal sg_20_coef	:	signed (29 downto 0);
  signal sg_21_coef	:	signed (29 downto 0);
  signal sg_22_coef	:	signed (29 downto 0);
  signal sg_23_coef	:	signed (29 downto 0);
  signal sg_24_coef	:	signed (29 downto 0);
  signal sg_25_coef	:	signed (29 downto 0);
  signal sg_26_coef	:	signed (30 downto 0);


-- adder chain signals 

  signal sg_zero		:	signed (31 downto 0);


--------------------------------------------------------------------------------
  signal sg_0_chain	:	signed (21 downto 0);
  signal sg_1_chain	:	signed (22 downto 0);
  signal sg_2_chain	:	signed (22 downto 0);
  signal sg_3_chain	:	signed (24 downto 0);
  signal sg_4_chain	:	signed (24 downto 0);
  signal sg_5_chain	:	signed (25 downto 0);
  signal sg_6_chain	:	signed (25 downto 0);
  signal sg_7_chain	:	signed (26 downto 0);
  signal sg_8_chain	:	signed (26 downto 0);
  signal sg_9_chain	:	signed (27 downto 0);
  signal sg_10_chain	:	signed (27 downto 0);
  signal sg_11_chain	:	signed (28 downto 0);
  signal sg_12_chain	:	signed (29 downto 0);
  signal sg_13_chain	:	signed (29 downto 0);
  signal sg_14_chain	:	signed (29 downto 0);
  signal sg_15_chain	:	signed (29 downto 0);
  signal sg_16_chain	:	signed (29 downto 0);
  signal sg_17_chain	:	signed (29 downto 0);
  signal sg_18_chain	:	signed (29 downto 0);
  signal sg_19_chain	:	signed (29 downto 0);
  signal sg_20_chain	:	signed (29 downto 0);
  signal sg_21_chain	:	signed (29 downto 0);
  signal sg_22_chain	:	signed (29 downto 0);
  signal sg_23_chain	:	signed (29 downto 0);
  signal sg_24_chain	:	signed (29 downto 0);
  signal sg_25_chain	:	signed (30 downto 0);
  signal sg_26_chain	:	signed (30 downto 0);
--------------------------------------------------------------------------------
begin

 -- b_InMapping:   mapping of input signal from std_logic to signed
--------------------------------------------------------------------------------
    sg_in(13 downto 0)<=signed(in_data(13 downto 0));
--  end b_InMapping; 

  --b_CofficientStructure: coefficient structure graph
--------------------------------------------------------------------------------
   i_0_node: DELAY --line: 0 result: 1 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>15, gamma=>0)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_1_module);

   i_1_node: P_MULT --line: 0 result: 3 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>16, gamma=>1)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_3_module);

   i_2_node: P_MULT --line: 0 result: 5 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>17, gamma=>2)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_5_module);

   i_3_node: S_MULT --line: 0 result: 7 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>17, gamma=>3)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_7_module);

   i_4_node: P_MULT --line: 0 result: 9 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>18, gamma=>3)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_9_module);

   i_5_node: S_MULT --line: 0 result: 127 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
      a_width=>14, b_width=>14, o_width=>21, gamma=>7)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_in, IN_B=>sg_in,
       OUT_DATA=>sg_0_127_module);

   i_6_node: DELAY --line: 1 result: 7 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>17, b_width=>17,
       o_width=>17, gamma=>0)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_7_module, IN_B=>sg_0_7_module,
       OUT_DATA=>sg_1_7_module);

   i_7_node: ADDER --line: 1 result: 11 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>17, b_width=>15,
       o_width=>18, gamma=>2)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_7_module, IN_B=>sg_0_1_module,
       OUT_DATA=>sg_1_11_module);

   i_8_node: ADDER --line: 1 result: 13 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>17, b_width=>15,
       o_width=>18, gamma=>3)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_5_module, IN_B=>sg_0_1_module,
       OUT_DATA=>sg_1_13_module);

   i_9_node: ADDER --line: 1 result: 29 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>17,
       o_width=>19, gamma=>2)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_9_module, IN_B=>sg_0_5_module,
       OUT_DATA=>sg_1_29_module);

   i_10_node: S_MULT --line: 1 result: 45 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>16, b_width=>16,
       o_width=>20, gamma=>4)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_3_module, IN_B=>sg_0_3_module,
       OUT_DATA=>sg_1_45_module);

   i_11_node: ADDER --line: 1 result: 103 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>17, b_width=>16,
       o_width=>21, gamma=>5)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_7_module, IN_B=>sg_0_3_module,
       OUT_DATA=>sg_1_103_module);

   i_12_node: ADDER --line: 1 result: 105 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>16,
       o_width=>21, gamma=>5)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_9_module, IN_B=>sg_0_3_module,
       OUT_DATA=>sg_1_105_module);

   i_13_node: ADDER --line: 1 result: 169 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>17,
       o_width=>22, gamma=>5)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_9_module, IN_B=>sg_0_5_module,
       OUT_DATA=>sg_1_169_module);

   i_14_node: ADDER --line: 1 result: 351 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>17,
       o_width=>23, gamma=>5)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_127_module, IN_B=>sg_0_7_module,
       OUT_DATA=>sg_1_351_module);

   i_15_node: ADDER --line: 1 result: 2561 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>15, b_width=>17,
       o_width=>26, gamma=>9)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_0_1_module, IN_B=>sg_0_5_module,
       OUT_DATA=>sg_1_2561_module);

   i_16_node: ADDER --line: 2 result: 25 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>17,
       o_width=>19, gamma=>1)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_11_module, IN_B=>sg_1_7_module,
       OUT_DATA=>sg_2_25_module);

   i_17_node: ADDER --line: 2 result: 149 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>20, b_width=>18,
       o_width=>22, gamma=>3)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_45_module, IN_B=>sg_1_13_module,
       OUT_DATA=>sg_2_149_module);

   i_18_node: A_SUB --line: 2 result: 199 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>17,
       o_width=>22, gamma=>1)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_103_module, IN_B=>sg_1_7_module,
       OUT_DATA=>sg_2_199_module);

   i_19_node: A_SUB --line: 2 result: 399 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>18,
       o_width=>23, gamma=>2)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_103_module, IN_B=>sg_1_13_module,
       OUT_DATA=>sg_2_399_module);

   i_20_node: ADDER --line: 2 result: 449 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>19, b_width=>21,
       o_width=>23, gamma=>2)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_29_module, IN_B=>sg_1_105_module,
       OUT_DATA=>sg_2_449_module);

   i_21_node: A_SUB --line: 2 result: 599 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>21,
       o_width=>24, gamma=>6)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_11_module, IN_B=>sg_1_105_module,
       OUT_DATA=>sg_2_599_module);

   i_22_node: A_SUB --line: 2 result: 1247 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>22, b_width=>21,
       o_width=>25, gamma=>3)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_169_module, IN_B=>sg_1_105_module,
       OUT_DATA=>sg_2_1247_module);

   i_23_node: A_SUB --line: 2 result: 1297 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>23,
       o_width=>25, gamma=>4)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_103_module, IN_B=>sg_1_351_module,
       OUT_DATA=>sg_2_1297_module);

   i_24_node: ADDER --line: 2 result: 1895 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>17,
       o_width=>25, gamma=>8)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_103_module, IN_B=>sg_1_7_module,
       OUT_DATA=>sg_2_1895_module);

   i_25_node: ADDER --line: 2 result: 6733 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>18, b_width=>21,
       o_width=>27, gamma=>6)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_13_module, IN_B=>sg_1_105_module,
       OUT_DATA=>sg_2_6733_module);

   i_26_node: A_SUB --line: 2 result: 10623 
     generic map (
      flag_clk=>flag_clk, flag_rst=>flag_rst, flag_en=>flag_en,
       a_width=>21, b_width=>26,
       o_width=>28, gamma=>7)
     port map (
       clk=>clk, rst=>rst, en=>en,
       IN_A=>sg_1_103_module, IN_B=>sg_1_2561_module,
       OUT_DATA=>sg_2_10623_module);

-- end b_CoefficientStructure 


--  b_MappingUnit: -- mapping coefficents to nedded width
--------------------------------------------------------------------------------
    i_0_map: REMAP
    generic map (
      in_width=>19, gamma=>2, out_width=>21)
    port map (
      IN_MAP=>sg_2_25_module, OUT_MAP=>sg_0_coef);


    i_1_map: REMAP
    generic map (
      in_width=>22, gamma=>0, out_width=>22)
    port map (
      IN_MAP=>sg_2_149_module, OUT_MAP=>sg_1_coef);


    i_2_map: REMAP
    generic map (
      in_width=>19, gamma=>1, out_width=>23)
    port map (
      IN_MAP=>sg_2_25_module, OUT_MAP=>sg_2_coef);


    i_3_map: REMAP
    generic map (
      in_width=>22, gamma=>0, out_width=>23)
    port map (
      IN_MAP=>sg_2_199_module, OUT_MAP=>sg_3_coef);


    i_4_map: REMAP
    generic map (
      in_width=>24, gamma=>0, out_width=>25)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_4_coef);


    i_5_map: REMAP
    generic map (
      in_width=>24, gamma=>0, out_width=>25)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_5_coef);


    i_6_map: REMAP
    generic map (
      in_width=>23, gamma=>0, out_width=>26)
    port map (
      IN_MAP=>sg_2_449_module, OUT_MAP=>sg_6_coef);


    i_7_map: REMAP
    generic map (
      in_width=>25, gamma=>0, out_width=>26)
    port map (
      IN_MAP=>sg_2_1297_module, OUT_MAP=>sg_7_coef);


    i_8_map: REMAP
    generic map (
      in_width=>25, gamma=>0, out_width=>27)
    port map (
      IN_MAP=>sg_2_1895_module, OUT_MAP=>sg_8_coef);


    i_9_map: REMAP
    generic map (
      in_width=>24, gamma=>1, out_width=>27)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_9_coef);


    i_10_map: REMAP
    generic map (
      in_width=>25, gamma=>1, out_width=>28)
    port map (
      IN_MAP=>sg_2_1247_module, OUT_MAP=>sg_10_coef);


    i_11_map: REMAP
    generic map (
      in_width=>27, gamma=>0, out_width=>28)
    port map (
      IN_MAP=>sg_2_6733_module, OUT_MAP=>sg_11_coef);


    i_12_map: REMAP
    generic map (
      in_width=>28, gamma=>0, out_width=>29)
    port map (
      IN_MAP=>sg_2_10623_module, OUT_MAP=>sg_12_coef);


    i_13_map: REMAP
    generic map (
      in_width=>23, gamma=>5, out_width=>30)
    port map (
      IN_MAP=>sg_2_399_module, OUT_MAP=>sg_13_coef);


    i_14_map: REMAP
    generic map (
      in_width=>28, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_10623_module, OUT_MAP=>sg_14_coef);


    i_15_map: REMAP
    generic map (
      in_width=>27, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_6733_module, OUT_MAP=>sg_15_coef);


    i_16_map: REMAP
    generic map (
      in_width=>25, gamma=>1, out_width=>30)
    port map (
      IN_MAP=>sg_2_1247_module, OUT_MAP=>sg_16_coef);


    i_17_map: REMAP
    generic map (
      in_width=>24, gamma=>1, out_width=>30)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_17_coef);


    i_18_map: REMAP
    generic map (
      in_width=>25, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_1895_module, OUT_MAP=>sg_18_coef);


    i_19_map: REMAP
    generic map (
      in_width=>25, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_1297_module, OUT_MAP=>sg_19_coef);


    i_20_map: REMAP
    generic map (
      in_width=>23, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_449_module, OUT_MAP=>sg_20_coef);


    i_21_map: REMAP
    generic map (
      in_width=>24, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_21_coef);


    i_22_map: REMAP
    generic map (
      in_width=>24, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_599_module, OUT_MAP=>sg_22_coef);


    i_23_map: REMAP
    generic map (
      in_width=>22, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_199_module, OUT_MAP=>sg_23_coef);


    i_24_map: REMAP
    generic map (
      in_width=>19, gamma=>1, out_width=>30)
    port map (
      IN_MAP=>sg_2_25_module, OUT_MAP=>sg_24_coef);


    i_25_map: REMAP
    generic map (
      in_width=>22, gamma=>0, out_width=>30)
    port map (
      IN_MAP=>sg_2_149_module, OUT_MAP=>sg_25_coef);


    i_26_map: REMAP
    generic map (
      in_width=>19, gamma=>2, out_width=>31)
    port map (
      IN_MAP=>sg_2_25_module, OUT_MAP=>sg_26_coef);


--  end b_MappingUnit;

--  b_AdderChain:chain to add taps
--------------------------------------------------------------------------------
    sg_zero<=(others=>'0');

    i_0_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>21, out_width=>22)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_zero(20 downto 0), IN_COEF=>sg_0_COEF,
      OUT_CHAIN=>sg_0_chain);


    i_1_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>22, out_width=>23)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_0_chain, IN_COEF=>sg_1_COEF,
      OUT_CHAIN=>sg_1_chain);


    i_2_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>23, out_width=>23)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_1_chain, IN_COEF=>sg_2_COEF,
      OUT_CHAIN=>sg_2_chain);


    i_3_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>23, out_width=>25)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_2_chain, IN_COEF=>sg_3_COEF,
      OUT_CHAIN=>sg_3_chain);


    i_4_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>25, out_width=>25)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_3_chain, IN_COEF=>sg_4_COEF,
      OUT_CHAIN=>sg_4_chain);


    i_5_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>25, out_width=>26)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_4_chain, IN_COEF=>sg_5_COEF,
      OUT_CHAIN=>sg_5_chain);


    i_6_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>26, out_width=>26)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_5_chain, IN_COEF=>sg_6_COEF,
      OUT_CHAIN=>sg_6_chain);


    i_7_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>26, out_width=>27)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_6_chain, IN_COEF=>sg_7_COEF,
      OUT_CHAIN=>sg_7_chain);


    i_8_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>27, out_width=>27)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_7_chain, IN_COEF=>sg_8_COEF,
      OUT_CHAIN=>sg_8_chain);


    i_9_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>27, out_width=>28)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_8_chain, IN_COEF=>sg_9_COEF,
      OUT_CHAIN=>sg_9_chain);


    i_10_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>28, out_width=>28)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_9_chain, IN_COEF=>sg_10_COEF,
      OUT_CHAIN=>sg_10_chain);


    i_11_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>28, out_width=>29)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_10_chain, IN_COEF=>sg_11_COEF,
      OUT_CHAIN=>sg_11_chain);


    i_12_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>29, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_11_chain, IN_COEF=>sg_12_COEF,
      OUT_CHAIN=>sg_12_chain);


    i_13_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_12_chain, IN_COEF=>sg_13_COEF,
      OUT_CHAIN=>sg_13_chain);


    i_14_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_13_chain, IN_COEF=>sg_14_COEF,
      OUT_CHAIN=>sg_14_chain);


    i_15_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_14_chain, IN_COEF=>sg_15_COEF,
      OUT_CHAIN=>sg_15_chain);


    i_16_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_15_chain, IN_COEF=>sg_16_COEF,
      OUT_CHAIN=>sg_16_chain);


    i_17_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_16_chain, IN_COEF=>sg_17_COEF,
      OUT_CHAIN=>sg_17_chain);


    i_18_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_17_chain, IN_COEF=>sg_18_COEF,
      OUT_CHAIN=>sg_18_chain);


    i_19_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_18_chain, IN_COEF=>sg_19_COEF,
      OUT_CHAIN=>sg_19_chain);


    i_20_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_19_chain, IN_COEF=>sg_20_COEF,
      OUT_CHAIN=>sg_20_chain);


    i_21_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_20_chain, IN_COEF=>sg_21_COEF,
      OUT_CHAIN=>sg_21_chain);


    i_22_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_21_chain, IN_COEF=>sg_22_COEF,
      OUT_CHAIN=>sg_22_chain);


    i_23_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_22_chain, IN_COEF=>sg_23_COEF,
      OUT_CHAIN=>sg_23_chain);


    i_24_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>30)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_23_chain, IN_COEF=>sg_24_COEF,
      OUT_CHAIN=>sg_24_chain);


    i_25_chain: CHAIN
    generic map (
      flag_sgnum=>1,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>30, out_width=>31)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_24_chain, IN_COEF=>sg_25_COEF,
      OUT_CHAIN=>sg_25_chain);


    i_26_chain: CHAIN
    generic map (
      flag_sgnum=>0,
      flag_rst=>flag_rst, flag_en=>flag_en,
      in_width=>31, out_width=>31)
    port map (
      clk=>clk, rst=>rst, en=>en,
      IN_CHAIN=>sg_25_chain, IN_COEF=>sg_26_COEF,
      OUT_CHAIN=>sg_26_chain);


--  end b_AdderChain;

 -- b_Out_Mapping: setting out data stream
--------------------------------------------------------------------------------
    out_data(27 downto 0)<=std_logic_vector(sg_26_chain(30 downto 3));
--   end b_Out_Mapping; 

--------------------------------------------------------------------------------
 end TestFIR_arch; 
 
 --------------------------------------------------------------------------------