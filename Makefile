all:
	@make -f slave.make slave/main_slave.a
	@make -f master.make master/main_master.a
	@cmd_ASA_loader -p 3 -h ./hex/main_master.hex
	@cmd_ASA_loader -p 5 -h ./hex/main_slave.hex
slave: slave/main_slave.c
	@make -f slave.make slave/main_slave.a
	@cmd_ASA_loader -p 5 -h ./hex/main_slave.hex
	@terminal -p 5
slave_jsb: slave_jsb/main_slave_jsb.c
	@make -f slave_jsb.make slave_jsb/main_slave_jsb.a
	@cmd_ASA_loader -p 5 -h ./hex/main_slave_jsb.hex
	@terminal -p 5
master: master/main_master.c
	@make -f master.make master/main_master.a
	@cmd_ASA_loader -p 3 -h ./hex/main_master.hex
	@terminal -p 3
slave_prog:
	@cmd_ASA_loader -p 5 -h ./hex/main_slave.hex
master_prog:
	@cmd_ASA_loader -p 3 -h ./hex/main_master.hex
