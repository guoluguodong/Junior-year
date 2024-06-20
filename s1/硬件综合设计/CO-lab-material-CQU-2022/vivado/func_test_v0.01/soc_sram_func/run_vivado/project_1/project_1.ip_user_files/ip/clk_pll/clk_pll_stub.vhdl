-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (win64) Build 2708876 Wed Nov  6 21:40:23 MST 2019
-- Date        : Fri Jan 12 22:44:17 2024
-- Host        : LAPTOP-09VEF5P5 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               C:/Users/a3840/Desktop/CO-lab-material-CQU-2022/CO-lab-material-CQU-2022/vivado/func_test_v0.01/soc_sram_func/rtl/xilinx_ip/clk_pll/clk_pll_stub.vhdl
-- Design      : clk_pll
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a100tcsg324-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clk_pll is
  Port ( 
    cpu_clk : out STD_LOGIC;
    timer_clk : out STD_LOGIC;
    clk_in1 : in STD_LOGIC
  );

end clk_pll;

architecture stub of clk_pll is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "cpu_clk,timer_clk,clk_in1";
begin
end;
