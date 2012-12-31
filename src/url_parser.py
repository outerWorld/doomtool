import sys
import os

class url_parser():
	url = ""
	host = ""
	# "www.deephex.com/AI/docs/index.html", uri_path="/AI/docs/"
	uri_path = ""
	# without res_type, for example: "index.html", res_name="index", res_type="html"
	res_name = ""
	res_type = ""
	# "www.deephex.com/AI/request.jpg?id=1&name=AIstory&type=pdf", uri_params= {'id':1, 'name':AIstory, 'type':pdf}
	uri_params = { }

	def __init__(self, url):
		self.url = url.lstrip(r"http://")
		segs = self.url.split(r"/")
		self.host = segs[0]
		for i in segs[1:len(segs)-1:]:
			self.uri_path = self.uri_path + "/" + i
		res = segs[-1]

		paras = ""
		name = ""
		if len(res.split(r"?")) < 2:
			paras = ""
		else:
			paras = res.split(r"?")[1]	

		name = res.split(r"?")[0]

		if len(name.split(r".")) < 2:
			self.res_name = ""
			self.res_type = ""
			# append name to uri_path
			self.uri_path = self.uri_path + r"/" + name
		else:
			self.res_name = name.split(r".")[0]
			self.res_type = name.split(r".")[1]

		# extract parameters
		for para in paras.split(r"&"):
			para = para.split(r"=")
			para_name = ""
			value = ""
			if len(para) <= 0:
				para_name = ""
				value = ""	
			elif len(para) == 1:
				para_name = para[0]	
				value = ""
			else:
				para_name = para[0]	
				value = para[1]	
			if len(para_name) > 0 and para_name not in self.uri_params:
				self.uri_params[para_name] = value
		
	def get_host(self):
		return self.host

	def get_uri_path(self):
		return self.uri_path
	
	def get_res(self):
		return self.res_name + r"." + self.res_type

	def get_res_type(self):
		return self.res_type

	def get_params(self):
		return self.uri_params

	def show(self):
		print "host = %s, uri_path = %s, resource=%s.%s" %(self.host, self.uri_path, self.res_name, self.res_type)
		for name in self.uri_params:
			print "\'%s\':%s" %(name, self.uri_params[name])

def test_url_parser(argv):
	url_result = url_parser(argv[1])	
	url_result.show()

if __name__ == "__main__":
	test_url_parser(sys.argv)
