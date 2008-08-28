--filename: components_redfir_tb

-----------------------------------------------------
--   redfir project information 
-----------------------------------------------------
--  project name:  redFIR
--  used by:       Franziska Kalb
--  date:          19.10. 2001

--  version:       1
--  revision:      0
--  
-----------------------------------------------------
-----------------------------------------------------
-- 
-----------------------------------------------------


library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
 use work.components_redfir.all;
 
 entity components_tb is
 generic (
 a_width: integer:= 31;
 b_width: integer:= 31;
 o_width: integer:=  32;
 in_width: integer:=10;
 out_width: integer:= 32;
 clk_time: time:= 10 ns
 );
 
 end components_tb;
 
 architecture cp_tb of components_tb is
 
 
 

 constant gamma : integer :=14;
 
  ----------------------------------------------
 --signals
 -----------------------------------------------
 signal tb_clk :std_logic := '1';
 signal tb_rst, tb_en:std_logic := '1';
 signal tb_IN_A     : signed (a_width-1 downto 0);
 signal tb_IN_B     : signed (b_width-1 downto 0);
 signal tb_Out_data_delay : signed (o_width-1 downto 0);
 signal tb_Out_data_ADDER : signed (o_width-1 downto 0);
 signal tb_Out_data_B_SUB : signed (o_width-1 downto 0);
 signal tb_Out_data_P_MULT: signed (o_width-1 downto 0);
 signal tb_Out_data_s_MULT: signed (o_width-1 downto 0);
 signal tb_Out_data_A_SUB : signed (o_width-1 downto 0);
 signal tb_IN_DATA:         signed (in_width-1 downto 0);
 signal tb_Out_data_REMAP:  signed (o_width-1 downto 0);
 signal tb_Out_CHAIN : signed (b_width-1 downto 0);
 ---------------------------------------------------------------------
 begin 
 
 
  i_delay: DELAY
  generic map(
       flag_clk=> 1,
       flag_rst=> 0,
       flag_en => 0,
       a_width => a_width,
       b_width => b_width,
       o_width => o_width,
       gamma   => 1)
  port map(
    clk=> tb_clk,
    rst=> tb_rst,
    en => tb_en,
    IN_A => tb_IN_A,
    IN_B => tb_IN_B,     -- ignored data
    OUT_DATA => tb_Out_data_delay); -- outgoing data
    
 ----------------------------------------------------------------------------
-- module is a build in component to delay input signal "in_A" 
-- one clock to "OUT_DATA"; in_B will be ignored
-- generic cases:
--	1.	a_width > o_width
-- 		MSB of IN_DATA will be ignored, result "OUT_DATA"
--		may be NOT identical with "IN_A"!
--	2.	a_width < o_width
--		MSB of "OUT_DATA" will be filled with signum of "IN_A"
 -----------------------------------------------------------------------------   

	i_ADDER: ADDER
  		generic map(
 			flag_clk => 1,
        flag_rst => 1,
       flag_en  => 1,
  	a_width  => a_width,
    b_width  => b_width,
   	o_width  => o_width,
    gamma    => 4)
 port map(
   clk => tb_clk,
   rst => tb_rst,
   en  => tb_en,
   IN_A => tb_IN_A,
   IN_B => tb_IN_B,
   OUT_DATA => tb_Out_data_ADDER );

--------------------------------------------------------------------------
-- module is a build in components to add input signal "IN_A" to input signal 
-- "IN_B".The output signal "OUT_DATA" will be shifted by gamma.
-- The result depends also on the width of a_width, b_width and o_width .
--    
    --------------------------------------------------------------------------
i_B_SUB : B_SUB
 generic map(
   flag_clk      => 1,
   flag_rst      => 1,
   flag_en       => 1,
   a_width       => a_width,
   b_width       => b_width,  -- will be ignored
   o_width       => o_width,
   gamma         => gamma)
 port map(
   clk    => tb_clk,
   rst    => tb_rst,
   en     => tb_en,
   IN_A   => tb_IN_A,
   IN_B   => tb_IN_B, -- port will be ignored
   OUT_DATA => tb_Out_data_B_SUB);


-----------------------------------------------
--module is a build in components to subtract input signal "IN_B" from input signal "IN_A".
--The output signal "out data "will be shifted by gamma.
-- The result depends on the width of a_width, b_width and o_width.
------------------------------------------------------------------------    
    
i_P_MULT : P_MULT
 generic map(
   flag_clk      => 1,
   flag_rst      => 1,
   flag_en       => 1,
   a_width       => a_width,
   b_width       => b_width,  -- will be ignored
   o_width       => o_width,
   gamma         => gamma)
 port map(
   clk    => tb_clk,
   rst    => tb_rst,
   en     => tb_en,
   IN_A   => tb_IN_A,
   IN_B   => tb_IN_B, -- port will be ignored
   OUT_DATA => tb_Out_data_P_MULT);  
    
---------------------------------------------------------------------------------
---------------------------------------------------------------------------------

i_S_Mult:  S_MULT
  generic map(
    flag_clk      => 1,
    flag_rst      => 1,
    flag_en       => 1,
    a_width       => a_width,
    b_width       => b_width,
    o_width       => o_width,
    gamma         => gamma)
  port map(
    clk   => tb_clk,
    rst   => tb_rst,
    en    => tb_en,
    IN_A  => tb_IN_A,
    IN_B  => tb_IN_B,
    OUT_DATA => tb_Out_data_S_MULT);
-----------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------

i_A_SUB: A_SUB
  generic map(
    flag_clk         => 1,
    flag_rst         => 1,
    flag_en          => 1,
    a_width          => a_width,
    b_width          => b_width,
    o_width          => o_width,
    gamma            => gamma)
  port map(
    clk     => tb_clk,
    rst     => tb_rst,
    en      => tb_en,
    IN_A    => tb_IN_A,
    IN_B    => tb_IN_B,
    OUT_DATA => tb_Out_data_A_SUB);

i_chain:CHAIN 
  generic map(
    flag_sgnum	=>1,
    flag_rst   =>1,
    flag_en			 =>1,
    in_width		 =>a_width,
    out_width  =>b_width)
  port map(
    clk  => tb_clk,
    rst  => tb_rst,
    en   => tb_en,
    IN_CHAIN => tb_in_A,
    IN_COEF  => tb_in_B,
    OUT_CHAIN=> tb_Out_chain);

i_my_clock: process

begin

tb_clk <= not (tb_clk);
wait for clk_time;

end process i_my_clock;

--------------------------------------------------------------------

i_my_tb_process :process

variable  count_value_A : signed (a_width -1 downto 0);
variable  count_value_B : signed (b_width -1 downto 0);


begin

 
  tb_rst<='1';
  tb_en<='1';


  tb_IN_A<=(others=>'0');
  tb_IN_B<=(others=>'0');


 wait for clk_time*2;
  tb_rst<='0';
  tb_en<='1';




  outer_loop: FOR count_value_B in b_width-1 downto 0 loop
   inner_loop: for  count_value_A in a_width-1 downto 0 loop

 
        tb_IN_A(count_value_A)<='1';
        tb_IN_B(count_value_B)<='1';


       wait for clk_time*2;

  		 tb_IN_A(count_value_A)<='0';
        tb_IN_B(count_value_B)<='0';

 

    end loop inner_loop ;
  end loop outer_loop ;

end process i_my_tb_process;

     
    
 
 end  architecture cp_tb ;
 
 
