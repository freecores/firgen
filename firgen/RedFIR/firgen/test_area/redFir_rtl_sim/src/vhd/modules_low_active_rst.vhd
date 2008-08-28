-- filename="modules_low_active_reset.vhd"

-----------------------------------------------------
--   redfir project information 
-----------------------------------------------------
--  project name:  redFIR
--  used by:       Franziska Kalb
--  date:          Tuesday, 19 march 2002
--  
--     1
--  module with a low active reset
-----------------------------------------------------
-----------------------------------------------------
-- initated new modules Wed Dec 12 18:00:00 2000 fhf
--
-- Wed Nov 28 22:00 2001 fhf: 
--  bugfix for fc2-compiler 3.61 : new modules and including
--  all signal width calulations into constants
--  prefix is "map"
--
-- Wed Dec 05 11:55 2001 fhf: 
--  bugfix for quartus II software: change handling with
--  and signum extantion. Quartus II does not except others 
--  int signum variables    
--
-- Wed Dec 07 10:50 2001 fhf: 
--  bugfix for quartus II software: including va_zero instat of
--  using (others=>'0') to avoid quartus II bug
--
-- Thue Sept 25 18:23 2003 fhf: 
--  bufix of flag_rst='0' and rst='0' then ...
--  to flag_rst='1' and rst='0' 
--
-----------------------------------------------------
-- known bugs:
-- fc2 HDL-177  
-- Warning: Local variable 'va_SUM' is being read before its value is assigned,
--          in routine ADDER_... line 170 ...
--          This may cause simulation not to match synthesis.  
-- Bugfix:  this seems to ba a fc2 misunderstanding call me if you know a workarround
-----------------------------------------------------

library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;

package components_redfir is

-----------------------------------------------------

component DELAY is
  generic (
    flag_clk: integer := 1;   --enable flag for pipelined modules
    flag_rst: integer := 1;   --enable clock for synchronous reset/negative rst
    flag_en : integer  := 1;   --enable flag for enable signal 
    a_width : integer := 14;  -- width of incomming data
    b_width : integer := 1;   -- will be ignored
    o_width : integer := 14;  -- width of outgoing data /clip or expand
    gamma   : integer := 1);  -- will be ignored
  port (
    clk: in std_logic;        -- clock signal if flag_clk=1
    rst: in std_logic;        -- reset signal if flag_rst=1
    en:  in std_logic;        -- enable signal if flag_en=1   
    IN_A:  in  signed ((a_width-1) downto 0);     --incoming data
    IN_B:  in  signed ((b_width-1) downto 0);     -- ignored data
    OUT_DATA: out signed ((o_width-1) downto 0)); -- outgoing data
  end component DELAY;
  
  
  --------------------------------------------------------------------
  
  component ADDER  is
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en : integer := 1;
    a_width : integer := 20;
    b_width : integer := 15;
    o_width : integer := 20;
    gamma   : integer := 4);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end component ADDER;
  
  
  ------------------------------------------------------------------------------
  
  component B_SUB is
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en: integer  := 1;
    a_width : integer := 20;
    b_width : integer := 17;
    o_width : integer := 19;
    gamma   : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);
    OUT_DATA: out signed ((o_width-1) downto 0));
  end component B_SUB;
  
  
  ------------------------------------------------------------------------------
  
  component P_MULT is
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en: integer  := 1;
    a_width : integer := 12;
    b_width : integer := 1;  -- will be ignored
    o_width : integer := 14;
    gamma   : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0); -- port will be ignored
    OUT_DATA: out signed ((o_width-1) downto 0));
  end component P_MULT;
  
  ----------------------------------------------------------------------------
 component S_MULT is
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en: integer  := 1;
    a_width : integer := 1;
    b_width : integer := 1;
    o_width : integer := 1;
    gamma   : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);
    OUT_DATA: out signed ((o_width-1) downto 0));
  end component S_MULT ; 
  
  -----------------------------------------------------------------------------
  component A_SUB is
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en: integer  := 1;
    a_width : integer := 18;
    b_width : integer := 22;
    o_width : integer := 24;
    gamma   : integer := 6);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);
    OUT_DATA: out signed ((o_width-1) downto 0));
  end component A_SUB;
------------------------------------------------------------------------------
  
