<?xml version="1.0" encoding="utf-8"?>
<?AutomationStudio Version=3.0.90.37 SP19?>
<SwConfiguration CpuAddress="SL1" xmlns="http://br-automation.co.at/AS/SwConfiguration">
  <TaskClass Name="Cyclic#1" />
  <TaskClass Name="Cyclic#2" />
  <TaskClass Name="Cyclic#3" />
  <TaskClass Name="Cyclic#4">
    <Task Name="ModSlave" Source="Programs.ModSlave.prg" Memory="UserROM" Language="IEC" Debugging="true" />
  </TaskClass>
  <DataObjects />
  <NcDataObjects />
  <VcDataObjects />
  <Binaries />
  <Libraries>
    <LibraryObject Name="MbusTCP" Source="Libraries.MbusTCP.lby" Memory="UserROM" Language="ANSIC" Debugging="true" />
    <LibraryObject Name="Runtime" Source="Libraries.Runtime.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="astcp" Source="Libraries.astcp.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="AsARCfg" Source="Libraries.AsARCfg.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="sys_lib" Source="Libraries.sys_lib.lby" Memory="UserROM" Language="Binary" Debugging="true" />
    <LibraryObject Name="standard" Source="Libraries.standard.lby" Memory="UserROM" Language="Binary" Debugging="true" />
  </Libraries>
</SwConfiguration>