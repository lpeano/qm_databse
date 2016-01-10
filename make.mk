MAKE=make
base_path=/home/lpeano/qm_databse

#mk_all=$(foreach rule,$(testlogger_rules), $(MAKE)  -C $(test_logger_path) -f Makefile $(rule);)
#mk_all=$(foreach rule,$(testlogger_rules), $(MAKE) -C $(testlogger_path) $(rule);)

testlogger_path=$(base_path)/test
testlogger_rules=clean testlogger

testmemory_path=$(base_path)/test
testmemory_rules=clean testmemory 

BitMap_path=$(base_path)/test
BitMap_rules=clean BitMap 
###############################################################
################ MODULES ######################################
all_modules_dep_list=log
testlogger_module_dep_list=log
testmemory_module_dep_list=log shmeme_block mapper
BitMap_module_dep_list=log mapper shmeme_block

mk_all=$(foreach rule,$($@_rules), $(MAKE) -C $($@_path) $(rule);)
mk_all_module=$(foreach module,$($@_module_dep_list), $(MAKE) -C $(base_path)/$(module) install;)