component REMAP is
    generic (
       in_width   :integer :=12;
       gamma      :integer :=2;
       out_width  :integer :=11);
    port (
       IN_MAP    : in  signed ((in_width-1) downto 0);
       OUT_MAP   : out signed ((out_width-1) downto 0));
end component REMAP;

--------------------------------------------------------------------------------

component CHAIN is
  generic (
    flag_sgnum:integer :=1;
    flag_rst: integer := 0;
    flag_en: integer  := 1;
    in_width : integer := 1;
    out_width : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_CHAIN:     in  signed ((in_width-1) downto 0);
    IN_COEF:  	  in  signed ((in_width-1) downto 0);
    OUT_CHAIN:    out signed ((out_width-1) downto 0));
  end component CHAIN;
  --------------------------------------------------------------------------------
  end components_redfir;

  
  
  
------------------------------------------------------

library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
 
package minmax_pkg is
  function f_min(a,b: integer) return integer;
  function f_max(a,b: integer) return integer;
end package minmax_pkg;

package body minmax_pkg is

function f_min(a,b: integer) return integer is
  begin
    if (a>b) then return b;
    else return a;
    end if;
  end function f_min;
function f_max(a,b: integer) return integer is
  begin
    if (a<b) then return b;
    else return a;
    end if;
  end function f_max;

end package body minmax_pkg;
  
-----------------------------------------------------


library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;

library work; 
 use work.minmax_pkg.all;

entity DELAY is
-----------------------------------------
-- synthesis synopsis ok 01/11/27
-- this module is a simple register with one clock
-- incomming signal will be fitted to given o_width
-- ! if a_width is bigger than o_width it will CLIP !
-- in this case signal may not be right
------------------------------------------
  generic (
    flag_clk: integer := 1;   --enable flag for pipelined modules
    flag_rst: integer := 0;   --enable clock for synchronous reset/ low active reset
    flag_en : integer  := 1;   --enable flag for enable signal 
    a_width : integer := 16;  -- width of incomming data
    b_width : integer := 32;   -- will be ignored
    o_width : integer := 8;  -- width of outgoing data /clip or expand
    gamma   : integer := 1);  -- will be ignored
  port (
    clk: in std_logic;        -- clock signal if flag_clk=1
    rst: in std_logic;        -- reset signal if flag_rst=1
    en:  in std_logic;        -- enable signal if flag_en=1   
    IN_A:  in  signed ((a_width-1) downto 0);     --incoming data
    IN_B:  in  signed ((b_width-1) downto 0);     -- ignored data
    OUT_DATA: out signed ((o_width-1) downto 0)); -- outgoing data
  end entity DELAY;
  
architecture  DELAY_arch of DELAY is

