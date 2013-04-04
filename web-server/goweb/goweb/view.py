# -*- encoding=utf-8 -*-
from django.http import HttpResponse
from django.http import HttpRequest
from django.template import Template, Context

class SubItem():
	"""
	subject Item
	"""
	def __init__(self, title):
		self.title = title
	def set_desc(self, desc):
		self.desc = desc
	def set_url(self, url):
		self.url = url

class Subject():
	def __init__(self, ID, name):
		self.name = name
		self.ID = ID
		self.Items = []
	def add_item(self, item_id, item):
		self.Items.append(item)

desc = "创建时间：2013-02-03 10:00:00, 更新时间:2013-03-01 08:04:00 评论人：黄春平"
preset_data = {}
preset_data["network"] = ("网络技术", "/network", ["libpcap代码剖析", desc, "sniffer代码剖析", desc, "nmap代码剖析", desc, "分片报文分析", desc, "原始套接字应用", desc, "路由安全机制探讨", desc])
preset_data["datamine"] = ("数据挖掘技术", "/datamine", ["分词算法", desc, "马尔可夫模型", desc, "分句算法", desc, "倾向性分析", desc, "情感词总结", desc, "字典构建方法总结", desc, "自动分类算法", desc])
preset_data["sw_safe"] = ("软件安全技术", "/sw_safe", ["逆向工程", desc, "软件破解", desc, "IDA软件介绍", desc, "汇编语言探讨", desc, "Intel CPU解析", desc, "攻击字典构建算法", desc])
preset_data["data_alg"] = ("数据结构及算法", "/data_alg", ["排序算法总结", desc, "查找算法总结", desc, "hash函数总结", desc])

subject_list = []
for s in preset_data:
	subject = Subject(s, preset_data[s][0])
	for i in range(0, len(preset_data[s][2])/2):
		item = SubItem(preset_data[s][2][i*2])
		item.set_desc(preset_data[s][2][i*2+1])
		item.set_url(preset_data[s][1])
		#print "%s %s %s" %(item.title, item.desc, item.url)
		subject.add_item(i, item)
	subject_list.append(subject)

def home(request):
	res = "goweb/pages/" + "index.html"
	t = Template(open(res, "r").read())
	ctx = Context()
	ctx["subjects"] = subject_list
	#t1 = Template("{% for subject in subjects %} <li><a href=\"#\"></a><li> {% endfor %} ")
	#print t1.render(ctx)
	#print t.render(ctx)
	return HttpResponse(t.render(ctx))	

def html(request):
	if request.path == "/":
		res = "goweb/pages/" + "index.html"
	else:
		res = "goweb/pages" + request.path
	#print res
	t = Template(open(res, "r").read())
	ctx = Context()
	return HttpResponse(t.render(ctx))

def css(request):
	res = "goweb/" + request.path
	t = Template(open(res).read())
	ctx = Context()
	return HttpResponse(t.render(ctx))	

def images(request):
	res = "goweb" + request.path
	#t = Template(open(res).read())
	#ctx = Context()
	return HttpResponse(open(res).read(), content_type="image/png")
