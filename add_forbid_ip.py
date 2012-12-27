import sys
import os
import commands

drop_ips = { }
rdline = ""
for rdline in sys.stdin:
	drop_ip = rdline.split("\n")[0].strip(" ")

	str=os.popen("iptables -L -n").read()
	lines=str.split("\n")
	# load dropped ips from output of iptables
	for line in lines:
		line=line.split(" ")
		if len(line) <= 1:
			continue
		if line[0] != "DROP":
			continue
		drop_line = [""]*20 
		i = 0
		for d in line:
			if len(d) > 0:
				drop_line[i] = d
				i = i + 1
			else:
				pass
		if drop_line[0] == "DROP":
			if drop_line[3] not in drop_ips:
				drop_ips[drop_line[3]] = 1
			else:
				pass
		else:
			pass
	#for ip in drop_ips:
		#cmd = "iptables -D INPUT -s " + ip + " -j DROP"
		#print "cmd=%s" %cmd
		#os.popen(cmd)
	# add the new ip being dropped to iptables
	if drop_ip not in drop_ips:
		cmd="iptables -I INPUT -s " + drop_ip + " -j DROP"
		os.popen(cmd)
		print "%s" %cmd