begin
  
  process (clk,rst,IN_A,en) is

    constant min_width : integer := f_min(a_width,o_width);
    constant max_width : integer := f_max(a_width,o_width);
 
    variable va_a 		: signed ((max_width-1) downto 0);    
  	variable va_sgnm_a 	: signed ((max_width-1) downto 0);  --QII_bugfix
  begin
    -- mapping in signal 
	  va_a((a_width-1) downto 0):=IN_A((a_width-1) downto 0);
	  va_sgnm_a:=(others=>IN_A(a_width-1));
	  --expanding signal with signum

	  if (o_width=(a_width+1)) then
		  va_a(max_width-1):=va_sgnm_a(0);
	  elsif (o_width>(a_width+1)) then
		  va_a((max_width-1) downto min_width):=va_sgnm_A((max_width-1) downto min_width);
	  end if;

	  -- output
    if ((flag_rst=1) and (rst='0')) then -- low active reset
      OUT_DATA<=(others=>'0');  
      
    else
      if (flag_en=0) or (en='1') then-- no enable flag or enable signal aktiv
         if (flag_clk=0) then -- no enable flag or enable signal aktiv
           OUT_DATA((o_width-1) downto 0)<=va_a((o_width-1) downto 0);
         elsif(clk'event and (clk='1')) then  -- clock signal or no clock enable
           OUT_DATA((o_width-1) downto 0)<=va_a((o_width-1) downto 0);
         end if; -- clock event 
       end if ;  -- clock enable 
    end if;   -- reset signal

  end process;
  
end DELAY_arch;

-----------------------------------------------------
library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity ADDER  is
-------------------------------------------
-- input IN_B will be shifted by gamma bit
-- and added to input IN_A
-- the result will be fitted to the o_width
---------------------------------------------
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en : integer := 1;
    a_width : integer := 20;
    b_width : integer := 18;
    o_width : integer := 20;
    gamma   : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end entity ADDER;
  
architecture  ADDER_arch of ADDER is
begin
  process (clk,rst,IN_A,IN_B,en) is
 	  constant max_width    : integer := f_max(a_width,(b_width+gamma))+1;
	  constant map_width    : integer := f_max(a_width,(b_width+gamma));  --fc2-bugfix
	  constant map_bgamma   : integer := gamma+b_width;                   --fc2-bugfix
    	  constant map_gamma    : integer := gamma-1;                       --fc2-bugfix
    	  constant vou_width    : integer := f_max(max_width,o_width);

	  variable va_A       : signed (map_width downto 0);
    	  variable va_sgnm_A  : signed (map_width downto 0);--QII-bugfix

	  variable va_B       : signed (map_width downto 0);
    	  variable va_sgnm_B  : signed (map_width downto 0);--QII-bugfix

	  variable va_SUM     : signed (map_width downto 0);
	
	  variable va_OUT : signed ((vou_width-1) downto 0);
    	  variable va_sgnm_OUT: signed ((vou_width-1) downto 0 );--QII-bugfix
7

  begin
   --mapping IN_A
    va_a((a_width-1) downto 0):=IN_A;
    va_sgnm_A:=(others=>IN_A(a_width-1));
    if (a_width=map_width) then
      va_A(map_width):=va_sgnm_A(0);
    elsif (a_width<map_width) then
      va_A(map_width downto a_width):=va_sgnm_A(map_width downto a_width);
    end if;
  
   
    --mapping IN_B
    va_B((b_width+gamma-1) downto gamma):=IN_B;  --ok
    va_sgnm_B:=(others=>IN_B(b_width-1));  --ok
   
    if ((b_width+gamma)=map_width) then
      va_B(map_width):=va_sgnm_B(0);
    elsif ((b_width+gamma)<map_width) then
      va_B(map_width downto map_bgamma):=va_sgnm_B(map_width downto map_bgamma);
    end if;
    --calculation of sum

    va_SUM(map_width downto gamma) := va_A (map_width downto gamma)+ va_B(map_width downto gamma);
   
    if (gamma>0) then
      if (gamma>a_width) then
        va_SUM(map_gamma downto a_width):=va_sgnm_A(map_gamma downto a_width);
        va_SUM((a_width-1) downto 0):=va_A((a_width-1) downto 0);
      else
        va_SUM(map_gamma downto 0):= va_A((gamma-1) downto 0);
      end if;
    end if;

    --mapping of out width
    va_OUT(map_width downto 0):=va_SUM(map_width downto 0);
    va_sgnm_OUT:=(others=>va_SUM(max_width-1));
    if (o_width=(max_width+1)) then
      va_OUT(o_width-1):=va_sgnm_OUT(0);
    elsif (o_width>(max_width+1)) then
      va_OUT((o_width-1)downto max_width):=va_sgnm_OUT((o_width-1)downto max_width);
    end if;
    -- registered transfer
    if ((flag_rst=1) and (rst='0')) then -- low active reset signal
      OUT_DATA<=(others=>'0');  
    else
      if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
        if (flag_clk=1) then
            if(clk'event and (clk='1')) then  -- clock signal or no clock enable
               OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
            end if;   -- enable clock flag
        elsif ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
          OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
        end if ;  -- clock enable 
      end if;

   end if;   -- reset signal
 end process;
end architecture ADDER_arch;


-----------------------------------------------------
library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity P_MULT  is
-------------------------------------------
-- input IN_A will be shifted by gamma bit
-- and added to input IN_A again
-- the result will be fitted to the o_width
---------------------------------------------
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;  -- low active rst signal
    flag_en : integer := 1;
    a_width : integer := 20;
    b_width : integer := 15;
    o_width : integer := 20;
    gamma   : integer := 4);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end entity P_MULT;

architecture  P_MULT_arch of P_MULT is 
begin
 process (clk,rst,IN_A,en) is

    constant vou_width    : integer := f_max(a_width+gamma+1,o_width);
    constant map_width    : integer := a_width+gamma;
    constant map_a        : integer := a_width-1;
    constant map_gamma    : integer := gamma-1;
    constant map_o        : integer := o_width-1;

	  variable va_A   : signed (map_width downto 0);
	  variable va_sgnm_A   : signed (map_width downto 0);

	  variable va_B   : signed (map_width downto 0);
	  variable va_SUM : signed (map_width downto 0);
	  variable va_OUT : signed ((vou_width-1) downto 0);
    variable va_sgnm_OUT : signed ((vou_width-1) downto 0);
	  
  begin
    --mapping IN_A
    va_a((a_width-1) downto 0):=IN_A;
    va_sgnm_A:=(others=>IN_A(a_width-1));
    if (gamma>0) then -- gamma =0 may never occure?
      va_A(map_width downto a_width):=va_sgnm_A(map_width downto a_width);
    end if;
     
    --mapping IN_A for shift
    va_B((map_width-1) downto gamma):=IN_A;  --ok
    va_B(map_width):=IN_A(a_width-1);
    
    --calculation of sum
    va_SUM(map_width downto gamma) := va_A (map_width downto gamma)+ va_B(map_width downto gamma);
    if (gamma>0) then
      if (gamma>a_width) then
        va_SUM(map_gamma downto a_width):=va_sgnm_A(map_gamma downto a_width);
        va_SUM(map_a downto 0):=va_A(map_a downto 0);
      else
        va_SUM(map_gamma downto 0):= va_A(map_gamma downto 0);
      end if;
    end if;

    --mapping of out width
    va_OUT(map_width downto 0):=va_SUM(map_width downto 0);
    va_sgnm_OUT:=(others=>va_SUM(map_width));

    if (o_width=(map_width+1)) then
      va_OUT(o_width-1):=va_sgnm_OUT(0);
    elsif (o_width>(map_width+1)) then
      va_OUT(map_o downto (map_width+1)):=va_sgnm_OUT(map_o downto (map_width+1));
    end if;

    -- registered transfer
    if ((flag_rst=1) and (rst='0')) then -- low active reset signal
      OUT_DATA<=(others=>'0');  
    else


      if (flag_clk=1) then
        if(clk'event and (clk='1')) then  -- clock signal or no clock enable
          if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
            OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
          end if;   -- enable clock flag
        end if; -- clock event 
      elsif ((flag_en=1) and (en='1')) then -- no enable flag or enable signal aktiv
        OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
      end if ;  -- clock enable 
    end if;   -- reset signal
  end process;
end architecture P_MULT_arch;
-----------------------------------------------------
-----------------------------------------------------
library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity B_SUB  is
-------------------------------------------
-- input IN_B will be shifted by gamma bit
-- and subtracted from input IN_A
-- the result will be fitted to the o_width
---------------------------------------------
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0; -- low active rst
    flag_en : integer := 1;
    a_width : integer := 5;
    b_width : integer := 6;
    o_width : integer := 63;
    gamma   : integer := 11);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end entity B_SUB;
  
architecture  B_SUB_arch of B_SUB is
begin
  process (clk,rst,IN_A,IN_B,en) is
    constant max_width    : integer := f_max(a_width,(b_width+gamma))+1;
    constant map_width    : integer := f_max(a_width,(b_width+gamma));
    constant map_gamma      : integer := gamma-1;
    constant map_bgamma     : integer := b_width+gamma;
    constant map_o		    : integer := o_width-1;

    constant vou_width    : integer := f_max(max_width,o_width);

    variable va_A       : signed (map_width downto 0);
    variable va_sgnm_A  : signed (map_width downto 0);

    variable va_B       : signed (map_width downto 0);
    variable va_sgnm_B  : signed (map_width downto 0);
    variable va_SUM     : signed (map_width downto 0);
    variable va_OUT     : signed ((vou_width-1) downto 0);
    variable va_sgnm_OUT: signed ((vou_width-1) downto 0);
    variable va_zero    : signed (map_width downto 0);

  begin
  va_zero:=(others=>'0');
   --mapping IN_A
    va_A((a_width-1) downto 0):=IN_A;
    va_sgnm_A:=(others=>IN_A(a_width-1));

    if (a_width=map_width) then
      va_A(map_width):=va_sgnm_A(0);
    elsif (a_width<map_width) then
      va_A(map_width downto a_width):=va_sgnm_A(map_width downto a_width);
    end if;
  
   
    --mapping IN_B
    va_B((b_width+gamma-1) downto gamma):=IN_B;  --ok
    va_sgnm_B:=(others=>IN_B(b_width-1));
  
    if ((b_width+gamma)=map_width) then
      va_B(map_width):=va_sgnm_B(0);
    elsif ((b_width+gamma)<map_width) then
      va_B((map_width) downto map_bgamma):=va_sgnm_B((map_width) downto map_bgamma);
    end if;
    if (gamma=1) then
      va_B(0):='0';
    elsif (gamma>1) then
      va_B(map_gamma downto 0):=va_zero(map_gamma downto 0);
    end if;

    --calculation of difference

    va_SUM(map_width downto 0) := va_A (map_width downto 0)- va_B(map_width downto 0);


    --mapping of out width
    va_OUT(map_width downto 0):=va_SUM(map_width downto 0);
    va_sgnm_OUT:=(others=>va_SUM(map_width));

    if (o_width=(max_width+1)) then
      va_OUT(map_o):=va_sgnm_OUT(0);
    elsif (o_width>(max_width+1)) then
      va_OUT(map_o downto max_width):=va_sgnm_OUT(map_o downto max_width);
    end if;
    -- registered transfer
   if ((flag_rst=1) and (rst='0')) then --  low active reset signal
      OUT_DATA<=(others=>'0');  
    else
      if (flag_clk=1) then
        if(clk'event and (clk='1')) then  -- clock signal or no clock enable
          if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
            OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
          end if;   -- enable clock flag
        end if; -- clock event 
      elsif ((flag_en=1) and (en='1')) then -- no enable flag or enable signal aktiv
        OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
      end if ;  -- clock enable 
    end if;   -- reset signal
  end process;
end architecture B_SUB_arch;

-----------------------------------------------------
library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity A_SUB  is
---------------------------------------------
-- input IN_A will be shifted by gamma bit
-- and  then input IN_B subtracted from
-- the result will be fitted to the o_width
---------------------------------------------

  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0;
    flag_en : integer := 1;
    a_width : integer := 5;
    b_width : integer := 6;
    o_width : integer := 63;
    gamma   : integer := 11);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end entity A_SUB;
  
architecture  A_SUB_arch of A_SUB is
begin
  process (clk,rst,IN_A,IN_B,en) is
 	  constant max_width    : integer := f_max(b_width,(a_width+gamma))+1;
	  constant map_width    : integer := f_max(b_width,(a_width+gamma));
    constant map_gamma    : integer := gamma-1;
    constant map_o        : integer := o_width-1;

    constant vou_width    : integer := f_max(max_width,o_width);

	  variable va_A         : signed (map_width downto 0);
    variable va_sgnm_A    : signed (map_width downto 0);

	  variable va_B         : signed (map_width downto 0);
    variable va_sgnm_B    : signed (map_width downto 0);

    variable va_zero      : signed (map_width downto 0);

	  variable va_SUM       : signed (map_width downto 0);
	  variable va_OUT       : signed ((vou_width-1) downto 0);
    variable va_sgnm_OUT  : signed ((vou_width-1) downto 0);
  

  begin
  
   va_zero:=(others=>'0');
    --mapping IN_A
    va_A((a_width+gamma-1) downto gamma):=IN_A;  --ok
    va_sgnm_A:=(others=>IN_A(a_width-1));
    if ((a_width+gamma)=map_width) then
      va_A(map_width):=va_sgnm_A(0);
    elsif ((a_width+gamma)<map_width) then
      va_A((map_width) downto (a_width+gamma)):=va_sgnm_A((map_width) downto (a_width+gamma));
    end if;
    if (gamma=1) then
      va_A(0):='0';
    elsif (gamma>1) then
      va_A(map_gamma downto 0):=va_zero(map_gamma downto 0);
    end if;

   --mapping IN_B
    va_b((b_width-1) downto 0):=IN_B;
    va_sgnm_b:=(others=>IN_B(b_width-1));

    if (b_width=map_width) then
      va_B(map_width):=va_sgnm_B(0);
    elsif (b_width<map_width) then
      va_B(map_width downto b_width):=va_sgnm_B(map_width downto b_width);
    end if;
  
    --calculation of difference

    va_SUM(map_width downto 0) := va_A (map_width downto 0)- va_B(map_width downto 0);
   

    --mapping of out width
    va_OUT(map_width downto 0):=va_SUM(map_width downto 0);
    va_sgnm_OUT:=(others=>va_SUM(map_width));

    if (o_width=(max_width+1)) then
      va_OUT(map_o):=va_sgnm_OUT(0);
    elsif (o_width>(max_width+1)) then
      va_OUT(map_o downto max_width):=va_sgnm_OUT(map_o downto max_width);
    end if;
    -- registered transfer
   if ((flag_rst=1) and (rst='0')) then --low active reset
      OUT_DATA<=(others=>'0');  
    else
      if (flag_clk=1) then
        if(clk'event and (clk='1')) then  -- clock signal or no clock enable
          if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
            OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
          end if;   -- enable clock flag
        end if; -- clock event 
      elsif ((flag_en=1) and (en='1')) then -- no enable flag or enable signal aktiv
        OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
      end if ;  -- clock enable 
    end if;   -- reset signal
  end process;
end architecture A_SUB_arch;
-----------------------------------------------------
 -----------------------------------------------------
library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity S_MULT  is
--------------------------------------------
-- input IN_A will be shifted by gamma bit
-- and then input IN_A subtracted from it
-- the result will be fitted to the o_width
---------------------------------------------
  generic (
    flag_clk: integer := 1;
    flag_rst: integer := 0; -- negative rst
    flag_en : integer := 1;
    a_width : integer := 10;
    b_width : integer := 5;
    o_width : integer := 11;
    gamma   : integer := 1);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_A:  in  signed ((a_width-1) downto 0);
    IN_B:  in  signed ((b_width-1) downto 0);    
    OUT_DATA: out signed ((o_width-1) downto 0));
  end entity S_MULT;
  
architecture  S_MULT_arch of S_MULT is
begin
  process (clk,rst,IN_A,en) is
 	  constant max_width    : integer := (a_width+gamma)+1;
	  constant map_width    : integer := (a_width+gamma);
    constant map_gamma    : integer := gamma-1;
    constant map_o        : integer := o_width-1;

    constant vou_width    : integer := f_max(max_width,o_width);

	  variable va_A         : signed (map_width downto 0);
	  variable va_B         : signed (map_width downto 0);
    variable va_sgnm_B    : signed (map_width downto 0);

	  variable va_SUM       : signed (map_width downto 0);
	  variable va_OUT       : signed ((vou_width-1) downto 0);
    variable va_sgnm_OUT  : signed ((vou_width-1) downto 0);
    
    variable va_zero      : signed (map_width downto 0);


  begin
    va_zero:=(others=>'0');
   
    --mapping IN_A
    va_A((a_width+gamma-1) downto gamma):=IN_A;  --ok
 
    va_A(map_width):=in_A(a_width-1);
    if (gamma=1) then
      va_A(0):='0';
    elsif (gamma>1) then
      va_A(map_gamma downto 0):=va_zero(map_gamma downto 0);
    end if;

   --mapping IN_A without shift
    va_B((a_width-1) downto 0):=IN_A;
    va_sgnm_B:=(others=>IN_A(a_width-1));

    if (gamma=0) then
      va_B(map_width):=va_sgnm_B(0);
    elsif (gamma>0) then
      va_B(map_width downto a_width):=va_sgnm_B(map_width downto a_width);
    end if;
  
    --calculation of difference

    va_SUM(map_width downto 0) := va_A (map_width downto 0)- va_B(map_width downto 0);
   

    --mapping of out width
    va_OUT(map_width downto 0):=va_SUM(map_width downto 0);
    va_sgnm_OUT:=(others=>va_SUM(map_width));


    if (o_width=(max_width+1)) then
      va_OUT(map_o):=va_sgnm_OUT(0);
    elsif (o_width>(max_width+1)) then
      va_OUT(map_o downto max_width):=va_sgnm_OUT(map_o downto max_width);
    end if;
    -- registered transfer
    if ((flag_rst=1) and (rst='0')) then -- low active reset signal
      OUT_DATA<=(others=>'0');  
    else
      if (flag_clk=1) then
        if(clk'event and (clk='1')) then  -- clock signal or no clock enable
          if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
            OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
          end if;   -- enable clock flag
        end if; -- clock event 
      elsif ((flag_en=1) and (en='1')) then -- no enable flag or enable signal aktiv
        OUT_DATA((o_width-1) downto 0)<=va_OUT((o_width-1) downto 0);
      end if ;  -- clock enable 
    end if;   -- reset signal
  end process;
end architecture S_MULT_arch;
-----------------------------------------------------
 
-- modified by fhf 27th of November 

library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;


entity REMAP is
  generic (
       in_width   :integer :=29;
       gamma      :integer :=0;
       out_width  :integer :=29);
    port (
       IN_MAP    : in  signed ((in_width-1) downto 0);
       OUT_MAP   : out signed ((out_width-1) downto 0));
  end entity REMAP;


architecture REMAP_arch of REMAP is
begin
 process (IN_MAP) is
	
 constant min_width	:	integer	:=f_min(in_width+gamma,out_width);--29
 constant max_width	:	integer	:=f_max(in_width+gamma,out_width);--29
 constant map_gamma	:	integer	:=gamma-1;-- -1
 constant map_inpgam: integer :=in_width+gamma-1; --28
 constant map_o     : integer :=out_width-1;      --28


 variable va_map : signed ((max_width-1) downto 0);
 variable va_sgnm_map : signed ((max_width-1) downto 0);
 variable va_zero : signed (max_width downto 0);

 begin
  va_zero:=(others=>'0');--
 	-- mapping gamma
 	if (gamma=1) then--
		va_map(0):='0';
	elsif (gamma>1) then--
		va_map(map_gamma downto 0):=va_zero(map_gamma downto 0);
    end if;
	------------------------------------------------
	-- mapping signal
	va_map(map_inpgam downto gamma):=IN_MAP((in_width-1) downto 0);
	va_sgnm_map:=(others=>IN_MAP(in_width-1));	 

	-- expanding signal
	if (out_width=in_width+gamma+1) then
		va_map(map_o):=va_sgnm_map(0);
	elsif (out_width>(in_width+gamma+1)) then
		va_map(map_o downto min_width):=va_sgnm_map(map_o downto min_width);
 	end if;

	--cutting signal
	out_MAP(map_o downto 0)<=va_map(map_o downto 0);
 
 end process;
end architecture REMAP_arch;

------------------------------------------
-- rewrite by fhf 29.11.01
------------------------------------------

library IEEE;
 use IEEE.std_logic_1164.all;
 use IEEE.std_logic_arith.all;
library work;
  use work.minmax_pkg.all;

entity CHAIN is
  generic (
    flag_sgnum:integer :=1;
    flag_rst: integer := 0;  -- negative rst
    flag_en: integer  := 1;
    in_width : integer := 31;
    out_width : integer := 32);
  port (
    clk: in std_logic;
    rst: in std_logic;
    en:  in std_logic;
    IN_CHAIN:     in  signed ((in_width-1) downto 0);
    IN_COEF:  	   in  signed ((in_width-1) downto 0);
    OUT_CHAIN:    out signed ((out_width-1) downto 0));
end entity CHAIN;
  
architecture  CHAIN_arch of CHAIN is
begin
  process (clk,rst,IN_CHAIN,IN_COEF,en) is

    
    constant map_width  : integer := out_width-1;


    variable va_in		: signed (map_width downto 0);
    variable va_sgnm_in		: signed (map_width downto 0);
 
	 variable va_coef 	: signed (map_width downto 0);
	 variable va_sgnm_coef 	: signed (map_width downto 0);

	variable va_out 	: signed (map_width downto 0);
  begin
	  -- mapping coef to size

		va_coef((in_width-1) downto 0):=IN_COEF;
		va_sgnm_coef:=(others=>IN_COEF(in_width-1));

     if (in_width<out_width) then
		  va_coef(map_width downto in_width):=va_sgnm_coef(map_width downto in_width);
	  end if;
  
	  -- expand in_chain
	  va_in((in_width-1) downto 0):=IN_CHAIN;
     va_sgnm_in:=(others=>IN_CHAIN(in_width-1));

	  if (in_width<out_width) then
		  va_in(map_width downto in_width):=va_sgnm_in(map_width downto in_width);
	  end if;

	  --calculating tap
	  if (flag_sgnum=0) then
		  va_out:=va_in+va_coef;
	  else
		  va_out:=va_in-va_coef;
	  end if;

 	  if ((flag_rst=1) and (rst='0')) then -- low active reset signal
        OUT_CHAIN<=(others=>'0');  
    elsif(clk'event and (clk='1')) then  -- clock signal or no clock enable
      if ((flag_en=0) or (en='1')) then -- no enable flag or enable signal aktiv
        OUT_CHAIN(map_width downto 0)<= va_out(map_width downto 0);
		  end if;
    end if;
	end process;
end architecture CHAIN_arch;





