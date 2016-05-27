-- copyright: Madhav Desai
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ahir;
use ahir.BaseComponents.all;
use ahir.Utilities.all;

--
-- higher in->out latency, but distributed in space (think ASIC, ASIC).
-- often good enough.
--
entity ShiftRegisterQueue is
  generic(name : string; queue_depth: integer := 1; data_width: integer := 32);
  port(clk: in std_logic;
       reset: in std_logic;
       data_in: in std_logic_vector(data_width-1 downto 0);
       push_req: in std_logic;
       push_ack: out std_logic;
       data_out: out std_logic_vector(data_width-1 downto 0);
       pop_ack : out std_logic;
       pop_req: in std_logic);
end entity ShiftRegisterQueue;

architecture behave of ShiftRegisterQueue is

  type QueueArray is array(natural range <>) of std_logic_vector(data_width-1 downto 0);
  constant n_stages : integer := (queue_depth + 1)/2;
  signal intermediate_data: QueueArray(0 to n_stages);
  signal intermediate_lr_req: std_logic_vector(0 to n_stages);
  signal intermediate_lr_ack: std_logic_vector(0 to n_stages);

begin  -- SimModel

     intermediate_data(0) <= data_in;
     intermediate_lr_req(0) <= push_req;
     push_ack <= intermediate_lr_ack(0);

     stageGen: for I in 0 to n_stages-1 generate
	qinst: QueueBase 
		generic map (name => name & "-stage-" & Convert_To_String(I),
				queue_depth => 2, data_width => data_width)
		port map ( reset => reset, clk => clk, 
			data_in => intermediate_data(I),
			push_req => intermediate_lr_req(I),
			push_ack => intermediate_lr_ack(I),
			pop_req  => intermediate_lr_ack(I+1), -- cross-over
			pop_ack => intermediate_lr_req(I+1), -- cross-over
			data_out => intermediate_data(I+1));
     end generate stageGen;

     data_out <= intermediate_data(n_stages);
     intermediate_lr_ack(n_stages) <= pop_req;
     pop_ack <= intermediate_lr_req(n_stages);

end behave;
