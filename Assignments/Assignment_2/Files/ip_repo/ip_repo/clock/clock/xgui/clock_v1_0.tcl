
# Loading additional proc with user specified bodies to compute parameter values.
source [file join [file dirname [file dirname [info script]]] gui/clock_v1_0.gtcl]

# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  ipgui::add_page $IPINST -name "Page 0"

  ipgui::add_param $IPINST -name "CLOCK_FREQ_HZ"
  ipgui::add_param $IPINST -name "DIV_VAL"

}

proc update_PARAM_VALUE.DIV_VAL { PARAM_VALUE.DIV_VAL PARAM_VALUE.CLOCK_FREQ_HZ } {
	# Procedure called to update DIV_VAL when any of the dependent parameters in the arguments change
	
	set DIV_VAL ${PARAM_VALUE.DIV_VAL}
	set CLOCK_FREQ_HZ ${PARAM_VALUE.CLOCK_FREQ_HZ}
	set values(CLOCK_FREQ_HZ) [get_property value $CLOCK_FREQ_HZ]
	set_property value [gen_USERPARAMETER_DIV_VAL_VALUE $values(CLOCK_FREQ_HZ)] $DIV_VAL
}

proc validate_PARAM_VALUE.DIV_VAL { PARAM_VALUE.DIV_VAL } {
	# Procedure called to validate DIV_VAL
	return true
}

proc update_PARAM_VALUE.CLOCK_FREQ_HZ { PARAM_VALUE.CLOCK_FREQ_HZ } {
	# Procedure called to update CLOCK_FREQ_HZ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.CLOCK_FREQ_HZ { PARAM_VALUE.CLOCK_FREQ_HZ } {
	# Procedure called to validate CLOCK_FREQ_HZ
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to update C_S00_AXI_DATA_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_DATA_WIDTH { PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to validate C_S00_AXI_DATA_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to update C_S00_AXI_ADDR_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_ADDR_WIDTH { PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to validate C_S00_AXI_ADDR_WIDTH
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to update C_S00_AXI_BASEADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_BASEADDR { PARAM_VALUE.C_S00_AXI_BASEADDR } {
	# Procedure called to validate C_S00_AXI_BASEADDR
	return true
}

proc update_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to update C_S00_AXI_HIGHADDR when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.C_S00_AXI_HIGHADDR { PARAM_VALUE.C_S00_AXI_HIGHADDR } {
	# Procedure called to validate C_S00_AXI_HIGHADDR
	return true
}


proc update_MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH { MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH PARAM_VALUE.C_S00_AXI_DATA_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_DATA_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_DATA_WIDTH}
}

proc update_MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH { MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH PARAM_VALUE.C_S00_AXI_ADDR_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.C_S00_AXI_ADDR_WIDTH}] ${MODELPARAM_VALUE.C_S00_AXI_ADDR_WIDTH}
}

proc update_MODELPARAM_VALUE.CLOCK_FREQ_HZ { MODELPARAM_VALUE.CLOCK_FREQ_HZ PARAM_VALUE.CLOCK_FREQ_HZ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.CLOCK_FREQ_HZ}] ${MODELPARAM_VALUE.CLOCK_FREQ_HZ}
}

proc update_MODELPARAM_VALUE.DIV_VAL { MODELPARAM_VALUE.DIV_VAL PARAM_VALUE.DIV_VAL } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.DIV_VAL}] ${MODELPARAM_VALUE.DIV_VAL}
}

