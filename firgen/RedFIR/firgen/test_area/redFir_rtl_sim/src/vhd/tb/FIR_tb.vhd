--pragma synopsys synthesis off 
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use std.textio.all;

entity FIR_TB is 
generic (
	flag_clk : integer := 1; 	-- this flag is to enable the 'en' signale for data enable
	flag_rst : integer := 1; 	-- this flag is to enable async reset in modules
	flag_en  : integer := 1; 	-- this flag is to enable the 'en' signale for data enable
	in_width : integer := 14; -- incomming width of data stream
	out_width: integer := 28;	-- outgoing width of data stream
	rst      : std_logic:= '0';  	-- asynchronous reset signal
	en       : std_logic:= '1';   -- data enable signal
	clk_time_2: time:= 5 ns;     -- half clock period
	tap_cnt : integer :=40); 	    -- number of tab in filter

end entity FIR_TB;


architecture FIR_TB_arch of FIR_TB is

  component TestFIR is 
  generic (
	  flag_clk : integer := 1; 	-- this flag is to enable the 'en' signale for data enable
	  flag_rst : integer := 1; 	-- this flag is to enable async reset in modules
	  flag_en  : integer := 1); 	-- this flag is to enable the 'en' signale for data enable
  port (
	  rst      : in std_logic;  	-- asynchronous reset signal
	  en       : in std_logic;  	-- data enable signal
	  clk      : in std_logic;  	-- clock signal
	  in_data  : in std_logic_vector  ((in_width-1)  downto 0);  	-- incomming data stream for filtering
	  out_data : out std_logic_vector ((out_width-1) downto 0));	-- outgoing data stream from filtering
  end component TestFIR;

 
  
  signal tb_rst : std_logic := rst;
  signal tb_en : std_logic :=  en;
  signal tb_clk : std_logic := '0';
  signal tb_in  : std_logic_vector (in_width-1 downto 0) :=(others=>'0');
  signal tb_out : std_logic_vector (out_width-1 downto 0) :=(others=>'0');
  signal tb_a_out : std_logic_vector (out_width-1 downto 0) :=(others=>'0');
  signal tb_compare: std_logic;
	signal tb_diff: std_logic_vector (out_width downto 0) :=(others=>'0');
