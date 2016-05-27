-- copyright: Madhav Desai
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity QueueBase is
  generic(name : string; queue_depth: integer := 1; data_width: integer := 32);
  port(clk: in std_logic;
       reset: in std_logic;
       data_in: in std_logic_vector(data_width-1 downto 0);
       push_req: in std_logic;
       push_ack: out std_logic;
       data_out: out std_logic_vector(data_width-1 downto 0);
       pop_ack : out std_logic;
       pop_req: in std_logic);
end entity QueueBase;

architecture behave of QueueBase is

  type QueueArray is array(natural range <>) of std_logic_vector(data_width-1 downto 0);
  function Incr(x: integer; M: integer) return integer is
  begin
    if(x < M) then
      return(x + 1);
    else
      return(0);
    end if;
  end Incr;


begin  -- SimModel

 --
 -- 0-depth queue is just a set of wires.
 --
 triv: if queue_depth = 0 generate
	push_ack <= pop_req;
	pop_ack  <= push_req;
	data_out <= data_in;
 end generate triv;



 qDGt0: if queue_depth > 0 generate 
  NTB: block 
  	signal queue_array : QueueArray(queue_depth-1 downto 0);
  	signal read_pointer, write_pointer : integer range 0 to queue_depth-1;
  	signal incr_write_pointer, incr_read_pointer : integer range 0 to queue_depth-1;
  	signal queue_size : integer range 0 to queue_depth;
  begin
 
    assert (queue_size < queue_depth) report "Queue " & name & " is full." severity note;

    qD1: if (queue_depth = 1) generate
     incr_read_pointer <= read_pointer;
     incr_write_pointer <= write_pointer;
    end generate qD1;

    qDG1: if (queue_depth > 1) generate
     incr_read_pointer <= Incr(read_pointer, queue_depth-1);
     incr_write_pointer <= Incr(write_pointer, queue_depth-1);
    end generate qDG1;

    push_ack <= '1' when (queue_size < queue_depth) else '0';
    pop_ack  <= '1' when (queue_size > 0) else '0';

    -- bottom pointer gives the data in FIFO mode..
    data_out <= queue_array(read_pointer);
  
    -- single process
    process(clk, reset, read_pointer, write_pointer, incr_read_pointer, incr_write_pointer, queue_size, push_req, pop_req)
      variable qsize : integer range 0 to queue_depth;
      variable push,pop : boolean;
      variable next_read_ptr,next_write_ptr : integer range 0 to queue_depth-1;
    begin

      qsize := queue_size;
      push  := false;
      pop   := false;
      next_read_ptr := read_pointer;
      next_write_ptr := write_pointer;
      
      if((qsize < queue_depth) and push_req = '1') then
          push := true;
      end if;
  
      if((qsize > 0) and pop_req = '1') then
          pop := true;
      end if;
  
  
      if(push) then
          next_write_ptr := incr_write_pointer;
      end if;
  
      if(pop) then
          next_read_ptr := incr_read_pointer;
      end if;
  
  
      if(pop and (not push)) then
          qsize := qsize - 1;
      elsif(push and (not pop)) then
          qsize := qsize + 1;
      end if;
        
  
      if(clk'event and clk = '1') then
        
	if(reset = '1') then
           queue_size <= 0;
           read_pointer <= 0;
           write_pointer <= 0;
	else
           if(push) then
          	queue_array(write_pointer) <= data_in;
           end if;
        
           queue_size <= qsize;
           read_pointer <= next_read_ptr;
           write_pointer <= next_write_ptr;

        end if;
      end if;

    end process;
   end block NTB;
  end generate qDGt0;
  

end behave;
