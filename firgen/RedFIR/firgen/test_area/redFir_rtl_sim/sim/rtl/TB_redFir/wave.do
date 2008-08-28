onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -format Logic /fir_tb/fir/rst
add wave -noupdate -format Logic /fir_tb/fir/en
add wave -noupdate -format Logic /fir_tb/fir/clk
add wave -noupdate -divider {New Divider}
add wave -noupdate -format Literal /fir_tb/fir/in_data
add wave -noupdate -format Literal /fir_tb/fir/out_data
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ns} 0}
configure wave -namecolwidth 150
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
WaveRestoreZoom {0 ns} {987 ns}