-- fault signals
	signal tb_stop: std_logic :='0';

	type type_array_reg is array (integer range 1100 downto 0) of signed (tb_in'length -1 downto 0);
	signal array_reg : type_array_reg;
	type 		type_array is array (integer range 1100 downto 0) of signed (tb_out'length-1 downto 0);
	signal coeff_array : type_array;
	signal sg_cnt_of_taps : integer :=10;


begin



i_clock: process 
  begin
    tb_clk <= not (tb_clk);
    wait for clk_time_2;
	 if tb_stop='1' then 
		 wait;
	 end if;
  end process i_clock;


FIR:TestFIR  
  generic map(
	  flag_clk =>flag_clk,
	  flag_rst =>flag_rst,
	  flag_en  =>flag_en)
  port map(
	  rst => tb_rst,
	  en  => tb_en,     
	  clk => tb_clk,     
	  in_data => tb_in, 
	  out_data => tb_out);





-- i_compare compares the output signal "tb_out" with the output signal "tb_a_out".
-- if both outgoing data streams are identical, the signal "tb_compare" is low, else "tb_compare" is high.
i_compare: process (tb_out,tb_a_out,tb_clk)
variable va_out			:signed ( tb_out'length downto 0);
variable va_a_out :signed ( tb_out'length downto 0);
variable va_diff  :signed ( tb_out'length downto 0);

begin
	va_out(tb_out'length-1 downto 0):=signed(tb_out);     
	va_out(tb_out'length):=tb_out(tb_out'length-1);
	va_a_out(tb_a_out'length-1 downto 0):=signed(tb_a_out);
	va_a_out(tb_a_out'length):=tb_a_out(tb_a_out'length-1);


   va_diff:=va_out-va_a_out; -- building the difference 
	if tb_clk'event and tb_clk='1' then
		if (va_diff=0) then
			tb_compare <= '0';
		else 
			tb_compare <= '1';
		end if;
		tb_diff<=std_logic_vector(va_diff);
	end if; --- synch clk
end process;

			
i_files:process (tb_clk)
	
	file tb_stimuli : text open write_mode is "redFIR_sim.txt";
  

	variable va_in: bit_vector(in_width-1 downto 0);
	variable va_out: bit_vector(out_width-1 downto 0);
	
	variable trace_line: line;

begin

	if (tb_clk'event and tb_clk='1') then
		va_in:=to_bitvector(tb_in);
		va_out:=to_bitvector(tb_out);
	 	write(trace_line,va_in);
		write(trace_line,string'("    "));
		write(trace_line,va_out);
		writeline(tb_stimuli,trace_line);
	 
	end if;
end process;

i_tb_in_shift:process (tb_clk)
 variable j:  integer :=0;
 variable va_tb_in: signed (in_width-1 downto 0):=(others=>'0');

begin

	if tb_clk'event and tb_clk='1' then
		va_tb_in (in_width-1 downto 0):= signed(tb_in); -- mapping
		
	   for j in 0 to 999 loop
		  array_reg(j+1)<=array_reg(j);
	   end loop;
	array_reg(0)<= va_tb_in;
	end if; -- synch clock
end process i_tb_in_shift;

i_conv:process (tb_clk,array_reg,coeff_array,sg_cnt_of_taps)
 	variable k:  integer :=0;
 	variable va_tb_in: signed (in_width-1 downto 0):=(others=>'0');
	variable product: signed (tb_out'length+tb_in'length -1 downto 0);
	variable sum: signed(tb_out'length+tb_in'length +100 downto 0);
begin
	if tb_clk'event and tb_clk='1' then
		product:=(others=>'0');
		sum:=(others=>'0');
		for k in 0 to sg_cnt_of_taps loop
			product := array_reg (k) * coeff_array (k);
			sum := product + sum;
		end loop;
		tb_a_out<=std_logic_vector(sum(tb_out'length-1+in_width-2 downto in_width-2));  -- (in_width-2)
	end if; -- synch clock
end process i_conv;

i_stimuli:process 
	variable va_cnt: integer :=0;
	variable result: std_logic_vector (in_width-1 downto 0):=(2=>'1', 3=>'1', others=>'0');
	variable digits: integer :=2**(in_width-1)-1;
	variable zerocount: integer;
	variable n: integer:=0;
	
variable va_tap: integer :=0;
variable i: integer:=0;
variable equal: integer:=0;	

variable j: integer:=0;
variable k: integer:=0;
variable max_in : std_logic_vector(in_width-1 downto 0):=(others=>'0');
variable va_out : std_logic_vector(out_width-1 downto 0):=(others=>'0');

  begin
  -- Grundinitialsierung
   tb_in<=(others=>'0');
   tb_rst<='1';
   tb_en<='1';
	 wait for 2*clk_time_2;
   tb_rst<='0';

	-- testen verschiedener Impulsantworten
 
  -- ausmessen
-- learnig the coefficients of the fir filter
	zerocount := 0;
	max_in:=(others=>'0');
	max_in(in_width-2) :='1';
 	tb_in<=max_in;

	wait for 2*clk_time_2;
	tb_in <= (others =>'0');   
 	wait for 2*clk_time_2;
	n:=0;
	while ((zerocount < 50 ) and ( n < 1100)) loop
		coeff_array(n) <= signed(tb_out);
		n := n+1;
		equal:=1;
		for i in 0 to (tb_out'length-1) loop
			if tb_out(i) = '1' then equal:=0;
		end if; 
		end loop;

		if (equal=0) then
			zerocount := 0;
			wait for 2*clk_time_2;
		else 
			zerocount := zerocount + 1;
			wait for 2*clk_time_2;
		end if;
	end loop;

	sg_cnt_of_taps<=n-zerocount;
	wait for 4*clk_time_2;
	va_tap:=integer(sg_cnt_of_taps);	
	wait for 4*clk_time_2;
	tb_in<=max_in;
	wait for 2*clk_time_2;
	tb_in <= (others =>'0');   
	wait for (va_tap+10)*2*clk_time_2;


	for va_cnt in 0 to (in_width-1) loop
		tb_in(va_cnt)<='1';
		wait for 2*clk_time_2;
		tb_in<=(others=>'0'); 
		wait for (va_tap+10)*2*clk_time_2;
	end loop;

	tb_in<=(others=>'1');
	for va_cnt in 0 to  (in_width-2) loop
	
		tb_in<=(va_cnt=>'0',others=>'1');
		wait for 2*clk_time_2;
		tb_in<=(others=>'0'); 
		wait for (va_tap+10)*2*clk_time_2;
	end loop;

	-- "lfsr" test
	
	
 	for va_cnt in 0 to (310000) loop
		result(in_width-1 downto 1):=tb_in(in_width-2 downto 0);
		result(0):=tb_in(in_width-1) xor result(0);
		result(2):=not(result(7));
		result(7):=not(result(5));
	
		tb_in<=result;
	 wait for 2*clk_time_2;
	end loop;

	report "ENDE";
	tb_stop<='1';

	--wait;
  end process i_stimuli;


end architecture FIR_TB_arch;
--pragma synopsys synthesis on
